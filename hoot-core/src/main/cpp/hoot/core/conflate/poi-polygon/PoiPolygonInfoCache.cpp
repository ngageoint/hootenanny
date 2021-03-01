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
#include "PoiPolygonInfoCache.h"

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiPolygonInfoCache::PoiPolygonInfoCache(const ConstOsmMapPtr& map) :
ConflateInfoCache(map),
_isTypeCache(CACHE_SIZE_DEFAULT),
_hasMoreThanOneTypeCache(CACHE_SIZE_DEFAULT),
_reviewDistanceCache(CACHE_SIZE_DEFAULT)
{
}

void PoiPolygonInfoCache::setConfiguration(const Settings& conf)
{
  ConflateInfoCache::setConfiguration(conf);

  const int maxCacheSize = ConfigOptions(conf).getConflateInfoMaxSizePerCache();
  if (maxCacheSize > 0)
  {
    _isTypeCache.setMaxCost(maxCacheSize);
    _hasMoreThanOneTypeCache.setMaxCost(maxCacheSize);
    _reviewDistanceCache.setMaxCost(maxCacheSize);
  }
  else
  {
    _cacheEnabled = false;
  }
}

void PoiPolygonInfoCache::clear()
{
  ConflateInfoCache::clear();

  if (_cacheEnabled)
  {
    LOG_DEBUG("Clearing cache...");

    _isTypeCache.clear();
    _hasMoreThanOneTypeCache.clear();
  }
}

bool PoiPolygonInfoCache::isType(const ConstElementPtr& element, const PoiPolygonSchemaType& type)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  QString key;

  if (_cacheEnabled)
  {
    key = element->getElementId().toString() % ";" % type.toString().toLower();
    const bool* cachedVal = _isTypeCache[key];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("isType");
      return *cachedVal;
    }
  }


  // A re-design is needed to make this a little less maintenance prone...possiby add custom
  // schema lookup files for poi/poly?
  bool isType = false;
  switch (type.getEnum())
  {
    case PoiPolygonSchemaType::Natural:
      isType = PoiPolygonSchema::isNatural(element);
      break;
    case PoiPolygonSchemaType::Park:
      isType = PoiPolygonSchema::isPark(element);
      break;
    case PoiPolygonSchemaType::Parking:
      isType = PoiPolygonSchema::isParking(element);
      break;
    case PoiPolygonSchemaType::Parkish:
      isType = PoiPolygonSchema::isParkish(element);
      break;
    case PoiPolygonSchemaType::Playground:
      isType = PoiPolygonSchema::isPlayground(element);
      break;
    case PoiPolygonSchemaType::Restaurant:
      isType = PoiPolygonSchema::isRestaurant(element);
      break;
    case PoiPolygonSchemaType::Religion:
      isType = PoiPolygonSchema::isReligion(element);
      break;
    case PoiPolygonSchemaType::Restroom:
      isType = PoiPolygonSchema::isRestroom(element);
      break;
    case PoiPolygonSchemaType::School:
      isType = PoiPolygonSchema::isSchool(element);
      break;
    case PoiPolygonSchemaType::SpecificSchool:
      isType = PoiPolygonSchema::isSpecificSchool(element);
      break;
    case PoiPolygonSchemaType::Sport:
      isType = PoiPolygonSchema::isSport(element);
      break;
    default:
      throw IllegalArgumentException("Unsupported POI/Polygon schema type.");
  }

  if (_cacheEnabled)
  {
    _isTypeCache.insert(key, new bool(isType));
    _incrementCacheSizeCount("isType");
  }

  return isType;
}

bool PoiPolygonInfoCache::hasMoreThanOneType(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  if (_cacheEnabled)
  {
    const bool* cachedVal = _hasMoreThanOneTypeCache[element->getElementId()];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("hasMoreThanOneType");
      return *cachedVal;
    }
  }

  const bool hasMoreThanOneType = PoiPolygonSchema::hasMoreThanOneType(element);

  if (_cacheEnabled)
  {
    _hasMoreThanOneTypeCache.insert(element->getElementId(), new bool(hasMoreThanOneType));
    _incrementCacheSizeCount("hasMoreThanOneType");
  }

  return hasMoreThanOneType;
}

bool PoiPolygonInfoCache::hasRelatedType(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  const bool hasRelatedType = PoiPolygonSchema::hasRelatedType(element);
  return hasRelatedType;
}

double PoiPolygonInfoCache::getReviewDistance(const ConstElementPtr& element,
                                          const Tags& polyTags,
                                          const double reviewDistanceThresholdDefault)
{
  if (_cacheEnabled)
  {
    const double* cachedVal = _reviewDistanceCache[element->getElementId()];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("reviewDistance");
      double distance = *cachedVal;
      LOG_TRACE("Found review distance: " << distance << " for: " << element->getElementId());
      return distance;
    }
  }

  double distance;
  const Tags& tags = element->getTags();
  //these distances could be moved to a config
  if (tags.get("leisure") == "park")
  {
    distance = 25.0;
  }
  else if ((tags.get("station").toLower() == "light_rail" ||
            tags.get("railway").toLower() == "platform") &&
           (polyTags.get("subway").toLower() == "yes" ||
            polyTags.get("tunnel").toLower() == "yes"))
  {
    distance = 150.0;
  }
  else
  {
    distance = reviewDistanceThresholdDefault;
  }

  if (_cacheEnabled)
  {
    _reviewDistanceCache.insert(element->getElementId(), new double(distance));
    _incrementCacheSizeCount("reviewDistance");
  }

  return distance;
}

}
