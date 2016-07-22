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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVENODEOP_H
#define REMOVENODEOP_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Remove the specified node from a map. If doCheck is true, a check
 * will be made to make sure the node is not part of any way before it is removed.
 *
 * If removeFully is true, the node will be removed from all relations,
 * then removed from teh map.
 */
class RemoveNodeOp : public OsmMapOperation
{
public:

  static string className() { return "hoot::RemoveNodeOp"; }

  RemoveNodeOp(bool doCheck = true, bool removeFully = false);

  RemoveNodeOp(long nId, bool doCheck = true, bool removeFully = false);

  void apply(shared_ptr<OsmMap>& map);

  string getClassName() const { return className(); }

  void setNodeId(long nId) { _nodeIdToRemove = nId; }

  static void removeNode(OsmMapPtr map, long nId)
  {
    RemoveNodeOp nodeRemover(nId);
    nodeRemover.apply(map);
  }

  static void removeNodeNoCheck(OsmMapPtr map, long nId)
  {
    RemoveNodeOp nodeRemover(nId, false);
    nodeRemover.apply(map);
  }

  static void removeNodeFully(OsmMapPtr map, long nId)
  {
    RemoveNodeOp nodeRemover(nId, true, true);
    nodeRemover.apply(map);
  }

private:
  long _nodeIdToRemove;
  bool _doCheck;
  bool _removeFully;

  void _removeNodeNoCheck(shared_ptr<OsmMap>& map, long nId);
  void _removeNode(shared_ptr<OsmMap>& map, long nId);
  void _removeNodeFully(shared_ptr<OsmMap>& map, long nId);

};

}

#endif // REMOVENODEOP_H
