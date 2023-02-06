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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */
#include "NodesShiftVisitor.h"

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/ElementToGeometryConverter.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/schema/TagDifferencer.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CompletelyContainedByMapElementVisitor.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesShiftVisitor)

NodesShiftVisitor::NodesShiftVisitor()
  : _northShift(0.0),
    _eastShift(0.0)
{
  _parseShift();
}

NodesShiftVisitor::NodesShiftVisitor(Meters north_shift, Meters east_shift)
  : _northShift(north_shift),
    _eastShift(east_shift)
{
}

void NodesShiftVisitor::setShift(Meters north_shift, Meters east_shift)
{
  _northShift = north_shift;
  _eastShift = east_shift;
}

void NodesShiftVisitor::visit(const std::shared_ptr<Element>& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    NodePtr node = std::dynamic_pointer_cast<Node>(e);
    node->setX(node->getX() + _eastShift);
    node->setY(node->getY() + _northShift);
  }
}

void NodesShiftVisitor::_parseShift()
{
  QString shift = ConfigOptions().getShiftNodesXy();
  QStringList values = shift.split(",");
  if (values.size() != 2)
    return;
  bool goodX = false;
  bool goodY = false;
  double x = values[0].toDouble(&goodX);
  double y = values[1].toDouble(&goodY);
  if (!goodX || !goodY)
    return;
  _northShift = y;
  _eastShift = x;
}

}
