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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef WORSTCIRCULARERRORVISITOR_H
#define WORSTCIRCULARERRORVISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

/**
 * A visitor for finding the worst circular error among elements. Worst = greatest.
 */
class WorstCircularErrorVisitor : public ConstElementVisitor, public SingleStatistic
{
public:

  static QString className() { return "WorstCircularErrorVisitor"; }

  WorstCircularErrorVisitor() : _worst(ElementData::CIRCULAR_ERROR_EMPTY) {}
  ~WorstCircularErrorVisitor() = default;

  double getStat() const override { return _worst; }

  void visit(const std::shared_ptr<const Element>& e) override;

  QString getDescription() const override
  { return "Determines the highest circular error value"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  Meters getWorstCircularError() const { return _worst; }
  static Meters getWorstCircularError(const OsmMapPtr& map);
  static Meters getWorstCircularError(const ConstOsmMapPtr& map);
  static Meters getWorstCircularError(const std::vector<ElementPtr>& elements);

private:

  Meters _worst;
};

}

#endif // WORSTCIRCULARERRORVISITOR_H
