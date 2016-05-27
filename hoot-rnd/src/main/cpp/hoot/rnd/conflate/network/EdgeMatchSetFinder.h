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
#ifndef EDGEMATCHSETFINDER_H
#define EDGEMATCHSETFINDER_H

#include "IndexedEdgeMatchSet.h"
#include "MatchScoreProvider.h"
#include "NetworkDetails.h"

namespace hoot
{

class EdgeMatchSetFinder
{
public:
  EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2);

  /**
   * Evaluate edges e1 and e2 for match. If the end vertices don't match then recursively search
   * for potential matches until an end is found.
   */
  void addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2);

private:

  NetworkDetailsPtr _details;
  IndexedEdgeMatchSetPtr _matchSet;
  ConstOsmNetworkPtr _n1, _n2;

  void _addEdgeMatches(EdgeMatchPtr em);

  void _addEdgeNeighborsToEnd(EdgeMatchPtr em, QList<ConstNetworkEdgePtr> neighbors1,
    QList<ConstNetworkEdgePtr> neighbors2);

  void _addEdgeNeighborsToStart(EdgeMatchPtr em, QList<ConstNetworkEdgePtr> neighbors1,
    QList<ConstNetworkEdgePtr> neighbors2);

  /// @todo replace with a real method
  double _scoreMatch(EdgeMatchPtr /*em*/) const { return 1.0; }
};

}

#endif // EDGEMATCHSETFINDER_H
