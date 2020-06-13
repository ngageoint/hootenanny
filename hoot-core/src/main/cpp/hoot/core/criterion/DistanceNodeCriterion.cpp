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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "DistanceNodeCriterion.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/ConfigOptions.h>

using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, DistanceNodeCriterion)

void DistanceNodeCriterion::setConfiguration(const Settings& s)
{
  ConfigOptions opts(s);

  _distance = opts.getDistanceNodeCriterionDistance();
  if (_distance < 0.0)
  {
    throw HootException(
      "Invalid distance passed to DistanceNodeCriterion: " + QString::number(_distance));
  }

  const QString errorMsg =
    "Invalid center value passed to DistanceNodeCriterion: " +
    opts.getDistanceNodeCriterionCenter();
  const QStringList centerParts = opts.getDistanceNodeCriterionCenter().split(",");
  if (centerParts.size() == 0)
  {
    throw HootException(errorMsg);
  }
  bool ok = false;
  const double x = centerParts[0].toDouble(&ok);
  if (!ok)
  {
    throw HootException(errorMsg);
  }
  _center.x = x;
  const double y = centerParts[1].toDouble(&ok);
  if (!ok)
  {
     throw HootException(errorMsg);
  }
  _center.y = y;
}

DistanceNodeCriterion::DistanceNodeCriterion(Coordinate center, Meters distance):
_center(center),
_distance(distance)
{
}

bool DistanceNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (e->getElementType() != ElementType::Node)
    return false;

  ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
  return _center.distance(n->toCoordinate()) < _distance;
}

}
