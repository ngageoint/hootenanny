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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

// Standard
#include <memory>
#include <vector>

// Hoot
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * This is a class for storing & maniuplating representations of roundabouts.
 * It holds nodes/ways that are part of the roundabout, and nodes/ways
 * used to replace the roundabout.
 *
 * This is a work in progress, but for now the class contains methods to extract
 * roundabout info from a map, replace a roundabout with a bunch of ways
 * connected to a center point, and put the roundabout back to the way it was
 *
 */
class Roundabout
{
public:

  /**
   * @brief Roundabout - Default constructor.
   */
  Roundabout();

  /**
   * @brief setRoundaboutWay - Set the roundabout way that this object represents
   * @param pWay - Pointer to the roundabout way
   */
  void setRoundaboutWay (WayPtr pWay);

  /**
   * @brief setRoundaboutCenter - Set the center node for this roundabout. You
   *                              can create one if needed by calling getNewCenter
   * @param pNode - Center node
   */
  void setRoundaboutCenter (NodePtr pNode);

  /**
   * @brief addRoundaboutNode - Add the node to our internal list of nodes that
   *                            make up the roundabout (perimeter)
   * @param pNode - Pointer to the node
   */
  void addRoundaboutNode (ConstNodePtr pNode) { _roundaboutNodes.push_back(pNode); }

  /**
   * @brief getRoundaboutWay - Get the roundabout way
   * @return - Pointer to the way
   */
  WayPtr getRoundaboutWay()   { return _roundaboutWay; }

  /**
   * @brief getRoundaboutNodes - Get the nodes that make up the roundabout
   * @return - Vector of node pointers
   */
  std::vector<ConstNodePtr> getRoundaboutNodes() { return _roundaboutNodes; }

  /**
   * @brief getNewCenter - Averages all of the locations of the nodes in the
   *                       roundabout to produce a center point, creates a node
   *                       at that location, and returns it
   * @param pMap - Map to operate on. We use this to get an ID for the node,
   *               but this method does not add the node to the map explicitly.
   * @return - Pointer to the newly created node
   */
  NodePtr getNewCenter(OsmMapPtr pMap);

  /**
   * @brief getCenter - Gets the node set as the roundabout's center
   * @return - Node
   */
  NodePtr getCenter() { return _pCenterNode; }

  /**
   * @brief makeRoundabout - Creates & populates a roundabout object using the
   *                         given way.
   * @param pMap - The map containing the roundabout
   * @param pWay - The roundabout way
   * @return - A newly constructed roundabout object
   */
  static std::shared_ptr<Roundabout> makeRoundabout(const OsmMapPtr &pMap,
                                                    WayPtr pWay);

  /**
   * @brief removeRoundabout - Removes this roundabout from the map, and
   *                           replaces it with a centerpoint that all ways
   *                           join up to.
   * @param pMap - map to operate on.
   */
  void removeRoundabout(OsmMapPtr pMap);

  /**
   * @brief replaceRoundabout - Does its best to put the roundabout back in
   *                            place in the map.
   * @param pMap - The map to operate on.
   */
  void replaceRoundabout(OsmMapPtr pMap);

  /**
   * @brief handleCrossingWays - Finds ways that cross the roundabout,
   *                             calculates intersection points, splits the ways
   *                             where they cross the ring, and then joins them
   *                             up with "spokes". This facilitates conflation
   *                             later on.
   * @param pMap - Map to operate upon.
   */
  void handleCrossingWays(OsmMapPtr pMap);

  /**
   * @brief overrideRoundabout
   */
  void overrideRoundabout() { _overrideStatus = true; }

private:

  // The original roundabout way
  WayPtr  _roundaboutWay;

  // The original roundabout status
  Status _status;

  // The "other" status (I.E. if this is 1, other will be 2)
  Status _otherStatus;

  // Has all the nodes that was in our original roundabout
  std::vector<ConstNodePtr> _roundaboutNodes;

  // The center node that we add
  NodePtr _pCenterNode;

  // The temp ways that we add
  std::vector<ConstWayPtr> _tempWays;

  //  Ways that should connect to the roundabout
  std::vector<WayPtr> _connectingWays;

  // For secondary roundabouts with no sibling, override the replacement
  bool _overrideStatus;

};

// For convenience
typedef std::shared_ptr<Roundabout> RoundaboutPtr;
typedef std::shared_ptr<const Roundabout> ConstRoundaboutPtr;

}

#endif //ROUNDABOUT_H
