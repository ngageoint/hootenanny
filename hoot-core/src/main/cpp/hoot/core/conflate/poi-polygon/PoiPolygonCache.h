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
 * TODO (Singleton)
 */
class PoiPolygonCache
{

public:

  ~PoiPolygonCache();

  static const PoiPolygonCachePtr& getInstance(const ConstOsmMapPtr& map);

  virtual void setOsmMap(const ConstOsmMapPtr& map);

  double getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point);

  double getArea(ConstElementPtr poly);

  bool polyContainsPoi(ConstWayPtr poly, ConstNodePtr point);

  bool elementIntersectsElement(ConstElementPtr element1, ConstElementPtr element2);

  bool isType(ConstElementPtr element, const QString& type);

  bool hasCrit(ConstElementPtr element, const QString& criterionClassName);

  /*
   * Determines if there exists a poi in the search radius of the poi being evaluated that is
   * closer to the poly being evaluated.  The operation becomes more expensive as the search radius
   * is increased.
   *
   * TODO: This only handles ways as polygons and not relations. See #3474.
   */
  bool polyHasPoiNeighborCloserThanPoi(ConstWayPtr poly, ConstNodePtr poi,
                                       const std::set<ElementId>& poiNeighborIds,
                                       const double poiPolyDistance);

  void printCacheInfo();

private:

  static std::shared_ptr<PoiPolygonCache> _theInstance;

  ConstOsmMapPtr _map;

  int _badGeomCount;

  // Using QHash here instead of QCache, since we're mostly just storing primitives for values.
  // Tried to use QCache with the geometries, but since ElementConverter returns a shared pointer
  // and QCache takes ownership, had trouble making it work. Also, not doing any management of the
  // size of these caches, which may eventually end up being needed to control memory usage.

  // ordering of the ID keys doesn't matter here; keys are "elementId1;elementId2"
  QHash<QString, double> _elementDistanceCache;
  QHash<QString, bool> _elementIntersectsCache;
  QHash<QString, bool> _poiNeighborCloserCache;

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
  QMap<QString, int> _cacheNumEntriesByCacheType;

  PoiPolygonCache();

  std::shared_ptr<geos::geom::Geometry> _getGeometry(ConstElementPtr element);
  std::shared_ptr<geos::geom::LineString> _getLineString(ConstWayPtr way);

  void _clear();

  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
