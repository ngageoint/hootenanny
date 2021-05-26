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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef INTERSECTING_WAY_CRITERION_H
#define INTERSECTING_WAY_CRITERION_H

// Hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Given a collection of way(s) identifies other ways that intersect with them.
 */
class IntersectingWayCriterion : public GeometryTypeCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::IntersectingWayCriterion"; }

  IntersectingWayCriterion() = default;
  IntersectingWayCriterion(
    const QSet<long>& wayIds, ConstOsmMapPtr map,
    const ElementCriterionPtr& crit = ElementCriterionPtr());
  virtual ~IntersectingWayCriterion() = default;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;

  /**
   * @see ElementCriterion
   */
  ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new IntersectingWayCriterion(_wayIds, _map, _crit)); }

  /**
   * @see GeometryTypeCriterion
   */
  GeometryType getGeometryType() const override { return GeometryType::Line; }

  /**
   * @see ConstOsmMapConsumer
   */
  void setOsmMap(const OsmMap* map) override { _map = map->shared_from_this(); }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Finds ways that intersect a given collection of ways"; }

  void setWayIds(const QSet<long>& ids) { _wayIds = ids; }

protected:

  // the IDs for the collection of ways to look for other intersecting ways with
  mutable QSet<long> _wayIds;

  ConstOsmMapPtr _map;

  // optional additional criteria for filtering what's considered an intersecting way
  ElementCriterionPtr _crit;
};

}

#endif // INTERSECTING_WAY_CRITERION_H
