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
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/criterion/BuildingWayNodeCriterion.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

PoiPolygonCachePtr PoiPolygonCache::_theInstance;

PoiPolygonCache::PoiPolygonCache()
{
}

PoiPolygonCache::~PoiPolygonCache()
{
}

const PoiPolygonCachePtr& PoiPolygonCache::getInstance(const ConstOsmMapPtr& map)
{
  if (_theInstance.get() == 0)
  {
    _theInstance.reset(new PoiPolygonCache());
    if (map)
    {
      _theInstance->setOsmMap(map);
    }
    _theInstance->setConfiguration(conf());
  }
  return _theInstance;
}

void PoiPolygonCache::setOsmMap(const ConstOsmMapPtr& map)
{
  _map = map;

  _clear();
  _init();
}

void PoiPolygonCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);
}

void PoiPolygonCache::_clear()
{
  _numCacheHitsByCacheType.clear();
  _cacheSizeByCacheType.clear();

  _elementDistanceCache.clear();
  _elementAreaCache.clear();
  _elementContainsCache.clear();
  _elementIntersectsCache.clear();
  _elementAngleHistogramCache.clear();
  _elementOverlapCache.clear();
  _nameScoreCache.clear();
  _typeScoreCache.clear();
  _isTypeCache.clear();
  _hasCriterionCache.clear();
  _specificSchoolMatchCache.clear();
  _hasTypeCache.clear();
  _hasMoreThanOneTypeCache.clear();
  _numAddressesCache.clear();
  _isInCategoryCache.clear();
  _matchingWayIdCache.clear();
  _poiNeighborCloserCache.clear();
  _criterionCache.clear();
  _isInCategoryCache.clear();
  _geometryCache.clear();
  _lineStringCache.clear();
}

void PoiPolygonCache::_init()
{
  assert(_map);

  _elementDistanceCache.reserve(_map->size());
  _elementAreaCache.reserve(_map->size());
  _elementContainsCache.reserve(_map->size());
  _elementIntersectsCache.reserve(_map->size());
  _elementAngleHistogramCache.reserve(_map->size());
  _elementOverlapCache.reserve(_map->size());
  _nameScoreCache.reserve(_map->size());
  _typeScoreCache.reserve(_map->size());
  _isTypeCache.reserve(_map->size());
  _hasCriterionCache.reserve(_map->size());
  _specificSchoolMatchCache.reserve(_map->size());
  _hasTypeCache.reserve(_map->size());
  _hasMoreThanOneTypeCache.reserve(_map->size());
  _numAddressesCache.reserve(_map->size());
  _isInCategoryCache.reserve(_map->size());
  _matchingWayIdCache.reserve(_map->size());
  _poiNeighborCloserCache.reserve(_map->size());
  _criterionCache.reserve(_map->size());
  _isInCategoryCache.reserve(_map->size());
  _geometryCache.reserve(_map->size());
  _lineStringCache.reserve(_map->size());
}

void PoiPolygonCache::printCacheInfo()
{
  // TODO: change back to debug
  LOG_INFO("POI/Polygon cache info:");
  LOG_INFO("Number of caches used: " << _numCacheHitsByCacheType.size());
  for (QMap<QString, int>::const_iterator numCacheHitsByCacheTypeItr = _numCacheHitsByCacheType.begin();
       numCacheHitsByCacheTypeItr != _numCacheHitsByCacheType.end(); ++numCacheHitsByCacheTypeItr)
  {
    const QString line =
      QString("%1:\t%2 hits     size: %3")
        .arg(numCacheHitsByCacheTypeItr.key())
        .arg(StringUtils::formatLargeNumber(numCacheHitsByCacheTypeItr.value()))
        .arg(
          StringUtils::formatLargeNumber(
            _cacheSizeByCacheType[numCacheHitsByCacheTypeItr.key()]));
    LOG_INFO(line);
  }
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
  if (!_cacheSizeByCacheType.contains(cacheTypeKey))
  {
    _cacheSizeByCacheType[cacheTypeKey] = 1;
  }
  else
  {
    _cacheSizeByCacheType[cacheTypeKey] = _cacheSizeByCacheType[cacheTypeKey] + 1;
  }
}


std::shared_ptr<geos::geom::Geometry> PoiPolygonCache::_getGeometry(ConstElementPtr element)
{
  if (!element)
  {
    return std::shared_ptr<geos::geom::Geometry>();
  }

  QHash<ElementId, std::shared_ptr<geos::geom::Geometry>>::const_iterator itr =
    _geometryCache.find(element->getElementId());
  if (itr != _geometryCache.end())
  {
    _incrementCacheHitCount("geometry");
    LOG_TRACE("Found geometry in cache for: " << element->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No geometry in cache for: " << element->getElementId());
    std::shared_ptr<geos::geom::Geometry> newGeom;
    try
    {
      newGeom = ElementConverter(_map).convertToGeometry(element);
    }
    catch (const geos::util::TopologyException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(
          "Feature passed to PoiPolygonReviewReducer caused topology exception on conversion to a " <<
          "geometry: " << element->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    LOG_VART(newGeom.get());
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
    LOG_VART(newGeom.get());
    _geometryCache[element->getElementId()] = newGeom;
    LOG_TRACE("Updated geometry cache for: " << element->getElementId());
    _incrementCacheSizeCount("geometry");
    return newGeom;
  }
}

std::shared_ptr<geos::geom::LineString> PoiPolygonCache::_getLineString(ConstWayPtr poly)
{
  if (!poly)
  {
    return std::shared_ptr<geos::geom::LineString>();
  }

  QHash<ElementId, std::shared_ptr<geos::geom::LineString>>::const_iterator itr =
    _lineStringCache.find(poly->getElementId());
  if (itr != _lineStringCache.end())
  {
    _incrementCacheHitCount("linestring");
    LOG_TRACE("Found line string in cache for: " << poly->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No line string in cache for: " << poly->getElementId());
    std::shared_ptr<geos::geom::LineString> newGeom;
    try
    {
      newGeom = ElementConverter(_map).convertToLineString(poly);
    }
    catch (const geos::util::TopologyException& e)
    {
      if (_badGeomCount <= Log::getWarnMessageLimit())
      {
        LOG_TRACE(
          "Feature passed to PoiPolygonReviewReducer caused topology exception on conversion to a " <<
          "geometry: " << poly->toString() << "\n" << e.what());
        _badGeomCount++;
      }
    }
    LOG_VART(newGeom.get());
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
    LOG_VART(newGeom.get());
    _lineStringCache[poly->getElementId()] = newGeom;
    LOG_TRACE("Updated line string cache for: " << poly->getElementId());
    _incrementCacheSizeCount("linestring");
    return newGeom;
  }
}

double PoiPolygonCache::getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point)
{
  if (!poly || !point)
  {
    return -1.0;
  }

  const QString key =
    poly->getElementId().toString() + ";" + point->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _elementDistanceCache.find(key);
  if (itr != _elementDistanceCache.end())
  {
    _incrementCacheHitCount("distance");
    LOG_TRACE("Found distance of " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No distance in cache for: " << key);
    std::shared_ptr<geos::geom::LineString> polyGeom = _getLineString(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
    LOG_VART(polyGeom.get());
    LOG_VART(pointGeom.get());
    double distance = -1.0;
    if (polyGeom && pointGeom)
    {
      distance = polyGeom->distance(pointGeom.get());
    }
    _elementDistanceCache[key] = distance;
    LOG_TRACE("Updated distance cache with: " << distance << " for: " << key);
    _incrementCacheSizeCount("distance");
    return distance;
  }
}

double PoiPolygonCache::getArea(ConstElementPtr poly)
{
  if (!poly)
  {
    return -1.0;
  }

  QHash<ElementId, double>::const_iterator itr = _elementAreaCache.find(poly->getElementId());
  if (itr != _elementAreaCache.end())
  {
    _incrementCacheHitCount("area");
    LOG_TRACE("Found area of " << itr.value() << " in cache for: " << poly->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No area in cache for: " << poly->getElementId());
    std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
    LOG_VART(polyGeom.get());
    double area = -1.0;
    if (polyGeom)
    {
      area = polyGeom->getArea();
    }
    _elementAreaCache[poly->getElementId()] = area;
    LOG_TRACE("Updated area cache with: " << area << " for: " << poly->getElementId());
    _incrementCacheSizeCount("area");
    return area;
  }
}

bool PoiPolygonCache::polyContainsPoi(ConstWayPtr poly, ConstNodePtr point)
{
  if (!poly || !point)
  {
    return false;
  }

  const QString key =
    poly->getElementId().toString() + ";" + point->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _elementContainsCache.find(key);
  if (itr != _elementContainsCache.end())
  {
    _incrementCacheHitCount("contains");
    LOG_TRACE("Found contains=" << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No contains in cache for: " << key);
    std::shared_ptr<geos::geom::LineString> polyGeom = _getLineString(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
    LOG_VART(polyGeom.get());
    LOG_VART(pointGeom.get());
    bool contains = false;
    if (polyGeom && pointGeom)
    {
      contains = polyGeom->contains(pointGeom.get());
    }
    _elementContainsCache[key] = contains;
    LOG_TRACE("Updated contains cache with: " << contains << " for: " << key);
    _incrementCacheSizeCount("contains");
    return contains;
  }
}

bool PoiPolygonCache::elementIntersectsElement(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return false;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _elementIntersectsCache.find(key);
  if (itr != _elementIntersectsCache.end())
  {
    _incrementCacheHitCount("intersects");
    LOG_TRACE("Found intersects=" << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No intersects in cache for: " << key);
    std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1);
    std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2);
    LOG_VART(geom1.get());
    LOG_VART(geom2.get());
    bool intersects = false;
    if (geom1 && geom2)
    {
      intersects = geom1->intersects(geom2.get());
    }
    _elementIntersectsCache[key] = intersects;
    LOG_TRACE("Updated intersects cache with: " << intersects << " for: " << key);
    _incrementCacheSizeCount("intersects");
    return intersects;
  }
}

double PoiPolygonCache::getAngleHistogramVal(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _elementAngleHistogramCache.find(key);
  if (itr != _elementAngleHistogramCache.end())
  {
    _incrementCacheHitCount("angleHist");
    LOG_TRACE("Found angle hist: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No angle hist in cache for: " << key);
    const double angleHistVal = AngleHistogramExtractor().extract(*_map, element1, element2);
    _elementAngleHistogramCache[key] = angleHistVal;
    LOG_TRACE("Updated angle hist cache with: " << angleHistVal << " for: " << key);
    _incrementCacheSizeCount("angleHist");
    return angleHistVal;
  }
}

double PoiPolygonCache::getOverlapVal(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _elementOverlapCache.find(key);
  if (itr != _elementOverlapCache.end())
  {
    _incrementCacheHitCount("overlap");
    LOG_TRACE("Found overlap: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No overlap in cache for: " << key);
    const double overlapVal = OverlapExtractor().extract(*_map, element1, element2);
    _elementOverlapCache[key] = overlapVal;
    LOG_TRACE("Updated overlap cache with: " << overlapVal << " for: " << key);
    _incrementCacheSizeCount("overlap");
    return overlapVal;
  }
}

double PoiPolygonCache::getNameScore(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _nameScoreCache.find(key);
  if (itr != _nameScoreCache.end())
  {
    _incrementCacheHitCount("name");
    LOG_TRACE("Found name score: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No name score in cache for: " << key);
    const double nameScore = PoiPolygonNameScoreExtractor().extract(*_map, element1, element2);
    _nameScoreCache[key] = nameScore;
    LOG_TRACE("Updated name score cache with: " << nameScore << " for: " << key);
    _incrementCacheSizeCount("name");
    return nameScore;
  }
}

double PoiPolygonCache::getTypeScore(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return -1.0;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, double>::const_iterator itr = _typeScoreCache.find(key);
  if (itr != _typeScoreCache.end())
  {
    _incrementCacheHitCount("type");
    LOG_TRACE("Found type score: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No type score in cache for: " << key);
    const double typeScore = PoiPolygonTypeScoreExtractor().extract(*_map, element1, element2);
    _typeScoreCache[key] = typeScore;
    LOG_TRACE("Updated type score cache with: " << typeScore << " for: " << key);
    _incrementCacheSizeCount("type");
    return typeScore;
  }
}

bool PoiPolygonCache::isType(ConstElementPtr element, const QString& type)
{
  if (!element || type.trimmed().isEmpty())
  {
    return false;
  }

  const QString key = element->getElementId().toString() + ";" + type;
  QHash<QString, bool>::const_iterator itr = _isTypeCache.find(key);
  if (itr != _isTypeCache.end())
  {
    _incrementCacheHitCount("isType");
    LOG_TRACE("Found is type: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No is type in cache for: " << key);
    // A redesign is need to make this less maintenance prone.
    bool isType = false;
    if (type == "park")
    {
      isType = PoiPolygonTypeScoreExtractor::isPark(element);
    }
    else if (type == "sport")
    {
      isType = PoiPolygonTypeScoreExtractor::isSport(element);
    }
    else if (type == "restaurant")
    {
      isType = PoiPolygonTypeScoreExtractor::isRestaurant(element);
    }
    else if (type == "specificSchool")
    {
      isType = PoiPolygonTypeScoreExtractor::isSpecificSchool(element);
    }
    else if (type == "natural")
    {
      isType = PoiPolygonTypeScoreExtractor::isNatural(element);
    }
    else if (type == "restroom")
    {
      isType = PoiPolygonTypeScoreExtractor::isRestroom(element);
    }
    else if (type == "parking")
    {
      isType = PoiPolygonTypeScoreExtractor::isParking(element);
    }
    else if (type == "school")
    {
      isType = PoiPolygonTypeScoreExtractor::isSchool(element);
    }
    else if (type == "playground")
    {
      isType = PoiPolygonTypeScoreExtractor::isPlayground(element);
    }
    else if (type == "parkish")
    {
      isType = PoiPolygonTypeScoreExtractor::isParkish(element);
    }
    else
    {
      throw IllegalArgumentException("TODO");
    }
    _isTypeCache[key] = isType;
    LOG_TRACE("Updated isType cache with: " << isType << " for: " << key);
    _incrementCacheSizeCount("isType");
    return isType;
  }
}

bool PoiPolygonCache::hasCrit(ConstElementPtr element, const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    return false;
  }

  const QString key = element->getElementId().toString() + ";" + criterionClassName;
  QHash<QString, bool>::const_iterator itr = _hasCriterionCache.find(key);
  if (itr != _hasCriterionCache.end())
  {
    _incrementCacheHitCount("hasCrit");
    LOG_TRACE("Found is has crit: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No has crit in cache for: " << key);
    ElementCriterionPtr crit = _getCrit(criterionClassName);
    const bool hasCrit = crit->isSatisfied(element);

    _hasCriterionCache[key] = hasCrit;
    LOG_TRACE("Updated has crit cache with: " << hasCrit << " for: " << key);
    _incrementCacheSizeCount("hasCrit");
    return hasCrit;
  }
}

ElementCriterionPtr PoiPolygonCache::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    return ElementCriterionPtr();
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
    _criterionCache[criterionClassName] = crit;
    return crit;
  }
}

bool PoiPolygonCache::hasType(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  QHash<ElementId, bool>::const_iterator itr = _hasTypeCache.find(element->getElementId());
  if (itr != _hasTypeCache.end())
  {
    _incrementCacheHitCount("hasType");
    LOG_TRACE("Found has type: " << itr.value() << " in cache for: " << element->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No has type in cache for: " << element->getElementId());
    const bool hasType = PoiPolygonTypeScoreExtractor::hasType(element);
    _hasTypeCache[element->getElementId()] = hasType;
    LOG_TRACE("Updated has type cache with: " << hasType << " for: " << element->getElementId());
    _incrementCacheSizeCount("hasType");
    return hasType;
  }
}

bool PoiPolygonCache::polyHasPoiNeighborCloserThanPoi(ConstWayPtr poly, ConstNodePtr poi,
                                                      const std::set<ElementId>& poiNeighborIds,
                                                      const double poiPolyDistance)
{
  if (!poly || !poi || poiNeighborIds.size() == 0)
  {
    return false;
  }

  const QString key = poly->getElementId().toString() + ";" + poi->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _poiNeighborCloserCache.find(key);
  if (itr != _poiNeighborCloserCache.end())
  {
    _incrementCacheHitCount("hasCloserPoiNeighbor");
    LOG_TRACE("Found closer poi neighbor: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No closer poi neighbor in cache for: " << key);
    bool hasCloserPoiNeighbor = false;
    LOG_VART(poiNeighborIds.size());
    //LOG_VART(_poiNeighborIds);
    for (std::set<ElementId>::const_iterator poiNeighborItr = poiNeighborIds.begin();
         poiNeighborItr != poiNeighborIds.end(); ++poiNeighborItr)
    {
      ConstElementPtr poiNeighbor = _map->getElement(*poiNeighborItr);
      if (poiNeighbor->getElementId() != poi->getElementId())
      {
        long neighborPoiToPolyDist = -1.0;
        neighborPoiToPolyDist =
          getPolyToPointDistance(poly, std::dynamic_pointer_cast<const Node>(poiNeighbor));
        LOG_VART(neighborPoiToPolyDist);
        if (neighborPoiToPolyDist != -1.0 && poiPolyDistance > neighborPoiToPolyDist)
        {
          hasCloserPoiNeighbor = true;
          break;
        }
      }
    }

    _poiNeighborCloserCache[key] = hasCloserPoiNeighbor;
    LOG_TRACE(
      "Updated closer poi neighbor cache with: " << hasCloserPoiNeighbor << " for: " << key);
    _incrementCacheSizeCount("hasCloserPoiNeighbor");
    return hasCloserPoiNeighbor;
  }
}

bool PoiPolygonCache::hasMoreThanOneType(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  QHash<ElementId, bool>::const_iterator itr =
    _hasMoreThanOneTypeCache.find(element->getElementId());
  if (itr != _hasMoreThanOneTypeCache.end())
  {
    _incrementCacheHitCount("hasMoreThanOneType");
    LOG_TRACE("Found has type: " << itr.value() << " in cache for: " << element->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No has more than one type in cache for: " << element->getElementId());
    const bool hasMoreThanOneType = PoiPolygonTypeScoreExtractor::hasMoreThanOneType(element);
    _hasMoreThanOneTypeCache[element->getElementId()] = hasMoreThanOneType;
    LOG_TRACE(
      "Updated has more than one type cache with: " << hasMoreThanOneType << " for: " <<
      element->getElementId());
    _incrementCacheSizeCount("hasMoreThanOneType");
    return hasMoreThanOneType;
  }
}

bool PoiPolygonCache::specificSchoolMatch(ConstElementPtr element1, ConstElementPtr element2)
{
  // We could also check for school type here to move it out of triggersRule.
  if (!element1 || !element2)
  {
    return false;
  }

  const QString key =
    element1->getElementId().toString() + ";" + element2->getElementId().toString();
  QHash<QString, bool>::const_iterator itr = _specificSchoolMatchCache.find(key);
  if (itr != _specificSchoolMatchCache.end())
  {
    _incrementCacheHitCount("specificSchoolMatch");
    LOG_TRACE("Found specific school match:" << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No specific school match in cache for: " << key);
    const bool specificSchoolMatch =
      PoiPolygonTypeScoreExtractor::specificSchoolMatch(element1, element2);
    _specificSchoolMatchCache[key] = specificSchoolMatch;
    LOG_TRACE(
      "Updated specific school match cache with: " << specificSchoolMatch << " for: " << key);
    _incrementCacheSizeCount("specificSchoolMatch");
    return specificSchoolMatch;
  }
}

int PoiPolygonCache::getNumAddresses(ConstElementPtr element)
{
  if (!element)
  {
    return false;
  }

  QHash<ElementId, int>::const_iterator itr = _numAddressesCache.find(element->getElementId());
  if (itr != _numAddressesCache.end())
  {
    _incrementCacheHitCount("numAddresses");
    LOG_TRACE(
      "Found num addresses: " << itr.value() << " in cache for: " << element->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No num addresses in cache for: " << element->getElementId());
    const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);
    _numAddressesCache[element->getElementId()] = numAddresses;
    LOG_TRACE(
      "Updated num addresses cache with: " << numAddresses << " for: " << element->getElementId());
    _incrementCacheSizeCount("numAddresses");
    return numAddresses;
  }
}

bool PoiPolygonCache::hasAddress(ConstElementPtr element)
{
  return getNumAddresses(element) > 0;
}

bool PoiPolygonCache::inCategory(ConstElementPtr element, const OsmSchemaCategory& category)
{
  if (!element || category == OsmSchemaCategory::Empty)
  {
    return false;
  }

  const QString key = element->getElementId().toString() + ";" + category.toString();
  QHash<QString, bool>::const_iterator itr = _isInCategoryCache.find(key);
  if (itr != _isInCategoryCache.end())
  {
    _incrementCacheHitCount("inCategory");
    LOG_TRACE("Found in category: " << itr.value() << " in cache for: " << key);
    return itr.value();
  }
  else
  {
    LOG_TRACE("No in category in cache for: " << key);
    const bool inCategory =
      OsmSchema::getInstance().getCategories(element->getTags()).intersects(category);
    _isInCategoryCache[key] = inCategory;
    LOG_TRACE("Updated in category cache with: " << inCategory << " for: " << key);
    _incrementCacheSizeCount("inCategory");
    return inCategory;
  }
}

long PoiPolygonCache::getMatchingWayId(ConstNodePtr node)
{
  if (!node)
  {
    return 0;
  }

  QHash<ElementId, long>::const_iterator itr = _matchingWayIdCache.find(node->getElementId());
  if (itr != _matchingWayIdCache.end())
  {
    _incrementCacheHitCount("matchingWayId");
    LOG_TRACE("Found matching way id: " << itr.value() << " in cache for: " << node->getElementId());
    return itr.value();
  }
  else
  {
    LOG_TRACE("No matching way id in cache for: " << node->getElementId());
    const long matchingWayId = BuildingWayNodeCriterion(_map).getMatchingWayId(node);
    _matchingWayIdCache[node->getElementId()] = matchingWayId;
    LOG_TRACE("Updated matching way id cache with: " << matchingWayId << " for: " << node->getElementId());
    _incrementCacheSizeCount("matchingWayId");
    return matchingWayId;
  }
}

}
