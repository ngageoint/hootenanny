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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef ADDRESSS_H
#define ADDRESSS_H

// Hoot
#include <hoot/core/algorithms/string/StringDistance.h>

namespace hoot
{

/**
 * @brief The Address class encapsulates a street address for conflation purposes.
 *
 * Note that a QMap can be used to represent the street type full names and their abbreviations
 * b/c we currently only support a one to one mapping. If we expand it to support multiple
 * abbreviations per full name, then we'll need to switch to a QMultiMap.
 */
class Address
{

public:

  Address();
  Address(const QString& address, const bool allowLenientHouseNumberMatching = true);

  bool operator==(const Address& address) const;

  QString toString() const { return "Address: " + _address; }

  /**
   * @brief isStreetIntersectionAddress Determines if an address string represents a street
   * intersection.
   * @param addressStr address to examine
   * @param requireStreetTypeInIntersection if true, a street type token must be present at the end
   * of the input
   * @return true if the input represents a street intersection address; false otherwise
   */
  static bool isStreetIntersectionAddress(const QString& addressStr,
                                          const bool requireStreetTypeInIntersection = false);
  /**
   * @brief isStreetIntersectionAddress determines if an address represents a street intersection.
   * @param address address to examine
   * @param requireStreetTypeInIntersection if true, a street type token must be present at the end
   * of the input
   * @return true if the input represents a street intersection address; false otherwise
   */
  static bool isStreetIntersectionAddress(const Address& address,
                                          const bool requireStreetTypeInIntersection = false);

  /**
   * @brief getStreetTypes returns a collection of street type (suffix) tokens; e.g. "Street".
   * @param includeAbbreviations if true, abbreviations for the tokens are also included
   * @return a collection of string tokens
   */
  static QSet<QString> getStreetTypes(const bool includeAbbreviations = true);
  /**
   * @brief getStreetFullTypesToTypeAbbreviations returns a mapping of street type (suffix) tokens
   * mapped to accepted abbreviations for them; e.g. "Street" --> "St"
   * @return a mapping of string tokens
   */
  static QMap<QString, QString> getStreetFullTypesToTypeAbbreviations();
  /**
   * @brief getStreetTypeAbbreviationsToFullTypes returns a mapping of street type (suffix)
   * abbreviation tokens mapped to accepted full names for them; e.g "St" --> "Street"
   * @return a mapping of string tokens
   */
  static QMap<QString, QString> getStreetTypeAbbreviationsToFullTypes();

  /**
   * @brief getIntersectionSplitTokens returns a collection of string tokens used to separate the
   * two parts of an intersection address.
   * @return a collection of string tokens
   */
  static QList<QRegExp> getIntersectionSplitTokens();
  /**
   * @brief getIntersectionParts returns the intersection parts of the address.
   * @return a string list with two entries for the intersection parts if the address is an
   * intersection address; an empty list otherwise
   */
  QStringList getIntersectionParts() const;

  /**
   * @brief removeStreetTypes removes street type (suffix) text from the address.
   */
  void removeStreetTypes();
  /**
   * @brief removeHouseNumber removes the house number from the address if its not an intersection
   * address.
   */
  void removeHouseNumber();

  bool getParsedFromAddressTag() const { return _parsedFromAddressTag; }
  QString getAddressStr() const { return _address; }
  bool getIsRange() const { return _isRange; }
  bool getIsSubLetter() const { return _isSubLetter; }

  void setParsedFromAddressTag(bool from) { _parsedFromAddressTag = from; }
  void setIsRange(bool isRange) { _isRange = isRange; }
  void setIsSubLetter(bool isSubLetter) { _isSubLetter = isSubLetter; }

  QString getHouseNumber() const;

private:

  QString _address;

  // This has been made configurable, but due to the fact that address strings are usually
  // normalized before being set on this class, its seems unlikely at this point that anything other
  // than ExactStringDistance will be used.
  static StringDistancePtr _stringComp;

  //see AddressParser::addressesMatchDespiteSubletterDiffs
  bool _allowLenientHouseNumberMatching;

  // was the address parsed from an OSM address tag or some other auxiliary tag (name, etc.)?
  bool _parsedFromAddressTag;

  // determines if the address has a house number range; like: 120-130 Sutter St
  bool _isRange;

  // determines if the address has a subletter in the house number; like 120a Sutter St
  bool _isSubLetter;

  // see getStreetTypes
  static QSet<QString> _streetTypes;
  // see getStreetFullTypesToTypeAbbreviations
  static QMap<QString, QString> _streetFullTypesToTypeAbbreviations;
  // see getStreetTypeAbbreviationsToFullTypes
  static QMap<QString, QString> _streetTypeAbbreviationsToFullTypes;

  void _initializeStringComparator() const;
};

}

#endif // POI_POLYGON_ADDRESSS_H
