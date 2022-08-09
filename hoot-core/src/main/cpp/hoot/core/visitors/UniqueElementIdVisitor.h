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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef UNIQUE_ELEMENT_ID_VISITOR_H
#define UNIQUE_ELEMENT_ID_VISITOR_H

// hoot
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Returns a unique, ordered set of the element ids visited.
 *
 * @todo Think that if ElementIdsVisitor gets converted over to work with sets of element IDs
 * instead of vectors, then maybe this class can go away.
 */
class UniqueElementIdVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "UniqueElementIdVisitor"; }

  UniqueElementIdVisitor() = default;
  ~UniqueElementIdVisitor() override = default;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override { return "Returns the unique element IDs visited"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * Returns a set containing all the ElementIds visited.
   */
  const std::set<ElementId>& getElementSet() const { return _elements; }

private:

  std::set<ElementId> _elements;
};

}

#endif // UNIQUE_ELEMENT_ID_VISITOR_H
