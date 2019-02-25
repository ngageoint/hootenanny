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
  LOG_VART(w1->getNodeIds());
  LOG_VART(w2->getNodeIds());

  // skip empty ways
  if (w1->getNodeIds().size() == 0 || w2->getNodeIds().size() == 0)
  {
    LOG_TRACE("Skipping one or more empty ways...");
    return false;
  }

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
  const bool sameDirection = dSumSame < dSumReverse;
  QString directionText = "**not the same direction**";
  if (sameDirection)
  {
    directionText = "**same direction**";
  }
  const int coordPrecision = ConfigOptions().getWriterPrecision();
  LOG_TRACE(
    "Comparing " << w1->getElementId() << " with " << w2->getElementId() << ": " << directionText <<
    ", same score: " << QString::number(dSumSame, 'g', coordPrecision) <<
    ", reverse score: " << QString::number(dSumReverse, 'g', coordPrecision) <<
    ", difference: " << QString::number((dSumReverse - dSumSame), 'g', coordPrecision) <<
    ", percentage difference: " << QString::number(percentageDiff, 'g', coordPrecision));
  return sameDirection;
}

bool DirectionFinder::isSimilarDirection2(const ConstOsmMapPtr& map, ConstWayPtr way1,
                                          ConstWayPtr way2)
{
  LOG_VART(way1->getNodeIds());
  LOG_VART(way2->getNodeIds());

  // skip empty ways
  if (way1->getNodeIds().size() == 0 || way2->getNodeIds().size() == 0)
  {
    LOG_TRACE("Skipping one or more empty ways...");
    return false;
  }

  const double diffAngle = _getAngleDiff(map, way1, way2);
  LOG_VART(diffAngle);
  const double angleThreshold = ConfigOptions().getDirectionFinderAngleThreshold();
  if (diffAngle >= angleThreshold)
  {
    LOG_TRACE(
      "Ways have large difference in orientation angle: " << diffAngle <<
      " degrees (threshold: " << angleThreshold << ").");
    return false;
  }
  else
  {
    return true;
  }
}

double DirectionFinder::_getAngleDiff(const ConstOsmMapPtr& map, ConstWayPtr way1, ConstWayPtr way2)
{
  const std::vector<long> way1NodeIds = way1->getNodeIds();
  const std::vector<long> way2NodeIds = way2->getNodeIds();
  const long startNodeId1 = way1NodeIds[0];
  const long startNodeId2 = way2NodeIds[0];
  const long endNodeId1 = way1NodeIds[way1NodeIds.size() - 1];
  const long endNodeId2 = way2NodeIds[way2NodeIds.size() - 1];
  return
    toDegrees(
      WayHeading::deltaMagnitude(
        WayHeading::calculateHeading(
          map->getNode(startNodeId1)->toCoordinate(),
          map->getNode(endNodeId1)->toCoordinate()),
        WayHeading::calculateHeading(
          map->getNode(startNodeId2)->toCoordinate(),
          map->getNode(endNodeId2)->toCoordinate())));
}

}
