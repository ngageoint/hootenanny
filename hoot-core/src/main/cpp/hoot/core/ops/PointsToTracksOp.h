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
 * @copyright Copyright (C) 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POINTSTOTRACKSOP_H
#define POINTSTOTRACKSOP_H

// hoot
#include <hoot/core/OsmMap.h>

// Standard
#include <deque>

#include "OsmMapOperation.h"

namespace hoot
{
using namespace Tgs;

/**
 * Useful when connecting nodes that make up a GPS track into a way.
 *
 * Any nodes that contain both "hoot:track:id" and "hoot:track:seq" keys will be grouped into
 * tracks, ordered and then placed into a way.
 *
 * The "hoot:track:id" tag specifies what track the node is part of. The id is treated as a string.
 *
 * The "hoot:track:seq" tag specifies the sequence of the nodes in the way. This is sorted
 * as a string so make sure you don't have string likes "13" and "112". Padding with 0 or spaces
 * should do the trick. I've also noticed some data sets us the date for the sequence. Please
 * be aware that the date string may only have second precision and may cause issues.
 *
 * @note If the nodes all contain the same highway tag then the highway tag will be added to the
 * way and removed from all the nodes. This is a bit of convenient hard coding.
 */
class PointsToTracksOp : public OsmMapOperation
{
public:
  static string className() { return "hoot::PointsToTracksOp"; }

  PointsToTracksOp();

  /**
   * Adds ways to all tracks as defined in the description above. No check is performed to make
   * sure the tracks are not duplicates.
   */
  virtual void apply(shared_ptr<OsmMap>& map);

private:

  /**
   * Sorts each of the tracks by "hoot:track:seq".
   */
  void _sortTracks(shared_ptr<OsmMap>& map, HashMap<QString, deque<long> >& tracks);

  /**
   * Creates one way for each "hoot:track:id".
   */
  void _createWays(shared_ptr<OsmMap>& map, HashMap<QString, deque<long> >& tracks);
};

}

#endif // POINTSTOTRACKSOP_H
