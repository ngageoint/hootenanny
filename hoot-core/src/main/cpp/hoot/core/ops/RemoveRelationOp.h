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
#ifndef REMOVERELATIONOP_H
#define REMOVERELATIONOP_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * @brief The RemoveRelationOp class removes a relation from an OsmMap
 */
class RemoveRelationOp : public OsmMapOperation
{
public:

  /**
   * @brief className gets the class name as a string
   * @return class name
   */
  static string className() { return "hoot::RemoveRelationOp"; }
  string getClassName() const { return className(); }

  /**
   * @brief RemoveRelationOp Default constructor
   */
  RemoveRelationOp();

  /**
   * @brief RemoveRelationOp
   * @param rId ID of relation to remove
   */
  RemoveRelationOp(long rId);

  /**
   * @brief apply Peform the remove operation
   * @param map Map to operate on
   */
  void apply(shared_ptr<OsmMap>& map);

  /**
   * @brief setRelationId Set the ID of the relation to remove
   * @param rId ID of relation to remove
   */
  void setRelationId(long rId) { _rIdToRemove = rId; }

  /**
   * @brief removeRelation Remove the specified relation from the given map
   * @param map Map to operate on
   * @param rId ID of relation to remove
   */
  static void removeRelation(OsmMapPtr map, long rId)
  {
    RemoveRelationOp relationRemover(rId);
    relationRemover.apply(map);
  }

private:
  long _rIdToRemove;

};

}

#endif // REMOVERELATIONOP_H
