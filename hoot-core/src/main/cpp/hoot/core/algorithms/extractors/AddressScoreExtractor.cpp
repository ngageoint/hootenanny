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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "AddressScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/conflate/address/Address.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

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
    const int maxCacheSize = config.getConflateInfoMaxSizePerCache();
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

double AddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& element1,
                                      const ConstElementPtr& element2) const
{
  // Experimented with partial addresses matches in the past and it had no positive affect. Search
  // the history for this class to see examples, to see if its worth experimenting with again at
  // some point.

  // see if the first element has any address
  const QList<Address> element1Addresses = _getElementAddresses(map, element1, element2);
  LOG_VART(element1Addresses.size());
  if (element1Addresses.empty())
  {
    LOG_TRACE("No element 1 addresses.");
    return -1.0;
  }

  // see if the second element has an address
  const QList<Address> element2Addresses = _getElementAddresses(map, element2, element1);
  LOG_VART(element2Addresses.size());
  if (element2Addresses.empty())
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

      const double partialMatchScore = _getPartialMatchScore(element1Address, element2Address);
      if (partialMatchScore > 0.0)
      {
        return partialMatchScore;
      }
    }
  }

  return 0.0;
}

QList<Address> AddressScoreExtractor::_getElementAddresses(
  const OsmMap& map, const ConstElementPtr& element,
  const ConstElementPtr& elementBeingComparedWith) const
{
  LOG_TRACE("Collecting addresses from: " << element->getElementId() << "...");

  if (_cacheEnabled)
  {
    const QList<Address>* cachedVal = _addressesCache[element->getElementId()];
    if (cachedVal != nullptr)
    {
      LOG_TRACE("Found cached address(es): " << *cachedVal);
      _addressCacheHits++;
      return *cachedVal;
    }
  }

  QList<Address> elementAddresses = _addressParser.parseAddresses(*element);
  if (elementAddresses.empty())
  {
    //if not, try to find the address from a poly way node instead
    if (element->getElementType() == ElementType::Way)
    {
      ConstWayPtr way = std::dynamic_pointer_cast<const Way>(element);
      elementAddresses =
        _addressParser.parseAddressesFromWayNodes(
          *way, map, elementBeingComparedWith->getElementId());
      if (!elementAddresses.empty())
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
      if (!elementAddresses.empty())
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

bool AddressScoreExtractor::_addressesMatchWithSuffixesRemoved(
  const Address& address1, const Address& address2) const
{
  LOG_TRACE("Attempting intersection match or partial street match without suffix...");

  Address elementAddress1Temp = address1;
  Address elementAddress2Temp = address2;
  elementAddress1Temp.removeStreetTypes();
  elementAddress2Temp.removeStreetTypes();
  LOG_VART(elementAddress1Temp.getAddressStr());
  LOG_VART(elementAddress2Temp.getAddressStr());

  return elementAddress2Temp == elementAddress1Temp;
}

bool AddressScoreExtractor::_intersectionAndStreetAddressesMatchWithHouseNumbersRemoved(
  const Address& address1, const Address& address2) const
{
  LOG_TRACE("Attempting street/intersection partial match without house number...");

  const bool element1IsIntersection = Address::isStreetIntersectionAddress(address1);
  const bool element2IsIntersection = Address::isStreetIntersectionAddress(address2);
  Address elementAddress1Temp = address1;
  Address elementAddress2Temp = address2;

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
      return true;
    }
  }
  return false;
}

bool AddressScoreExtractor::_addressesMatchWithNameComparisonRelaxed(
  const Address& address1, const Address& address2) const
{
   LOG_TRACE("Attempting street partial match with looser street name comparison...");

   Address elementAddress1Temp = address1;
   Address elementAddress2Temp = address2;
   elementAddress1Temp.removeStreetTypes();
   elementAddress2Temp.removeStreetTypes();

   elementAddress1Temp.removeHouseNumber();
   elementAddress2Temp.removeHouseNumber();
   LOG_VART(elementAddress1Temp.getAddressStr());
   LOG_VART(elementAddress2Temp.getAddressStr());

   MeanWordSetDistance stringComp(
     std::make_shared<LevenshteinDistance>(ConfigOptions().getLevenshteinDistanceAlpha()));
   const double stringSim =
     stringComp.compare(elementAddress1Temp.getAddressStr(), elementAddress2Temp.getAddressStr());
   LOG_VART(stringSim);
   return stringSim >= 0.8; // TODO: tie this to a config var?
}

double AddressScoreExtractor::_getPartialMatchScore(const Address& address1,
                                                    const Address& address2) const
{
  const bool element1IsIntersection = Address::isStreetIntersectionAddress(address1);
  const bool element2IsIntersection = Address::isStreetIntersectionAddress(address2);
  const bool onlyOneIsIntersection =
    (element1IsIntersection && !element2IsIntersection) ||
    (!element1IsIntersection && element2IsIntersection);
  LOG_VART(onlyOneIsIntersection);

  // These partial matches (except for the first one) are getting a fairly arbitrary scores, which
  // could be tweaked going forward. Currently, address partial match scoring is primarily being
  // used by POI/Polygon conflation to prevent removing reviews for features that have addresses
  // with some similarity.

  // remove the street types (suffixes) from each and see if we have an address string match
  if (_addressesMatchWithSuffixesRemoved(address1, address2))
  {
    // If both addresses being compared are intersections and possibly one has street types
    // in one or both of its intersection parts and the other doesn't, let's try dropping
    // all street type tokens and comparing the address strings again.
    if (address1.getParsedFromAddressTag() &&
        address2.getParsedFromAddressTag() &&
        element1IsIntersection && element2IsIntersection)
    {
      LOG_TRACE(
        "Found address intersection match after removing suffixes. 1: " <<
        address1 << ", 2: " << address2);
      // arguably this could be made into a partial match score like it is for non-intersections
      return 1.0;
    }
    else
    {
      LOG_TRACE(
        "Found partial address match after removing suffixes. 1: " << address1 <<
         ", 2: " << address2);
      return 0.8;
    }
  }

  // remove the house numbers from each and see if we have an address string match; only do it if
  // one of them is an intersection
  if (onlyOneIsIntersection &&
      _intersectionAndStreetAddressesMatchWithHouseNumbersRemoved(
        address1, address2))
  {
    LOG_TRACE(
      "Found partial address intersection/street address match: " << address1 <<
      ", 2: " << address2);
    return 0.8;
  }

  // slight street name misspelling but everything else matches; only do it with basic street
  // addresses, no intersections, house number ranges or subletters
  if (!element1IsIntersection && !element2IsIntersection &&
      !address1.getIsSubLetter() && !address2.getIsSubLetter() &&
      !address1.getIsRange() && !address2.getIsRange() &&
      _addressesMatchWithNameComparisonRelaxed(address1, address2))
  {
    LOG_TRACE(
      "Found partial address match based on string similarity. 1: " <<
      address1 << ", 2: " << address2);
    return 0.8;
  }

  return 0.0;
}

}
