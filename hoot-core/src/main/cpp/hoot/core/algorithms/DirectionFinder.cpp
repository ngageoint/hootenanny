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

  //dSumSame = round(dSumSame * 10000) / 10000;
  //dSumReverse = round(dSumReverse * 10000) / 10000;
  const bool sameDirection = dSumSame < dSumReverse;
  QString directionText = "not the same direction";
  if (sameDirection)
  {
    directionText = "same direction";
  }
  LOG_DEBUG(
    "Comparing " << w1->getElementId() << " with " << w2->getElementId() << ": " << directionText <<
    ", same score: " << QString::number(dSumSame, 'g', 17) << ", reverse score: " <<
    QString::number(dSumReverse, 'g', 17) << ", difference: " << (dSumReverse - dSumSame));
  return sameDirection;
}

}
