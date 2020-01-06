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
#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

// Std
#include <float.h>

namespace hoot
{

PoiPolygonCache::PoiPolygonCache(const ConstOsmMapPtr& map) :
_map(map)
{
}

PoiPolygonCache::~PoiPolygonCache()
{
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
}

void PoiPolygonCache::printCacheInfo()
{
  LOG_DEBUG(
    "POI/Polygon caches used: " << StringUtils::formatLargeNumber(_numCacheHitsByCacheType.size()));
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

std::shared_ptr<geos::geom::Geometry> PoiPolygonCache::_getGeometry(ConstElementPtr element)
{
  if (!element)
  {
    return std::shared_ptr<geos::geom::Geometry>();
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
    const bool contains = itr.value();
    LOG_TRACE("Found cached contains: " << contains << " for key: " << key);
    return contains;
  }
  else
  {
    std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
    std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
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
    return itr.value();
  }
  else
  {
    // A redesign is needed to make this less maintenance prone.
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
      throw IllegalArgumentException("Invalid type passed to PoiPolygonCache::isType: " + type);
    }
    _isTypeCache[key] = isType;
    _incrementCacheSizeCount("isType");
    return isType;
  }
}

bool PoiPolygonCache::hasCriterion(ConstElementPtr element, const QString& criterionClassName)
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
    if (!crit)
    {
      throw IllegalArgumentException(
        "Invalid criterion passed to PoiPolygonCache::hasCriterion: " + criterionClassName);
    }
    _criterionCache[criterionClassName] = crit;
    return crit;
  }
}

bool PoiPolygonCache::elementIntersectsElement(ConstElementPtr element1, ConstElementPtr element2)
{
  if (!element1 || !element2)
  {
    return false;
  }

  std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1);
  std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2);
  bool intersects = false;
  if (geom1 && geom2)
  {
    intersects = geom1->intersects(geom2.get());
  }
  return intersects;
}

double PoiPolygonCache::getPolyToPointDistance(ConstWayPtr poly, ConstNodePtr point)
{   
  if (!poly || !point)
  {
    return -1.0;
  }

  const QString key = point->getElementId().toString() + ";" + poly->getElementId().toString();
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

    if (polyContainsPoi(poly, point)) // TODO: remove
    {
      distance = 0.0;
    }
    else
    {
      std::shared_ptr<geos::geom::Geometry> polyGeom = _getGeometry(poly);
      std::shared_ptr<geos::geom::Geometry> pointGeom = _getGeometry(point);
      if (polyGeom && pointGeom)
      {
        distance = polyGeom->distance(pointGeom.get());
        LOG_TRACE("Calculated distance: " << distance << " for key: " << key);
      }
      else
      {
        LOG_TRACE("Unable to calculate distance for key: " << key);
      }
    }

    _elementDistanceCache[key] = distance;
    _incrementCacheSizeCount("polyToPointDistance");

    return distance;
  }
}

double PoiPolygonCache::getPolyToPointDistance(ConstRelationPtr poly, ConstNodePtr point)
{
  double smallestDistance = DBL_MAX;
  std::set<ElementId> polyWayIds = poly->getWayMemberIds();
  for (std::set<ElementId>::const_iterator polyWayIdItr = polyWayIds.begin();
       polyWayIdItr != polyWayIds.end(); ++polyWayIdItr)
  {
    const double distance = getPolyToPointDistance(_map->getWay(*polyWayIdItr), point);
    if (distance != -1.0 && distance < smallestDistance)
    {
      smallestDistance = distance;
      //LOG_VART(smallestDistance);
    }
  }
  if (smallestDistance != DBL_MAX)
  {
    //LOG_VART(smallestDistance);
    return smallestDistance;
  }
  else
  {
    return -1.0;
  }
}

double PoiPolygonCache::getArea(ConstElementPtr element)
{
  //LOG_VART(element.get());
  if (!element)
  {
    return -1.0;
  }

  std::shared_ptr<geos::geom::Geometry> geom = _getGeometry(element);
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
