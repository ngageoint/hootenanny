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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVENODEOP_H
#define REMOVENODEOP_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * @brief The RemoveNodeOp class removes the specified node from a map. If
 * doCheck is true, a check will be made to make sure the node is not
 * part of any way before it is removed.
 *
 * If removeFully is true, the node will be removed from all relations,
 * then removed from the map.
 */
class RemoveNodeOp : public OsmMapOperation
{
public:

  /**
   * @brief className the the class name as a string
   * @return the name of the class
   */
  static string className() { return "hoot::RemoveNodeOp"; }
  string getClassName() const { return className(); }

  /**
   * @brief RemoveNodeOp Removes a node from a map
   * @param doCheck check to make sure node doesn't belong to a way
   * @param removeFully makes sure node is removed from all ways, relations in the map.
   *                    Using removeFully 'overrides' the use of doCheck.
   */
  RemoveNodeOp(bool doCheck = true, bool removeFully = false);

  /**
   * @brief RemoveNodeOp Removes a node from a map
   * @param nId Node to remove
   * @param doCheck check to make sure node doesn't belong to a way
   * @param removeFully makes sure node is removed from all ways, relations in the map
   *                    Using removeFully 'overrides' the use of doCheck.
   */
  RemoveNodeOp(long nId, bool doCheck = true, bool removeFully = false);

  /**
   * @brief apply Peform the op on the given map
   * @param map map upon which to operate
   */
  void apply(shared_ptr<OsmMap>& map);

  /**
   * @brief setNodeId Sets the ID of the node to remove
   * @param nId NodeId
   */
  void setNodeId(long nId) { _nodeIdToRemove = nId; }

  /**
   * @brief removeNode Removes the node from the map. If the node belongs to a
   *                   way, a HootException is thrown.
   * @param map Map to operate on
   * @param nId ID of node to remove
   */
  static void removeNode(OsmMapPtr map, long nId)
  {
    RemoveNodeOp nodeRemover(nId);
    nodeRemover.apply(map);
  }

  /**
   * @brief removeNode Simply removes the node from the map (from index, from nodes
   *                   collection)
   * @param map Map to operate on
   * @param nId ID of the node to remove
   */
  static void removeNodeNoCheck(OsmMapPtr map, long nId)
  {
    RemoveNodeOp nodeRemover(nId, false);
    nodeRemover.apply(map);
  }

  /**
   * @brief removeNodeFully Removes node from relations, ways, then from node collection
   * @param map Map to operate on
   * @param nId ID of the node to remove
   */
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
