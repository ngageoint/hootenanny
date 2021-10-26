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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIVISITOR_H
#define MULTIVISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

/**
 * @brief The MultiVisitor class allows for iterating over multiple visitors.
 */
class MultiVisitor : public ElementVisitor
{
public:

  static QString className() { return "MultiVisitor"; }

  MultiVisitor() = default;
   ~MultiVisitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  void addVisitor(ElementOsmMapVisitor* v) { _visitors.push_back(v); }

  QString getDescription() const override { return "Allows for combining multiple visitors"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  std::vector<ElementOsmMapVisitor*> _visitors;
};

}
#endif // MULTIVISITOR_H
