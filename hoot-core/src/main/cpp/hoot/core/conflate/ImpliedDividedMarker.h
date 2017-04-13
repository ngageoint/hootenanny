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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef IMPLIEDDIVIDEDMARKER_H
#define IMPLIEDDIVIDEDMARKER_H

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/ops/OsmMapOperation.h>

// Standard
#include <set>
#include <vector>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{
  using namespace std;

  class NodeToWayMap;
  class OsmMap;
  class Way;

/**
 * Locates sections that implicitly appear to be divided highways. Primarily this is bridges and
 * tunnels that are divided before and after the bridge/tunnel, but not tagged appropriately in
 * the tunnel.
 */
class ImpliedDividedMarker : public OsmMapOperation
{
public:

  static string className() { return "hoot::ImpliedDividedMarker"; }

  ImpliedDividedMarker();

  ImpliedDividedMarker(boost::shared_ptr<const OsmMap> map);

  void apply(boost::shared_ptr<OsmMap>& map);

  /**
   * Splits all the ways in the input map and returns the resulting map.
   */
  static boost::shared_ptr<OsmMap> markDivided(boost::shared_ptr<const OsmMap> map);

  boost::shared_ptr<OsmMap> markDivided();

protected:

  boost::shared_ptr<const OsmMap> _inputMap;
  boost::shared_ptr<OsmMap> _result;
  boost::shared_ptr<NodeToWayMap> _n2w;

  /**
   * Returns true if the given way has a divider highway connected on both ends.
   */
  bool _dividerSandwhich(boost::shared_ptr<Way> w);

  bool _hasDividerConnected(long nodeId, long excludedWayId);
};

}

#endif // IMPLIEDDIVIDEDMARKER_H
