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
#ifndef HIGHWAYINTERSECTIONCRITERION_H
#define HIGHWAYINTERSECTIONCRITERION_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * This criterion is satisified for all highway intersections with 3 or more interacting ways.
 * Multilinestring relations are not handled.
 *
 * This class assumes that IntersectionSplitter was applied to the map before being called.
 */
class HighwayIntersectionCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "HighwayIntersectionCriterion"; }

  HighwayIntersectionCriterion() = default;
  explicit HighwayIntersectionCriterion(ConstOsmMapPtr map);
  ~HighwayIntersectionCriterion() override = default;

  ElementCriterionPtr clone() override
  { return std::make_shared<HighwayIntersectionCriterion>(_map); }
  bool isSatisfied(const ConstElementPtr& e) const override;

  void setOsmMap(const OsmMap* map) override;

  QString getDescription() const override { return "Identifies highway intersections"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }

private:

  ConstOsmMapPtr _map;
};

}

#endif // HIGHWAYINTERSECTIONCRITERION_H
