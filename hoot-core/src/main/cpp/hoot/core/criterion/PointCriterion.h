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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef POINT_CRITERION_H
#define POINT_CRITERION_H

// Hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

/**
 * Identifies point features
 */
class PointCriterion : public ConflatableElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::PointCriterion"; }

  PointCriterion() = default;
  PointCriterion(ConstOsmMapPtr map);
  virtual ~PointCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new PointCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies point features"; }

  virtual GeometryType getGeometryType() const
  { return GeometryType::Point; }

  virtual void setOsmMap(const OsmMap* map);

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual bool supportsSpecificConflation() const { return false; }

private:

  ConstOsmMapPtr _map;

  WayNodeCriterion _wayNodeCrit;
};

}
#endif // POINT_CRITERION_H
