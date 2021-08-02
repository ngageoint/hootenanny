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

#ifndef WAYDIRECTIONCRITERION_H
#define WAYDIRECTIONCRITERION_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

class Way;

class WayDirectionCriterion : public ElementCriterion
{
public:

  static QString className() { return "hoot::WayDirectionCriterion"; }

  WayDirectionCriterion(
    const ConstOsmMapPtr& map, ConstWayPtr baseWay, bool similarDirection = true);
  ~WayDirectionCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<WayDirectionCriterion>(_map, _baseWay, _similarDirection); }

  QString getDescription() const override
  { return "Determines if two ways point in a similar direction"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  ConstOsmMapPtr _map;
  ConstWayPtr _baseWay;
  bool _similarDirection;
};

}

#endif // WAYDIRECTIONCRITERION_H
