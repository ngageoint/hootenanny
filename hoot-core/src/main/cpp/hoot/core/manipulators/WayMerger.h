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
 * @copyright Copyright (C) 2012, 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef WAYMERGER_H
#define WAYMERGER_H

#include "../Conflator.h"
#include "WayManipulator.h"
#include "WayMatchSet.h"

// GEOS
namespace geos {
  namespace geom {
    class LineString;
  }
}

// Hoot
#include <hoot/core/Units.h>
namespace hoot {
  class OsmMap;
  class Node;
  class Way;
}

namespace hoot {

  using namespace std;

/**
 * This class merges roads that are similar to roads in the companion data set. The result of the
 * merge is a new conflated highway.
 */
class WayMerger : public WayManipulator
{
public:
  static std::string className() { return "hoot::WayMerger"; }

  WayMerger();

  virtual ~WayMerger() {}

  virtual const vector< shared_ptr<Manipulation> >& findAllManipulations(
          shared_ptr<const OsmMap> map);

  virtual const vector< shared_ptr<Manipulation> >& findWayManipulations(
          shared_ptr<const OsmMap> map, const vector<long>& wids);

protected:
  /**
   * Creates a new manipulation and returns it. This may be delegated to a subclass.
   */
  virtual Manipulation* _createManipulation(long leftId, long rightId,
    shared_ptr<const OsmMap> map, Meters minSplitSize);

private:

  vector< shared_ptr<Manipulation> > _result;
  boost::shared_ptr<const OsmMap> _map;

  Meters _minSplitSize;

  /**
   * Find all parallel ways that are potential matches for the given way.
   */
  vector<long> _findOtherWays(shared_ptr<const Way> way);

  void _findMatches(long baseWayId);

};

//class WayMerger : public Manipulator
//{
//public:
//
//  WayMerger();
//
//  virtual const std::vector< boost::shared_ptr<const WorkingMap> >& apply(
//          boost::shared_ptr<const WorkingMap> wm);
//
//  /** TODO */
//  virtual const vector< shared_ptr<const Manipulation> >& findAllManipulations(
//      shared_ptr<const OsmMap> /*map*/) { return _result2; }
//
//  virtual shared_ptr<WorkingMap> initialize(shared_ptr<WorkingMap> baseMap);
//
//protected:
//
//  vector< shared_ptr<const Manipulation> > _result2;
//  size_t _maxResults;
//  // a map of all possible matches
//  WayMatchSet _possibleMatches;
//  // a set of all ids that have been evaluated for matches from both unknown1 and unknown2
//  HashSet<long> _evaluatedWays;
//  std::vector< boost::shared_ptr<const WorkingMap> > _result;
//  boost::shared_ptr<const WorkingMap> _map;
//  boost::shared_ptr<OsmMap> _newMap;
//  std::priority_queue<
//          boost::shared_ptr<const WorkingMap>,
//          std::deque< boost::shared_ptr<const WorkingMap> >,
//          Conflator::LesserWorkingMap > _resultHeap;
//
//  double _matchPercent;
//  double _matchBuffer;
//
//  Meters _distance(boost::shared_ptr<hoot::Node> n1,
//                 boost::shared_ptr<hoot::Node> n2);
//
//  /**
//   * Ensures that both ways are approximately pointed in the same direction as w1 by reversing
//   * the order of w2 if necessary.
//   */
//  void _ensureDirection(boost::shared_ptr<const hoot::Way> w1,
//                        boost::shared_ptr<hoot::Way> w2);
//
//  void _findMatches(boost::shared_ptr<const hoot::Way> way);
//
//  bool _isMatched(long wayId1, long wayId2);
//
//  long _mergeNodes(long nid1, long nid2, std::map<long, long>& replacementMap);
//
//  void _mergeWays(long wayId1, long wayId2, shared_ptr<WorkingMap> result);
//
//  void _moveTowardLine(hoot::Node* n, const geos::geom::LineString* line);
//};

}

#endif // WAYMERGER_H
