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
#ifndef REUSE_NODE_IDS_ON_WAY_OP_H
#define REUSE_NODE_IDS_ON_WAY_OP_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementConsumer.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * ReuseNodeIdsOnWayOp class attempts to reuse the node IDs of a way with status UNKNOWN1 that is
 * being replaced by a way with status UNKNOWN2.  This preserves IDs of the original nodes and ends
 * up creating a modify in a changeset for all preserved IDs rather than deleting the originals and
 * creating new ones.
 */
class ReuseNodeIdsOnWayOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:

  static std::string className() { return "hoot::ReuseNodeIdsOnWayOp"; }

  ReuseNodeIdsOnWayOp(ElementId from, ElementId to);
  ReuseNodeIdsOnWayOp();

  virtual void apply(const std::shared_ptr<OsmMap> &map) override;
  virtual void addElement(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Reuses the node IDs from one way in another."; }

  virtual std::string getClassName() const { return className(); }

private:

  ElementId _from;
  ElementId _to;

};

}

#endif  //  REUSE_NODE_IDS_ON_WAY_OP_H
