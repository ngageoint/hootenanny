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
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchemaType.h>

namespace hoot
{

class PoiPolygonInfoCache;

typedef std::shared_ptr<PoiPolygonInfoCache> PoiPolygonInfoCachePtr;

/**
 * Cached back class storing POI/Polygon feature comparison info
 *
 * The caches used in this class were determined on 9/30/18 running against the regression test
 * unifying-tests.child/somalia.child/somalia-test3.child and also on 1/7/20 against the data in
 * africom/somalia/229_Mogadishu_SOM_Translated. Further caching could be deemed necessary
 * given performance with other datasets.
 *
 * Note: I did notice later in the second round of performance testing that largely differing
 * runtimes were being returned on a vagrant instance with the same configuration. I'm not sure
 * what could be causing this, but that means the results are suspect. Going to go ahead and use
 * the caching config from the best runtime, but it still may need to be tweaked.
 *
 * Some of the geometry comparisons in this class could be abstracted out beyond poi/poly geoms and
 * moved into another cache class if we ever need them to be used with other conflation algs.
 */
class PoiPolygonInfoCache : public Configurable
{

public:

  PoiPolygonInfoCache(const ConstOsmMapPtr& map);

  virtual void setConfiguration(const Settings& conf);

  /**
   * Returns the distance from a point node to a polygon way
   *
   * @param poly the way polygon to measure distance from
   * @param point the point to measure distance from
   * @return the distance between the point and polygon or -1.0 if the distance could not be
   * calculated
   */
  double getDistance(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Calculates the area of an element
   *
   * @param element the feature to calculate the area of
   * @return the area of the feature or -1.0 if the area could not be calculated
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
  bool elementContains(const ConstElementPtr& containingElement,
                       const ConstElementPtr& containedElement);

  /**
   * Determines if an element intersects another element
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if the two elements intersect; false otherwise or if the intersection could not
   * be calculated
   */
  bool elementsIntersect(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if an element is of the given type
   *
   * @param element the element to determine type membership for
   * @param type the type to determine membership for the element
   * @return true if the element is of the requested type; false otherwise
   * @throws if the requested type is invalid
   */
  bool isType(const ConstElementPtr& element, const PoiPolygonSchemaType& type);

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
  bool hasRelatedType(const ConstElementPtr& element);

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
   */
  bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * TODO
   *
   * @param element
   * @param polyTags
   * @param reviewDistanceThresholdDefault
   * @return
   */
  double getReviewDistance(const ConstElementPtr& element, const Tags& polyTags,
                           const double reviewDistanceThresholdDefault);

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

  // TODO: should this be static?
  int _badGeomCount;

  AddressParser _addressParser;

  // Using QHash here instead of QCache, since we're mostly just storing primitives for values and
  // they all take up the same amount of space. Tried to use QCache with the geometries, but since
  // ElementConverter returns a shared pointer and QCache takes ownership, had trouble making it
  // work. Also, not doing any management of the size of these caches, which may eventually end up
  // being needed to control memory usage.

  // key is "elementID 1;elementID 2"; ordering of the ID keys doesn't matter here, since we check
  // in both directions
  QHash<QString, bool> _elementIntersectsCache;

  // key is "elementId;type string"
  QHash<QString, bool> _isTypeCache;

  // key is "elementId;criterion class name"
  QHash<QString, bool> _hasCriterionCache;

  // key is element criterion class name
  QHash<QString, ElementCriterionPtr> _criterionCache;

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>> _geometryCache;

  QHash<ElementId, bool> _hasMoreThanOneTypeCache;
  QHash<ElementId, int> _numAddressesCache;

  QHash<ElementId, double> _reviewDistanceCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _numCacheEntriesByCacheType;

  /*
   * TODO
   *
   * @param element
   * @return
   */
  std::shared_ptr<geos::geom::Geometry> _getGeometry(const ConstElementPtr& element);

  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
