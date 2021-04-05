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
#ifndef CALCULATEAREAVISITOR_H
#define CALCULATEAREAVISITOR_H

// hoot
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/info/SingleStatistic.h>

namespace hoot
{

/**
 * Sums the area of all the ways. The map projection is used so to get meters the map must be
 * first reprojected into meters.
 */
class CalculateAreaVisitor : public ElementConstOsmMapVisitor, public SingleStatistic
{
public:

  static QString className() { return "hoot::CalculateAreaVisitor"; }

  CalculateAreaVisitor() : _total(0) { }
  ~CalculateAreaVisitor() = default;

  double getArea() const { return _total; }

  double getStat() const override { return getArea(); }

  void visit(const ConstElementPtr& e) override;

  QString getDescription() const override { return "Calculates feature area totals"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  Meters _total;
};

}

#endif // ADDREFVISITOR_H
