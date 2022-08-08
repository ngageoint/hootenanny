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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef CONSTELEMENTVISITOR_H
#define CONSTELEMENTVISITOR_H

// hoot
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * Visits elements in a collection. See Element::visit* and OsmMap::visit* for ways to use the
 * class. See AddRefVisitor for an example implementation.
 */
class ConstElementVisitor : public ElementVisitor
{
public:

  static QString className() { return "ConstElementVisitor"; }

  ConstElementVisitor() = default;
  virtual ~ConstElementVisitor() = default;

  virtual void visit(const ConstElementPtr& e) = 0;

  void visit(const ElementPtr& e) override
  {
    visit(std::dynamic_pointer_cast<const Element>(e));
  }
};

using ConstElementVisitorPtr = std::shared_ptr<ConstElementVisitor>;

}

#endif // CONSTELEMENTVISITOR_H
