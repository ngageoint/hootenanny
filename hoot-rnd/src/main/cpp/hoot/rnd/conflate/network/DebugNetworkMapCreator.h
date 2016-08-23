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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef DEBUGNETWORKMAPCREATOR_H
#define DEBUGNETWORKMAPCREATOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/rnd/conflate/network/NetworkEdgeScore.h>
#include <hoot/rnd/conflate/network/NetworkVertexScore.h>

namespace hoot
{

/**
 * Create a number of elements in the specified map that describe the network matches that were
 * established. This is useful for debugging.
 */
class DebugNetworkMapCreator
{
public:
  DebugNetworkMapCreator(double matchThreshold = 0.15);

  void addDebugElements(OsmMapPtr map, QList<NetworkEdgeScorePtr> edgeScores,
    QList<NetworkVertexScorePtr> vertexScores);

private:
  double _matchThreshold;

  void _addEdgeLink(OsmMapPtr map, NetworkEdgeScorePtr edgeScore);

  void _addVertexLink(OsmMapPtr map, NetworkVertexScorePtr vertexScore);

  ConstNodePtr _getMedianNode(ConstOsmMapPtr map, QList<ConstElementPtr> e);
  ConstNodePtr _getMedianNode(ConstOsmMapPtr map, ConstElementPtr e);
};

}

#endif // DEBUGNETWORKMAPCREATOR_H
