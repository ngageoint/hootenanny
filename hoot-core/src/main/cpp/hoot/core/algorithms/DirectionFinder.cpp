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

  // fix for #2888
  if (w1->getNodeIds().size() == 0 || w2->getNodeIds().size() == 0)
  {
    return false;
  }
  else if ((w1->getNodeIds()[0] == w2->getNodeIds()[0]) ||
           (w1->getNodeIds()[w1->getNodeIds().size() - 1] ==
            w2->getNodeIds()[w2->getNodeIds().size() - 1]))
  {
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

  // TODO: probably won't be using the rounding or the percent diff...remove
  const double dSumSameRounded = round(dSumSame * 10) / 10;
  const double dSumReverseRounded = round(dSumReverse * 10) / 10;
//  const bool sameDirection = dSumSameRounded < dSumReverseRounded;
  const double percentageDiffRounded =
    abs((dSumReverseRounded - dSumSameRounded) / dSumReverseRounded);
  const double percentageRounded = abs((dSumReverseRounded - dSumSame) / dSumReverse);

  const bool sameDirection = dSumSame < dSumReverse;
  QString directionText = "not the same direction";
  if (sameDirection)
  {
    directionText = "same direction";
  }
  const int coordPrecision = ConfigOptions().getWriterPrecision();
  LOG_DEBUG(
    "Comparing " << w1->getElementId() << " with " << w2->getElementId() << ": " << directionText <<
    ", rounded same score: " << QString::number(dSumSameRounded, 'g', coordPrecision) <<
    ", rounded reverse score: " << QString::number(dSumReverseRounded, 'g', coordPrecision) <<
    ", rounded difference: " << (dSumReverseRounded - dSumSameRounded) <<
    ", rounded percentage difference: " << QString::number(percentageDiffRounded, 'g', coordPrecision) <<
    ", original same score: " << QString::number(dSumSame, 'g', coordPrecision) <<
    ", original reverse score: " << QString::number(dSumReverse, 'g', coordPrecision) <<
    ", original difference: " << QString::number((dSumReverse - dSumSame), 'g', coordPrecision) <<
    ", original percentage difference: " << QString::number(percentageRounded, 'g', coordPrecision));
  return sameDirection;
}

}
