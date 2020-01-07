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
#include "PoiPolygonCache.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonType.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonDistance.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>

// Std
#include <float.h>

#include <QStringBuilder>

namespace hoot
{

PoiPolygonCache::PoiPolygonCache(const ConstOsmMapPtr& map) :
_map(map)
{
  _elementContainsCache.reserve(_map->size());
  _isTypeCache.reserve(_map->size() * 11);
  _hasCriterionCache.reserve(_map->size() * 2);
  _geometryCache.reserve(_map->size());
  _elementDistanceCache.reserve(_map->getNodes().size());
  _hasMoreThanOneTypeCache.reserve(_map->size());
  _inBuildingCategoryCache.reserve(_map->size());
  _numAddressesCache.reserve(_map->size());
  _containsMemberCache.reserve(_map->getWays().size() + _map->getRelations().size());
  _hasRelatedTypeCache.reserve(_map->size());
  _elementIntersectsCache.reserve(_map->getNodes().size());
}

PoiPolygonCache::~PoiPolygonCache()
{
}

void PoiPolygonCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

void PoiPolygonCache::clear()
{
  _numCacheHitsByCacheType.clear();
  _numCacheEntriesByCacheType.clear();

  _elementContainsCache.clear();
  _isTypeCache.clear();
  _hasCriterionCache.clear();
  _criterionCache.clear();
  _geometryCache.clear();
  _elementDistanceCache.clear();
  _hasMoreThanOneTypeCache.clear();
  _inBuildingCategoryCache.clear();
  _numAddressesCache.clear();
  _containsMemberCache.clear();
  _hasRelatedTypeCache.clear();
  _elementIntersectsCache.clear();
}

void PoiPolygonCache::printCacheInfo()
{
  // Add one for review distance cache on PoiPolygonDistance, one for the address cache on
  // AddressScoreExtractor, one for the name cache on NameExtractor, and one for the related tags
  // cache on the type score extractor.
  LOG_VARD(_numCacheHitsByCacheType.size());
  LOG_DEBUG("POI/Polygon caches used: " <<  (_numCacheHitsByCacheType.size() + 3));
  for (QMap<QString, int>::const_iterator numCacheHitsByCacheTypeItr = _numCacheHitsByCacheType.begin();
       numCacheHitsByCacheTypeItr != _numCacheHitsByCacheType.end(); ++numCacheHitsByCacheTypeItr)
  {
    const QString line =
      QString("%1:\t%2 hits     entries: %3")
        .arg(numCacheHitsByCacheTypeItr.key())
        .arg(StringUtils::formatLargeNumber(numCacheHitsByCacheTypeItr.value()))
        .arg(
          StringUtils::formatLargeNumber(
            _numCacheEntriesByCacheType[numCacheHitsByCacheTypeItr.key()]));
    LOG_DEBUG(line);
  }
  QString line =
    QString("%1:\t%2 hits     entries: %3")
      .arg("reviewDistance")
      .arg(StringUtils::formatLargeNumber(PoiPolygonDistance::getNumReviewDistanceCacheHits()))
      .arg(StringUtils::formatLargeNumber(PoiPolygonDistance::getReviewDistanceCacheSize()));
  LOG_DEBUG(line);
  line =
    QString("%1:\t%2 hits     entries: %3")
      .arg("address")
      .arg(StringUtils::formatLargeNumber(AddressScoreExtractor::getNumAddressCacheHits()))
      .arg(StringUtils::formatLargeNumber(AddressScoreExtractor::getAddressCacheSize()));
  LOG_DEBUG(line);
  line =
    QString("%1:\t%2 hits     entries: %3")
      .arg("name")
      .arg(StringUtils::formatLargeNumber(NameExtractor::getNumNameCacheHits()))
      .arg(StringUtils::formatLargeNumber(NameExtractor::getNameCacheSize()));
  LOG_DEBUG(line);
  line =
    QString("%1:\t%2 hits     entries: %3")
      .arg("relatedTags")
      .arg(StringUtils::formatLargeNumber(PoiPolygonTypeScoreExtractor::getNumRelatedTagsCacheHits()))
      .arg(StringUtils::formatLargeNumber(PoiPolygonTypeScoreExtractor::getRelatedTagsCacheSize()));
  LOG_DEBUG(line);
}

void PoiPolygonCache::_incrementCacheHitCount(const QString& cacheTypeKey)
{
  if (!_numCacheHitsByCacheType.contains(cacheTypeKey))
  {
    _numCacheHitsByCacheType[cacheTypeKey] = 1;
  }
  else
  {
    _numCacheHitsByCacheType[cacheTypeKey] = _numCacheHitsByCacheType[cacheTypeKey] + 1;
  }
}

void PoiPolygonCache::_incrementCacheSizeCount(const QString& cacheTypeKey)
{
  if (!_numCacheEntriesByCacheType.contains(cacheTypeKey))
  {
    _numCacheEntriesByCacheType[cacheTypeKey] = 1;
  }
  else
  {
    _numCacheEntriesByCacheType[cacheTypeKey] = _numCacheEntriesByCacheType[cacheTypeKey] + 1;
  }
}

std::shared_ptr<geos::geom::Geometry> PoiPolygonCache::getGeometry(const ConstElementPtr& element)
{
  if (!element)
  {
    //return std::shared_ptr<geos::geom::Geometry>();
    throw IllegalArgumentException();
  }
  //LOG_VART(element->getElementType());

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>>::const_iterator itr =
    _geometryCache.find(element->getElementId());
  if (itr != _geometryCache.end())
  {
    //LOG_VART(itr.value());
    _incrementCacheHitCount("geometry");
    return itr.value();
  }
  else
  {
    std::shared_ptr<geos::geom::Geometry> newGeom;
    QString errorMsg =
      "Feature passed to PoiPolygonReviewReducer caused topology exception on conversion to a geometry: ";
    try
    {
      newGeom = ElementConverter(_map).convertToGeometry(element);
    }
    catch (const geos::util::TopologyException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    catch (const HootException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(errorMsg << element->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    //LOG_VART(newGeom.get());
    if (newGeom.get() &&
        QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE("Invalid element passed to PoiPolygonReviewReducer: " << newGeom->toString());
        _badGeomCount++;
      }
      newGeom.reset();
    }
    //LOG_VART(newGeom.get());
    _geometryCache[element->getElementId()] = newGeom;
    _incrementCacheSizeCount("geometry");
    return newGeom;
  }
}

bool PoiPolygonCache::polyContainsPoi(const ConstElementPtr& poly, const ConstElementPtr& point)
{
  if (!poly || !point)
  {
    throw IllegalArgumentException();
  }
  if (poly->getElementType() != ElementType::Way && poly->getElementType() != ElementType::Relation)
  {
    throw IllegalArgumentException();
  }
  if (point->getElementType() != ElementType::Node)
  {
    throw IllegalArgumentException();
  }
  LOG_VART(point->getElementId());
  LOG_VART(poly->getElementId());

  const QString key =
    poly->getElementId().toString() % ";" % point->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _elementContainsCache.find(key);
  if (itr != _elementContainsCache.end())
  {
    _incrementCacheHitCount("contains");
    const bool contains = itr.value();
    LOG_TRACE("Found cached contains: " << contains << " for key: " << key);
    return contains;
  }
  else
  {
    std::shared_ptr<geos::geom::Geometry> polyGeom = getGeometry(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = getGeometry(point);
    bool contains = false;
    if (polyGeom && pointGeom)
    {
      contains = polyGeom->contains(pointGeom.get());
      LOG_TRACE("Calculated contains: " << contains << " for key: " << key);
    }
    else
    {
      LOG_TRACE("Unable to calculate contains for key: " << key);
    }
    _elementContainsCache[key] = contains;
    _incrementCacheSizeCount("contains");
    return contains;
  }
}

bool PoiPolygonCache::isType(const ConstElementPtr& element, const QString& type)
{
  if (!element || type.trimmed().isEmpty())
  {
    throw IllegalArgumentException();
  }

  const QString key = element->getElementId().toString() % ";" % type.toLower();
  QHash<QString, bool>::const_iterator itr = _isTypeCache.find(key);
  if (itr != _isTypeCache.end())
  {
    _incrementCacheHitCount("isType");
    return itr.value();
  }
  else
  {
    // A re-design is needed to make this a little less maintenance prone.
    bool isType = false;
    const QString typeForComparison = type.toLower();
    if (typeForComparison == QLatin1String("natural"))
    {
      isType = PoiPolygonType::isNatural(element);
    }
    else if (typeForComparison == QLatin1String("park"))
    {
      isType = PoiPolygonType::isPark(element);
    }
    else if (typeForComparison == QLatin1String("parking"))
    {
      isType = PoiPolygonType::isParking(element);
    }
    else if (typeForComparison == QLatin1String("parkish"))
    {
      isType = PoiPolygonType::isParkish(element);
    }
    else if (typeForComparison == QLatin1String("playground"))
    {
      isType = PoiPolygonType::isPlayground(element);
    }
    else if (typeForComparison == QLatin1String("restaurant"))
    {
      isType = PoiPolygonType::isRestaurant(element);
    }
    else if (typeForComparison == QLatin1String("religion"))
    {
      isType = PoiPolygonType::isReligion(element);
    }
    else if (typeForComparison == QLatin1String("restroom"))
    {
      isType = PoiPolygonType::isRestroom(element);
    }
    else if (typeForComparison == QLatin1String("school"))
    {
      isType = PoiPolygonType::isSchool(element);
    }
    else if (typeForComparison == QLatin1String("specificschool"))
    {
      isType = PoiPolygonType::isSpecificSchool(element);
    }
    else if (typeForComparison == QLatin1String("sport"))
    {
      isType = PoiPolygonType::isSport(element);
    }
    else
    {
      throw IllegalArgumentException("Invalid type passed to PoiPolygonCache::isType: " + type);
    }
    _isTypeCache[key] = isType;
    _incrementCacheSizeCount("isType");
    return isType;
  }
}

bool PoiPolygonCache::hasCriterion(const ConstElementPtr& element, const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException();
  }

  const QString key = element->getElementId().toString() % ";" % criterionClassName;
  QHash<QString, bool>::const_iterator itr = _hasCriterionCache.find(key);
  if (itr != _hasCriterionCache.end())
  {
    _incrementCacheHitCount("hasCrit");
    return itr.value();
  }
  else
  {
    ElementCriterionPtr crit = _getCrit(criterionClassName);
    const bool hasCrit = crit->isSatisfied(element);
    _hasCriterionCache[key] = hasCrit;
    _incrementCacheSizeCount("hasCrit");
    return hasCrit;
  }
}

bool PoiPolygonCache::hasMoreThanOneType(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

  QHash<ElementId, bool>::const_iterator itr =
    _hasMoreThanOneTypeCache.find(element->getElementId());
  if (itr != _hasMoreThanOneTypeCache.end())
  {
    _incrementCacheHitCount("hasMoreThanOneType");
    return itr.value();
  }
  else
  {
    const bool hasMoreThanOneType = PoiPolygonType::hasMoreThanOneType(element);
    _hasMoreThanOneTypeCache[element->getElementId()] = hasMoreThanOneType;
    _incrementCacheSizeCount("hasMoreThanOneType");
    return hasMoreThanOneType;
  }
}

bool PoiPolygonCache::hasRelatedType(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

  QHash<ElementId, bool>::const_iterator itr =
    _hasRelatedTypeCache.find(element->getElementId());
  if (itr != _hasRelatedTypeCache.end())
  {
    _incrementCacheHitCount("hasRelatedType");
    return itr.value();
  }
  else
  {
    const bool hasRelatedType = PoiPolygonType::hasRelatedType(element);
    _hasRelatedTypeCache[element->getElementId()] = hasRelatedType;
    _incrementCacheSizeCount("hasRelatedType");
    return hasRelatedType;
  }
}

bool PoiPolygonCache::inBuildingCategory(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

  QHash<ElementId, bool>::const_iterator itr =
    _inBuildingCategoryCache.find(element->getElementId());
  if (itr != _inBuildingCategoryCache.end())
  {
    _incrementCacheHitCount("inBuildingCategory");
    return itr.value();
  }
  else
  {
    const bool inBuildingCategory =
      OsmSchema::getInstance().getCategories(element->getTags()).intersects(
        OsmSchemaCategory::building());
    _inBuildingCategoryCache[element->getElementId()] = inBuildingCategory;
    _incrementCacheSizeCount("inBuildingCategory");
    return inBuildingCategory;
  }
}

int PoiPolygonCache::numAddresses(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

  QHash<ElementId, int>::const_iterator itr = _numAddressesCache.find(element->getElementId());
  if (itr != _numAddressesCache.end())
  {
    _incrementCacheHitCount("numAddresses");
    return itr.value();
  }
  else
  {
    const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);
     _numAddressesCache[element->getElementId()] = numAddresses;
    _incrementCacheSizeCount("numAddresses");
    return numAddresses;
  }
}

bool PoiPolygonCache::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
{
  if (!parent ||
      (parent->getElementType() != ElementType::Way &&
       parent->getElementType() != ElementType::Relation))
  {
    throw IllegalArgumentException();
  }
  if (parent->getElementType() != ElementType::Way && memberId.getType() != ElementType::Node)
  {
    throw IllegalArgumentException();
  }
  if (parent->getElementType() != ElementType::Relation &&
      memberId.getType() == ElementType::Unknown)
  {
    throw IllegalArgumentException();
  }

  const QString key = parent->getElementId().toString() % ";" % memberId.toString();
  QHash<QString, bool>::const_iterator itr = _containsMemberCache.find(key);
  if (itr != _containsMemberCache.end())
  {
    _incrementCacheHitCount("containsMember");
    return itr.value();
  }
  else
  {
    bool containsMember = false;
    if (parent->getElementType() == ElementType::Way)
    {
      containsMember =
        (std::dynamic_pointer_cast<const Way>(parent))->containsNodeId(memberId.getId());
    }
    else
    {
      containsMember =
        (std::dynamic_pointer_cast<const Relation>(parent))->contains(memberId);
    }
    _containsMemberCache[key] = containsMember;
    _incrementCacheSizeCount("containsMember");
    return containsMember;
  }
}

ElementCriterionPtr PoiPolygonCache::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException();
  }

  QHash<QString, ElementCriterionPtr>::const_iterator itr =
    _criterionCache.find(criterionClassName);
  if (itr != _criterionCache.end())
  {
    return itr.value();
  }
  else
  {
    ElementCriterionPtr crit =
      ElementCriterionPtr(
        Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
    if (!crit)
    {
      throw IllegalArgumentException(
        "Invalid criterion passed to PoiPolygonCache::hasCriterion: " + criterionClassName);
    }
    _criterionCache[criterionClassName] = crit;
    return crit;
  }
}

bool PoiPolygonCache::elementIntersectsElement(
  const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException();
  }

  const QString key1 =
    element1->getElementId().toString() % ";" % element2->getElementId().toString();
  const QString key2 =
    element2->getElementId().toString() % ";" % element1->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _elementIntersectsCache.find(key1);
  if (itr != _elementIntersectsCache.end())
  {
    _incrementCacheHitCount("intersects");
    const bool intersects = itr.value();
    LOG_TRACE("Found cached intersects: " << intersects << " for key: " << key1);
    return intersects;
  }
  itr = _elementIntersectsCache.find(key2);
  if (itr != _elementIntersectsCache.end())
  {
    _incrementCacheHitCount("intersects");
    const bool intersects = itr.value();
    LOG_TRACE("Found cached intersects: " << intersects << " for key: " << key2);
    return intersects;
  }

  std::shared_ptr<geos::geom::Geometry> geom1 = getGeometry(element1);
  std::shared_ptr<geos::geom::Geometry> geom2 = getGeometry(element2);
  bool intersects = false;
  if (geom1 && geom2)
  {
    intersects = geom1->intersects(geom2.get());
    _elementIntersectsCache[key1] = intersects;
  }
  return intersects;
}

double PoiPolygonCache::getPolyToPointDistance(
  const ConstElementPtr& poly, const ConstElementPtr& point)
{
  if (!poly || !point)
  {
    throw IllegalArgumentException();
  }
  if (poly->getElementType() != ElementType::Way && poly->getElementType() != ElementType::Relation)
  {
    throw IllegalArgumentException();
  }
  if (point->getElementType() != ElementType::Node)
  {
    throw IllegalArgumentException();
  }
  LOG_VART(point->getElementId());
  LOG_VART(poly->getElementId());

  // TODO: remove the distance cache if it doesn't increase performance
  const QString key = point->getElementId().toString() % ";" % poly->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _elementDistanceCache.find(key);
  if (itr != _elementDistanceCache.end())
  {
    _incrementCacheHitCount("polyToPointDistance");
    double distance = itr.value();
    LOG_TRACE("Found cached distance: " << distance << " for key: " << key);
    return distance;
  }
  else
  {
    double distance = -1.0;

    /*if (polyContainsPoi(poly, point)) // TODO: remove??
    {
      distance = 0.0;
    }
    else
    {*/
      std::shared_ptr<geos::geom::Geometry> polyGeom = getGeometry(poly);
      std::shared_ptr<geos::geom::Geometry> pointGeom = getGeometry(point);
      if (polyGeom && pointGeom)
      {
        distance = polyGeom->distance(pointGeom.get());
        LOG_TRACE("Calculated distance: " << distance << " for key: " << key);
      }
      else
      {
        LOG_TRACE("Unable to calculate distance for key: " << key);
      }
    //}

    _elementDistanceCache[key] = distance;
    _incrementCacheSizeCount("polyToPointDistance");

    return distance;
  }
}

double PoiPolygonCache::getArea(const ConstElementPtr& element)
{
  //LOG_VART(element.get());
  if (!element)
  {
    throw IllegalArgumentException();
  }

  std::shared_ptr<geos::geom::Geometry> geom = getGeometry(element);
  //LOG_VART(geom.get());
  double area = -1.0;
  if (geom)
  {
    area = geom->getArea();
  }
  //LOG_VART(area);
  return area;
}

}
