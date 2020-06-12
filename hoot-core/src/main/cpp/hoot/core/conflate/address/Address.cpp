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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "Address.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/address/AddressParser.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

QSet<QString> Address::_streetTypes;
QMap<QString, QString> Address::_streetFullTypesToTypeAbbreviations;
QMap<QString, QString> Address::_streetTypeAbbreviationsToFullTypes;

Address::Address() :
_address(""),
_allowLenientHouseNumberMatching(true),
_parsedFromAddressTag(true)
{
}

Address::Address(const QString& address, const bool allowLenientHouseNumberMatching) :
_address(address),
_allowLenientHouseNumberMatching(allowLenientHouseNumberMatching),
_parsedFromAddressTag(true)
{
}

bool Address::operator==(const Address& address) const
{
  LOG_VART(_address);
  LOG_VART(address._address);

  return
    !_address.isEmpty() &&
      (_addrComp.compare(_address, address._address) == 1.0 ||
       (_allowLenientHouseNumberMatching &&
        !isIntersectionAddress(_address, !_parsedFromAddressTag) &&
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

bool Address::isIntersectionAddress(const QString& addressStr,
                                    const bool requireStreetTypeInIntersection)
{
  if (addressStr.trimmed().isEmpty())
  {
    return false;
  }

  // libpostal doesn't recognize intersections correctly, so doing it with very simple custom logic

  if (requireStreetTypeInIntersection &&
      !StringUtils::endsWithAny(addressStr, getStreetTypes().toList()))
  {
    return false;
  }

  return StringUtils::bisectsAny(addressStr, getIntersectionSplitTokens());
}

bool Address::isIntersectionAddress(const Address& address,
                                    const bool requireStreetTypeInIntersection)
{
  return isIntersectionAddress(address._address, requireStreetTypeInIntersection);
}

void Address::removeStreetTypes()
{
  LOG_TRACE(_address);

  if (!isIntersectionAddress(_address, !_parsedFromAddressTag))
  {
    StringUtils::removeLastIndexOf(_address, _streetTypes.toList());
  }
  else
  {
    QStringList addressParts = StringUtils::splitOnAny(_address, getIntersectionSplitTokens(), 2);
    assert(addressParts.size() == 2);
    const QStringList streetTypes = getStreetTypes().toList();

    QString firstIntersectionPart = addressParts[0].trimmed();
    StringUtils::removeLastIndexOf(firstIntersectionPart, streetTypes);
    QString secondIntersectionPart = addressParts[1].trimmed();
    StringUtils::removeLastIndexOf(secondIntersectionPart, streetTypes);

    _address = firstIntersectionPart.trimmed() + " and " + secondIntersectionPart.trimmed();
  }

  LOG_TRACE(_address);
}

}
