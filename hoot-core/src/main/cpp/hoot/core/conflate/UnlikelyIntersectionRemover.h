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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef UNLIKELYINTERSECTIONREMOVER_H
#define UNLIKELYINTERSECTIONREMOVER_H

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

  class OsmMap;
  class Way;

/**
 * Locates intersections that are likely mistakes and separates them. This is typically a problem
 * with data ingested into OSM (e.g. goverment data such as TIGER).
 */
class UnlikelyIntersectionRemover : public OsmMapOperation
{
public:

  static string className() { return "hoot::UnlikelyIntersectionRemover"; }

  UnlikelyIntersectionRemover();

  void apply(boost::shared_ptr<OsmMap>& map);

  /**
   * Splits all the ways in the input map and returns the resulting map.
   */
  static void removeIntersections(boost::shared_ptr<OsmMap> map);

protected:

  boost::shared_ptr<OsmMap> _result;

  void _evaluateAndSplit(long intersectingNode, const set<long>& wayIds);

  double _pIntersection(long intersectingNode, boost::shared_ptr<Way> w1, boost::shared_ptr<Way> w2);

  void _splitIntersection(long intersectingNode, const vector< boost::shared_ptr<Way> >& g2);

};

}


#endif // UNLIKELYINTERSECTIONREMOVER_H
