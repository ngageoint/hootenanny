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
#ifndef NODES_SHIFT_VISITOR_H
#define NODES_SHIFT_VISITOR_H

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * @brief The NodesShiftVisitor class shifts all nodes in a map by Y meters north and X meters east
 *  NOTE: The map must be in planar coordinates for this visitor to work
 */
class NodesShiftVisitor : public ElementVisitor
{
public:

  static QString className() { return "NodesShiftVisitor"; }

  NodesShiftVisitor();
  NodesShiftVisitor(Meters north_shift, Meters east_shift);
  ~NodesShiftVisitor() override = default;

  void setShift(Meters north_shift, Meters east_shift);

  /**
   * @see ElementVisitor
   */
  void visit(const std::shared_ptr<Element>& e) override;

  QString getDescription() const override { return "Shift all nodes by an offset"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  void _parseShift();

  Meters _northShift;
  Meters _eastShift;
};

}

#endif // NODES_SHIFT_VISITOR_H
