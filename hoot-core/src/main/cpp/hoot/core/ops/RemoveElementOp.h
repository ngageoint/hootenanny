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
#ifndef REMOVEELEMENTOP_H
#define REMOVEELEMENTOP_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * @brief The RemoveElementOp class removes an element from a map.
 *
 * Removes an element from a map. First, we will check to see if the
 * element exists as part of other elements in the map. If it does, the
 * element is removed from those others, then removed from the map.
 *
 * If this element contains children (e.g. multipolygon) the children will not be
 * removed from the map.
 *
 * If you would like to remove an element and all its children then
 * see RecursiveElementRemover.
 */
class RemoveElementOp : public OsmMapOperation
{
public:

  /**
   * @brief className gets the name of the class
   * @return class name string
   */
  static string className() { return "hoot::RemoveElementOp"; }

  /**
   * @brief RemoveElementOp removes an element from the map
   * @param doCheck passed down to RemoveNode, RemoveWay, RemoveRelation
   *                to indicate if we should check to see if the element
   *                exists as a component of another element
   */
  RemoveElementOp(bool doCheck = true);

  /**
   * @brief RemoveElementOp removes an element from the map
   * @param eId ID of element to remove
   * @param doCheck check to see if element belongs to another element
   */
  RemoveElementOp(ElementId eId, bool doCheck = true);

  /**
   * @brief apply performs the removal operation
   * @param map to operate on
   */
  void apply(shared_ptr<OsmMap>& map);

  /**
   * @brief getClassName gest the name of the class
   * @return class name string
   */
  string getClassName() const { return className(); }

  /**
   * @brief setElementId sets the id of the element to remove
   * @param eId ElementId
   */
  void setElementId(ElementId eId) {_eIdToRemove = eId; }

  /**
   * @brief removeElement Removes an element from a map. If the element exists
   * as part of other elements it is removed from those elements before
   * being removed from the map.
   *
   * If this element contains children (e.g. multipolygon) the children will not be
   * removed from the map.
   *
   * @param map to operate on
   * @param eId ID of the element to remove
   */
  static void removeElement(OsmMapPtr map, ElementId eId)
  {
    RemoveElementOp elementRemover(eId);
    elementRemover.apply(map);
  }

  /**
   * @brief removeElementNoCheck Removes an element from the map. No check
   *                             is made before the removal, so removing
   *                             an element used by another Way or Relation
   *                             will result in undefined behaviour.
   * @param map to operate on
   * @param eId ID of the element to remove
   */
  static void removeElementNoCheck(OsmMapPtr map, ElementId eId)
  {
    RemoveElementOp elementRemover(eId, false);
    elementRemover.apply(map);
  }

private:
  ElementId _eIdToRemove;
  bool _doCheck;

};

}

#endif // REMOVEELEMENTOP_H
