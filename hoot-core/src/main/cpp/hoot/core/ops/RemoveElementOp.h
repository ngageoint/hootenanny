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
#ifndef REMOVEELEMENTOP_H
#define REMOVEELEMENTOP_H

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Removes an element from a map. If the element exists as part of other elements it is
 * removed from those elements before being removed from the map.
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

  static string className() { return "hoot::RemoveElementOp"; }

  RemoveElementOp(bool doCheck = true);

  RemoveElementOp(ElementId eId, bool doCheck = true);

  void apply(shared_ptr<OsmMap>& map);

  string getClassName() const { return className(); }

  void setElementId(ElementId eId) {_eIdToRemove = eId; }

  /**
   * Removes an element from a map. If the element exists as part of other elements it is
   * removed from those elements before being removed from the map.
   *
   * If this element contains children (e.g. multipolygon) the children will not be
   * removed from the map.
   */
  static void removeElement(OsmMapPtr map, ElementId eId)
  {
    RemoveElementOp elementRemover(eId);
    elementRemover.apply(map);
  }

  /**
   * Removes an element from the map. No check is made before the removal, so
   * removing an element used by another Way or Relation will result in
   * undefined behaviour.
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
