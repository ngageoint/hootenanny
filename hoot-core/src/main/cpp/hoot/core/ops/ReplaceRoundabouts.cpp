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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "ReplaceRoundabouts.h"

// Hoot
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/ElementIdsVisitor.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ReplaceRoundabouts)

void ReplaceRoundabouts::replaceRoundabouts(const std::shared_ptr<OsmMap>& pMap)
{
  LOG_TRACE("Replacing roundabouts...");

  // Make sure we are planar
  MapProjector::projectToPlanar(pMap);

  // Get a list of roundabouts from the map, go through & process them
  std::vector<RoundaboutPtr> roundabouts = pMap->getRoundabouts();
  LOG_VART(roundabouts.size());
  for (auto const& roundabout : roundabouts)
  {
    roundabout->replaceRoundabout(pMap);
    _numAffected++;
  }
  OsmMapWriterFactory::writeDebugMap(pMap, className(), "after-replacing-roundabouts-1");

  // We may have lost track of some of the connectors b/c of ID changes, so let's do another pass
  // at removing them.
  std::vector<long> connectors =
    ElementIdsVisitor::findElementsByTag(
      pMap, ElementType::Way, MetadataTags::HootSpecial(), MetadataTags::RoundaboutConnector());
  LOG_VART(connectors.size());
  for (auto id : connectors)
  {
    LOG_TRACE("Removing center node: " << id << "...");
    RemoveWayByEid::removeWayFully(pMap, id);
  }
  OsmMapWriterFactory::writeDebugMap(pMap, className(), "after-replacing-roundabouts-2");

  //  Clean up any roundabout centers that didn't clean themselves up earlier
  std::vector<long> centers =
    ElementIdsVisitor::findElementsByTag(
      pMap, ElementType::Node, MetadataTags::HootSpecial(), MetadataTags::RoundaboutCenter());
  LOG_VART(centers.size());
  for (auto id : centers)
  {
    LOG_TRACE("Removing center node: " << id << "...");
    RemoveNodeByEid::removeNode(pMap, id, true);
  }
  OsmMapWriterFactory::writeDebugMap(pMap, className(), "after-removing-center-nodes");
}

void ReplaceRoundabouts::apply(std::shared_ptr<OsmMap>& pMap)
{
  _numAffected = 0;

  replaceRoundabouts(pMap);
}

QStringList ReplaceRoundabouts::getCriteria() const
{
  return QStringList(HighwayCriterion::className());
}

}
