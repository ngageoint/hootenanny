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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonAdvancedMatcher.h"

// geos
#include <geos/geom/LineString.h>
#include <geos/util/TopologyException.h>
#include <geos/geom/Geometry.h>

// hoot
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/algorithms/Translator.h>

#include "extractors/PoiPolygonTypeScoreExtractor.h"
#include "extractors/PoiPolygonNameScoreExtractor.h"
#include "extractors/PoiPolygonAddressScoreExtractor.h"

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
  //TODO: can probably get rid of this list and make the logic work against all landuse
  _genericLandUseTagVals.append("cemetery");
  _genericLandUseTagVals.append("commercial");
  _genericLandUseTagVals.append("construction");
  _genericLandUseTagVals.append("farm");
  _genericLandUseTagVals.append("forest");
  _genericLandUseTagVals.append("grass");
  _genericLandUseTagVals.append("industrial");
  _genericLandUseTagVals.append("meadow");
  _genericLandUseTagVals.append("residential");
  _genericLandUseTagVals.append("retail");
  _genericLandUseTagVals.append("village_green");
}

bool PoiPolygonAdvancedMatcher::triggersRule(ConstElementPtr poi, ConstElementPtr poly)
{
  //to suppress the ElementConverter poly warnings...warnings worth looking into at some point
  DisableLog dl(Log::Warn);

  ElementConverter elementConverter(_map);
  shared_ptr<Geometry> polyGeom = elementConverter.convertToGeometry(poly);
  if (QString::fromStdString(polyGeom->toString()).toUpper().contains("EMPTY"))
  {
    throw geos::util::TopologyException();
  }
  //shared_ptr<Geometry> poiGeom = elementConverter.convertToGeometry(poi);

  //bool poiContainedInAnotherParkPoly = false;
  //bool polyContainedInAnotherParkPoly = false;
  //the name translations are very expensive here, so are disabled
//  QString polyName = PoiPolygonNameScoreExtractor::getElementName(poly);
//  if (!polyName.isEmpty())
//  {
//    polyName = Translator::getInstance().toEnglish(polyName);
//  }
  //  QString poiName = PoiPolygonNameScoreExtractor::getElementName(poi);
  //  if (!poiName.isEmpty())
  //  {
  //    poiName = Translator::getInstance().toEnglish(poiName);
  //  }
//  const bool polyHasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(poly);
//  const bool polyHasType = PoiPolygonTypeScoreExtractor::hasType(poly);
//  bool polyHasSpecificType = polyHasType;
//  if ((poly->getTags().get("building") == "yes" || poly->getTags().get("poi") == "yes") &&
//      !polyHasMoreThanOneType)
//  {
//    polyHasSpecificType = false;
//  }

//  set<ElementId>::const_iterator polyNeighborItr = _polyNeighborIds.begin();
//  while (polyNeighborItr != _polyNeighborIds.end())
//  {
//    ConstElementPtr polyNeighbor = _map->getElement(*polyNeighborItr);
//    if (polyNeighbor->getElementId() != poly->getElementId())
//    {
//      shared_ptr<Geometry> polyNeighborGeom;
//      try
//      {
//        polyNeighborGeom = ElementConverter(_map).convertToGeometry(polyNeighbor);

//        if (polyNeighborGeom.get() &&
//            QString::fromStdString(polyNeighborGeom->toString()).toUpper().contains("EMPTY"))
//        {
//          if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
//          {
//            LOG_TRACE(
//              "Invalid area neighbor polygon passed to PoiPolygonMatchCreator: " <<
//              polyNeighborGeom->toString());
//            _badGeomCount++;
//          }
//        }
//        else if (polyNeighborGeom.get())
//        {
//          if (PoiPolygonTypeScoreExtractor::isPark(polyNeighbor))
//          {
//            if (polyNeighborGeom->contains(poiGeom.get()))
//            {
//              poiContainedInAnotherParkPoly = true;

//              LOG_TRACE(
//                    "poi examined and found to be contained within a park poly outside of this match " <<
//                    "comparison: " << poi->toString());
//              LOG_TRACE("park poly it is very close to: " << polyNeighbor->toString());
//            }

//            if (polyNeighborGeom->contains(polyGeom.get()))
//            {
//              //TODO: probably need to be specific that the poi and the poly are in the same park...
//              polyContainedInAnotherParkPoly = true;
//            }
//          }
//        }
//      }
//      catch (const geos::util::TopologyException& e)
//      {
//        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
//        {
//          LOG_TRACE(
//            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
//            "geometry: " << polyNeighbor->toString() << "\n" << e.what());
//          _badGeomCount++;
//        }
//      }
//    }
//    polyNeighborItr++;
//  }

  const QString poiAddress =
    poi->getTags().get(PoiPolygonAddressScoreExtractor::FULL_ADDRESS_TAG_NAME).toLower().trimmed();
  if (poiAddress.isEmpty())
  {
    return false;
  }
  set<ElementId>::const_iterator poiNeighborItr = _poiNeighborIds.begin();
  while (poiNeighborItr != _poiNeighborIds.end())
  {
    ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
    if (poiNeighbor->getElementId() != poi->getElementId())
    {
      try
      {
        const QString poiNeighborName = PoiPolygonNameScoreExtractor::getElementName(poiNeighbor);
        if (!poiNeighborName.isEmpty() && poiNeighborName == poiAddress)
        {
          shared_ptr<Geometry> poiNeighborGeom =
            ElementConverter(_map).convertToGeometry(poiNeighbor);
          //If a building in the area contains a another POI with a matching address to the
          //POI being examined, increase the evidence.
          if (polyGeom->contains(poiNeighborGeom.get()))
          {
            LOG_TRACE("Found evidence per rule #2...");
            return true;
          }
        }
      }
      catch (const geos::util::TopologyException& e)
      {
        if (_badGeomCount <= ConfigOptions().getOgrLogLimit())
        {
          LOG_TRACE(
            "Feature passed to PoiPolygonMatchCreator caused topology exception on conversion to a " <<
            "geometry: " << poiNeighbor->toString() << "\n" << e.what());
          _badGeomCount++;
        }
      }
    }
    poiNeighborItr++;
  }

  return false;
}

}