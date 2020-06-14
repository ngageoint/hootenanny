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
#ifndef ADDRESS_PARSER_H
#define ADDRESS_PARSER_H

// hoot
#include <hoot/core/util/Configurable.h>
#include <hoot/core/language/ToEnglishAddressTranslator.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/address/AddressNormalizer.h>
#include <hoot/core/conflate/address/AddressTagKeys.h>

// Qt
#include <QSet>
#include <QList>

namespace hoot
{

class Tags;
class Way;
class OsmMap;
class Address;
class Node;
class ElementId;
class Relation;

/**
 * Parses element address tags into addresses
 *
 * Address normalization and translation is handled by libpostal, which uses ML trained on OSM
 * data.  There is also an option to do language pre-translation with a custom translator.
 */
class AddressParser : public Configurable
{
public:

  AddressParser();

  virtual void setConfiguration(const Settings& conf);

  /**
   * Parse all addresses from an element
   *
   * @param element the element to parse addresses from
   * @return a collection of addresses
   */
  QList<Address> parseAddresses(const Element& element, const bool normalizeAddresses = true) const;

  /**
   * Parse all addresses from a way's nodes
   *
   * @param way the way to parse addresses from
   * @param map the map the way belongs to
   * @param skipElementId no addresses from elements with this ID will be parsed
   * @return a collection of addresses
   */
  QList<Address> parseAddressesFromWayNodes(const Way& way, const OsmMap& map,
                                            const ElementId& skipElementId = ElementId()) const;

  /**
   * Parses all addresses from a relation's members
   *
   * @param relation the relation to parse addresses from
   * @param map the map the relation belongs to
   * @param skipElementId no addresses from elements with this ID will be parsed
   * @return a collection of addresses
   */
  QList<Address> parseAddressesFromRelationMembers(const Relation& relation, const OsmMap& map,
                                                   const ElementId& skipElementId = ElementId()) const;

  /**
   * Determines if an element has an address
   *
   * @param element the element to examine for an address
   * @return true if the element has an address; false otherwise
   * @note This could be made more efficient, if necessary, by only counting the first address.
   */
  bool hasAddress(const Element& element) const;

  /**
   * Counts the number of address an element contains
   *
   * @param element the element to examine for an address
   * @return the number of addresses the element contains
   */
  int numAddresses(const Element& element) const;

  /**
   * Determines if an element and any elements it contains has an address
   *
   * @param element the element to examine for an address
   * @param map map the element being examined belongs to
   * @return true if the element or its children have an address; false otherwise
   * @note This could be made more efficient, if necessary, by only counting the first address.
   */
  bool hasAddressRecursive(const ConstElementPtr& element, const OsmMap& map) const;

  /**
   * Counts the number of address an element contains, as well as the addresses of any contained
   * elements
   *
   * @param element the element to examine for addresses
   * @param map map the element being examined belongs to
   * @return the number of addresses the element and its children contain
   */
  int numAddressesRecursive(const ConstElementPtr& element, const OsmMap& map) const;

  /**
   * Determines whether two addresses match despite house number subletter differences
   *
   * So, allow "34 elm street" to match "34a elm street".
   *
   * @param address1 first address to compare
   * @param address2 second address to compare
   * @return true if the address match despite subletter differences; false otherwise
   */
  static bool addressesMatchDespiteSubletterDiffs(const QString& address1, const QString& address2);

  void setPreTranslateTagValuesToEnglish(bool translate, const Settings& conf);
  void setAllowLenientHouseNumberMatching(bool allow) { _allowLenientHouseNumberMatching = allow; }

private:

  // see addressesMatchDespiteSubletterDiffs
  bool _allowLenientHouseNumberMatching;
  //when enabled, will attempt to translate address tags to English with a translator other than
  //the one built into libpostal before address normalization
  mutable bool _preTranslateTagValuesToEnglish;
  // is the current address being parsed from an OSM address tag or some other auxiliary tag (name,
  // etc.)?
  mutable bool _parsedFromAddressTag;

  ToEnglishAddressTranslator _addressTranslator;
  AddressNormalizer _addressNormalizer;

  QSet<QString> _parseAddresses(const Element& element, QString& houseNum, QString& street) const;

  QSet<QString> _parseAddressAsRange(const QString& houseNum, const QString& street) const;
  bool _isRangeAddress(const QString& houseNum) const;
  bool _isParseableAddressFromComponents(const Tags& tags, QString& houseNum,
                                         QString& street) const;
  bool _isValidAddressStr(QString& address, QString& houseNum,  QString& street,
                          const bool requireStreetTypeInIntersection = false) const;
  /*
   * Parses tags which contain a complete address string
   */
  QString _parseFullAddress(const QString& fullAddress, QString& houseNum, QString& street) const;
  /*
   * Parses tags where multiple tags are combined to make a complete address
   */
  QSet<QString> _parseAddressFromComponents(const Tags& tags, QString& houseNum,
                                            QString& street) const;
  /*
   * Parses address from tags specified in the additional tag keys
   */
  QString _parseAddressFromAltTags(const Tags& tags, QString& houseNum, QString& street) const;

  static QString _getSubLetterCleanedAddress(const QString& address);
};

}

#endif // ADDRESS_PARSER_H
