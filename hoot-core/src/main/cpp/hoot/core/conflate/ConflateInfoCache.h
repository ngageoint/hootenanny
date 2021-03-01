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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CONFLATE_INFO_CACHE_H
#define CONFLATE_INFO_CACHE_H

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/address/AddressParser.h>

// Tgs
#include <tgs/LruCache.h>

// Qt
#include <QCache>

namespace hoot
{

class MatchCreator;

/**
 * Caches conflatable elements, their relationships with other elements, and conflation
 * configuration details that may aid in speeding up conflation jobs.
 *
 * This is generally expected to be initialized separately for each section of the code using it. It
 * is done this way rather than implementing a Singleton or using global static instances, since
 * conflate case tests are launched as multiple threads within the same process. Not using separate
 * instances of the cache which would result in a conflicted cache state if the cache was shared by
 * different threads (different elements with the same ID could have different info; @todo We may be
 * able to use thread_local storage for this purpose with static member vars and avoid using
 * separate instances). Currently, this cache is only used in a few places and not by all match
 * creators.
 *
 * The caches used in this class were determined on 9/30/18 running POI/Polygon conflation against
 * the regression test unifying-tests.child/somalia.child/somalia-test3.child and also on 1/7/20
 * against the data in africom/somalia/229_Mogadishu_SOM_Translated. Further caching enhancements
 * may be deemed necessary for conflation against other feature types.
 *
 * Its worth noting that if you are doing cache performance tweaks on a Vagrant VM, the runtimes may
 * vary somewhat widely with subsequent executions of the exact same conflate job. So in that case,
 * its best to look for tweaks with fairly large improvements and ignore the smaller performance
 * improvements.
 *
 * WARNING: This class can get very memory hungry depending on how the cache size is configured.
 */
class ConflateInfoCache : public Configurable
{

public:

  ConflateInfoCache(const ConstOsmMapPtr& map);
  virtual ~ConflateInfoCache() = default;

  virtual void setConfiguration(const Settings& conf) override;

  /**
   * Returns the distance between two elements; backed by a cache
   *
   * @param element1 the first element to measure distance from
   * @param element2 the second element to measure distance from
   * @return the distance between the two elements or -1.0 if the distance could not be calculated
   */
  double getDistance(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Calculates the area of an element; backed by a cache
   *
   * @param element the feature to calculate the area of
   * @return the area of the feature or -1.0 if the area could not be calculated
   */
  double getArea(const ConstElementPtr& element);

  /**
   * Determines if an element contains another element geographically
   *
   * Unlike ElementGeometryUtils::haveGeometricRelationship, this backs the element to geometry
   * conversion with a cache. This will contribute to increased runtime performance if the same
   * elements are being used in a comparison many times within the same conflate job.
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
   * Unlike ElementGeometryUtils::haveGeometricRelationship, this backs the element to geometry
   * conversion with a cache. This will contribute to increased runtime performance if the same
   * elements are being used in a comparison many times within the same conflate job.
   *
   * @param element1 the first element to examine
   * @param element2 the second element to examine
   * @return true if the two elements intersect; false otherwise or if the intersection could not
   * be calculated
   */
  bool elementsIntersect(const ConstElementPtr& element1, const ConstElementPtr& element2);

  /**
   * Determines if an element has a given criterion; backed by a cache
   *
   * Unlike CriterionUtils::hasCriterion, this backs the criterion creation with a cache. This will
   * contribute to increased runtime performance if the same elements are being used in a comparison
   * many times within the same conflate job.
   *
   * @param element the element to examine
   * @param criterionClassName class name of the ElementCriterion to determine membership of
   * @return true if the element has the criterion; false otherwise
   * @throws if the criterion class name is invalid
   */
  bool hasCriterion(const ConstElementPtr& element, const QString& criterionClassName);

  /**
   * Returns the number of addresses contained by an element; backed by a cache
   *
   * @param element the element to examine
   * @return a number of addresses
   */
  int numAddresses(const ConstElementPtr& element);

  /**
   * Determines if one element a child of another; e.g. way node or relation member; backed by a
   * cache
   *
   * @param parent the parent element
   * @param memberId the element ID of the child
   * @return true if parent has the element with memberId as a child; false otherwise
   */
  bool containsMember(const ConstElementPtr& parent, const ElementId& memberId);

  /**
   * Clears the contents of the cache
   */
  virtual void clear();

  /**
   * Prints information about the caches used by this class
   */
  void printCacheInfo();

  /**
   * Determines if an element can be conflated by any of the actively configured matchers for
   * conflation.
   *
   * @param element element to examine
   * @param caller optional name of the class calling this method for debugging purposes
   * @return true if the conflate matchers are configured with at least one matcher that
   * can conflate the input element; false otherwise
   */
  bool elementCanBeConflatedByActiveMatcher(
    const ConstElementPtr& element, const QString& caller = QString());

  /**
   * Determines if a ConflatableCriterion is in use by any actively configured conflate matcher
   *
   * @param criterionClassName class name of the criterion
   * @return true if an active matcher uses the criterion with the specified name; false otherwise
   */
  bool elementCriterionInUseByActiveMatcher(const QString& criterionClassName);

protected:

  static const int CACHE_SIZE_DEFAULT = 10000;

  ConstOsmMapPtr _map;
  bool _cacheEnabled;

  void _incrementCacheHitCount(const QString& cacheTypeKey);
  void _incrementCacheSizeCount(const QString& cacheTypeKey);

private:

  int _badGeomCount;

  AddressParser _addressParser;

  // Didn't see any performance improvement by reserving initial sizes for these caches.

  // key is "elementID 1;elementID 2"; ordering of the ID keys doesn't matter here, since we check
  // in both directions
  QCache<QString, bool> _elementIntersectsCache;

  // key is "elementId;criterion class name"
  QCache<QString, bool> _hasCriterionCache;

  // key is element criterion class name; leaving this as a hash, since it shouldn't ever get big
  // enough to require any cache size management
  QHash<QString, ElementCriterionPtr> _criterionCache;

  // QCache doesn't play nicely with shared pointers created elsewhere...tried using them with it
  // for _geometryCache but failed. Another alternative was to modify ElementToGeometryConverter to
  // also allow for returing Geometry raw pointers in addition to shared pointers...tried that and
  // failed as well. Decided to use LruCache for this one instead.
  std::shared_ptr<Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>> _geometryCache;

  QCache<ElementId, int> _numAddressesCache;

  QMap<QString, int> _numCacheHitsByCacheType;
  QMap<QString, int> _numCacheEntriesByCacheType;

  // This cache must store ElementCriterion and not ConflatableElementCriterion, b/c we're also
  // checking against conflate child criteria (e.g. RailwayWayNodeCriterion for RailwayCriterion),
  // which don't inherit from ConflatableElementCriterion.
  QHash<QString, ElementCriterionPtr> _conflatableCritCache;
  QHash<ElementId, bool> _conflatableElementCache;
  QHash<QString, bool> _conflatableCritActiveCache;
  std::vector<std::shared_ptr<MatchCreator>> _activeMatchCreators;

  std::shared_ptr<geos::geom::Geometry> _getGeometry(const ConstElementPtr& element);
  ElementCriterionPtr _getCrit(const QString& criterionClassName);
};

}

#endif // CONFLATE_INFO_CACHE_H
