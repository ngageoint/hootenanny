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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef CALCULATEAREAFORSTATSVISITOR_H
#define CALCULATEAREAFORSTATSVISITOR_H

// hoot
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

/**
 * Sums the area of all the ways. The map projection is used so to get meters the map must be
 * first reprojected into meters. This is meant for use in calculating map statistics only.
 */
class CalculateAreaForStatsVisitor : public ElementConstOsmMapVisitor, public SingleStatistic
{
public:

  static QString className() { return "CalculateAreaForStatsVisitor"; }

  CalculateAreaForStatsVisitor() : _total(0) { }
  ~CalculateAreaForStatsVisitor() = default;

  double getArea() const { return _total; }

  double getStat() const override { return getArea(); }

  void visit(const ConstElementPtr& e) override;

  // We don't use this outside of stats, so hide it from the info API.
  QString getDescription() const override { return ""; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  Meters _total;
};

}

#endif // ADDREFVISITOR_H
