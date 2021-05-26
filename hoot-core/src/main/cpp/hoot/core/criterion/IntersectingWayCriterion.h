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
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Given a collection of way(s) identifies other ways that intersect with them.
 *
 * To get more mileage out of this if needed, we could replace _sourceWayIds with a source way
 * criterion (a crit inside of a crit...is that strange?). That would probably add some processing
 * time, though, as each intersecting way would have to be checked against the crit. OR maybe we
 * make use of FindIntersectionsOp for that purpose instead...
 */
class IntersectingWayCriterion : public GeometryTypeCriterion, public ConstOsmMapConsumer,
  public Configurable
{
public:

  static QString className() { return "hoot::IntersectingWayCriterion"; }

  IntersectingWayCriterion() = default;
  IntersectingWayCriterion(ConstOsmMapPtr map);
  IntersectingWayCriterion(const QSet<long>& sourceWayIds, ConstOsmMapPtr map);
  ~IntersectingWayCriterion() override = default;

  /**
   * @see ElementCriterion
   */
  bool isSatisfied(const ConstElementPtr& e) const override;

  /**
   * @see ElementCriterion
   */
  ElementCriterionPtr clone() override;

  /**
   * @see GeometryTypeCriterion
   */
  GeometryType getGeometryType() const override { return GeometryType::Line; }

  /**
   * @see ConstOsmMapConsumer
   */
  void setOsmMap(const OsmMap* map) override { _map = map->shared_from_this(); }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
  QString getDescription() const override
  { return "Finds ways that intersect a specified ways"; }

private:

  // the IDs for the ways to look for other intersecting ways with
  mutable QSet<long> _sourceWayIds;

  ConstOsmMapPtr _map;

  static const QString EMPTY_SOURCE_IDS_ERROR_MESSAGE;
};

}

#endif // INTERSECTING_WAY_CRITERION_H
