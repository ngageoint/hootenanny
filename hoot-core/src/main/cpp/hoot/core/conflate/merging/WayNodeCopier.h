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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef WAY_NODE_COPIER_H
#define WAY_NODE_COPIER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Performs way node copying from one way to another
 */
class WayNodeCopier : public OsmMapConsumer, public ElementCriterionConsumer, public Configurable
{
public:

  WayNodeCopier();
  ~WayNodeCopier() = default;

  /**
   * Copies all nodes from one way to another based on an optional filtering criterion set
   * beforehand
   *
   * @param toReplaceWayId ID of the way to be replaced
   * @param replacingWayId ID of the replacing way
   */
  void copy(const ElementId& toReplaceWayId, const ElementId& replacingWayId) const;

  /**
   * @see OsmMapConsumer
   */
  void setOsmMap(OsmMap* map) override { _map = map->shared_from_this(); }

  /**
   * @see ElementCriterionConsumer
   */
  void addCriterion(const ElementCriterionPtr& e) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

private:

  OsmMapPtr _map;
  // optional filtering criterion
  ElementCriterionPtr _crit;
  // allows for some leeway in what's considered a duplicate node; passed into WayLocation; 0.0 to
  // 1.0; a higher value makes duplicates more likely
  double _duplicateNodeTolerance;
};

}

#endif // WAY_NODE_COPIER_H
