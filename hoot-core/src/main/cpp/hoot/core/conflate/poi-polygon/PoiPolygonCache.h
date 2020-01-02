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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POI_POLYGON_CACHE_H
#define POI_POLYGON_CACHE_H

// geos
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

class PoiPolygonCache;

typedef std::shared_ptr<PoiPolygonCache> PoiPolygonCachePtr;

/**
 * Cached back class storing POI/Polygon feature comparison info
 *
 * The caches used in this class were determined on 9/30/18 running against the regression test:
 * unifying-tests.child/somalia.child/somalia-test3.child. Further caching could be deemed necessary
 * for other datasets going forward.
 */
class PoiPolygonCache
{

public:

  PoiPolygonCache(const ConstOsmMapPtr& map);
  ~PoiPolygonCache();

  /**
   * Returns the distance from a point node to a polygon way
   *
   * @param poly the polygon to measure distance from
   * @param point the point to measure distance from
   * @return the distance between the point and polygon or -1.0 if the distance could not be
   * calculated
   * @note The distance calc is not backed by a cache, but the geometries used to derive it are.
   */
  double getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point);

  /**
   * Calculates the area of an element
   *
   * @param element the feature to calculate the area of
   * @return the area of the feature or -1.0 if the area could not be calculated
   * @note The area calc is not backed by a cache, but the geometries used to derive it are.
   */
  double getArea(ConstElementPtr element);

  /**
   * Determines if a polygon contains a point
   *
   * @param poly the polygon to examine
   * @param point the point to examine
   * @return true if the polygon contains the point; false otherwise or if the containment could
   * not be calculated
   */
  bool polyContainsPoi(ConstWayPtr poly, ConstNodePtr point);

  /**
   * Determines if an element intersects another element
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if the two elements intersect; false otherwise or if the intersection could not
   * be calculated
   */
  bool elementIntersectsElement(ConstElementPtr element1, ConstElementPtr element2);

  /**
   * Determines if an element is of the given type
   *
   * @param element the element to determine type membership for
   * @param type the type to determine membership for the element
   * @return true if the element is of the requested type; false otherwise
   * @throws if the requested type is invalid
   * @todo change type to enum
   */
  bool isType(ConstElementPtr element, const QString& type);

  /**
   * Determines if an element has a given criterion
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   */
  bool hasCriterion(ConstElementPtr element, const QString& criterionClassName);

  /**
   * Determines if there exists a neighboring poi that is closer to a specified poly than a
   * specified distance
   *
   * @param polyId element ID of the polygon to examine
   * @param poi the POI to examine
   * @param poiNeighborIds IDs of POI neighbors to the polygon
   * @param poiPolyDistance the pre-calculated distance between the input poi and poly
   * @return true if at least one other POI is closer to the poly with ID=polyId than poi;
   * false otherwise
   * @note The operation may become much more expensive as the search radius is increased.
   */
  bool polyHasPoiNeighborCloserThanPoi(const ElementId& polyId, ConstNodePtr poi,
                                       const std::set<ElementId>& poiNeighborIds,
                                       const double poiPolyDistance);

  /**
   * Determines if there exists a neighboring poly that is closer to a specified poi than a
   * specified distance
   *
   * @param poi the POI to examine
   * @param polyId element ID of the polygon to examine
   * @param polyNeighborIds IDs of polygon neighbors to the POI
   * @param poiPolyDistance the pre-calculated distance between the input poi and poly
   * @return true if at least one other poly is closer to poi than the poly with ID=polyId; false
   * otherwise
   * @note The operation may become much more expensive as the search radius is increased.
   */
  bool poiHasPolyNeighborCloserThanPoly(
    ConstNodePtr poi, const ElementId& polyId, const std::set<ElementId>& polyNeighborIds,
    const double poiPolyDistance);

  /**
   * Clears the contents of the cache
   */
  void clear();

  /**
   * Prints information about the caches used by this class
   */
  void printCacheInfo();

private:

  ConstOsmMapPtr _map;

  int _badGeomCount;

  // Using QHash here instead of QCache, since we're mostly just storing primitives for values and
  // they all take up the same amount of space. Tried to use QCache with the geometries, but since
  // ElementConverter returns a shared pointer and QCache takes ownership, had trouble making it
  // work. Also, not doing any management of the size of these caches, which may eventually end up
  // being needed to control memory usage.

  // ordering of the ID keys doesn't matter here; keys are "elementId1;elementId2"
  QHash<QString, bool> _poiNeighborCloserCache;
  QHash<QString, bool> _polyNeighborCloserCache;

  // ordering of the ID keys does matter here; does first element contain second element?; keys are
  // "elementId1;elementId2"
  QHash<QString, bool> _elementContainsCache;

  // key is "elementId;type string"
  QHash<QString, bool> _isTypeCache;

  // key is "elementId;criterion class name"
  QHash<QString, bool> _hasCriterionCache;

  // key is element criterion class name
  QHash<QString, ElementCriterionPtr> _criterionCache;

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> _geometryCache;
  QHash<ElementId, std::shared_ptr<geos::geom::LineString>> _lineStringCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _numCacheEntriesByCacheType;

  std::shared_ptr<geos::geom::Geometry> _getGeometry(ConstElementPtr element);
  std::shared_ptr<geos::geom::LineString> _getLineString(ConstWayPtr way);

  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
