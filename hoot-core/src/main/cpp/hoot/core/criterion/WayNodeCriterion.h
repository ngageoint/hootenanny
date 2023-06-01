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
#ifndef WAY_NODE_CRITERION_H
#define WAY_NODE_CRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class WayNodeCriterion : public GeometryTypeCriterion, public ConstOsmMapConsumerImpl
{
public:

  static QString className() { return "WayNodeCriterion"; }

  WayNodeCriterion() = default;
  WayNodeCriterion(ConstOsmMapPtr map) : ConstOsmMapConsumerImpl(map) { }
  ~WayNodeCriterion() override = default;

  /**
   * Returns the ID of the first way that owns input node
   *
   * @param node node to search for
   * @return the first way ID found that contains the node
   * @todo move to WayUtils?
   */
  long getFirstOwningWayId(const ConstNodePtr& node) const;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<WayNodeCriterion>(_map); }

  GeometryType getGeometryType() const override { return GeometryType::Point; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Identifies way nodes"; }
  QString toString() const override { return className(); }

protected:

  ElementCriterionPtr _parentCriterion;
};

}

#endif // WAY_NODE_CRITERION_H
