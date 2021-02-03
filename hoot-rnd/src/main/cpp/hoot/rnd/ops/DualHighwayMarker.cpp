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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DualHighwayMarker.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/SpatialIndexer.h>
#include <hoot/core/criterion/ArbitraryCriterion.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/OneWayCriterion.h>
#include <hoot/core/algorithms/extractors/ParallelScoreExtractor.h>
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/schema/MetadataTags.h>

// tgs
#include <tgs/RStarTree/MemoryPageStore.h>

// Boost
#include <boost/bind.hpp>

// Standard
#include <functional>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DualHighwayMarker)

DualHighwayMarker::DualHighwayMarker() :
_minParallelScore(0.9),
_taskStatusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
}

void DualHighwayMarker::setConfiguration(const Settings& conf)
{
  ConfigOptions confOpts(conf);
  _minParallelScore = confOpts.getDualHighwayMarkerParallelScoreThreshold;
}

Meters DualHighwayMarker::_getSearchRadius(const ConstElementPtr& e) const
{
  return e->getCircularError();
}

bool DualHighwayMarker::_isMatchCandidate(ConstElementPtr element) const
{
  if (!element)
  {
    return false;
  }
  // TODO
  return OneWayCriterion().isSatisfied(element) && HighwayCriterion(_map).isSatisfied(element);
}

void DualHighwayMarker::_createIndex()
{
  LOG_DEBUG("Creating spatial index...");

  // No tuning done - see #3054
  std::shared_ptr<Tgs::MemoryPageStore> mps(new Tgs::MemoryPageStore(728));
  _index.reset(new Tgs::HilbertRTree(mps, 2));

  // Only index elements that satisfy isMatchCandidate.
  std::function<bool (ConstElementPtr e)> f =
    std::bind(&DualHighwayMarker::_isMatchCandidate, this, std::placeholders::_1);
  std::shared_ptr<ArbitraryCriterion> pCrit(new ArbitraryCriterion(f));

  SpatialIndexer v(
    _index, _indexToEid, pCrit,
    std::bind(&DualHighwayMarker::_getSearchRadius, this, std::placeholders::_1), _map);
  _map->visitRo(v);
  v.finalizeIndex();
}

void DualHighwayMarker::apply(const OsmMapPtr& map)
{
  _numAffected = 0;
  _numProcessed = 0;
  _map = map;
  _createIndex();

  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator waysItr = ways.begin(); waysItr != ways.end(); ++waysItr)
  {
    WayPtr way = waysItr->second;
    LOG_VART(way->getElementId());

    if (!way || !_isMatchCandidate(way))
    {
      continue;
    }

    std::shared_ptr<geos::geom::Envelope> env(way->getEnvelope(_map));
    LOG_VART(env);
    const std::set<ElementId> neighborIds =
      SpatialIndexer::findNeighbors(*env, _index, _indexToEid, _map, ElementType::Way, false);
    LOG_VART(neighborIds.size());

    for (std::set<ElementId>::const_iterator neighborsItr = neighborIds.begin();
         neighborsItr != neighborIds.end(); ++neighborsItr)
    {
      LOG_VART(*neighborsItr);
      ElementPtr neighbor = _map->getElement(*neighborsItr);
      if (neighbor) // No match candidate check needed here, as the index takes care of that for us.
      {
        const double parallelScore = ParallelScoreExtractor().extract(*_map, way, neighbor);
        LOG_VART(parallelScore);
        const bool roadsInSameDirection =
          DirectionFinder::isSimilarDirection2(_map, way, std::dynamic_pointer_cast<Way>(neighbor));
        LOG_VART(roadsInSameDirection);

        // TODO: explain
        if (parallelScore >= _minParallelScore && !roadsInSameDirection)
        {
          if (way->getTags()[MetadataTags::HootDualHighway()] != "yes")
          {
            way->getTags().set(MetadataTags::HootDualHighway(), "yes");
            _numAffected++;
          }
          if (neighbor->getTags()[MetadataTags::HootDualHighway()] != "yes")
          {
            neighbor->getTags().set(MetadataTags::HootDualHighway(), "yes");
            _numAffected++;
          }
        }
      }
    }
  }
  LOG_VARD(_numAffected);
}

QStringList DualHighwayMarker::getCriteria() const
{
  QStringList criteria;
  criteria.append(HighwayCriterion::className());
  return criteria;
}

}
