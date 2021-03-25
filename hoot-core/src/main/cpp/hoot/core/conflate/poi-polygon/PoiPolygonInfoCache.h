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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POI_POLYGON_INFO_CACHE_H
#define POI_POLYGON_INFO_CACHE_H

// Hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchemaType.h>
#include <hoot/core/conflate/ConflateInfoCache.h>

namespace hoot
{

class PoiPolygonInfoCache;

using PoiPolygonInfoCachePtr = std::shared_ptr<PoiPolygonInfoCache>;

/**
 * Cached storing POI/Polygon feature comparison info
 *
 * @sa ConflateInfoCache.
 */
class PoiPolygonInfoCache : public ConflateInfoCache
{

public:

  PoiPolygonInfoCache(const ConstOsmMapPtr& map);
  ~PoiPolygonInfoCache() = default;

  void setConfiguration(const Settings& conf) override;

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
   * Determines the max distance to use for generating reviews during poi/polygon matching; backed
   * by a cache
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
  void clear() override;

private:

  // key is "elementId;type string"
  QCache<QString, bool> _isTypeCache;

  QCache<ElementId, bool> _hasMoreThanOneTypeCache;
  QCache<ElementId, double> _reviewDistanceCache;
};

}

#endif // POI_POLYGON_INFO_CACHE_H
