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

#ifndef DISTANCENODECRITERION_H
#define DISTANCENODECRITERION_H

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * isSatisfied returns true if an element is within the specified distance of the given center
 */
class DistanceNodeCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "hoot::DistanceNodeCriterion"; }

  DistanceNodeCriterion() = default;
  DistanceNodeCriterion(geos::geom::Coordinate center, Meters distance);
  ~DistanceNodeCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override
  { return std::make_shared<DistanceNodeCriterion>(_center, _distance); }

  void setConfiguration(const Settings& s) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Determines if an element is within the specified distance of a point"; }

private:

  geos::geom::Coordinate _center;
  Meters _distance;
};

}

#endif // DISTANCENODECRITERION_H
