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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MULTIVISITOR_H
#define MULTIVISITOR_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/elements/ElementVisitor.h>

namespace hoot
{

class MultiVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::MultiVisitor"; }

  MultiVisitor();

  virtual void visit(const std::shared_ptr<Element>& e);

  void addVisitor(ElementOsmMapVisitor *v) { _visitors.push_back(v); }

  virtual QString getDescription() const { return "Allows for combining multiple visitors"; }

  virtual std::string getClassName() const { return className(); }

private:

  std::vector<ElementOsmMapVisitor *> _visitors;
};

}
#endif // MULTIVISITOR_H
