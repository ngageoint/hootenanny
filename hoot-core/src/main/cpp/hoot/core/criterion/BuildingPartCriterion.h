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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef BUILDING_PART_CRITERION_H
#define BUILDING_PART_CRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>

namespace hoot
{

/**
 * Identifies parts of buildings
 */
class BuildingPartCriterion : public GeometryTypeCriterion
{
public:

  static QString className() { return "hoot::BuildingPartCriterion"; }

  BuildingPartCriterion() = default;
  virtual ~BuildingPartCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new BuildingPartCriterion()); }

  virtual QString getDescription() const { return "Identifies parts of buildings"; }

  virtual GeometryType getGeometryType() const
  { return GeometryType::Polygon; }

  virtual QString getName() const override { return className(); }
};

}

#endif // BUILDING_PART_CRITERION_H
