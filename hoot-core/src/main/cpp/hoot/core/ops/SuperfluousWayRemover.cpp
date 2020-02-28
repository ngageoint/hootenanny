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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "SuperfluousWayRemover.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayByEid.h>
#include <hoot/core/criterion/LinearCriterion.h>
#include <hoot/core/criterion/PolygonCriterion.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, SuperfluousWayRemover)

SuperfluousWayRemover::SuperfluousWayRemover()
{

}

SuperfluousWayRemover::SuperfluousWayRemover(const std::shared_ptr<OsmMap>& map) :
_inputMap(map)
{
}

long SuperfluousWayRemover::removeWays(const std::shared_ptr<OsmMap>& map)
{
  SuperfluousWayRemover wayRemover(map);
  LOG_DEBUG(wayRemover.getInitStatusMessage());
  wayRemover.removeWays();
  LOG_DEBUG(wayRemover.getCompletedStatusMessage());
  return wayRemover.getNumFeaturesAffected();
}

void SuperfluousWayRemover::removeWays()
{
  _numAffected = 0;
  std::shared_ptr<ElementToRelationMap> e2r = _inputMap->getIndex().getElementToRelationMap();
  LOG_VART(e2r->size());

  // make a copy of the ways to avoid issues when removing.
  const WayMap ways = _inputMap->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    const ConstWayPtr& w = it->second;
    //LOG_VART(w->getElementId());
    LOG_VART(w);

    bool same = true;
    const vector<long>& nodeIds = w->getNodeIds();
    LOG_VART(nodeIds);
    if (nodeIds.size() > 0)
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

    const bool inRelation = e2r->getRelationByElement(w).size() > 0;
    LOG_VART(inRelation);
    LOG_VART(w->getTags().size());

    // if all the nodes in a way are the same or there are zero nodes
    if ((same || w->getTags().size() == 0) && !inRelation)
    {  
      LOG_TRACE("Removing superflous way: " << w->getElementId() << "...");
      RemoveWayByEid::removeWayFully(_inputMap, w->getId());
      _numAffected++;
    }
  }
}

void SuperfluousWayRemover::apply(std::shared_ptr<OsmMap>& map)
{
  removeWays(map);
}

QStringList SuperfluousWayRemover::getCriteria() const
{
  QStringList criteria;
  criteria.append(QString::fromStdString(LinearCriterion::className()));
  criteria.append(QString::fromStdString(PolygonCriterion::className()));
  return criteria;
}

}
