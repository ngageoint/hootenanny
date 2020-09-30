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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEELEMENTBYEID_H
#define REMOVEELEMENTBYEID_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * @brief The RemoveElementByEid class removes an element from a map.
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
class RemoveElementByEid : public OsmMapOperation
{
public:

  /**
   * @brief className gets the name of the class
   * @return class name string
   */
  static std::string className() { return "hoot::RemoveElementByEid"; }

  /**
   * @brief RemoveElementByEid removes an element from the map
   * @param doCheck passed down to RemoveNode, RemoveWay, RemoveRelation
   *                to indicate if we should check to see if the element
   *                exists as a component of another element
   */
  RemoveElementByEid(bool doCheck = true);

  /**
   * @brief RemoveElementByEid removes an element from the map
   * @param eId ID of element to remove
   * @param doCheck check to see if element belongs to another element
   */
  RemoveElementByEid(ElementId eId, bool doCheck = true);

  /**
   * Constructor
   *
   * @param eId ID of element to remove
   * @param doCheck check to see if element belongs to another element
   * @param removeNodeFully ensures nodes are removed from all ways and relations in the map
   */
  RemoveElementByEid(ElementId eId, bool doCheck, bool removeNodeFully);

  virtual ~RemoveElementByEid() = default;

  /**
   * @brief apply performs the removal operation
   * @param map to operate on
   */
  void apply(OsmMapPtr& map) override;

  /**
   * @brief getClassName gest the name of the class
   * @return class name string
   */
  std::string getClassName() const { return className(); }

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
  static void removeElement(OsmMapPtr map, ElementId eId);

  /**
   * @brief removeElementNoCheck Removes an element from the map. No check
   *                             is made before the removal, so removing
   *                             an element used by another Way or Relation
   *                             will result in undefined behaviour.
   * @param map to operate on
   * @param eId ID of the element to remove
   */
  static void removeElementNoCheck(OsmMapPtr map, ElementId eId);

  /**
   * @brief removeElement Removes an element from a map only if it does not have a parent element.
   *
   * If this element contains children (e.g. multipolygon) the children will not be removed from
   * the map.
   *
   * @param map to operate on
   * @param eId ID of the element to remove
   */
  static void removeUnusedElementsOnly(OsmMapPtr map, ElementId eId);

  virtual QString getDescription() const override
  { return "Removes a single element by element ID"; }

  void setRemoveNodeFully(bool remove) { _removeNodeFully = remove; }
  void setRemoveOnlyUnusedNodes(bool remove) { _removeOnlyUnusedNodes = remove; }

private:

  ElementId _eIdToRemove;
  bool _doCheck;
  bool _removeNodeFully;
  bool _removeOnlyUnusedNodes;
};

}

#endif // REMOVEELEMENTBYEID_H
