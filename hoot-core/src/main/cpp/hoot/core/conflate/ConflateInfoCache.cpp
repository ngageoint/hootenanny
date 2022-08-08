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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ConflateInfoCache.h"

// geos
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/Factory.h>

// Std
#include <float.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

ConflateInfoCache::ConflateInfoCache(const ConstOsmMapPtr& map) :
_map(map),
_cachingEnabled(true),
_elementIntersectsCache(CACHE_SIZE_DEFAULT),
_hasCriterionCache(CACHE_SIZE_DEFAULT),
_numAddressesCache(CACHE_SIZE_DEFAULT),
_conflatableElementCache(CACHE_SIZE_DEFAULT)
{
  setConfiguration(conf());
}

void ConflateInfoCache::setConfiguration(const Settings& conf)
{
  _addressParser.setConfiguration(conf);

  const int maxCacheSize = ConfigOptions(conf).getConflateInfoMaxSizePerCache();
  if (maxCacheSize > 0)
  {
    _elementIntersectsCache.setMaxCost(maxCacheSize);
    _hasCriterionCache.setMaxCost(maxCacheSize);
    _numAddressesCache.setMaxCost(maxCacheSize);
    _geometryCache =
      std::make_shared<Tgs::LruCache<ElementId, std::shared_ptr<geos::geom::Geometry>>>(
        maxCacheSize);
    _conflatableElementCache.setMaxCost(maxCacheSize);
  }
  else
  {
    _cachingEnabled = false;
  }
}

void ConflateInfoCache::clear()
{
  if (_cachingEnabled)
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

    _conflatableElementCache.clear();
    _conflatableCritActiveCache.clear();
    _activeMatchCreators.clear();
  }
}

void ConflateInfoCache::printCacheInfo()
{
  if (_cachingEnabled)
  {
    // Add one for the address cache on AddressScoreExtractor.
    LOG_VARD(_numCacheHitsByCacheType.size());
    LOG_DEBUG("Conflate caches used: " <<  (_numCacheHitsByCacheType.size() + 1));
    for (QMap<QString, int>::const_iterator numCacheHitsByCacheTypeItr =
           _numCacheHitsByCacheType.begin();
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
  if (_cachingEnabled && _geometryCache->get(element->getElementId(), cachedVal))
  {
    _incrementCacheHitCount("geometry");
    return cachedVal;
  }

  std::shared_ptr<geos::geom::Geometry> newGeom;
  QString errorMsg =
    "Feature passed to ConflateInfoCache caused topology exception on conversion to a geometry: ";
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
      (newGeom->isEmpty() ||
       QString::fromStdString(newGeom->toString()).toUpper().contains("EMPTY")))
  {
    if (_badGeomCount <= Log::getWarnMessageLimit())
    {
      LOG_TRACE("Invalid element passed: " << newGeom->toString());
      _badGeomCount++;
    }
    newGeom.reset();
  }

  if (_cachingEnabled)
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

  if (_cachingEnabled)
  {
    const int* cachedVal = _numAddressesCache[element->getElementId()];
    if (cachedVal != nullptr)
    {
      _incrementCacheHitCount("numAddresses");
      return *cachedVal;
    }
  }

  const int numAddresses = _addressParser.numAddressesRecursive(element, *_map);

  if (_cachingEnabled)
  {
    _numAddressesCache.insert(element->getElementId(), new int(numAddresses));
    _incrementCacheSizeCount("numAddresses");
  }

  return numAddresses;
}

bool ConflateInfoCache::containsMember(const ConstElementPtr& parent, const ElementId& memberId) const
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

  if (_cachingEnabled)
  {
    key1 = element1->getElementId().toString() % ";" % element2->getElementId().toString();
    key2 = element2->getElementId().toString() % ";" % element1->getElementId().toString();

    const bool* cachedVal = _elementIntersectsCache[key1];
    if (cachedVal != nullptr)
    {
      _incrementCacheHitCount("intersects");
      const bool intersects = *cachedVal;
      LOG_TRACE("Found cached intersects: " << intersects << " for key: " << key1);
      return intersects;
    }
    cachedVal = _elementIntersectsCache[key2];
    if (cachedVal != nullptr)
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

  if (_cachingEnabled)
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

  if (_cachingEnabled)
  {
    key = element->getElementId().toString() % ";" % criterionClassName;
    const bool* cachedVal = _hasCriterionCache[key];
    if (cachedVal != nullptr)
    {
      _incrementCacheHitCount("hasCrit");
      return *cachedVal;
    }
  }

  ElementCriterionPtr crit = _getCrit(criterionClassName);
  const bool hasCrit = crit->isSatisfied(element);

  if (_cachingEnabled)
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

  if (_cachingEnabled)
  {
    QHash<QString, ElementCriterionPtr>::const_iterator itr =
      _criterionCache.find(criterionClassName);
    if (itr != _criterionCache.end())
    {
      return itr.value();
    }
  }

  ElementCriterionPtr crit =
    Factory::getInstance().constructObject<ElementCriterion>(criterionClassName);
  if (!crit)
  {
    throw IllegalArgumentException(
      "Invalid criterion passed to ConflateInfoCache::hasCriterion: " + criterionClassName);
  }

  std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
    std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
  LOG_VART(mapConsumer.get());
  if (mapConsumer)
  {
    mapConsumer->setOsmMap(_map.get());
  }

  if (_cachingEnabled)
  {
    _criterionCache[criterionClassName] = crit;
  }

  return crit;
}

bool ConflateInfoCache::elementCanBeConflatedByActiveMatcher(
  const ConstElementPtr& element, const QString& caller)
{
  if (_cachingEnabled)
  {
    // Check the cache to see if the element can be conflated first.
    const bool* cachedVal = _conflatableElementCache[element->getElementId()];
    if (cachedVal != nullptr)
    {
      _incrementCacheHitCount("conflatable");
      const bool conflatable = *cachedVal;
      LOG_TRACE("Found cached conflatable: " << conflatable << " for: " << element->getElementId());
      return conflatable;
    }
  }

  LOG_VART(element->getElementId());

  // Get all the configured matchers.
  if (_activeMatchCreators.empty())
  {
    _activeMatchCreators = MatchFactory::getInstance().getCreators();
  }
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = _activeMatchCreators.begin();
       itr != _activeMatchCreators.end(); ++itr)
  {
    // Get the element criterion this matcher uses for matching elements.
    std::shared_ptr<MatchCreator> activeMatchCreator = *itr;
    LOG_VART(activeMatchCreator->getName());
    const QStringList supportedCriteriaClassNames = activeMatchCreator->getCriteria();
    for (int i = 0; i < supportedCriteriaClassNames.size(); i++)
    {
      const QString criterionClassName = supportedCriteriaClassNames.at(i);
      LOG_VART(criterionClassName);
      ElementCriterionPtr crit = _getCrit(criterionClassName);
      std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
        std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);

      if (conflatableCrit)
      {
        // If any matcher's crit matches the element, return true.
        if (crit->isSatisfied(element))
        {
          if (_cachingEnabled)
          {
            _conflatableElementCache.insert(element->getElementId(), new bool(true));
          }
          return true;
        }
        // Otherwise, also check for the crit's conflate "child" criteria
        // (e.g. RailwayWayNodeCriterion for RailwayCriterion).
        else if (!conflatableCrit->getChildCriteria().isEmpty())
        {
          const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
          for (int j = 0; j < childCritClassNames.size(); j++)
          {
            const QString childCritClassName = childCritClassNames.at(j);
            ElementCriterionPtr childCrit = _getCrit(childCritClassName);
            // These don't inherit from ConflatableElementCriterion, since matchers look for
            // their parents only. So, don't require that it casts to ConflatableElementCriterion.
            if (childCrit)
            {
              LOG_VART(childCrit->isSatisfied(element));
              // If any matcher's crit matches the element, return true.
              if (childCrit->isSatisfied(element))
              {
                if (_cachingEnabled)
                {
                  _conflatableElementCache.insert(element->getElementId(), new bool(true));
                }
                return true;
              }
            }
          }
        }
      }
    }
  }

  if (_cachingEnabled)
  {
    _conflatableElementCache.insert(element->getElementId(), new bool(false));
  }
  if (Log::getInstance().getLevel() <= Log::Trace)
  {
    const QString mostSpecificType = OsmSchema::getInstance().mostSpecificType(element->getTags());
    const QString name = element->getTags().getName();
    LOG_TRACE(
      element->getElementId() << " rejected as conflatable element. Name: " << name <<
      ", most specific type: " << mostSpecificType << ", caller: " << caller);
    if (mostSpecificType.trimmed().isEmpty())
    {
      LOG_VART(element);
    }
  }
  return false;
}

bool ConflateInfoCache::elementCriterionInUseByActiveMatcher(const QString& criterionClassName)
{
  LOG_VART(criterionClassName);

  // Check the in use cache first.
  if (_cachingEnabled && _conflatableCritActiveCache.contains(criterionClassName))
  {
    return _conflatableCritActiveCache[criterionClassName];
  }

  // Get all the configured matchers.
  if (_activeMatchCreators.empty())
  {
    _activeMatchCreators = MatchFactory::getInstance().getCreators();
  }
  for (std::vector<std::shared_ptr<MatchCreator>>::const_iterator itr = _activeMatchCreators.begin();
       itr != _activeMatchCreators.end(); ++itr)
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
        if (_cachingEnabled)
        {
          _conflatableCritActiveCache[matcherCriterionClassName] = true;
        }
        return true;
      }
      else
      {
        ElementCriterionPtr crit = _getCrit(matcherCriterionClassName);
        std::shared_ptr<ConflatableElementCriterion> conflatableCrit =
          std::dynamic_pointer_cast<ConflatableElementCriterion>(crit);
        if (conflatableCrit && !conflatableCrit->getChildCriteria().isEmpty())
        {
          // Also, check for conflate "child" criteria (e.g. RailwayWayNodeCriterion). These
          // don't inherit from ConflatableElementCriterion, since matchers look for their
          // parents only.
          const QStringList childCritClassNames = conflatableCrit->getChildCriteria();
          for (int j = 0; j < childCritClassNames.size(); ++j)
          {
            const QString childCritClassName = childCritClassNames.at(j);
            LOG_VART(childCritClassName);
            // No need to instantiate this child crit. Just check for a crit class name match.
            if (childCritClassName == criterionClassName)
            {
              if (_cachingEnabled)
              {
                _conflatableCritActiveCache[childCritClassName] = true;
              }
              return true;
            }
          }
        }
      }
    }
  }

  LOG_TRACE("Conflate crit rejected: " << criterionClassName);
  if (_cachingEnabled)
  {
    _conflatableCritActiveCache[criterionClassName] = false;
  }
  return false;
}

}
