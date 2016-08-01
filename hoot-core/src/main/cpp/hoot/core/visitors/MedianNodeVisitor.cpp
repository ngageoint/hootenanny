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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MedianNodeVisitor.h"

// Standard
#include <numeric>

namespace hoot
{

MedianNodeVisitor::MedianNodeVisitor()
{
}

ConstNodePtr MedianNodeVisitor::calculateMedianNode() const
{
  ConstNodePtr best;
  double bestScore = numeric_limits<double>::max();

  for (int i = 0; i < _nodes.size(); ++i)
  {
    double score = _calculateRmsd(_nodes[i]);
    if (score < bestScore)
    {
      bestScore = score;
      best = _nodes[i];
    }
  }

  return best;
}

double MedianNodeVisitor::_calculateRmsd(ConstNodePtr n) const
{
  double sum = 0.0;

  Coordinate c1 = n->toCoordinate();
  for (int i = 0; i < _nodes.size(); ++i)
  {
    double d = c1.distance(_nodes[i]->toCoordinate());
    sum += d;
  }

  return sqrt(sum / _nodes.size());
}

void MedianNodeVisitor::visit(const ConstElementPtr& e)
{
  if (e->getElementType() == ElementType::Node)
  {
    ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
    _nodes.append(n);
  }
}

}
