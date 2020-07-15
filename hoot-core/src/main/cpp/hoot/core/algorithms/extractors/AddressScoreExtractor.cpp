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
#include "AddressScoreExtractor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/address/Address.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>

using namespace std;

namespace hoot
{

QCache<ElementId, QList<Address>> AddressScoreExtractor::_addressesCache(
  AddressScoreExtractor::CACHE_SIZE_DEFAULT);
int AddressScoreExtractor::_addressCacheHits = 0;

HOOT_FACTORY_REGISTER(FeatureExtractor, AddressScoreExtractor)

AddressScoreExtractor::AddressScoreExtractor() :
_addressesProcessed(0),
_matchAttemptMade(false),
_cacheEnabled(true)
{
}

void AddressScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  _addressParser.setAllowLenientHouseNumberMatching(
    config.getAddressAllowLenientHouseNumberMatching());
  _addressParser.setConfiguration(conf);

  bool preTranslateTagValuesToEnglish = config.getAddressTranslateToEnglish();
  // The default translation is what libpostal does during normalization and is always done.  We're
  // providing an option to some additional translation before normalization with a
  // ToEnglishTranslator.  If the default only option is enabled, it overrides any use of the
  // ToEnglishTranslator.
  if (config.getAddressUseDefaultLanguageTranslationOnly())
  {
    preTranslateTagValuesToEnglish = false;
  }
  _addressParser.setPreTranslateTagValuesToEnglish(preTranslateTagValuesToEnglish, conf);

  _cacheEnabled = config.getAddressScorerEnableCaching();
  if (_cacheEnabled)
  {
    // Sharing this setting with POI/Poly's max cache sizes for now.
    const int maxCacheSize = config.getPoiPolygonMaxSizePerCache();
    if (maxCacheSize > 0)
    {
      _addressesCache.setMaxCost(maxCacheSize);
    }
    else
    {
      _cacheEnabled = false;
    }
  }
}

QList<Address> AddressScoreExtractor::_getElementAddresses(
  const OsmMap& map, const ConstElementPtr& element,
  const ConstElementPtr& elementBeingComparedWith) const
{
  LOG_TRACE("Collecting addresses from: " << element->getElementId() << "...");

  if (_cacheEnabled)
  { 
    const QList<Address>* cachedVal = _addressesCache[element->getElementId()];
    if (cachedVal != 0)
    {
      LOG_TRACE("Found cached address(es): " << *cachedVal);
      _addressCacheHits++;
      return *cachedVal;
    }
  }

  QList<Address> elementAddresses = _addressParser.parseAddresses(*element);
  if (elementAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (element->getElementType() == ElementType::Way)
    {
      ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
      elementAddresses =
        _addressParser.parseAddressesFromWayNodes(
          *way, map, elementBeingComparedWith->getElementId());
      if (elementAddresses.size() != 0)
      {
        LOG_TRACE(
          "Found " << elementAddresses.size() << " address(es) on the way nodes of " <<
          element->getElementId());
      }
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (element->getElementType() == ElementType::Relation)
    {
      ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
      elementAddresses =
        _addressParser.parseAddressesFromRelationMembers(
          *relation, map, elementBeingComparedWith->getElementId());
      if (elementAddresses.size() != 0)
      {
        LOG_TRACE(
          "Found " << elementAddresses.size() << " address(es) on the relation members of " <<
          element->getElementId());
      }
    }
  }
  else
  {
    LOG_TRACE(
      "Found " << elementAddresses.size() << " address(es) on " << element->getElementId() <<
      ": " << elementAddresses);
  }

  if (_cacheEnabled)
  {
    _addressesCache.insert(element->getElementId(), new QList<Address>(elementAddresses));
  }

  return elementAddresses;
}

double AddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& element1,
                                      const ConstElementPtr& element2) const
{
  // Experimented with partial addresses matches in the past and it had no positive affect. Search
  // the history for this class to see examples, to see if its worth experimenting with again at
  // some point.

  // see if the first element has any address
  const QList<Address> element1Addresses = _getElementAddresses(map, element1, element2);
  LOG_VART(element1Addresses.size());
  if (element1Addresses.size() == 0)
  {
    LOG_TRACE("No element 1 addresses.");
    return -1.0;
  }

  // see if the second element has an address
  const QList<Address> element2Addresses = _getElementAddresses(map, element2, element1);
  LOG_VART(element2Addresses.size());
  if (element2Addresses.size() == 0)
  {
    LOG_TRACE("No element 2 addresses.");
    return -1.0;
  }

  _matchAttemptMade = true;
  _addressesProcessed += element2Addresses.size();
  _addressesProcessed += element1Addresses.size();

  // check for address matches
  for (QList<Address>::const_iterator element2AddrItr = element2Addresses.begin();
       element2AddrItr != element2Addresses.end(); ++element2AddrItr)
  {
    Address element2Address = *element2AddrItr;
    for (QList<Address>::const_iterator element1AddrItr = element1Addresses.begin();
         element1AddrItr != element1Addresses.end(); ++element1AddrItr)
    {
      Address element1Address = *element1AddrItr;
      if (element2Address == element1Address)
      {
        LOG_TRACE("Found address match: 1: " << element1Address << ", 2: " << element2Address);
        return 1.0;
      }

      // TODO: move everything after here into separate methods

      const bool element1IsIntersection = Address::isStreetIntersectionAddress(element1Address);
      const bool element2IsIntersection = Address::isStreetIntersectionAddress(element2Address);
      const bool element1ParsedFromAddressTag = element1Address.getParsedFromAddressTag();
      const bool element2ParsedFromAddressTag = element2Address.getParsedFromAddressTag();
      LOG_VART(element1IsIntersection);
      LOG_VART(element2IsIntersection);
      LOG_VART(element1ParsedFromAddressTag);
      LOG_VART(element2ParsedFromAddressTag);

      {
        LOG_TRACE("Attempting intersection match or partial street match without suffix...");

        Address elementAddress1Temp = element1Address;
        Address elementAddress2Temp = element2Address;
        elementAddress1Temp.removeStreetTypes();
        elementAddress2Temp.removeStreetTypes();
        LOG_VART(elementAddress1Temp.getAddressStr());
        LOG_VART(elementAddress2Temp.getAddressStr());

        if (elementAddress2Temp == elementAddress1Temp)
        {
          // If both addresses being compared are intersections and possibly one has street types
          // in one or both of its intersection parts and the other doesn't, let's try dropping
          // all street type tokens and comparing the address strings again (may be a better way
          // to do it or a better place to put this in the code).
          if (element1ParsedFromAddressTag && element2ParsedFromAddressTag &&
              element1IsIntersection && element2IsIntersection)
          {
            LOG_TRACE(
              "Found address intersection match after removing suffixes. 1: " <<
              element1Address << ", 2: " << element2Address);
            return 1.0;
          }
          else
          {
            LOG_TRACE(
              "Found partial address match after removing suffixes. 1: " << element1Address <<
               ", 2: " << element2Address);
            return 0.8;
          }
        }
      }

      const bool onlyOneIsIntersection =
        (element1IsIntersection && !element2IsIntersection) ||
        (!element1IsIntersection && element2IsIntersection);
      LOG_VART(onlyOneIsIntersection);
      if (onlyOneIsIntersection)
      {
        LOG_TRACE("Attempting street/intersection partial match without house number...");

        Address elementAddress1Temp = element1Address;
        Address elementAddress2Temp = element2Address;

        QStringList intersectionParts;
        if (element1IsIntersection)
        {
          intersectionParts = elementAddress1Temp.getIntersectionParts();
        }
        else if (element2IsIntersection)
        {
          intersectionParts = elementAddress2Temp.getIntersectionParts();
        }
        LOG_VART(intersectionParts);
        QString nonIntersection;
        if (element1IsIntersection)
        {
          elementAddress2Temp.removeHouseNumber();
          nonIntersection = elementAddress2Temp.getAddressStr();
        }
        else
        {
          elementAddress1Temp.removeHouseNumber();
          nonIntersection = elementAddress1Temp.getAddressStr();
        }
        LOG_VART(nonIntersection);
        for (int i = 0; i < intersectionParts.size(); i++)
        {
          LOG_VART(intersectionParts.at(i).trimmed());
          if (nonIntersection == intersectionParts.at(i).trimmed())
          {
            LOG_TRACE(
              "Found partial address intersection/street address match: " << element1Address <<
              ", 2: " << element2Address);
            return 0.8;
          }
        }
      }

      // slight street name misspelling but everything else matches
      if (!element1IsIntersection && !element2IsIntersection &&
          !element1Address.getIsSubLetter() && !element2Address.getIsSubLetter() &&
          !element1Address.getIsRange() && !element2Address.getIsRange())
      {
        LOG_TRACE("Attempting street partial match with looser street name similarity...");

        Address elementAddress1Temp = element1Address;
        Address elementAddress2Temp = element2Address;
        elementAddress1Temp.removeStreetTypes();
        elementAddress2Temp.removeStreetTypes();

        elementAddress1Temp.removeHouseNumber();
        elementAddress2Temp.removeHouseNumber();
        LOG_VART(elementAddress1Temp.getAddressStr());
        LOG_VART(elementAddress2Temp.getAddressStr());

        MeanWordSetDistance stringComp(
          StringDistancePtr(
            new LevenshteinDistance(ConfigOptions().getLevenshteinDistanceAlpha())));
        const double stringSim =
          stringComp.compare(
            elementAddress1Temp.getAddressStr(), elementAddress2Temp.getAddressStr());
        LOG_VART(stringSim);
        if (stringSim >= 0.8)
        {
          LOG_TRACE(
            "Found partial address match based on string similarity. 1: " <<
            element1Address << ", 2: " << element2Address);
          return 0.8;
        }
      }
    }
  }

  return 0.0;
}

}
