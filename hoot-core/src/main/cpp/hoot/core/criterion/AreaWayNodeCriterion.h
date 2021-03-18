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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef AREA_WAY_NODE_CRITERION_H
#define AREA_WAY_NODE_CRITERION_H

// hoot
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

class AreaWayNodeCriterion : public WayNodeCriterion
{
public:

  static QString className() { return "hoot::AreaWayNodeCriterion"; }

  AreaWayNodeCriterion();
  AreaWayNodeCriterion(ConstOsmMapPtr map);
  ~AreaWayNodeCriterion() = default;

  void setOsmMap(const OsmMap* map) override;

  ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new AreaWayNodeCriterion(_map)); }

  QString getDescription() const override { return "Identifies area nodes"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }
};

}

#endif // AREA_WAY_NODE_CRITERION_H
