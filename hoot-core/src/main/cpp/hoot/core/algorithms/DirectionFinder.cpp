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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DirectionFinder.h"

// GEOS
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
using namespace geos::geom;

// Hoot
#include <hoot/core/algorithms/Distance.h>
#include <hoot/core/algorithms/WayDiscretizer.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/util/Units.h>

// Standard
#include <iostream>
#include <vector>
using namespace std;

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

bool DirectionFinder::isSimilarDirection(const ConstOsmMapPtr& map, ConstWayPtr w1, ConstWayPtr w2)
{
  LOG_VARD(w1->getNodeIds());
  LOG_VARD(w2->getNodeIds());

  // skip empty ways
  if (w1->getNodeIds().size() == 0 || w2->getNodeIds().size() == 0)
  {
    LOG_DEBUG("Skipping one or more empty ways...");
    return false;
  }
  // check for shared start/end node combos that show reversal; fix for #2888
  else if ((w1->getNodeIds()[0] == w2->getNodeIds()[0]) ||
           (w1->getNodeIds()[w1->getNodeIds().size() - 1] ==
            w2->getNodeIds()[w2->getNodeIds().size() - 1]))
  {
    const long startNodeId1 = w1->getNodeIds()[0];
    const long startNodeId2 = w2->getNodeIds()[0];
    const long endNodeId1 = w1->getNodeIds()[w1->getNodeIds().size() - 1];
    const long endNodeId2 = w2->getNodeIds()[w2->getNodeIds().size() - 1];
    //if (map->containsNode(startNodeId1) && map->containsNode(startNodeId2))
    //{
      const Radians angle1 =
        //toDegrees(
          WayHeading::calculateHeading(
            map->getNode(startNodeId1)->toCoordinate(),
            map->getNode(endNodeId1)->toCoordinate())/*)*/;
      const Radians angle2 =
        //toDegrees(
          WayHeading::calculateHeading(
            map->getNode(startNodeId2)->toCoordinate(),
            map->getNode(endNodeId2)->toCoordinate())/*)*/;
      Radians deltaMagnitude = WayHeading::deltaMagnitude(angle1, angle2);
      const double diffAngle = toDegrees(deltaMagnitude);
      LOG_VARD(diffAngle);
      if (diffAngle > 20.0)
      {
        LOG_DEBUG(
          "Skipping ways with large difference in orientation angle: " << diffAngle << " degrees...");
        return false;
      }
//    }
//    else
//    {
//      LOG_DEBUG("Couldn't find one of the nodes: " << startNodeId1 << " or " << startNodeId2);
//    }
  }
//  else if ((w1->getNodeIds()[w1->getNodeIds().size() - 1] ==
//            w2->getNodeIds()[w2->getNodeIds().size() - 1]))
//  {
//    const long startNodeId1 = w1->getNodeIds()[w1->getNodeIds().size() - 1];
//    const long startNodeId2 = w2->getNodeIds()[w2->getNodeIds().size() - 1];
//    if (map->containsNode(startNodeId1) && map->containsNode(startNodeId2))
//    {
//      const double angle =
//        toDegrees(
//          WayHeading::calculateHeading(
//            map->getNode(startNodeId1)->toCoordinate(),
//            map->getNode(startNodeId2)->toCoordinate()));
//      LOG_VARD(angle);
//      if (angle > 20.0)
//      {
//        LOG_DEBUG(
//          "Skipping ways with large difference in orientation angle: " << angle << " degrees...");
//        return false;
//      }
//    }
//    else
//    {
//      LOG_DEBUG("Couldn't find one of the nodes: " << startNodeId1 << " or " << startNodeId2);
//    }
//  }

  WayDiscretizer wd1(map, w1);
  WayDiscretizer wd2(map, w2);
  vector<Coordinate> cs1, cs2;
  wd1.discretize(5.0, cs1);
  wd2.discretize(5.0, cs2);

  double dSumSame = 0;
  double dSumReverse = 0;

  size_t pointCount = std::min(cs1.size(), cs2.size());
  for (size_t i = 0; i < pointCount; i++)
  {
    dSumSame += Distance::euclidean(cs1[i], cs2[i]);
    dSumReverse += Distance::euclidean(cs1[i], cs2[pointCount - i - 1]);
  }

  const double percentageDiff = abs((dSumReverse - dSumSame) / dSumReverse);
  const bool sameDirection = dSumSame < dSumReverse /*|| percentageDiff <= 0.0001*/;
  QString directionText = "**not the same direction**";
  if (sameDirection)
  {
    directionText = "**same direction**";
  }
  const int coordPrecision = ConfigOptions().getWriterPrecision();
  LOG_DEBUG(
    "Comparing " << w1->getElementId() << " with " << w2->getElementId() << ": " << directionText <<
    ", same score: " << QString::number(dSumSame, 'g', coordPrecision) <<
    ", reverse score: " << QString::number(dSumReverse, 'g', coordPrecision) <<
    ", difference: " << QString::number((dSumReverse - dSumSame), 'g', coordPrecision) <<
    ", percentage difference: " << QString::number(percentageDiff, 'g', coordPrecision));
  return sameDirection;
}

}
