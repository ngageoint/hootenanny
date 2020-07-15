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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "Address.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/address/AddressParser.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/string/StringDistanceConsumer.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>

namespace hoot
{

QSet<QString> Address::_streetTypes;
QMap<QString, QString> Address::_streetFullTypesToTypeAbbreviations;
QMap<QString, QString> Address::_streetTypeAbbreviationsToFullTypes;
StringDistancePtr Address::_stringComp;

Address::Address() :
_address(""),
_allowLenientHouseNumberMatching(true),
_parsedFromAddressTag(true),
_isRange(false),
_isSubLetter(false)
{
  if (!_stringComp)
  {
    _initializeStringComparator();
  }
}

Address::Address(const QString& address, const bool allowLenientHouseNumberMatching) :
_address(address),
_allowLenientHouseNumberMatching(allowLenientHouseNumberMatching),
_parsedFromAddressTag(true),
_isRange(false),
_isSubLetter(false)
{
  if (!_stringComp)
  {
    _initializeStringComparator();
  }
}

void Address::_initializeStringComparator()
{
  const QString stringCompClassName = ConfigOptions().getAddressStringComparer().trimmed();
  if (stringCompClassName.isEmpty())
  {
    throw IllegalArgumentException(
      "No address string comparer specified (must implement StringDistance).");
  }
  else
  {
    _stringComp =
      StringDistancePtr(
        Factory::getInstance().constructObject<StringDistance>(stringCompClassName));
    if (!_stringComp)
    {
      throw IllegalArgumentException(
        "Invalid address string comparer (must implement StringDistance): " +
        stringCompClassName);
    }
    std::shared_ptr<StringDistanceConsumer> strDistConsumer =
      std::dynamic_pointer_cast<StringDistanceConsumer>(_stringComp);
    if (strDistConsumer)
    {
      strDistConsumer->setStringDistance(
        StringDistancePtr(new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha())));
    }
  }
}

bool Address::operator==(const Address& address) const
{
  LOG_VART(_address);
  LOG_VART(address._address);

  return
    !_address.isEmpty() &&
      (_stringComp->compare(_address, address._address) == 1.0 ||
       (_allowLenientHouseNumberMatching &&
        // don't do subletter matching on an intersection, as it won't have house numbers
        !isStreetIntersectionAddress(_address, !_parsedFromAddressTag) &&
        AddressParser::addressesMatchDespiteSubletterDiffs(_address, address._address)));
}

QList<QRegExp> Address::getIntersectionSplitTokens()
{
  QList<QRegExp> intersectionSplitTokens;
  intersectionSplitTokens.append(QRegExp("\\s+and\\s+", Qt::CaseInsensitive));
  intersectionSplitTokens.append(QRegExp("\\s+&\\s+", Qt::CaseInsensitive));
  intersectionSplitTokens.append(QRegExp("\\s+&amp;\\s+", Qt::CaseInsensitive));
  return intersectionSplitTokens;
}

QSet<QString> Address::getStreetTypes(const bool includeAbbreviations)
{
  if (_streetTypes.isEmpty())
  {
    const QStringList streetTypesRaw =
      FileUtils::readFileToList(ConfigOptions().getStreetTypesFile());
    // This list could be expanded.  See the note in the associated config file.
    for (int i = 0; i < streetTypesRaw.size(); i++)
    {
      const QString streetTypeEntry = streetTypesRaw.at(i);
      const QStringList streetTypeEntryParts = streetTypeEntry.split("\t");
      if (streetTypeEntryParts.size() != 2)
      {
        throw HootException("Invalid street type entry: " + streetTypeEntry);
      }
      _streetTypes.insert(streetTypeEntryParts.at(0).toLower());
      if (includeAbbreviations)
      {
        _streetTypes.insert(streetTypeEntryParts.at(1).toLower());
      }
    }
  }
  return _streetTypes;
}

QMap<QString, QString> Address::getStreetFullTypesToTypeAbbreviations()
{
  if (_streetFullTypesToTypeAbbreviations.isEmpty())
  {
    const QStringList streetTypesRaw =
      FileUtils::readFileToList(ConfigOptions().getStreetTypesFile());
    // This list could be expanded.  See the note in the associated config file.
    for (int i = 0; i < streetTypesRaw.size(); i++)
    {
      const QString streetTypeEntry = streetTypesRaw.at(i);
      const QStringList streetTypeEntryParts = streetTypeEntry.split("\t");
      if (streetTypeEntryParts.size() != 2)
      {
        throw HootException("Invalid street type entry: " + streetTypeEntry);
      }
      _streetFullTypesToTypeAbbreviations[streetTypeEntryParts.at(0).toLower()] =
        streetTypeEntryParts.at(1).toLower();
    }
  }
  return _streetFullTypesToTypeAbbreviations;
}

QMap<QString, QString> Address::getStreetTypeAbbreviationsToFullTypes()
{
  if (_streetTypeAbbreviationsToFullTypes.isEmpty())
  {
    const QStringList streetTypesRaw =
      FileUtils::readFileToList(ConfigOptions().getStreetTypesFile());
    // This list could be expanded.  See the note in the associated config file.
    for (int i = 0; i < streetTypesRaw.size(); i++)
    {
      const QString streetTypeEntry = streetTypesRaw.at(i);
      const QStringList streetTypeEntryParts = streetTypeEntry.split("\t");
      if (streetTypeEntryParts.size() != 2)
      {
        throw HootException("Invalid street type entry: " + streetTypeEntry);
      }
      _streetTypeAbbreviationsToFullTypes[streetTypeEntryParts.at(1).toLower()] =
        streetTypeEntryParts.at(0).toLower();
    }
  }
  return _streetTypeAbbreviationsToFullTypes;
}

QStringList Address::getIntersectionParts() const
{
  return _address.split("and");
}

bool Address::isStreetIntersectionAddress(const QString& addressStr,
                                          const bool requireStreetTypeInIntersection)
{
  if (addressStr.trimmed().isEmpty())
  {
    return false;
  }

  // libpostal doesn't recognize intersections correctly, so doing it with some very simple custom
  // logic. Not even using a regex here, b/c our definition of an intersection is very loose. We
  // tighten it up some with 'requireStreetTypeInIntersection', which is meant to be used when an
  // address is parsed from a non-address tag (name, etc.). Our definition of an intersection
  // address is likely not foolproof.

  if (requireStreetTypeInIntersection &&
      !StringUtils::endsWithAny(addressStr, getStreetTypes().toList()))
  {
    return false;
  }

  return StringUtils::bisectsAny(addressStr, getIntersectionSplitTokens());
}

bool Address::isStreetIntersectionAddress(const Address& address,
                                          const bool requireStreetTypeInIntersection)
{
  return isStreetIntersectionAddress(address._address, requireStreetTypeInIntersection);
}

void Address::removeStreetTypes()
{
  LOG_TRACE(_address);

  if (!isStreetIntersectionAddress(_address, !_parsedFromAddressTag))
  {
    // If its a non-intersection, just remove the last street type token. We're assuming its at the
    // end, which may not be alway true.
    StringUtils::removeLastIndexOf(_address, _streetTypes.toList());
  }
  else
  {
    // If we have an intersection address, split it into its two parts and remove street type tokens
    // from the end of each.

    QStringList addressParts = StringUtils::splitOnAny(_address, getIntersectionSplitTokens(), 2);
    assert(addressParts.size() == 2);
    const QStringList streetTypes = getStreetTypes().toList();

    QString firstIntersectionPart = addressParts[0].trimmed();
    StringUtils::removeLastIndexOf(firstIntersectionPart, streetTypes);
    QString secondIntersectionPart = addressParts[1].trimmed();
    StringUtils::removeLastIndexOf(secondIntersectionPart, streetTypes);

    _address = firstIntersectionPart.trimmed() + " and " + secondIntersectionPart.trimmed();
  }

  LOG_VART(_address);
}

void Address::removeHouseNumber()
{
  LOG_VART(_address);
  if (!isStreetIntersectionAddress(_address, false))
  {
    // house num should be the first token on a non-intersection address, so remove it
    StringUtils::splitAndRemoveAtIndex(_address, QRegExp("\\s+"), 0);
  }
  LOG_VART(_address);
}

}
