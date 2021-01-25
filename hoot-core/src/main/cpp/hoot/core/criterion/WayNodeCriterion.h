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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAY_NODE_CRITERION_H
#define WAY_NODE_CRITERION_H

// hoot
#include <hoot/core/criterion/GeometryTypeCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

class WayNodeCriterion : public GeometryTypeCriterion, public ConstOsmMapConsumer
{
public:

  static std::string className() { return "hoot::WayNodeCriterion"; }

  WayNodeCriterion() = default;
  WayNodeCriterion(ConstOsmMapPtr map) : _map(map) { }
  virtual ~WayNodeCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new WayNodeCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies way nodes"; }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

  virtual GeometryType getGeometryType() const { return GeometryType::Point; }

  virtual QString getName() const override { return QString::fromStdString(className()); }

  /**
   * Returns the ID of the first way that owns input node
   *
   * @param node node to search for
   * @return the first way ID found that contains the node
   * @todo move to WayUtils
   */
  long getFirstOwningWayId(const ConstNodePtr& node);

protected:

  ConstOsmMapPtr _map;
  ElementCriterionPtr _parentCriterion;
};

}

#endif // WAY_NODE_CRITERION_H
