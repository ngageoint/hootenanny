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
#include "MedianNodeVisitor.h"

// Standard
#include <numeric>

// Hoot
#include <hoot/core/util/Factory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, MedianNodeVisitor)

ConstNodePtr MedianNodeVisitor::calculateMedianNode() const
{
  ConstNodePtr best;
  double bestScore = numeric_limits<double>::max();

  for (const auto& node : qAsConst(_nodes))
  {
    double score = _calculateRmsd(node);
    if (score < bestScore)
    {
      bestScore = score;
      best = node;
    }
  }

  return best;
}

double MedianNodeVisitor::_calculateRmsd(ConstNodePtr n) const
{
  double sum = 0.0;

  Coordinate c1 = n->toCoordinate();
  for (const auto& node : qAsConst(_nodes))
  {
    double d = c1.distance(node->toCoordinate());
    sum += d;
  }

  return sqrt(sum / _nodes.size());
}

void MedianNodeVisitor::visit(const ConstElementPtr& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
    _nodes.append(n);
  }
}

}
