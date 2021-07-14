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

#include "DuplicateNodeRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/index/ClosePointHash.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/elements/TagUtils.h>
#include <hoot/core/schema/ExactTagDifferencer.h>
#include <hoot/core/conflate/ConflateUtils.h>

// Qt
#include <QTime>

// Standard
#include <iostream>

// TGS
#include <tgs/StreamUtils.h>
#include <tgs/RStarTree/HilbertRTree.h>

using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DuplicateNodeRemover)

double calcDistanceSquared(const NodePtr& n1, const NodePtr& n2)
{
  double dx = n1->getX() - n2->getX();
  double dy = n1->getY() - n2->getY();
  return dx * dx + dy * dy;
}

DuplicateNodeRemover::DuplicateNodeRemover(Meters distanceThreshold) :
_ignoreStatus(false)
{
  _distance = distanceThreshold;
  if (_distance < 0.0)
  {
    // This default value was found experimentally with the building data from #3446 and #3495 and
    // validated with JOSM validation. Further tweaks may be required to the default value for other
    // datasets.
    _distance = ConfigOptions().getDuplicateNodeRemoverDistanceThreshold();
    if (_distance <= 0.0)
    {
      throw IllegalArgumentException(
        "Nearby node merging distance must be greater than zero. Distance specified: " +
        QString::number(_distance));
    }
  }
}

void DuplicateNodeRemover::apply(std::shared_ptr<OsmMap>& map)
{
  QTime time;
  time.start();

  std::shared_ptr<OsmMap> wgs84;
  std::shared_ptr<OsmMap> planar;

  if (MapProjector::isGeographic(map))
  {
    wgs84 = map;
    planar = std::make_shared<OsmMap>();
    MapProjector::projectToPlanar(planar);
  }
  else
  {
    planar = map;
  }

  ClosePointHash cph(_distance);

  int startNodeCount = 0;
  const NodeMap& nodes = planar->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    const NodePtr& n = it->second;
    if (!n)
    {
      continue;
    }

    // We don't need to check for existence of the nodes parent here, b/c if it ends up being a dupe
    // we'll replace it with another node instead of removing it from the map.
    cph.addPoint(n->getX(), n->getY(), n->getId());
    startNodeCount++;

    if (startNodeCount % 100000 == 0)
    {
      PROGRESS_INFO(
        "\tInitialized " << StringUtils::formatLargeNumber(startNodeCount) << " nodes / " <<
        StringUtils::formatLargeNumber(nodes.size()) << " for duplicate removal.");
    }
  }
  LOG_VART(cph.size());

  double distanceSquared = _distance * _distance;
  int processedCount = 0;
  cph.resetIterator();
  ExactTagDifferencer tagDiff;

  while (cph.next())
  {
    const std::vector<long>& v = cph.getMatch();

    for (size_t i = 0; i < v.size(); i++)
    {
      const long matchIdI = v[i];

      if (!map->containsNode(matchIdI)) continue;

      for (size_t j = 0; j < v.size(); j++)
      {
        bool replace = false;
        double calcdDistanceSquared = -1.0;
        const long matchIdJ = v[j];

        if (matchIdI != matchIdJ && map->containsNode(matchIdJ))
        {
          const NodePtr& n1 = planar->getNode(matchIdI);
          const NodePtr& n2 = planar->getNode(matchIdJ);
          LOG_VART(n1);
          LOG_VART(n2);

          if ((n1->getStatus() == n2->getStatus()) || _ignoreStatus)
          {
            calcdDistanceSquared = calcDistanceSquared(n1, n2);
            LOG_VART(distanceSquared);
            LOG_VART(calcdDistanceSquared);
            if (distanceSquared > calcdDistanceSquared)
            {
              // Since this class operates on elements with generic types, an additional check must
              // be performed here during conflation to enure we don't modify any element not
              // associated with an active conflate matcher in the current conflation
              // configuration.
              if (_conflateInfoCache &&
                  (!_conflateInfoCache->elementCanBeConflatedByActiveMatcher(n1, className()) ||
                   !_conflateInfoCache->elementCanBeConflatedByActiveMatcher(n2, className())))
              {
                LOG_TRACE(
                  "Skipping processing of " << n1->getElementId() << " and " <<
                  n2->getElementId() << " at least one of them cannot be conflated by any " <<
                  "actively configured conflate matcher...");
                replace = false;
              }
              else
              {
                replace = true;
              }
              LOG_VART(replace);

              LOG_VART(tagDiff.diff(map, n1, n2));
              if (replace && tagDiff.diff(map, n1, n2) != 0.0)
              {
                LOG_TRACE(
                  "Skipping merge for " << n1->getElementId() << " and " << n2->getElementId() <<
                  " due to tag diff.");
                replace = false;
              }

              LOG_VART(replace);
              if (replace)
              {
                LOG_TRACE(
                  "Merging nodes: " << ElementId(ElementType::Node, matchIdJ) << " and " <<
                  ElementId(ElementType::Node, matchIdI) << "...");
                map->replaceNode(matchIdJ, matchIdI);
                _numAffected++;
              }
            }
          }
        }

        // custom logging for debugging purposes
        if (Log::getInstance().getLevel() <= Log::Trace)
        {
          if (calcdDistanceSquared != -1.0)
          {
            _logMergeResult(
              matchIdI, matchIdJ, map, replace, std::sqrt(distanceSquared),
              std::sqrt(calcdDistanceSquared));
          }
          else
          {
            _logMergeResult(matchIdI, matchIdJ, map, replace);
          }
        }
      }
    }

    processedCount++;
    if (processedCount % 10000 == 0)
    {
      PROGRESS_INFO(
        "\tMerged " << StringUtils::formatLargeNumber(_numAffected) << " node groups from " <<
        StringUtils::formatLargeNumber(startNodeCount) << " total nodes.");
    }
  }
}

bool DuplicateNodeRemover::_passesLogMergeFilter(
  const long nodeId1, const long nodeId2, const OsmMapPtr& map) const
{
  // can add various filtering criteria here for debugging purposes...

  QStringList kvps;
  kvps.append("OBJECTID=168008");
  kvps.append("OBJECTID=76174");

  std::set<ElementId> wayIdsOwning1;
  const std::set<long> waysOwning1 = WayUtils::getContainingWayIds(nodeId1, map);
  for (std::set<long>::const_iterator it = waysOwning1.begin(); it != waysOwning1.end(); ++it)
  {
    wayIdsOwning1.insert(ElementId(ElementType::Way, *it));
  }
  if (TagUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning1, map))
  {
    return true;
  }

  std::set<ElementId> wayIdsOwning2;
  const std::set<long> waysOwning2 = WayUtils::getContainingWayIds(nodeId2, map);
  for (std::set<long>::const_iterator it = waysOwning2.begin(); it != waysOwning2.end(); ++it)
  {
    wayIdsOwning2.insert(ElementId(ElementType::Way, *it));
  }
  if (TagUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning2, map))
  {
    return true;
  }

  return false;
}

void DuplicateNodeRemover::_logMergeResult(
  const long nodeId1, const long nodeId2, const OsmMapPtr& map, const bool replaced,
  const double distance, const double calcdDistance) const
{
  if (_passesLogMergeFilter(nodeId1, nodeId2, map))
  {
    QString msg = "merging nodes: ";
    if (!replaced)
    {
      msg.prepend("not ");
    }
    msg += QString::number(nodeId1) + " and " + QString::number(nodeId2);
    if (calcdDistance != -1.0)
    {
      msg +=
        " at a distance of: " + QString::number(calcdDistance) +
        " where the distance threshold is: " + QString::number(distance);
    }
    msg += "...";
    LOG_TRACE(msg);
    LOG_TRACE(
      "Node " << nodeId1 << " belongs to ways: " <<
      WayUtils::getContainingWayIds(nodeId1, map));
    LOG_TRACE(
      "Node " << nodeId2 << " belongs to ways: " <<
      WayUtils::getContainingWayIds(nodeId2, map));
    LOG_VART(WayUtils::nodesAreContainedInTheSameWay(nodeId1, nodeId2, map));
  }
}

void DuplicateNodeRemover::removeNodes(
  std::shared_ptr<OsmMap> map, Meters distanceThreshold, const bool ignoreStatus)
{
  DuplicateNodeRemover dupeRemover(distanceThreshold);
  dupeRemover.setIgnoreStatus(ignoreStatus);
  dupeRemover.apply(map);
}

}
