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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVENODEBYEID_H
#define REMOVENODEBYEID_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * Removes the specified node from a map.
 */
class RemoveNodeByEid : public ConstOsmMapOperation
{
public:

  static QString className() { return "RemoveNodeByEid"; }

  /**
   * Constructor
   *
   * @param doCheck check to make sure node doesn't belong to a way
   * @param removeFully makes sure node is removed from all ways, relations in the map; Using
   * removeFully 'overrides' the use of doCheck.
   */
  RemoveNodeByEid(bool doCheck = true, bool removeFully = false);
  /**
   * Constructor
   *
   * @param nId ID of node to remove
   * @param doCheck check to make sure node doesn't belong to a way
   * @param removeFully makes sure node is removed from all ways, relations in the map; Using
   * removeFully 'overrides' the use of doCheck.
   * @param removeOnlyUnused only removes nodes that aren't a part of a way doCheck must be true and
   * removeFully false
   */
  RemoveNodeByEid(long nId, bool doCheck = true, bool removeFully = false, bool removeOnlyUnused = false);
  ~RemoveNodeByEid() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * Removes the node from the map. If the node belongs to a way, a HootException is thrown.
   *
   * @param map Map to operate on
   * @param nId ID of node to remove
   * @param removeOnlyUnused remove only nodes that aren't a part of a way
   */
  static void removeNode(const OsmMapPtr& map, long nId, bool removeOnlyUnused = false);
  /**
   * Simply removes the node from the map (from index, from nodes collection)
   *
   * @param map Map to operate on
   * @param nId ID of the node to remove
   */
  static void removeNodeNoCheck(const OsmMapPtr& map, long nId);
  /**
   * Removes node from relations, ways, then from node collection
   *
   * @param map Map to operate on
   * @param nId ID of the node to remove
   */
  static void removeNodeFully(const OsmMapPtr& map, long nId);

  void setNodeId(long nId) { _nodeIdToRemove = nId; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes a single node by element ID"; }

private:

  long _nodeIdToRemove;
  bool _doCheck;
  bool _removeFully;
  bool _removeOnlyUnused;

  void _removeNodeNoCheck(const OsmMapPtr& map, long nId) const;
  void _removeNode(const OsmMapPtr& map, long nId) const;
  void _removeNodeFully(const OsmMapPtr& map, long nId) const;
};

}

#endif // REMOVENODEBYEID_H
