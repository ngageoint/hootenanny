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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ReplaceRoundabouts.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/MapProjector.h>

// Qt
#include <QDebug>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ReplaceRoundabouts)

ReplaceRoundabouts::ReplaceRoundabouts()
{
  // blank
}

void ReplaceRoundabouts::replaceRoundabouts(boost::shared_ptr<OsmMap> pMap)
{
  // Make sure we are planar
  MapProjector::projectToPlanar(pMap);

  // Get a list of roundabouts from the map, go through & process them
  std::vector<RoundaboutPtr> roundabouts = pMap->getRoundabouts();

  for (size_t i = 0; i < roundabouts.size(); i++)
  {
    RoundaboutPtr pRoundabout = roundabouts[i];
    pRoundabout->replaceRoundabout(pMap);
  }
}

void ReplaceRoundabouts::apply(boost::shared_ptr<OsmMap> &pMap)
{
  replaceRoundabouts(pMap);
}

}
