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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTCOUNTVISITOR_H
#define ELEMENTCOUNTVISITOR_H

// hoot
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

/**
 * Counts the number of elements.
 */
class ElementCountVisitor : public ConstElementVisitor, public SingleStatistic
{
public:

  static std::string className() { return "hoot::ElementCountVisitor"; }

  ElementCountVisitor() : _count(0) {}

  int getCount() const { return _count; }

  virtual double getStat() const { return _count; }

  virtual void visit(const ConstElementPtr& e);

  virtual QString getDescription() const { return "Counts the number of elements"; }

  virtual std::string getClassName() const { return className(); }

private:

  int _count;
};

}

#endif // ELEMENTCOUNTVISITOR_H
