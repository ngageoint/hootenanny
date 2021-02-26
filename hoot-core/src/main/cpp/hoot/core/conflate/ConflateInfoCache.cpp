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
#include "ConflateInfoCache.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>

// Std
#include <float.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

ConflateInfoCache::ConflateInfoCache(const ConstOsmMapPtr& map) :
_map(map),
_cacheEnabled(true),
_elementIntersectsCache(CACHE_SIZE_DEFAULT),
_hasCriterionCache(CACHE_SIZE_DEFAULT),
_numAddressesCache(CACHE_SIZE_DEFAULT)
{
  _geometryCache.reset(
    new Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>(CACHE_SIZE_DEFAULT));
}

void ConflateInfoCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);

  const int maxCacheSize = ConfigOptions(conf).getPoiPolygonMaxSizePerCache();
  if (maxCacheSize > 0)
  {
    _elementIntersectsCache.setMaxCost(maxCacheSize);
    _hasCriterionCache.setMaxCost(maxCacheSize);
    _numAddressesCache.setMaxCost(maxCacheSize);
    _geometryCache.reset(
        new Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>(maxCacheSize));
  }
  else
  {
    _cacheEnabled = false;
  }
}

void ConflateInfoCache::clear()
{
  if (_cacheEnabled)
  {
    LOG_DEBUG("Clearing cache...");

    _numCacheHitsByCacheType.clear();
    _numCacheEntriesByCacheType.clear();

    _hasCriterionCache.clear();
    _criterionCache.clear();
    _geometryCache->clear();
    _numAddressesCache.clear();
    _elementIntersectsCache.clear();
    _numAddressesCache.clear();
  }
}

void ConflateInfoCache::printCacheInfo()
{
  if (_cacheEnabled)
  {
    // Add one for the address cache on AddressScoreExtractor.
    LOG_VARD(_numCacheHitsByCacheType.size());
    LOG_DEBUG("Conflate caches used: " <<  (_numCacheHitsByCacheType.size() + 1));
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

void ConflateInfoCache::_incrementCacheHitCount(const QString& cacheTypeKey)
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

void ConflateInfoCache::_incrementCacheSizeCount(const QString& cacheTypeKey)
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

std::shared_ptr<geos::geom::Geometry> ConflateInfoCache::_getGeometry(
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

bool ConflateInfoCache::elementContains(const ConstElementPtr& containingElement,
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

int ConflateInfoCache::numAddresses(const ConstElementPtr& element)
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

bool ConflateInfoCache::containsMember(const ConstElementPtr& parent, const ElementId& memberId)
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

bool ConflateInfoCache::elementsIntersect(
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

double ConflateInfoCache::getDistance(
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

double ConflateInfoCache::getArea(const ConstElementPtr& element)
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

bool ConflateInfoCache::hasCriterion(const ConstElementPtr& element,
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

ElementCriterionPtr ConflateInfoCache::_getCrit(const QString& criterionClassName)
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

bool ConflateInfoCache::elementCanBeConflatedByActiveMatcher(const ConstElementPtr& element)
{
  LOG_VART(element->getElementId());
  LOG_VART(_conflatableElementCache.contains(element->getElementId()));

  // Check the element can be conflated cache first.
  if (_conflatableElementCache.contains(element->getElementId()))
  {
    return _conflatableElementCache[element->getElementId()];
  }

  LOG_VART(element->getElementId());

  // Get all the configured matchers.
  const std::vector<std::shared_ptr<MatchCreator>> activeMatchCreators =
    MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = activeMatchCreators.begin();
       itr != activeMatchCreators.end(); ++itr)
  {
    // Get the element criterion this matcher uses for matching elements.
    std::shared_ptr<MatchCreator> activeMatchCreator = *itr;
    LOG_VART(activeMatchCreator->getName());
    const QStringList supportedCriteriaClassNames = activeMatchCreator->getCriteria();
    for (int i = 0; i < supportedCriteriaClassNames.size(); i++)
    {
      const QString criterionClassName = supportedCriteriaClassNames.at(i);
      LOG_VART(criterionClassName);

      ElementCriterionPtr crit;
      std::shared_ptr<ConflatableElementCriterion> conflatableCrit;
      ElementCriterionPtr childCrit;
      LOG_VART(_conflatableCritCache.contains(criterionClassName));
      if (_conflatableCritCache.contains(criterionClassName)) // Check the cache for the criterion.
      {
        crit = _conflatableCritCache[criterionClassName];
        // We didn't cache it unless it was a conflatable crit in the first place, so this cast
        // will always work.
        conflatableCrit = std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
      }
      else
      {
        // Create the crit.
        crit.reset(
          Factory::getInstance().constructObject<ElementCriterion>(criterionClassName));
        conflatableCrit = std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
        if (conflatableCrit)
        {
          // All ElementCriterion that are map consumers inherit from ConstOsmMapConsumer, so this
          // works.
          std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
            std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
          if (mapConsumer)
          {
            mapConsumer->setOsmMap(_map.get());
          }

          // Crit creation can be expensive, so cache those created.
          _conflatableCritCache[criterionClassName] = crit;
        }
      }

      // Crit creation can be expensive, so cache those created.(crit->isSatisfied(element));
      // If any matcher's crit matches the element, return true.
      if (crit->isSatisfied(element))
      {
        _conflatableElementCache[element->getElementId()] = true;
        return true;
      }
      // Otherwise, also check for the crit's conflate "child" criteria
      // (e.g. RailwayWayNodeCriterion for RailwayCriterion).
      else if (!conflatableCrit->getChildCriteria().isEmpty())
      {
        const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
        for (int i = 0; i < childCritClassNames.size(); i++)
        {
          const QString childCritClassName = childCritClassNames.at(i);
          // Crit creation can be expensive, so cache those created.(childCritClassName);
          LOG_VART(_conflatableCritCache.contains(childCritClassName));
          // Check the cache for the child criterion.
          if (_conflatableCritCache.contains(childCritClassName))
          {
            childCrit = _conflatableCritCache[childCritClassName];
          }
          else
          {
            // Create the child crit.
            childCrit.reset(
              Factory::getInstance().constructObject<ElementCriterion>(childCritClassName));
            if (childCrit)
            {
              std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
                std::dynamic_pointer_cast<ConstOsmMapConsumer>(childCrit);
              if (mapConsumer)
              {
                mapConsumer->setOsmMap(_map.get());
              }

              // Crit creation can be expensive, so cache those created.
              _conflatableCritCache[childCritClassName] = childCrit;
            }
          }

          // These don't inherit from ConflatableElementCriterion, since matchers look for
          // their parents only. So, don't require that it casts to ConflatableElementCriterion.
          if (childCrit)
          {
            LOG_VART(childCrit->isSatisfied(element));
            // If any matcher's crit matches the element, return true.
            if (childCrit->isSatisfied(element))
            {
              _conflatableElementCache[element->getElementId()] = true;
              return true;
            }
          }
        }
      }
    }
  }

  _conflatableElementCache[element->getElementId()] = false;
  LOG_WARN(
    element->getElementId() << " rejected as conflatable element. Most specific type: " <<
    OsmSchema::getInstance().mostSpecificType(element->getTags()));
  return false;
}

bool ConflateInfoCache::elementCriterionInUseByActiveMatcher(const QString& criterionClassName)
{
  LOG_VART(criterionClassName);

  // Check the in use cache first.
  if (_conflatableCritActiveCache.contains(criterionClassName))
  {
    return _conflatableCritActiveCache[criterionClassName];
  }

  // Get all the configured matchers.
  const std::vector<std::shared_ptr<MatchCreator>> activeMatchCreators =
    MatchFactory::getInstance().getCreators();
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = activeMatchCreators.begin();
       itr != activeMatchCreators.end(); ++itr)
  {
    // Get the element criterion this matcher uses for matching elements.
    std::shared_ptr<MatchCreator> activeMatchCreator = *itr;
    const QStringList supportedCriteriaClassNames = activeMatchCreator->getCriteria();
    for (int i = 0; i < supportedCriteriaClassNames.size(); i++)
    {
      const QString matcherCriterionClassName = supportedCriteriaClassNames.at(i);
      LOG_VART(matcherCriterionClassName);
      if (matcherCriterionClassName == criterionClassName)
      {
        _conflatableCritActiveCache[matcherCriterionClassName] = true;
        return true;
      }
      else
      {
        ElementCriterionPtr crit;
        std::shared_ptr<ConflatableElementCriterion> conflatableCrit;
        // Check the cache for the criterion.
        if (_conflatableCritCache.contains(matcherCriterionClassName))
        {
          crit = _conflatableCritCache[matcherCriterionClassName];
          // We didn't cache it unless it was a conflatable crit in the first place, so this cast
          // will always work.
          conflatableCrit = std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
        }
        else
        {
          // Create the crit.
          crit.reset(
            Factory::getInstance().constructObject<ElementCriterion>(matcherCriterionClassName));
          conflatableCrit = std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
          if (conflatableCrit)
          {
            // All ElementCriterion that are map consumers inherit from ConstOsmMapConsumer, so this
            // works.
            std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
              std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
            LOG_VART(mapConsumer.get());
            if (mapConsumer)
            {
              mapConsumer->setOsmMap(_map.get());
            }

            // Crit creation can be expensive, so cache those created.
            _conflatableCritCache[matcherCriterionClassName] = crit;
          }
        }

        if (crit && conflatableCrit && !conflatableCrit->getChildCriteria().isEmpty())
        {
          // Also, check for conflate "child" criteria (e.g. RailwayWayNodeCriterion). These
          // don't inherit from ConflatableElementCriterion, since matchers look for their
          // parents only.
          const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
          for (int i = 0; i < childCritClassNames.size(); i++)
          {
            const QString childCritClassName = childCritClassNames.at(i);
            LOG_VART(childCritClassName);
            // No need to instantiate this child crit. Just check for a crit class name match.
            if (childCritClassName == criterionClassName)
            {
              _conflatableCritActiveCache[childCritClassName] = true;
              return true;
            }
          }
        }
      }
    }
  }

  LOG_WARN("Conflate crit rejected: " << criterionClassName);
  _conflatableCritActiveCache[criterionClassName] = false;
  return false;
}

}
