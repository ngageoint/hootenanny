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
#include <hoot/core/conflate/address/AddressParser.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

class PoiPolygonCache;

typedef std::shared_ptr<PoiPolygonCache> PoiPolygonCachePtr;

/**
 * TODO (Singleton)
 */
class PoiPolygonCache : public Configurable
{

public:

  //static const int MAX_CACHE_SIZE = 100000;

  ~PoiPolygonCache();

  static const PoiPolygonCachePtr& getInstance(const ConstOsmMapPtr& map);

  virtual void setConfiguration(const Settings& conf);

  virtual void setOsmMap(const ConstOsmMapPtr& map);

  double getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point);

  double getArea(ConstElementPtr poly);

  bool polyContainsPoi(ConstWayPtr poly, ConstNodePtr point);

  bool elementIntersectsElement(ConstElementPtr element1, ConstElementPtr element2);

  double getAngleHistogramVal(ConstElementPtr element1, ConstElementPtr element2);

  double getOverlapVal(ConstElementPtr element1, ConstElementPtr element2);

  double getNameScore(ConstElementPtr element1, ConstElementPtr element2);

  double getTypeScore(ConstElementPtr element1, ConstElementPtr element2);

  bool isType(ConstElementPtr element, const QString& type);

  bool hasCrit(ConstElementPtr element, const QString& criterionClassName);

  bool hasType(ConstElementPtr element);

  bool hasMoreThanOneType(ConstElementPtr element);

  bool specificSchoolMatch(ConstElementPtr element1, ConstElementPtr element2);

  bool hasAddress(ConstElementPtr element);

  int getNumAddresses(ConstElementPtr element);

  bool inCategory(ConstElementPtr element, const OsmSchemaCategory& category);

  long getMatchingWayId(ConstNodePtr node);

  void printCacheInfo();

private:

  static std::shared_ptr<PoiPolygonCache> _theInstance;

  ConstOsmMapPtr _map;

  int _badGeomCount;

  AddressParser _addressParser;

  // Using QHash here instead of QCache, since we're mostly just storing primitives. Tried to use
  // QCache with the geometries, but since ElementConverter returns a shared pointer and QCache
  // takes ownership, had trouble making it work. Also, not doing any management of the size of
  // these caches, which may end up being needed to control memory usage.

  // ordering of the ID keys doesn't matter here; keys are "elementId1;elementId2"
  QHash<QString, double> _elementDistanceCache;
  QHash<QString, bool> _elementIntersectsCache;
  QHash<QString, double> _elementAngleHistogramCache;
  QHash<QString, double> _elementOverlapCache;
  QHash<QString, double> _nameScoreCache;
  QHash<QString, double> _typeScoreCache;
  QHash<QString, bool> _specificSchoolMatchCache;

  // ordering of the ID keys does matter here; does first element contain second element; keys are
  // "elementId1;elementId2"
  QHash<QString, bool> _elementContainsCache;

  // key is "elementId;type string"
  QHash<QString, bool> _isTypeCache;

  // key is "elementId;criterion class name"
  QHash<QString, bool> _hasCriterionCache;

  // key is element criterion class name
  QHash<QString, ElementCriterionPtr> _criterionCache;

  // key is "elementId;category name"
  QHash<QString, bool> _isInCategoryCache;

  QHash<ElementId, double> _elementAreaCache;
  QHash<ElementId, bool> _hasTypeCache;
  QHash<ElementId, bool> _hasMoreThanOneTypeCache;
  QHash<ElementId, int> _numAddressesCache;
  QHash<ElementId, long> _matchingWayIdCache;

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> _geometryCache;
  QHash<ElementId, std::shared_ptr<geos::geom::LineString>> _lineStringCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _cacheSizeByCacheType;

  PoiPolygonCache();

  void _clear();
  void _init();

  std::shared_ptr<geos::geom::Geometry> _getGeometry(ConstElementPtr element);
  std::shared_ptr<geos::geom::LineString> _getLineString(ConstWayPtr way);
  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
