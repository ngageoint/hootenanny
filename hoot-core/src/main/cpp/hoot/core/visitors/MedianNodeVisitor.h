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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MEDIANNODEVISITOR_H
#define MEDIANNODEVISITOR_H

// hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Node.h>

namespace hoot
{

/**
 * Calculates the median node. This is very inefficient and shouldn't be used on large maps. E.g.
 * O(n^2).
 */
class MedianNodeVisitor : public ElementVisitor
{
public:

  MedianNodeVisitor();

  virtual void visit(const ConstElementPtr& e);

  ConstNodePtr calculateMedianNode() const;

private:

  QList<ConstNodePtr> _nodes;

  double _calculateRmsd(ConstNodePtr n) const;
};

}

#endif // MEDIANNODEVISITOR_H
