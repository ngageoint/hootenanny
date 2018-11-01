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
#include <hoot/core/algorithms/string/MostEnglishName.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/Address.h>
#include <hoot/core/util/StringUtils.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonAddressScoreExtractor)

PoiPolygonAddressScoreExtractor::PoiPolygonAddressScoreExtractor() :
_addressesProcessed(0),
_matchAttemptMade(false)
{
}

void PoiPolygonAddressScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  _addressParser.setConfiguration(conf);
  _addressParser.setAllowLenientHouseNumberMatching(
    config.getPoiPolygonAddressAllowLenientHouseNumberMatching());
  _addressParser.setAdditionalTagKeys(
    QSet<QString>::fromList(config.getPoiPolygonAddressAdditionalTagKeys()));

  bool preTranslateTagValuesToEnglish = config.getPoiPolygonAddressTranslateToEnglish();
  // The default translation is what libpostal does during normalization and is always done.  We're
  // providing an option to some additional translation before normalization with a
  // ToEnglishTranslator.  If the default only option is enabled, it overrides any use of the
  // ToEnglishTranslator.
  if (config.getPoiPolygonAddressUseDefaultLanguageTranslationOnly())
  {
    preTranslateTagValuesToEnglish = false;
  }
  _addressParser.setPreTranslateTagValuesToEnglish(preTranslateTagValuesToEnglish, conf);
}

double PoiPolygonAddressScoreExtractor::extract(const OsmMap& map, const ConstElementPtr& poi,
                                                const ConstElementPtr& poly) const
{
  //Experimented with partial addresses matches in the past and it had no positive affect.  Search
  //the history for this class to see examples, to see if its worth experimenting with again at
  //some point.

  //see if the poly has any address
  LOG_TRACE("Collecting addresses from polygon...");
  LOG_VART(poly);
  QList<Address> polyAddresses = _addressParser.parseAddresses(*poly);
  if (polyAddresses.size() == 0)
  {
    //if not, try to find the address from a poly way node instead
    if (poly->getElementType() == ElementType::Way)
    {
      ConstWayPtr wayPoly = boost::dynamic_pointer_cast<const Way>(poly);
      polyAddresses = _addressParser.parseAddressesFromWayNodes(*wayPoly, map, poi->getElementId());
    }
    //if still no luck, try to find the address from a poly way node that is a relation member
    else if (poly->getElementType() == ElementType::Relation)
    {
      ConstRelationPtr relationPoly = boost::dynamic_pointer_cast<const Relation>(poly);
      polyAddresses =
        _addressParser.parseAddressesFromRelationMembers(*relationPoly, map, poi->getElementId());
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
  const QList<Address> poiAddresses = _addressParser.parseAddresses(*poi);
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

}
