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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonAddressScoreExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/Address.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressScoreExtractor)

PoiPolygonAddressScoreExtractor::PoiPolygonAddressScoreExtractor() :
_translateTagValuesToEnglish(false),
_addressesProcessed(0),
_matchAttemptMade(false)
{
}

void PoiPolygonAddressScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  _allowLenientHouseNumberMatching = config.getPoiPolygonAddressAllowLenientHouseNumberMatching();
  _addressParser.setConfiguration(conf);
  _addressParser.setAdditionalTagKeys(
    QSet<QString>::fromList(config.getPoiPolygonAddressAdditionalTagKeys()));

  _translateTagValuesToEnglish = config.getPoiPolygonAddressTranslateToEnglish();
  // The default translation is what libpostal does during normalization and is always done.  We're
  // providing an option to some additional translation before normalization with a
  // ToEnglishTranslator.  If the default only option is enabled, it overrides any use of the
  // ToEnglishTranslator.
  if (config.getPoiPolygonAddressUseDefaultLanguageTranslationOnly())
  {
    _translateTagValuesToEnglish = false;
  }
  if (_translateTagValuesToEnglish)
  {
    _addressTranslator.setConfiguration(conf);
  }
}

double PoiPolygonAddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& poi,
                                                const ConstElementPtr& poly) const
{
  //Experimented with partial addresses matches in the past and it had no positive affect.  Search
  //the history for this class to see examples, to see if its worth experimenting with again at
  //some point.

  QList<Address> polyAddresses;
  //see if the poly has any address
  LOG_TRACE("Collecting addresses from polygon...");
  LOG_VART(poly);
  _collectAddressesFromElement(*poly, polyAddresses);
  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayPoly = boost::dynamic_pointer_cast<const Way>(poly);
      _collectAddressesFromWayNodes(*wayPoly, polyAddresses, map, poi->getElementId());
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      ConstRelationPtr relationPoly = boost::dynamic_pointer_cast<const Relation>(poly);
      _collectAddressesFromRelationMembers(*relationPoly, polyAddresses, map, poi->getElementId());
    }
  }
  LOG_VART(polyAddresses.size());
  if (polyAddresses.size() == 0)
  {
    LOG_TRACE("No poly addresses.");
    return 0.0;
  }

  //see if the poi has an address
  LOG_TRACE("Collecting addresses from POI...");
  LOG_VART(poi);
  QList<Address> poiAddresses;
  _collectAddressesFromElement(*poi, poiAddresses);
  LOG_VART(poiAddresses.size());
  if (poiAddresses.size() == 0)
  {
    LOG_TRACE("No POI addresses.");
    return 0.0;
  }

  _matchAttemptMade = true;
  _addressesProcessed += poiAddresses.size();
  _addressesProcessed += polyAddresses.size();

  //check for address matches
  for (QList<Address>::const_iterator polyAddrItr = polyAddresses.begin();
       polyAddrItr != polyAddresses.end(); ++polyAddrItr)
  {
    const Address polyAddress = *polyAddrItr;
    for (QList<Address>::const_iterator poiAddrItr = poiAddresses.begin();
         poiAddrItr != poiAddresses.end(); ++poiAddrItr)
    {
      const Address poiAddress = *poiAddrItr;
      if (polyAddress == poiAddress)
      {
        LOG_TRACE("Found address match.");
        return 1.0;
      }
    }
  }

  return 0.0;
}

bool PoiPolygonAddressScoreExtractor::nodeHasAddress(const Node& node)
{
  PoiPolygonAddressScoreExtractor addressExtractor;
  // We're just getting a count here, so translation isn't needed (can be expensive).
  addressExtractor._translateTagValuesToEnglish = false;
  QList<Address> addresses;
  addressExtractor._collectAddressesFromElement(node, addresses);
  return addresses.size() > 0;
}

bool PoiPolygonAddressScoreExtractor::elementHasAddress(const ConstElementPtr& element,
                                                        const OsmMap& map)
{
  PoiPolygonAddressScoreExtractor addressExtractor;
  // We're just getting a count here, so translation isn't needed (can be expensive).
  addressExtractor._translateTagValuesToEnglish = false;
  QList<Address> addresses;
  if (element->getElementType() == ElementType::Node)
  {
    return nodeHasAddress(*boost::dynamic_pointer_cast<const Node>(element));
  }
  else if (element->getElementType() == ElementType::Way)
  {
    addressExtractor._collectAddressesFromWayNodes(
      *boost::dynamic_pointer_cast<const Way>(element), addresses, map);
  }
  else if (element->getElementType() == ElementType::Relation)
  {
    addressExtractor._collectAddressesFromRelationMembers(
      *boost::dynamic_pointer_cast<const Relation>(element), addresses, map);
  }
  return addresses.size() > 0;
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromElement(const Element& element,
                                                                   QList<Address>& addresses) const
{
  QString houseNum;
  QString street;
  const QSet<QString> parsedAddresses = _addressParser.parseAddresses(element, houseNum, street);
  LOG_VART(parsedAddresses);

  // add the parsed addresses to a collection in which they will later be compared to each other
  for (QSet<QString>::const_iterator parsedAddressItr = parsedAddresses.begin();
       parsedAddressItr != parsedAddresses.end(); ++parsedAddressItr)
  {
    QString parsedAddress = *parsedAddressItr;
    LOG_VART(parsedAddress);

    //optional additional lang pre-normalization translation
    if (_translateTagValuesToEnglish)
    {
      //only translating the street portion of the address string...the number isn't translatable
       assert(!street.isEmpty());
      QString preTranslatedStreet = street;
      _addressTranslator.translateAddressToEnglish(preTranslatedStreet);
      parsedAddress = houseNum + " " + preTranslatedStreet;
    }
    LOG_VART(parsedAddress);

    //normalize and translate the address strings, so we end up comparing apples to apples
    const QSet<QString> normalizedAddresses = _addressParser.normalizeAddress(parsedAddress);
    LOG_VART(normalizedAddresses);

    for (QSet<QString>::const_iterator normalizedAddressItr = normalizedAddresses.begin();
         normalizedAddressItr != normalizedAddresses.end(); ++normalizedAddressItr)
    {
      const QString normalizedAddress = *normalizedAddressItr;
      LOG_VART(normalizedAddress);
      Address address(normalizedAddress, _allowLenientHouseNumberMatching);
      if (!addresses.contains(address))
      {
        LOG_TRACE("Adding address: " << address);
        addresses.append(address);
      }
    }
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromWayNodes(const Way& way,
                                                                    QList<Address>& addresses,
                                                                    const OsmMap& map,
                                                                    const ElementId& poiId) const
{
  LOG_TRACE("Collecting addresses from way nodes...");
  const std::vector<long> wayNodeIds = way.getNodeIds();
  for (size_t i = 0; i < wayNodeIds.size(); i++)
  {
    ConstElementPtr wayNode = map.getElement(ElementType::Node, wayNodeIds.at(i));
    // see explanation for this check in _collectAddressesFromRelationMembers
    if (poiId.isNull() || wayNode->getElementId() != poiId)
    {
      _collectAddressesFromElement(*wayNode, addresses);
    }
  }
}

void PoiPolygonAddressScoreExtractor::_collectAddressesFromRelationMembers(const Relation& relation,
                                                                          QList<Address>& addresses,
                                                                           const OsmMap& map,
                                                                       const ElementId& poiId) const
{
  LOG_TRACE("Collecting addresses from relation members...");
  const std::vector<RelationData::Entry> relationMembers = relation.getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    ConstElementPtr member = map.getElement(relationMembers[i].getElementId());
    // If the poly contains the poi being compared to as a way node or relation member, then the
    // POI's address will be added to both the poly and poi group of addresses and yield a fake
    // address match
    if (poiId.isNull() || member->getElementId() != poiId)
    {
      if (member->getElementType() == ElementType::Node)
      {
        _collectAddressesFromElement(*member, addresses);
      }
      else if (member->getElementType() == ElementType::Way)
      {
        ConstWayPtr wayMember = boost::dynamic_pointer_cast<const Way>(member);
        _collectAddressesFromWayNodes(*wayMember, addresses, map, poiId);
      }
    }
  }
}

}
