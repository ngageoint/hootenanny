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
#include "PoiPolygonAdvancedMatcher.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/util/TopologyException.h>
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

PoiPolygonAdvancedMatcher::PoiPolygonAdvancedMatcher(const ConstOsmMapPtr& map,
                                                     const set<ElementId>& polyNeighborIds,
                                                     const set<ElementId>& poiNeighborIds,
                                                     double distance) :
_map(map),
_polyNeighborIds(polyNeighborIds),
_poiNeighborIds(poiNeighborIds),
_distance(distance),
_badGeomCount(0)
{
}

void PoiPolygonAdvancedMatcher::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

bool PoiPolygonAdvancedMatcher::triggersRule(ConstElementPtr poi, ConstElementPtr poly)
{
  //to suppress the ElementConverter poly warnings...warnings worth looking into at some point
  DisableLog dl(Log::Warn);

  ElementConverter elementConverter(_map);
  std::shared_ptr<Geometry> polyGeom = elementConverter.convertToGeometry(poly);
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    throw geos::util::TopologyException();
  }

  const QString poiAddress =
    AddressTagKeys::getInstance()->getAddressTagValue(poi->getTags(), "full_address");
  if (poiAddress.isEmpty())
  {
    return false;
  }
  for (set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
       poiNeighborItr != _poiNeighborIds.end(); ++poiNeighborItr)
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      try
      {
        const QString poiNeighborName = poiNeighbor->getTags().getName();
        if (!poiNeighborName.isEmpty() && poiNeighborName == poiAddress)
        {
          std::shared_ptr<Geometry> poiNeighborGeom =
            ElementConverter(_map).convertToGeometry(poiNeighbor);
          //If a building in the area contains a another POI with a matching address to the
          //POI being examined, increase the evidence.
          if (polyGeom->contains(poiNeighborGeom.get()))
          {
            LOG_TRACE("Found evidence per rule #1...");
            return true;
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= Log::getWarnMessageLimit())
        {
          LOG_TRACE(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << poiNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
  }

  return false;
}

}
