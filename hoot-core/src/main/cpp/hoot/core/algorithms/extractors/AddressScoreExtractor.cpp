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
      LOG_TRACE("Found cached address.");
      _addressCacheHits++;
      return *cachedVal;
    }
  }

  //LOG_VART(element);
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
          "Found " << elementAddresses.size() << " addresses on the way nodes of " <<
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
          "Found " << elementAddresses.size() << " addresses on the relation members of " <<
          element->getElementId());
      }
    }
  }
  else
  {
    LOG_TRACE("Found " << elementAddresses.size() << " addresses on " << element->getElementId());
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
  // Experimented with partial addresses matches in the past and it had no positive affect.  Search
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
        LOG_TRACE("Found address match.");
        return 1.0;
      }
      // This is kind of a last ditch effort to get an street intersection match (may be a better
      // way to do it or a better place to put this code). If both addresses being compared are
      // intersections and possibly one has street types in one or both of its intersection parts
      // and the other doesn't, let's try dropping all street type tokens and comparing the address
      // strings again.
      else if (element1Address.getParsedFromAddressTag() &&
               element2Address.getParsedFromAddressTag() &&
               Address::isStreetIntersectionAddress(element1Address) &&
               Address::isStreetIntersectionAddress(element2Address))
      {
        element1Address.removeStreetTypes();
        element2Address.removeStreetTypes();
        if (element2Address == element1Address)
        {
          LOG_TRACE("Found address match.");
          return 1.0;
        }
      }
    }
  }

  return 0.0;
}

}
