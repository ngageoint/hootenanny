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

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/address/AddressParser.h>

namespace hoot
{

class PoiPolygonCache;

typedef std::shared_ptr<PoiPolygonCache> PoiPolygonCachePtr;

/**
 * Cached back class storing POI/Polygon feature comparison info
 *
 * The caches used in this class were determined on 9/30/18 running against the regression test:
 * unifying-tests.child/somalia.child/somalia-test3.child. Further caching could be deemed necessary
 * given performance with other datasets.
 *
 * Some of the distance comparisons in this class could be abstracted out beyond poi/poly geoms and
 * moved into another cache class if we ever need them to be used with other conflation algs.
 */
class PoiPolygonCache : public Configurable
{

public:

  PoiPolygonCache(const ConstOsmMapPtr& map);
  ~PoiPolygonCache();

  virtual void setConfiguration(const Settings& conf);

  /**
   * TODO
   *
   * @param poly
   * @param point
   * @return
   */
  double getPolyToPointDistance(const ConstElementPtr& poly, const ConstElementPtr& point);

  /**
   * Calculates the area of an element
   *
   * @param element the feature to calculate the area of
   * @return the area of the feature or -1.0 if the area could not be calculated
   * @note The area calc is not backed by a cache, but the geometries used to derive it are.
   */
  double getArea(const ConstElementPtr& element);

  /**
   * Determines if a polygon contains a point
   *
   * @param poly the polygon to examine
   * @param point the point to examine
   * @return true if the polygon contains the point; false otherwise or if the containment could
   * not be calculated
   */
  bool polyContainsPoi(const ConstWayPtr& poly, const ConstNodePtr& point);

  /**
   * Determines if an element intersects another element
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if the two elements intersect; false otherwise or if the intersection could not
   * be calculated
   */
  bool elementIntersectsElement(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if an element is of the given type
   *
   * @param element the element to determine type membership for
   * @param type the type to determine membership for the element
   * @return true if the element is of the requested type; false otherwise
   * @throws if the requested type is invalid
   * @todo change type to enum
   */
  bool isType(const ConstElementPtr& element, const QString& type);

  /**
   * Determines if an element has a given criterion
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   */
  bool hasCriterion(const ConstElementPtr& element, const QString& criterionClassName);

  /**
   * TODO
   *
   * @param element
   * @return
   */
  bool hasMoreThanOneType(const ConstElementPtr& element);

  /**
   * TODO
   *
   * @param element
   * @return
   */
  bool inBuildingCategory(const ConstElementPtr& element);

  /**
   * TODO
   *
   * @param element
   * @return
   */
  int numAddresses(const ConstElementPtr& element);

  /**
   * TODO
   *
   * @param parent
   * @param memberId
   * @return
   */
  bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * TODO
   *
   * @param element
   * @return
   */
  std::shared_ptr<geos::geom::Geometry> getGeometry(const ConstElementPtr& element);

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

  AddressParser _addressParser;

  // Using QHash here instead of QCache, since we're mostly just storing primitives for values and
  // they all take up the same amount of space. Tried to use QCache with the geometries, but since
  // ElementConverter returns a shared pointer and QCache takes ownership, had trouble making it
  // work. Also, not doing any management of the size of these caches, which may eventually end up
  // being needed to control memory usage.

  // ordering of the ID keys does matter here; "poi element ID;poly element ID"
  QHash<QString, double> _elementDistanceCache;

  // ordering of the ID keys does matter here; we're asking the question: does the first element
  // contain the second element?; keys are "elementId1;elementId2"
  QHash<QString, bool> _elementContainsCache;

  // key is "elementId;type string"
  QHash<QString, bool> _isTypeCache;

  // key is "elementId;criterion class name"
  QHash<QString, bool> _hasCriterionCache;

  // key is element criterion class name
  QHash<QString, ElementCriterionPtr> _criterionCache;

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> _geometryCache;

  QHash<ElementId, bool> _hasMoreThanOneTypeCache;
  QHash<ElementId, bool> _inBuildingCategoryCache;
  QHash<ElementId, int> _numAddressesCache;

  // key = parent ID;child ID
  QHash<QString, bool> _containsMemberCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _numCacheEntriesByCacheType;

  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);

  /**
   * Returns the distance from a point node to a polygon way
   *
   * @param poly the way polygon to measure distance from
   * @param point the point to measure distance from
   * @return the distance between the point and polygon or -1.0 if the distance could not be
   * calculated
   */
  //double _getPolyToPointDistance(const ConstWayPtr& poly, const ConstNodePtr& point);

  /**
   * Returns the shortest distance from a point node to a relation containing polygons
   *
   * @param poly the relation containing polygons to measure distance from
   * @param point the point to measure distance from
   * @return the distance between the point and closest polygon in the relation or -1.0 if the
   * distance could not be calculated
   */
  //double _getPolyToPointDistance(const ConstRelationPtr& poly, const ConstNodePtr& point);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
