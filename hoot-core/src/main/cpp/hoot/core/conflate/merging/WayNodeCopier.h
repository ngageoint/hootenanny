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

namespace hoot
{

/**
 * TODO
 */
class WayNodeCopier : public OsmMapConsumer, public ElementCriterionConsumer
{
public:

  WayNodeCopier();

  /**
   * TODO
   *
   * @param toReplaceWayId
   * @param replacingWayId
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

private:

  OsmMapPtr _map;
  ElementCriterionPtr _crit;
};

}

#endif // WAY_NODE_COPIER_H
