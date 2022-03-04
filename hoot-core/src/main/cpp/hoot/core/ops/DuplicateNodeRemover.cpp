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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "DuplicateNodeRemover.h"

// Hoot
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/TagUtils.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/index/ClosePointHash.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/ExactTagDifferencer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QTime>

// TGS
#include <tgs/StreamUtils.h>
#include <tgs/RStarTree/HilbertRTree.h>

#include <unordered_set>

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

DuplicateNodeRemover::DuplicateNodeRemover(Meters distanceThreshold)
  : _distance(distanceThreshold),
    _ignoreStatus(false)
{
  if (_distance < 0.0)
  {
    // This default value was found experimentally with the building data from #3446 and #3495 and
    // validated with JOSM validation. Further tweaks may be required to the default value for other
    // datasets.
    _distance = ConfigOptions().getDuplicateNodeRemoverDistanceThreshold();
    if (_distance <= 0.0)
    {
      throw IllegalArgumentException(
        QString("Nearby node merging distance must be greater than zero. Distance specified: %1").arg(QString::number(_distance)));
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
    planar = std::make_shared<OsmMap>(map);
    MapProjector::projectToPlanar(planar);
  }
  else
    planar = map;

  ClosePointHash cph(_distance);

  int startNodeCount = 0;
  const NodeMap& nodes = planar->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    const NodePtr& n = it->second;
    if (!n)
      continue;

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

  //  Keep a map of the replacements to do all of the replacements at the same time
  std::map<long, long> replacements;
  std::multimap<long, long> reverse_replacements;
  //  Keep track of nodes that have already been deleted so they aren't used as replacements
  std::unordered_set<long> deleted;

  while (cph.next())
  {
    const std::vector<long>& v = cph.getMatch();
    //  Iterate all of the nodes in the match group against all other nodes in the group
    for (auto matchId1 : v)
    {
      if (!map->containsNode(matchId1))
        continue;

      for (auto matchId2 : v)
      {
        bool replace = true;
        double calcdDistanceSquared = -1.0;
        //  Don't compare a node against itself of if the node doesn't exist or won't exist later
        if (matchId1 != matchId2 && map->containsNode(matchId2) &&
            replacements.find(matchId2) == replacements.end() &&
            deleted.find(matchId1) == deleted.end())
        {
          NodePtr n1 = planar->getNode(matchId1);
          NodePtr n2 = planar->getNode(matchId2);
          LOG_VART(n1);
          LOG_VART(n2);
          //  Must have the node to compare the tags
          if ((n1 != nullptr && n2 != nullptr) && (n1->getStatus() == n2->getStatus() || _ignoreStatus))
          {
            calcdDistanceSquared = calcDistanceSquared(n1, n2);
            LOG_VART(distanceSquared);
            LOG_VART(calcdDistanceSquared);
            if (distanceSquared > calcdDistanceSquared)
            {
              // Since this class operates on elements with generic types, an additional check must
              // be performed here during conflation to enure we don't modify any element not
              // associated with an active conflate matcher in the current conflation configuration.
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

              Tags tags1 = n1->getTags();
              tags1.removeMetadata();
              Tags tags2 = n2->getTags();
              tags2.removeMetadata();
              //  Check the tags
              if ((tags1.empty() && tags2.empty()) ||
                  (!tags1.empty() && tags2.empty()))
              {
                //  When both sets of tags are empty or the first set isn't and the second set is, replace
                replace = true;
              }
              else if (tags1.empty() && !tags2.empty())
              {
                //  When the first set of tags is empty but the second set isn't, don't replace the node
                //  because a later iteration of the loop with merge the two nodes in the correct order
                replace = false;
              }
              else if (replace && tagDiff.diff(map, n1, n2) != 0.0)
              {
                //  Both sets of tags aren't empty and the tag differencer score is non-zero they can't be merged
                LOG_VART(tagDiff.diff(map, n1, n2));
                LOG_TRACE(
                  "Skipping merge for " << n1->getElementId() << " and " << n2->getElementId() <<
                  " due to tag diff.");
                replace = false;
              }

              LOG_VART(replace);
              //  Check for already deleted nodes pointing to node 2
              if (replace)
              {
                //  Check for circular deletes
                bool found = replacements.find(matchId1) != replacements.end();
                if (!found || (found && replacements[matchId1] != matchId2))
                {
                  LOG_TRACE("Merging nodes: " << ElementId(ElementType::Node, matchId2) << " and " <<
                            ElementId(ElementType::Node, matchId1) << "...");
                  //  Check if node 1 is already in the replacements as node 2
                  if (reverse_replacements.find(matchId2) != reverse_replacements.end())
                  {
                    std::vector<long> updates;
                    auto range = reverse_replacements.equal_range(matchId2);
                    for (auto it = range.first; it != range.second; ++it)
                    {
                      //  Update the replacements with the new value
                      replacements[it->second] = matchId1;
                      //  Keep track of the values to update in the reverse
                      updates.emplace_back(it->second);
                    }
                    //  Remove all instances of ID2
                    reverse_replacements.erase(matchId2);
                    //  Add all updates back into the reverse list
                    for (auto id : updates)
                      reverse_replacements.emplace(matchId1, id);
                  }
                  //  Add this set to the replacements lists
                  replacements.emplace(matchId2, matchId1);
                  reverse_replacements.emplace(matchId1, matchId2);
                  //  Mark the second element as deleted now
                  deleted.emplace(matchId2);
                  _numAffected++;
                }
              }
            }
          }
        }

        // custom logging for debugging purposes
        if (Log::getInstance().getLevel() <= Log::Trace)
        {
          if (calcdDistanceSquared != -1.0)
          {
            _logMergeResult(matchId1, matchId2, map, replace, std::sqrt(distanceSquared),
                            std::sqrt(calcdDistanceSquared));
          }
          else
            _logMergeResult(matchId1, matchId2, map, replace);
        }
      }
    }

    processedCount++;
    if (processedCount % 10000 == 0)
    {
      PROGRESS_INFO("\tMerged " << StringUtils::formatLargeNumber(_numAffected) <<
                    " node groups from " << StringUtils::formatLargeNumber(startNodeCount) <<
                    " total nodes.");
    }
  }
  //  Reset the CPH before moving on to the replacement in the map
  cph.reset();
  //  Debugging to see replacement matches
  LOG_TRACE("Replacements:\n" << replacements);
  //  Replace all of the nodes in one fell swoop
  map->replaceNodes(replacements);
}

bool DuplicateNodeRemover::_passesLogMergeFilter(const long nodeId1, const long nodeId2,
                                                 const OsmMapPtr& map) const
{
  // can add various filtering criteria here for debugging purposes...

  QStringList kvps;
  kvps.append("OBJECTID=168008");
  kvps.append("OBJECTID=76174");

  std::set<ElementId> wayIdsOwning1;
  const std::set<long> waysOwning1 = WayUtils::getContainingWayIds(nodeId1, map);
  for (auto it = waysOwning1.begin(); it != waysOwning1.end(); ++it)
    wayIdsOwning1.insert(ElementId(ElementType::Way, *it));

  if (TagUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning1, map))
    return true;

  std::set<ElementId> wayIdsOwning2;
  const std::set<long> waysOwning2 = WayUtils::getContainingWayIds(nodeId2, map);
  for (auto it = waysOwning2.begin(); it != waysOwning2.end(); ++it)
    wayIdsOwning2.insert(ElementId(ElementType::Way, *it));

  if (TagUtils::anyElementsHaveAnyKvp(kvps, wayIdsOwning2, map))
    return true;

  return false;
}

void DuplicateNodeRemover::_logMergeResult(const long nodeId1, const long nodeId2,
                                           const OsmMapPtr& map, const bool replaced,
                                           const double distance, const double calcdDistance) const
{
  //  Since all log messages below are TRACE level messages, if the log level is above trace, exit
  if (Log::getInstance().getLevel() > Log::Trace)
    return;
  if (_passesLogMergeFilter(nodeId1, nodeId2, map))
  {
    QString msg = "merging nodes: ";
    if (!replaced)
      msg.prepend("not ");
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

void DuplicateNodeRemover::removeNodes(std::shared_ptr<OsmMap> map, Meters distanceThreshold, const bool ignoreStatus)
{
  DuplicateNodeRemover dupeRemover(distanceThreshold);
  dupeRemover.setIgnoreStatus(ignoreStatus);
  dupeRemover.apply(map);
}

}
