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
#include <hoot/core/conflate/address/AddressTagKeys.h>

// libpostal
#include <libpostal/libpostal.h>

namespace hoot
{

AddressNormalizer::AddressNormalizer() :
_numNormalized(0)
{
}

void AddressNormalizer::normalizeAddresses(const ElementPtr& e)
{
  const QSet<QString> addressTagKeys = AddressTagKeys::getInstance().getAddressTagKeys(*e);
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
  // See note about init of this in AddressParser::parseAddresses.
  LibPostalInit::getInstance();

  LOG_VART(address);
  const QString addressToNormalize = address.trimmed().simplified();
  LOG_VART(addressToNormalize);
  QSet<QString> normalizedAddresses;

  size_t num_expansions;
  // specifying a language in the options is optional, but could we get better performance if
  // we did specify one when we know what it is (would have to check to see if it was supported
  // first, of course)?
  char** expansions =
    libpostal_expand_address(
      addressToNormalize.toUtf8().data(), libpostal_get_default_options(),
      &num_expansions);
  // add all the normalizations libpostal finds as possible addresses
  for (size_t i = 0; i < num_expansions; i++)
  {
    const QString normalizedAddress = QString::fromUtf8(expansions[i]);
    LOG_VART(normalizedAddress);
    if (_isValidNormalizedAddress(addressToNormalize, normalizedAddress) &&
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

bool AddressNormalizer::_isValidNormalizedAddress(const QString& inputAddress,
                                                  const QString& normalizedAddress)
{
  // force normalization of "&" to "and"
  if (normalizedAddress.contains(" & "))
  {
    return false;
  }
  // This is a bit of hack, but I don't like the way libpostal is turning "St" or "Street" into
  // "Saint".  Should probably look into configuration of libpostal for a possible fix instead.
  else if (normalizedAddress.endsWith("saint", Qt::CaseInsensitive) &&
           (inputAddress.endsWith("street", Qt::CaseInsensitive) ||
            inputAddress.endsWith("st", Qt::CaseInsensitive)))
  {
    return false;
  }
  return true;
}

}
