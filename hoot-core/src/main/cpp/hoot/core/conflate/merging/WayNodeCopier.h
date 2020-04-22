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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

  /**
   * Copies all nodes from one way to another based on an optional filtering criterion set
   * beforehand
   *
   * @param toReplaceWayId ID of the way to be replaced
   * @param replacingWayId ID of the replacing way
   */
  void copy(const ElementId& toReplaceWayId, const ElementId& replacingWayId);

  /**
   * @see OsmMapConsumer
   */
  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

  /**
   * @see ElementCriterionConsumer
   */
  virtual void addCriterion(const ElementCriterionPtr& e);

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

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
