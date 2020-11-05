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
#include "PoiPolygonInfoCache.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/conflate/poi-polygon/PoiPolygonSchema.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>

// Std
#include <float.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PoiPolygonInfoCache::PoiPolygonInfoCache(const ConstOsmMapPtr& map) :
_map(map),
_cacheEnabled(true),
_elementIntersectsCache(CACHE_SIZE_DEFAULT),
_isTypeCache(CACHE_SIZE_DEFAULT),
_hasCriterionCache(CACHE_SIZE_DEFAULT),
_hasMoreThanOneTypeCache(CACHE_SIZE_DEFAULT),
_numAddressesCache(CACHE_SIZE_DEFAULT),
_reviewDistanceCache(CACHE_SIZE_DEFAULT)
{
  _geometryCache.reset(
    new Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>(CACHE_SIZE_DEFAULT));
}

void PoiPolygonInfoCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);

  const int maxCacheSize = ConfigOptions(conf).getPoiPolygonMaxSizePerCache();
  if (maxCacheSize > 0)
  {
    _elementIntersectsCache.setMaxCost(maxCacheSize);
    _isTypeCache.setMaxCost(maxCacheSize);
    _hasCriterionCache.setMaxCost(maxCacheSize);
    _hasMoreThanOneTypeCache.setMaxCost(maxCacheSize);
    _numAddressesCache.setMaxCost(maxCacheSize);
    _reviewDistanceCache.setMaxCost(maxCacheSize);
    _geometryCache.reset(
        new Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>(maxCacheSize));
  }
  else
  {
    _cacheEnabled = false;
  }
}

void PoiPolygonInfoCache::clear()
{
  if (_cacheEnabled)
  {
    LOG_DEBUG("Clearing cache...");

    _numCacheHitsByCacheType.clear();
    _numCacheEntriesByCacheType.clear();

    _isTypeCache.clear();
    _hasCriterionCache.clear();
    _criterionCache.clear();
    _geometryCache->clear();
    _hasMoreThanOneTypeCache.clear();
    _numAddressesCache.clear();
    _elementIntersectsCache.clear();
    _numAddressesCache.clear();
  }
}

void PoiPolygonInfoCache::printCacheInfo()
{
  if (_cacheEnabled)
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
  else
  {
    LOG_DEBUG("POI/Polygon caching disabled.")
  }
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

/////////////////NOT POI/POLY SPECIFIC///////////////////

std::shared_ptr<geos::geom::Geometry> PoiPolygonInfoCache::_getGeometry(
  const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  std::shared_ptr<geos::geom::Geometry> cachedVal;
  if (_cacheEnabled && _geometryCache->get(element->getElementId(), cachedVal))
  {
    _incrementCacheHitCount("geometry");
    return cachedVal;
  }

  std::shared_ptr<geos::geom::Geometry> newGeom;
  QString errorMsg =
    "Feature passed to PoiPolygonInfoCache caused topology exception on conversion to a geometry: ";
  try
  {
    newGeom = ElementToGeometryConverter(_map).convertToGeometry(element);
  }
  catch (const geos::util::TopologyException& e)
  {
    // try to clean it
    newGeom.reset(GeometryUtils::validateGeometry(newGeom.get()));
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

  if (_cacheEnabled)
  {
    _geometryCache->insert(element->getElementId(), newGeom);
    _incrementCacheSizeCount("geometry");
  }

  return newGeom;
}

bool PoiPolygonInfoCache::elementContains(const ConstElementPtr& containingElement,
                                          const ConstElementPtr& containedElement)
{
  if (!containingElement || !containedElement)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }
  if (containingElement->getElementType() != ElementType::Way &&
      containingElement->getElementType() != ElementType::Relation)
  {
    throw IllegalArgumentException("One of the input elements is of the wrong type.");
  }
  LOG_VART(containedElement->getElementId());
  LOG_VART(containingElement->getElementId());

  std::shared_ptr<geos::geom::Geometry> containingElementGeom = _getGeometry(containingElement);
  std::shared_ptr<geos::geom::Geometry> containedElementGeom = _getGeometry(containedElement);
  bool contains = false;
  if (containingElementGeom && containedElementGeom)
  {
    contains = containingElementGeom->contains(containedElementGeom.get());
    LOG_TRACE(
      "Calculated contains: " << contains << " for containing element: " <<
      containingElement->getElementId() <<
      " and contained element: " << containedElement->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate contains for containing element: " <<
      containingElement->getElementId() << " and contained element: " <<
      containedElement->getElementId() << ".");
  }
  return contains;
}

int PoiPolygonInfoCache::numAddresses(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  if (_cacheEnabled)
  {
    const int* cachedVal = _numAddressesCache[element->getElementId()];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("numAddresses");
      return *cachedVal;
    }
  }

  const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);

  if (_cacheEnabled)
  {
    _numAddressesCache.insert(element->getElementId(), new int(numAddresses));
    _incrementCacheSizeCount("numAddresses");
  }

  return numAddresses;
}

bool PoiPolygonInfoCache::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
{
  if (!parent ||
      (parent->getElementType() != ElementType::Way &&
       parent->getElementType() != ElementType::Relation))
  {
    throw IllegalArgumentException("The parent element is null or of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Way && memberId.getType() != ElementType::Node)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }
  if (parent->getElementType() != ElementType::Relation &&
      memberId.getType() == ElementType::Unknown)
  {
    throw IllegalArgumentException("The inputs are of the wrong element type.");
  }

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
  return containsMember;
}

bool PoiPolygonInfoCache::elementsIntersect(
  const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }

  QString key1;
  QString key2;

  if (_cacheEnabled)
  {
    key1 = element1->getElementId().toString() % ";" % element2->getElementId().toString();
    key2 = element2->getElementId().toString() % ";" % element1->getElementId().toString();

    bool* cachedVal = _elementIntersectsCache[key1];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("intersects");
      const bool intersects = *cachedVal;
      LOG_TRACE("Found cached intersects: " << intersects << " for key: " << key1);
      return intersects;
    }
    cachedVal = _elementIntersectsCache[key2];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("intersects");
      const bool intersects = *cachedVal;
      LOG_TRACE("Found cached intersects: " << intersects << " for key: " << key2);
      return intersects;
    }
  }

  std::shared_ptr<geos::geom::Geometry> geom1 = _getGeometry(element1);
  std::shared_ptr<geos::geom::Geometry> geom2 = _getGeometry(element2);
  bool intersects = false;
  if (geom1 && geom2)
  {
    intersects = geom1->intersects(geom2.get());

    LOG_TRACE(
      "Calculated intersects: " << intersects << " for elements: " <<
      element1->getElementId() << " and " << element2->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate intersects for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }

  if (_cacheEnabled)
  {
    _elementIntersectsCache.insert(key1, new bool(intersects));
    _incrementCacheSizeCount("intersects");
  }

  return intersects;
}

double PoiPolygonInfoCache::getDistance(
  const ConstElementPtr& element1, const ConstElementPtr& element2)
{
  if (!element1 || !element2)
  {
    throw IllegalArgumentException("One of the input elements is null.");
  }
  LOG_VART(element1->getElementId());
  LOG_VART(element2->getElementId());

  double distance = -1.0;

  std::shared_ptr<geos::geom::Geometry> element1Geom = _getGeometry(element1);
  std::shared_ptr<geos::geom::Geometry> element2Geom = _getGeometry(element2);
  if (element1Geom && element2Geom)
  {
    distance = element1Geom->distance(element2Geom.get());
    LOG_TRACE(
      "Calculated distance: " << distance << " for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }
  else
  {
    LOG_TRACE(
      "Unable to calculate distance for: " << element1->getElementId() <<
      " and: " << element2->getElementId() << ".");
  }

  return distance;
}

double PoiPolygonInfoCache::getArea(const ConstElementPtr& element)
{
  if (!element)
  {
    throw IllegalArgumentException("The input element is null.");
  }

  std::shared_ptr<geos::geom::Geometry> geom = _getGeometry(element);
  double area = -1.0;
  if (geom)
  {
    area = geom->getArea();
  }
  else
  {
    LOG_TRACE("Unable to calculate area for: " << element->getElementId() << ".");
  }
  return area;
}

bool PoiPolygonInfoCache::hasCriterion(const ConstElementPtr& element,
                                       const QString& criterionClassName)
{
  if (!element || criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException(
      "The input element is null or the criterion class name is empty.");
  }

  QString key;

  if (_cacheEnabled)
  {
    key = element->getElementId().toString() % ";" % criterionClassName;
    const bool* cachedVal = _hasCriterionCache[key];
    if (cachedVal != 0)
    {
      _incrementCacheHitCount("hasCrit");
      return *cachedVal;
    }
  }

  ElementCriterionPtr crit = _getCrit(criterionClassName);
  const bool hasCrit = crit->isSatisfied(element);

  if (_cacheEnabled)
  {
    _hasCriterionCache.insert(key, new bool(hasCrit));
    _incrementCacheSizeCount("hasCrit");
  }

  return hasCrit;
}

ElementCriterionPtr PoiPolygonInfoCache::_getCrit(const QString& criterionClassName)
{
  if (criterionClassName.trimmed().isEmpty())
  {
    throw IllegalArgumentException("The criterion class name is empty.");
  }

  if (_cacheEnabled)
  {
    QHash<QString, ElementCriterionPtr>::const_iterator itr =
      _criterionCache.find(criterionClassName);
    if (itr != _criterionCache.end())
    {
      return itr.value();
    }
  }

  ElementCriterionPtr crit =
    ElementCriterionPtr(
      Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
  if (!crit)
  {
    throw IllegalArgumentException(
      "Invalid criterion passed to PoiPolygonInfoCache::hasCriterion: " + criterionClassName);
  }

  if (_cacheEnabled)
  {
     _criterionCache[criterionClassName] = crit;
  }

  return crit;
}

/////////////////POI/POLY SPECIFIC/////////////////

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
