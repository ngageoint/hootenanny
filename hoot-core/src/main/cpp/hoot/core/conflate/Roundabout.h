#ifndef ROUNDABOUT_H
#define ROUNDABOUT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * Basic container class for holding roundabouts. Holds nodes/ways
 * that are part of the roundabout, and nodes/ways used to replace
 * the roundabout.
 *
 * Get all the nodes in the roundabout way.
 * Iterate through them, and see if they belong to another way.
 * If they do, keep them.
 *
 * Delete the roundabout
 *
 * Iterate through the nodes that were kept, and connect them to
 * the centerpoint with temp ways.
 *
 * Maybe needs a map pointer or something to operate...
 *
 */
class Roundabout
{
public:
  Roundabout();
  void setRoundaboutWay (WayPtr pWay)   { _roundaboutWay = pWay; }
  void addRoundaboutNode (ConstNodePtr pNode) { _roundaboutNodes.push_back(pNode); }
  ConstWayPtr getRoundaboutWay()   { return _roundaboutWay; }
  std::vector<ConstNodePtr> getRoundaboutNodes() { return _roundaboutNodes; }

  // Gets a node with the coords of the center
  boost::shared_ptr<Node> getCenter(boost::shared_ptr<OsmMap> pMap);

  static boost::shared_ptr<Roundabout> makeRoundabout (const boost::shared_ptr<OsmMap> &pMap,
                                                       WayPtr pWay);

  // TODO: put this somewhere
  void handleCrossingWays(boost::shared_ptr<OsmMap> pMap);

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

typedef boost::shared_ptr<Roundabout> RoundaboutPtr;
typedef boost::shared_ptr<const Roundabout> ConstRoundaboutPtr;

} // namespace hoot

#endif
