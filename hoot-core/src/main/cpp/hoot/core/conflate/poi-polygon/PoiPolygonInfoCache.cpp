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
#include "PoiPolygonInfoCache.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>

// Std
#include <float.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiPolygonInfoCache::PoiPolygonInfoCache(const ConstOsmMapPtr& map) :
_map(map)
{
//  _elementContainsCache.reserve(_map->size());
//  _isTypeCache.reserve(_map->size() * 11);
//  _hasCriterionCache.reserve(_map->size() * 2);
//  _geometryCache.reserve(_map->size());
//  _elementDistanceCache.reserve(_map->getNodes().size());
//  _hasMoreThanOneTypeCache.reserve(_map->size());
//  _inBuildingCategoryCache.reserve(_map->size());
//  _numAddressesCache.reserve(_map->size());
//  _containsMemberCache.reserve(_map->getWays().size() + _map->getRelations().size());
//  _hasRelatedTypeCache.reserve(_map->size());
//  _elementIntersectsCache.reserve(_map->getNodes().size());
//  _areaCache.reserve(_map->size());
}

void PoiPolygonInfoCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

void PoiPolygonInfoCache::clear()
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
  _areaCache.clear();
  _numAddressesCache.clear();
}

void PoiPolygonInfoCache::printCacheInfo()
{
  // Add one for the address cache on AddressScoreExtractor.
  LOG_VARD(_numCacheHitsByCacheType.size());
  LOG_DEBUG("POI/Polygon caches used: " <<  (_numCacheHitsByCacheType.size() + 1));
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
  const QString line =
    QString("%1:\t%2 hits     entries: %3")
      .arg("address")
      .arg(StringUtils::formatLargeNumber(AddressScoreExtractor::getNumAddressCacheHits()))
      .arg(StringUtils::formatLargeNumber(AddressScoreExtractor::getAddressCacheSize()));
  LOG_DEBUG(line);
}

void PoiPolygonInfoCache::_incrementCacheHitCount(const QString& cacheTypeKey)
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

void PoiPolygonInfoCache::_incrementCacheSizeCount(const QString& cacheTypeKey)
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

std::shared_ptr<geos::geom::Geometry> PoiPolygonInfoCache::_getGeometry(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>>::const_iterator itr =
    _geometryCache.find(element->getElementId());
  if (itr != _geometryCache.end())
  {
    _incrementCacheHitCount("geometry");
    return itr.value();
  }

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
  if (newGeom.get() &&
      QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY"))
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE("Invalid element passed: " << newGeom->toString());
      _badGeomCount++;
    }
    newGeom.reset();
  }
  _geometryCache[element->getElementId()] = newGeom;
  _incrementCacheSizeCount("geometry");
  return newGeom;
}

bool PoiPolygonInfoCache::polyContainsPoint(const ConstElementPtr& poly, const ConstElementPtr& point)
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

//  const QString key =
//    poly->getElementId().toString() % ";" % point->getElementId().toString();
//  QHash<QString, bool>::const_iterator itr = _elementContainsCache.find(key);
//  if (itr != _elementContainsCache.end())
//  {
//    _incrementCacheHitCount("contains");
//    const bool contains = itr.value();
//    LOG_TRACE(
//      "Found cached contains: " << contains << " for poly: " << poly->getElementId() <<
//      " and poi: " << point->getElementId() << ".");
//    return contains;
//  }

  std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
  std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
  bool contains = false;
  if (polyGeom && pointGeom)
  {
    contains = polyGeom->contains(pointGeom.get());
    LOG_TRACE(
      "Calculated contains: " << contains << " for poly: " << poly->getElementId() <<
      " and poi: " << point->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate contains for poly: " << poly->getElementId() <<
      " and poi: " << point->getElementId() << ".");
  }
  //_elementContainsCache[key] = contains;
  //_incrementCacheSizeCount("contains");
  return contains;
}

bool PoiPolygonInfoCache::isType(const ConstElementPtr& element, const QString& type)
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

  // A re-design is needed to make this a little less maintenance prone...possiby add custom
  // schema lookup files for poi/poly?
  bool isType = false;
  const QString typeForComparison = type.toLower();
  if (typeForComparison == QLatin1String("natural"))
  {
    isType = PoiPolygonSchema::isNatural(element);
  }
  else if (typeForComparison == QLatin1String("park"))
  {
    isType = PoiPolygonSchema::isPark(element);
  }
  else if (typeForComparison == QLatin1String("parking"))
  {
    isType = PoiPolygonSchema::isParking(element);
  }
  else if (typeForComparison == QLatin1String("parkish"))
  {
    isType = PoiPolygonSchema::isParkish(element);
  }
  else if (typeForComparison == QLatin1String("playground"))
  {
    isType = PoiPolygonSchema::isPlayground(element);
  }
  else if (typeForComparison == QLatin1String("restaurant"))
  {
    isType = PoiPolygonSchema::isRestaurant(element);
  }
  else if (typeForComparison == QLatin1String("religion"))
  {
    isType = PoiPolygonSchema::isReligion(element);
  }
  else if (typeForComparison == QLatin1String("restroom"))
  {
    isType = PoiPolygonSchema::isRestroom(element);
  }
  else if (typeForComparison == QLatin1String("school"))
  {
    isType = PoiPolygonSchema::isSchool(element);
  }
  else if (typeForComparison == QLatin1String("specificschool"))
  {
    isType = PoiPolygonSchema::isSpecificSchool(element);
  }
  else if (typeForComparison == QLatin1String("sport"))
  {
    isType = PoiPolygonSchema::isSport(element);
  }
  else
  {
    throw IllegalArgumentException("Invalid type passed to PoiPolygonInfoCache::isType: " + type);
  }
  _isTypeCache[key] = isType;
  _incrementCacheSizeCount("isType");
  return isType;
}

bool PoiPolygonInfoCache::hasCriterion(const ConstElementPtr& element, const QString& criterionClassName)
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

  ElementCriterionPtr crit = _getCrit(criterionClassName);
  const bool hasCrit = crit->isSatisfied(element);
  _hasCriterionCache[key] = hasCrit;
  _incrementCacheSizeCount("hasCrit");
  return hasCrit;
}

ElementCriterionPtr PoiPolygonInfoCache::_getCrit(const QString& criterionClassName)
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

  ElementCriterionPtr crit =
    ElementCriterionPtr(
      Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
  if (!crit)
  {
    throw IllegalArgumentException(
      "Invalid criterion passed to PoiPolygonInfoCache::hasCriterion: " + criterionClassName);
  }
  _criterionCache[criterionClassName] = crit;
  return crit;
}

bool PoiPolygonInfoCache::hasMoreThanOneType(const ConstElementPtr& element)
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

  const bool hasMoreThanOneType = PoiPolygonSchema::hasMoreThanOneType(element);
  _hasMoreThanOneTypeCache[element->getElementId()] = hasMoreThanOneType;
  _incrementCacheSizeCount("hasMoreThanOneType");
  return hasMoreThanOneType;
}

bool PoiPolygonInfoCache::hasRelatedType(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

//  QHash<ElementId, bool>::const_iterator itr =
//    _hasRelatedTypeCache.find(element->getElementId());
//  if (itr != _hasRelatedTypeCache.end())
//  {
//    _incrementCacheHitCount("hasRelatedType");
//    return itr.value();
//  }

  const bool hasRelatedType = PoiPolygonSchema::hasRelatedType(element);
  //_hasRelatedTypeCache[element->getElementId()] = hasRelatedType;
  //_incrementCacheSizeCount("hasRelatedType");
  return hasRelatedType;
}

double PoiPolygonInfoCache::getReviewDistance(const ConstElementPtr& element,
                                          const Tags& polyTags,
                                          const double reviewDistanceThresholdDefault)
{
  QHash<ElementId, double>::const_iterator itr = _reviewDistanceCache.find(element->getElementId());
  if (itr != _reviewDistanceCache.end())
  {
    _incrementCacheHitCount("reviewDistance");
    double distance = itr.value();
    LOG_TRACE("Found review distance: " << distance << " for: " << element->getElementId());
    return distance;
  }
  else
  {
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
    _reviewDistanceCache[element->getElementId()] = distance;
    _incrementCacheSizeCount("reviewDistance");
    return distance;
  }
}

bool PoiPolygonInfoCache::inBuildingCategory(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

//  QHash<ElementId, bool>::const_iterator itr =
//    _inBuildingCategoryCache.find(element->getElementId());
//  if (itr != _inBuildingCategoryCache.end())
//  {
//    _incrementCacheHitCount("inBuildingCategory");
//    return itr.value();
//  }

  const bool inBuildingCategory =
    OsmSchema::getInstance().getCategories(element->getTags()).intersects(
      OsmSchemaCategory::building());
  //_inBuildingCategoryCache[element->getElementId()] = inBuildingCategory;
  //_incrementCacheSizeCount("inBuildingCategory");
  return inBuildingCategory;
}

int PoiPolygonInfoCache::numAddresses(const ConstElementPtr& element)
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

  const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);
  _numAddressesCache[element->getElementId()] = numAddresses;
  _incrementCacheSizeCount("numAddresses");
  return numAddresses;
}

bool PoiPolygonInfoCache::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
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

//  const QString key = parent->getElementId().toString() % ";" % memberId.toString();
//  QHash<QString, bool>::const_iterator itr = _containsMemberCache.find(key);
//  if (itr != _containsMemberCache.end())
//  {
//    _incrementCacheHitCount("containsMember");
//    return itr.value();
//  }

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
  //_containsMemberCache[key] = containsMember;
  //_incrementCacheSizeCount("containsMember");
  return containsMember;
}

bool PoiPolygonInfoCache::elementIntersectsElement(
  const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  if (!element1 || !element2)
  {
    // TODO: add message
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

  std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1);
  std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2);
  bool intersects = false;
  if (geom1 && geom2)
  {
    intersects = geom1->intersects(geom2.get());
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate intersects for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }
  _elementIntersectsCache[key1] = intersects;
  _incrementCacheSizeCount("intersects");
  return intersects;
}

double PoiPolygonInfoCache::getPolyToPointDistance(
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

//  const QString key = point->getElementId().toString() % ";" % poly->getElementId().toString();
//  QHash<QString, double>::const_iterator itr = _elementDistanceCache.find(key);
//  if (itr != _elementDistanceCache.end())
//  {
//    _incrementCacheHitCount("polyToPointDistance");
//    double distance = itr.value();
//    LOG_TRACE(
//      "Found cached distance: " << distance << " for poly: " << poly->getElementId() <<
//      " and poi: " << point->getElementId() << ".");
//    return distance;
//  }

  double distance = -1.0;

  std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
  std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
  if (polyGeom && pointGeom)
  {
    distance = polyGeom->distance(pointGeom.get());
    LOG_TRACE(
      "Calculated distance: " << distance << " for poly: " << poly->getElementId() <<
      " and poi: " << point->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate distance for poly: " << poly->getElementId() <<
      " and poi: " << point->getElementId() << ".");
  }

  //_elementDistanceCache[key] = distance;
  //_incrementCacheSizeCount("polyToPointDistance");

  return distance;
}

double PoiPolygonInfoCache::getArea(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException();
  }

//  QHash<ElementId, double>::const_iterator itr = _areaCache.find(element->getElementId());
//  if (itr != _areaCache.end())
//  {
//    _incrementCacheHitCount("area");
//    double area = itr.value();
//    LOG_TRACE("Found cached area: " << area << " for: " << element->getElementId());
//    return area;
//  }

  std::shared_ptr<geos::geom::Geometry> geom = _getGeometry(element);;
  double area = -1.0;
  if (geom)
  {
    area = geom->getArea();
  }
  else
  {
    LOG_TRACE("Unable to calculate area for: " << element->getElementId() << ".");
  }
  //_areaCache[element->getElementId()] = area;
  //_incrementCacheSizeCount("area");;
  return area;
}

}
