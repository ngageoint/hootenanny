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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef NONBUILDINGAREACRITERION_H
#define NONBUILDINGAREACRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * A criterion that will either keep or remove areas, but not if the areas are also buildings.
 *
 * Should be able to accomplish the same thing with a not building and is area chain but
 * couldn't. See comments in train-area/RemoveIrrelevants.js in the regression tests.
 */
class NonBuildingAreaCriterion : public GeometryTypeCriterion, public ConstOsmMapConsumerImpl
{
public:

  static QString className() { return "NonBuildingAreaCriterion"; }

  NonBuildingAreaCriterion() = default;
  NonBuildingAreaCriterion(ConstOsmMapPtr map) : ConstOsmMapConsumerImpl(map) { }
  ~NonBuildingAreaCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<NonBuildingAreaCriterion>(_map); }

  GeometryType getGeometryType() const override { return GeometryType::Polygon; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Identifies features that are areas but not buildings"; }

};

}

#endif // NONBUILDINGAREACRITERION_H
