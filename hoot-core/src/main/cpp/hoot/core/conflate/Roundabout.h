#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <hoot/core/OsmMap.h>

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
   * @brief setRoundaboutWay -Set the roundabout way that this object represents
   * @param pWay - Pointer to the roundabout way
   */
  void setRoundaboutWay (WayPtr pWay)   { _roundaboutWay = pWay; }

  /**
   * @brief addRoundaboutNode - Add the node to our internal list of nodes that
   *                            make up the roundabout
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
   * @brief getCenter - Averages all of the locations of the nodes in the
   *                    roundabout to produce a center point, creates a node
   *                    at that location, and returns it
   * @param pMap - Map to operate on. We use this to get an ID for the node,
   *               but this method does not add the node to the map explicitly.
   * @return - Pointer to the newly created node
   */
  boost::shared_ptr<Node> getCenter(boost::shared_ptr<OsmMap> pMap);

  /**
   * @brief makeRoundabout - Creates & populates a roundabout object using the
   *                         given way.
   * @param pMap - The map containing the roundabout
   * @param pWay - The roundabout way
   * @return - A newly constructed roundabout object
   */
  static boost::shared_ptr<Roundabout> makeRoundabout (const boost::shared_ptr<OsmMap> &pMap,
                                                       WayPtr pWay);

  /**
   * @brief removeRoundabout - Removes this roundabout from the map, and
   *                           replaces it with a centerpoint that all ways
   *                           join up to.
   * @param pMap - map to operate on.
   */
  void removeRoundabout(boost::shared_ptr<OsmMap> pMap);

  /**
   * @brief replaceRoundabout - Does its best to put the roundabout back in
   *                            place in the map.
   * @param pMap - The map to operate on.
   */
  void replaceRoundabout(boost::shared_ptr<OsmMap> pMap);

  /**
   * @brief connectCrossingWays - Finds ways that cross the roundabout,
   *                              calculates intersection points, joins the
   *                              ways up with the roundabout, and removes
   *                              the bits out of the middle. Not fully
   *                              implemented yet.
   * @param pMap - Map to operate upon.
   */
  void connectCrossingWays(boost::shared_ptr<OsmMap> pMap);

private:

  // The original roundabout way
  WayPtr  _roundaboutWay;

  // Has all the nodes that was in our original roundabout
  std::vector<ConstNodePtr> _roundaboutNodes;

  // The center node that we add
  NodePtr _pCenterNode;

  // The temp ways that we add
  std::vector<ConstWayPtr> _tempWays;

}; // class Roundabout

// For convenience
typedef boost::shared_ptr<Roundabout> RoundaboutPtr;
typedef boost::shared_ptr<const Roundabout> ConstRoundaboutPtr;

} // namespace hoot

#endif //ROUNDABOUT_H
