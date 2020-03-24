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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "RoadsCrossingPolysReviewMarker.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/criterion/OrCriterion.h>
#include <hoot/core/elements/OsmUtils.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/elements/Way.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, RoadsCrossingPolysReviewMarker)

RoadsCrossingPolysReviewMarker::RoadsCrossingPolysReviewMarker()
{
}

void RoadsCrossingPolysReviewMarker::setConfiguration(const Settings& conf)
{
  _polyFilterList = ConfigOptions(conf).getRoadsCrossingPolysReviewMarkerPolyFilter();
}

void RoadsCrossingPolysReviewMarker::apply(const OsmMapPtr& map)
{
  _map = map;
  _markedRoads.clear();

  _createPolyFilter(_polyFilterList);
  assert(_polyFilter.get());

  ReviewMarker reviewMarker;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    WayPtr way = it->second;
    LOG_VART(way->getElementId());

    // for all roads that we haven't already marked for review
    if (!_markedRoads.contains(way->getElementId()) && HighwayCriterion(_map).isSatisfied(way))
    {
      std::shared_ptr<geos::geom::Envelope> env(way->getEnvelope(_map));
      LOG_VART(env);
      // get all nearby polys that match our poly crit
      const std::set<ElementId> neighborIdsSet =
        SpatialIndexer::findNeighbors(
          *env, _getIndex(), _indexToEid, _map, ElementType::Way, false);
      LOG_VART(neighborIdsSet.size());
      for (std::set<ElementId>::const_iterator neighborIdsItr = neighborIdsSet.begin();
           neighborIdsItr != neighborIdsSet.end(); ++neighborIdsItr)
      {
        const ElementId neighborId = *neighborIdsItr;
        LOG_VART(neighborId);
        ConstElementPtr neighbor = _map->getElement(neighborId);
        // If the road intersects one of them, flag it for review.
        if (neighbor && !HighwayCriterion(_map).isSatisfied(neighbor) &&
            OsmUtils::elementsIntersect(way, neighbor, _map))
        {
          LOG_TRACE("Marking " << way->getElementId() << " for review...");
          reviewMarker.mark(
            _map, way, "Road crossing polygon", HighwayMatch::getHighwayMatchName(), 1.0);
          _markedRoads.insert(way->getElementId());
        }
      }
    }
  }
}

ElementCriterionPtr RoadsCrossingPolysReviewMarker::_kvpStringToTagCrit(const QString& kvpStr)
{
  ElementCriterionPtr crit;

  const QStringList filterStrParts = kvpStr.split("=");
  LOG_VART(filterStrParts);
  if (filterStrParts.size() != 2)
  {
    throw IllegalArgumentException("TODO");
  }
  if (filterStrParts[1] == "*")
  {
    crit.reset(new TagKeyCriterion(filterStrParts[0]));
  }
  else
  {
    crit.reset(new TagCriterion(filterStrParts[0], filterStrParts[1]));
  }

  return crit;
}

void RoadsCrossingPolysReviewMarker::_createPolyFilter(const QStringList& polyFilterList)
{
  if (polyFilterList.isEmpty())
  {
    throw IllegalArgumentException("TODO");
  }

  LOG_DEBUG("Creating poly filter...");

  if (polyFilterList.size() == 1)
  {
    const QString filterStr = polyFilterList.at(0);
    LOG_VART(filterStr);
    if (filterStr.startsWith("hoot::"))
    {
      _polyFilter.reset(
        Factory::getInstance().constructObject<ElementCriterion>(filterStr.trimmed()));
    }
    else if (filterStr.contains("="))
    {
      _polyFilter = _kvpStringToTagCrit(filterStr);
    }
    else
    {
      throw IllegalArgumentException("TODO");
    }
  }
  else
  {
    std::shared_ptr<OrCriterion> orCrit(new OrCriterion());
    for (int i = 0; i < polyFilterList.size(); i++)
    {
      const QString filterStr = polyFilterList.at(i);

      std::shared_ptr<ElementCriterion> crit;
      if (filterStr.startsWith("hoot::"))
      {
        crit.reset(Factory::getInstance().constructObject<ElementCriterion>(filterStr.trimmed()));
      }
      else if (filterStr.contains("="))
      {
        crit = _kvpStringToTagCrit(filterStr);
      }
      else
      {
        throw IllegalArgumentException("TODO");
      }

      orCrit->addCriterion(crit);
    }
    _polyFilter = orCrit;
  }
  LOG_VARD(_polyFilter);
}

std::shared_ptr<Tgs::HilbertRTree>& RoadsCrossingPolysReviewMarker::_getIndex()
{
  if (!_index)
  {
    LOG_STATUS("Creating roads crossing polys index...");

    // create an index for all roads and all polys that satisfy our crit within the default
    // search radius

    // No tuning was done, I just copied these settings from OsmMapIndex.
    // 10 children - 368 - see #3054
    std::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
    _index.reset(new Tgs::HilbertRTree(mps, 2));

    // Only index elements satisfy isMatchCandidate(e)
    std::function<bool (ConstElementPtr e)> f =
      std::bind(&RoadsCrossingPolysReviewMarker::_isMatchCandidate, this, std::placeholders::_1);
    std::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

    SpatialIndexer v(
      _index, _indexToEid, pCrit,
      std::bind(&RoadsCrossingPolysReviewMarker::_getSearchRadius, this, std::placeholders::_1),
      _map);

    _map->visitRo(v);
    v.finalizeIndex();

    LOG_STATUS(
      "Roads crossing polys feature index created with " <<
      StringUtils::formatLargeNumber(v.getSize()) << " elements.");
  }

  return _index;
}

Meters RoadsCrossingPolysReviewMarker::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool RoadsCrossingPolysReviewMarker::_isMatchCandidate(ConstElementPtr element)
{
  // special tag is currently only used by roundabout processing to mark temporary features
  if (element->getTags().contains(MetadataTags::HootSpecial()))
  {
    return false;
  }
  return HighwayCriterion(_map).isSatisfied(element) || _polyFilter->isSatisfied(element);
}

}
