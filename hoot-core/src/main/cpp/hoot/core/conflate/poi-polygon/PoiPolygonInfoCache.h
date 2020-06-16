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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

// Tgs
#include <tgs/LruCache.h>

// Qt
#include <QCache>

namespace hoot
{

class PoiPolygonInfoCache;

typedef std::shared_ptr<PoiPolygonInfoCache> PoiPolygonInfoCachePtr;

/**
 * Cached back class storing POI/Polygon feature comparison info
 *
 * This class is passed into PoiPolygonMatch as a one stop shop to get poi/poly comparison info
 * with a cache backing. It is done this way rather than implementing a Singleton, since case tests
 * are launched as multiple threads within the same process, which would result in a conflicted
 * cache state.
 *
 * The caches used in this class were determined on 9/30/18 running against the regression test
 * unifying-tests.child/somalia.child/somalia-test3.child and also on 1/7/20 against the data in
 * africom/somalia/229_Mogadishu_SOM_Translated. Further caching could be deemed necessary
 * given performance with other datasets.
 *
 * Its worth noting that if you are doing performance tweaks on a Vagrant VM, the runtimes may vary
 * with subsequent executions of the exact same conflate job. So in that case, its best to look for
 * tweaks with fairly large improvements and ignore the smaller performance improvements.
 *
 * Some of the geometry comparisons in this class could be abstracted out beyond poi/poly geoms and
 * moved into a class like OsmGeometryUtils if we ever need them to be used with other conflation
 * algs. If that's done there will be some work to make any caching being performed work across unit
 * tests.
 */
class PoiPolygonInfoCache : public Configurable
{

public:

  PoiPolygonInfoCache(const ConstOsmMapPtr& map);
  virtual ~PoiPolygonInfoCache() = default;

  virtual void setConfiguration(const Settings& conf);

  /**
   * Returns the distance between two elements
   *
   * @param element1 the first element to measure distance from
   * @param element2 the second element to measure distance from
   * @return the distance between the two elements or -1.0 if the distance could not be calculated
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
   * Determines if an element contains another element geographically
   *
   * @param containingElement the element to check for containing containedElement
   * @param containedElement the element to check if contained by containingElement
   * @return true if containingElement contains the containedElement geographicaly; false otherwise
   * or if the containment could not be calculated
   */
  bool elementContains(const ConstElementPtr& containingElement,
                       const ConstElementPtr& containedElement);

  /**
   * Determines if an element intersects another element; backed by a cache
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if the two elements intersect; false otherwise or if the intersection could not
   * be calculated
   */
  bool elementsIntersect(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if an element is of the given type as defined by the poi/poly schema; backed by a
   * cache
   *
   * @param element the element to determine type membership for
   * @param type the type to determine membership for the element
   * @return true if the element is of the requested type; false otherwise
   * @throws if the requested type is invalid
   */
  bool isType(const ConstElementPtr& element, const PoiPolygonSchemaType& type);

  /**
   * Determines if an element has a given criterion; backed by a cache
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   */
  bool hasCriterion(const ConstElementPtr& element, const QString& criterionClassName);

  /**
   * Determines if an element has more than one type as defined by the poi/poly schema; backed by a
   * cache
   *
   * @param element the element to examine
   * @return true if the element has more than one type; false otherwise
   */
  bool hasMoreThanOneType(const ConstElementPtr& element);

  /**
   * Determines if an element has a poi/polygon related type
   *
   * @param element the element to examine
   * @return true if the element has a poi/poly related type; false otherwise
   */
  bool hasRelatedType(const ConstElementPtr& element);

  /**
   * Returns the number of addresses contained by an element; backed by a cache
   *
   * @param element the element to examine
   * @return a number of addresses
   */
  int numAddresses(const ConstElementPtr& element);

  /**
   * Determines if one element a child of another; e.g. way node or relation memeber
   *
   * @param parent the parent element
   * @param memberId the element ID of the child
   * @return true if parent has the element with memberId as a child; false otherwise
   */
  bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * Determines the max distance to use for generating reviews during matching; backed by a cache
   *
   * @param element the element to examine
   * @param polyTags the tags of the polygon involved in the current match
   * @param reviewDistanceThresholdDefault the configured default review distance
   * @return a review distance
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

  bool _cacheEnabled;

  // Didn't see any performance improvement by reserving initial sizes for these caches.

  // key is "elementID 1;elementID 2"; ordering of the ID keys doesn't matter here, since we check
  // in both directions
  QCache<QString, bool> _elementIntersectsCache;

  // key is "elementId;type string"
  QCache<QString, bool> _isTypeCache;

  // key is "elementId;criterion class name"
  QCache<QString, bool> _hasCriterionCache;

  // key is element criterion class name; leaving this as a hash, since it shouldn't ever get big
  // enough to require any cache size management
  QHash<QString, ElementCriterionPtr> _criterionCache;

  // QCache doesn't play nicely with shared pointers created elsewhere...tried using them with it
  // for _geometryCache but failed. Another alternative was to modify ElementConverter to also
  // allow for returing Geometry raw pointers in addition to shared pointers...tried that and failed
  // as well. Decided to use LruCache for this one instead.
  std::shared_ptr<Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>> _geometryCache;

  QCache<ElementId, bool> _hasMoreThanOneTypeCache;
  QCache<ElementId, int> _numAddressesCache;
  QCache<ElementId, double> _reviewDistanceCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _numCacheEntriesByCacheType;

  static const int CACHE_SIZE_UPDATE_INTERVAL = 100000;
  static const int CACHE_SIZE_DEFAULT = 10000;

  std::shared_ptr<geos::geom::Geometry> _getGeometry(const ConstElementPtr& element);
  ElementCriterionPtr _getCrit(const QString& criterionClassName);

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);
};

}

#endif // POIPOLYGONREVIEWREDUCER_H
