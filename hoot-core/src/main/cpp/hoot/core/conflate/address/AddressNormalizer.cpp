/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "AddressNormalizer.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/address/LibPostalInit.h>
#include <hoot/core/conflate/address/Address.h>
#include <hoot/core/util/StringUtils.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

AddressNormalizer::AddressNormalizer() :
_numNormalized(0)
{
  _addressTagKeys.reset(new AddressTagKeys());
}

void AddressNormalizer::normalizeAddresses(const ElementPtr& e)
{
  const QSet<QString> addressTagKeys = _addressTagKeys->getAddressTagKeys(*e);
  LOG_VART(addressTagKeys);
  for (QSet<QString>::const_iterator addressTagKeyItr = addressTagKeys.begin();
       addressTagKeyItr != addressTagKeys.end(); ++addressTagKeyItr)
  {
    const QString addressTagKey = *addressTagKeyItr;
    // normalization may find multiple addresses; we'll arbitrarily take the first one and put the
    // rest in an alt field
    const QSet<QString> normalizedAddresses = normalizeAddress(e->getTags().get(addressTagKey));
    bool firstAddressParsed = false;
    QString altAddresses;
    for (QSet<QString>::const_iterator normalizedAddressItr = normalizedAddresses.begin();
         normalizedAddressItr != normalizedAddresses.end(); ++normalizedAddressItr)
    {
      const QString normalizedAddress = *normalizedAddressItr;
      if (!firstAddressParsed)
      {
        e->getTags().set(addressTagKey, normalizedAddress);
        LOG_TRACE(
          "Set normalized address: " << normalizedAddress << " for tag key: " << addressTagKey);
        firstAddressParsed = true;
      }
      else
      {
        altAddresses += normalizedAddress + ";";
      }
    }
    if (!altAddresses.isEmpty())
    {
      altAddresses.chop(1);
      e->getTags().set("alt_address", altAddresses);
      LOG_TRACE(
        "Set alt normalized address(es): " << altAddresses << " for tag key: " << addressTagKey);
    }
  }
}

QSet<QString> AddressNormalizer::normalizeAddress(const QString& address) const
{
  const QString addressToNormalize = address.trimmed().simplified();
  if (!Address::isStreetIntersectionAddress(addressToNormalize))
  {
    return _normalizeAddressWithLibPostal(addressToNormalize);
  }
  else
  {
    // libpostal doesn't handle intersections very well, so doing intersection normalization with
    // custom logic
    return _normalizeAddressIntersection(addressToNormalize);
  }
}

QSet<QString> AddressNormalizer::_normalizeAddressWithLibPostal(const QString& address) const
{
  LOG_TRACE("Normalizing " << address << " with libpostal...");

  QSet<QString> normalizedAddresses;
  QString addressCopy = address;

  // See note about init of this in AddressParser::parseAddresses.
  LibPostalInit::getInstance();

  _prepareAddressForLibPostalNormalization(addressCopy);

  size_t num_expansions;
  // specifying a language in the options is optional, but could we get better performance if
  // we did specify one when we know what it is (would have to check to see if it was supported
  // first, of course)?
  char** expansions =
    libpostal_expand_address(
      addressCopy.toUtf8().data(), libpostal_get_default_options(), &num_expansions);
  // add all the normalizations libpostal finds as possible addresses
  for (size_t i = 0; i < num_expansions; i++)
  {
    const QString normalizedAddress = QString::fromUtf8(expansions[i]);
    LOG_VART(normalizedAddress);
    if (_isValidNormalizedAddress(addressCopy, normalizedAddress) &&
        !normalizedAddresses.contains(normalizedAddress))
    {
      normalizedAddresses.insert(normalizedAddress);
      LOG_TRACE("Normalized address from: " << address << " to: " << normalizedAddress);
      _numNormalized++;
    }
    else
    {
      LOG_TRACE("Skipping normalized address: " << normalizedAddress << "...");
    }
  }
  libpostal_expansion_array_destroy(expansions, num_expansions);

  return normalizedAddresses;
}

QSet<QString> AddressNormalizer::_normalizeAddressIntersection(const QString& address) const
{
  LOG_TRACE("Normalizing intersection: " << address << "...");

  const QMap<QString, QString> streetTypeAbbreviationsToFullTypes =
    Address::getStreetTypeAbbreviationsToFullTypes();
  const QStringList addressParts =
    StringUtils::splitOnAny(address, Address::getIntersectionSplitTokens(), 2);
  LOG_VART(addressParts.size());

  if (addressParts.size() != 2)
  {
    throw IllegalArgumentException(
      "A non-intersection address was passed into street intersection address normalization.");
  }

  // replace all street type abbreviations in both intersection parts in the address with their full
  // name counterparts

  QString modifiedAddress;
  for (int i = 0; i < addressParts.size(); i++)
  {
    QString addressPart = addressParts.at(i).trimmed();
    LOG_VART(addressPart);
    for (QMap<QString, QString>::const_iterator itr = streetTypeAbbreviationsToFullTypes.begin();
         itr != streetTypeAbbreviationsToFullTypes.end(); ++itr)
    {
      const QString abbrev = itr.key().trimmed();
      LOG_VART(abbrev);
      const QString fullType = itr.value().trimmed();
      LOG_VART(fullType);

      LOG_VART(addressPart.endsWith(abbrev, Qt::CaseInsensitive));
      if (addressPart.endsWith(abbrev, Qt::CaseInsensitive))
      {
        StringUtils::replaceLastIndexOf(addressPart, abbrev, fullType);
        LOG_VART(addressPart);
      }
    }
    LOG_VART(addressPart);

    modifiedAddress += addressPart.trimmed();
    if (i == 0)
    {
      modifiedAddress += " and ";
    }
  }
  LOG_VART(modifiedAddress);

  // If one of the intersection parts has a street type and the other doesn't we'll copy one street
  // type over to the other. This isn't foolproof as we could end up giving one of the intersections
  // an incorrect street type (the actual element's address tags never get modified, though).
  // However, it does help with address matching and will remain in place unless its found to be
  // causing harm in some way.

  // TODO: this needs cleanup

  QStringList modifiedAddressParts =
    StringUtils::splitOnAny(modifiedAddress, Address::getIntersectionSplitTokens(), 2);
  assert(modifiedAddressParts.size() == 2);
  LOG_VART(modifiedAddressParts);
  QString firstIntersectionPart = modifiedAddressParts[0].trimmed();
  LOG_VART(firstIntersectionPart);
  QString secondIntersectionPart = modifiedAddressParts[1].trimmed();
  LOG_VART(secondIntersectionPart);

  QStringList streetFullTypes;
  QStringList streetPluralTypes;
  const QStringList streetFullTypesTemp =
    Address::getStreetFullTypesToTypeAbbreviations().keys();
  streetFullTypes = streetFullTypesTemp;
  // TODO
  for (int i = 0; i < streetFullTypesTemp.size(); i++)
  {
    const QString pluralType = streetFullTypesTemp.at(i) + "s";
    if (!streetFullTypes.contains(pluralType))
    {
      streetFullTypes.append(pluralType);
    }
    streetPluralTypes.append(pluralType);
  }
  LOG_VART(streetPluralTypes);

  if (StringUtils::endsWithAny(firstIntersectionPart.trimmed(), streetPluralTypes))
  {
    firstIntersectionPart.chop(1);
    LOG_VART(firstIntersectionPart);
  }
  if (StringUtils::endsWithAny(secondIntersectionPart.trimmed(), streetPluralTypes))
  {
    secondIntersectionPart.chop(1);
    LOG_VART(secondIntersectionPart);
  }

  QStringList modifiedAddressPartsTemp;
  for (int i = 0; i < modifiedAddressParts.size(); i++)
  {
    QString modifiedAddressPart = modifiedAddressParts.at(i);
    if (StringUtils::endsWithAny(modifiedAddressPart.trimmed(), streetPluralTypes))
    {
      modifiedAddressPart.chop(1);
      LOG_VART(modifiedAddressPart);
    }
    modifiedAddressPartsTemp.append(modifiedAddressPart);
  }
  modifiedAddressParts = modifiedAddressPartsTemp;

  QString firstIntersectionEndingStreetType =
    StringUtils::endsWithAnyAsStr(firstIntersectionPart.trimmed(), streetFullTypes).trimmed();
  // TODO
  if (firstIntersectionEndingStreetType.endsWith('s'))
  {
    firstIntersectionEndingStreetType.chop(1);
  }
  LOG_VART(firstIntersectionEndingStreetType);
  QString secondIntersectionEndingStreetType =
    StringUtils::endsWithAnyAsStr(secondIntersectionPart.trimmed(), streetFullTypes).trimmed();
  if (secondIntersectionEndingStreetType.endsWith('s'))
  {
    secondIntersectionEndingStreetType.chop(1);
  }
  LOG_VART(secondIntersectionEndingStreetType);
  if (!firstIntersectionEndingStreetType.isEmpty() &&
      secondIntersectionEndingStreetType.isEmpty())
  {
    LOG_VART(modifiedAddressParts[1]);
    modifiedAddressParts[1] =
      modifiedAddressParts[1].trimmed() + " " + firstIntersectionEndingStreetType.trimmed();
    LOG_VART(modifiedAddressParts[1]);
  }
  else if (firstIntersectionEndingStreetType.isEmpty() &&
           !secondIntersectionEndingStreetType.isEmpty())
  {
    LOG_VART(modifiedAddressParts[0]);
    modifiedAddressParts[0] =
      modifiedAddressParts[0].trimmed() + " " + secondIntersectionEndingStreetType.trimmed();
    LOG_VART(modifiedAddressParts[0]);
  }
  modifiedAddress = modifiedAddressParts[0].trimmed() + " and " + modifiedAddressParts[1].trimmed();
  LOG_VART(modifiedAddress);

  // go ahead and send it to libpostal to finish out the normalization to avoid duplicating some
  // code, although it probably won't change any from this point
  return _normalizeAddressWithLibPostal(modifiedAddress);
}

void AddressNormalizer::_prepareAddressForLibPostalNormalization(QString& address)
{
  LOG_TRACE("Before normalization fix: " << address);
  LOG_VART(Address::isStreetIntersectionAddress(address));
  // This is a nasty thing libpostal does where it changes "St" to "Saint" when it should be
  // "Street".
  if (address.endsWith("st", Qt::CaseInsensitive) && !Address::isStreetIntersectionAddress(address))
  {
    StringUtils::replaceLastIndexOf(address, "st", "Street");
  }
  LOG_TRACE("After normalization fix: " << address);
}

bool AddressNormalizer::_isValidNormalizedAddress(const QString& inputAddress,
                                                  const QString& normalizedAddress)
{
  // force normalization of "&" to "and"
  if (normalizedAddress.contains(" & "))
  {
    return false;
  }
  // This is a bit of hack, but I don't like the way libpostal is turning "St" or "Street" into
  // "Saint". Should probably look into configuration of libpostal or update it for a possible fix
  // instead.
  else if (normalizedAddress.endsWith("saint", Qt::CaseInsensitive) &&
           (inputAddress.endsWith("street", Qt::CaseInsensitive) ||
            inputAddress.endsWith("st", Qt::CaseInsensitive)))
  {
    return false;
  }
  return true;
}

}
