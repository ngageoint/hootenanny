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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "SuperfluousWayRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>
#include <hoot/core/conflate/ConflateUtils.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SuperfluousWayRemover)

void SuperfluousWayRemover::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);

  const QSet<QString> excludeIdsStrs = opts.getSuperfluousWayRemoverExcludeIds().toSet();
  for (QSet<QString>::const_iterator it = excludeIdsStrs.begin(); it != excludeIdsStrs.end(); ++it)
  {
    bool ok = false;
    _excludeIds.insert((*it).toLong(&ok));
    if (!ok)
    {
      throw IllegalArgumentException(
        QString("Invalid element exclude ID passed to ") + className());
    }
  }
  LOG_VARD(_excludeIds.size());
}

void SuperfluousWayRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _removeWays(map);
}

long SuperfluousWayRemover::removeWays(std::shared_ptr<OsmMap>& map)
{
  SuperfluousWayRemover wayRemover;
  LOG_INFO(wayRemover.getInitStatusMessage());
  wayRemover.apply(map);
  LOG_DEBUG(wayRemover.getCompletedStatusMessage());
  return wayRemover.getNumFeaturesAffected();
}

void SuperfluousWayRemover::_removeWays(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _numExplicitlyExcluded = 0;
  std::shared_ptr<ElementToRelationMap> e2r = map->getIndex().getElementToRelationMap();
  LOG_VART(e2r->size());

  // make a copy of the ways to avoid issues when removing.
  const WayMap ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    _numProcessed++;
    if (!w)
    {
      continue;
    }
    else if (_conflateInfoCache &&
             !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(w, className()))
    {
      LOG_TRACE(
        "Skipping processing of " << w->getElementId() << " as it cannot be conflated by any " <<
        "actively configured conflate matcher...");
      continue;
    }
    LOG_VART(w->getElementId());

    if (_excludeIds.contains(w->getId()))
    {
      _numExplicitlyExcluded++;
      continue;
    }

    bool same = true;
    const vector<long>& nodeIds = w->getNodeIds();
    LOG_VART(nodeIds);
    if (!nodeIds.empty())
    {
      long firstId = nodeIds[0];
      for (size_t i = 1; i < nodeIds.size(); i++)
      {
        if (nodeIds[i] != firstId)
        {
          same = false;
        }
      }
    }
    LOG_VART(same);

    const bool inRelation = !e2r->getRelationByElement(w).empty();
    LOG_VART(inRelation);
    LOG_VART(w->getTags().size());

    // if all the nodes in a way are the same or there are zero nodes
    if ((same || w->getTags().empty()) && !inRelation)
    {  
      LOG_TRACE("Removing superflous way: " << w->getElementId() << "...");
      RemoveWayByEid::removeWayFully(map, w->getId());
      _numAffected++;
    }
  }
}

QStringList SuperfluousWayRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(LinearCriterion::className());
  criteria.append(PolygonCriterion::className());
  return criteria;
}

}
