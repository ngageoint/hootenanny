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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WORSTCIRCULARERRORVISITOR_H
#define WORSTCIRCULARERRORVISITOR_H

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

/**
 * A visitor for finding the worst circular error among elements. Worst = greatest.
 */
namespace hoot
{

class WorstCircularErrorVisitor : public ConstElementVisitor, public SingleStatistic
{
public:

  static std::string className() { return "hoot::WorstCircularErrorVisitor"; }

  WorstCircularErrorVisitor() : _worst(ElementData::CIRCULAR_ERROR_EMPTY) {}

  Meters getWorstCircularError() { return _worst; }

  virtual double getStat() const { return _worst; }

  virtual void visit(const std::shared_ptr<const Element>& e);

  // Convenient way to get worst circular error
  static Meters getWorstCircularError(const OsmMapPtr& map);

  // Handle const pointers to const
  static Meters getWorstCircularError(const ConstOsmMapPtr& map);

  static Meters getWorstCircularError(const std::vector<ElementPtr>& elements);

  virtual QString getDescription() const
  { return "Determines the highest circular error value"; }

  virtual std::string getClassName() const { return className(); }

private:

  Meters _worst;
};

}

#endif // WORSTCIRCULARERRORVISITOR_H
