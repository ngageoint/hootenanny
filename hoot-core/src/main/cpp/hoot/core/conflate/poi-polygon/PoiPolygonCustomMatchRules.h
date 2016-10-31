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
#ifndef POIPOLYGONCUSTOMMATCHRULES_H
#define POIPOLYGONCUSTOMMATCHRULES_H

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

class MatchClassification;
class SchemaVertex;

/**
 * A set of custom rules that allow for additional match tweaking beyond the additive model used
 * in PoiPolygonMatch.
 *
 * Originally, this class was used to reduce unnecessary reviews only and had several custom rules.
 * It was successful at doing that without introducing incorrect matches when running PoiPolygon
 * conflation in isolation, but when combined with other conflation types, errors were introduced.
 * The remaining rules are very specific and only have a small impact on the test data we've used
 * so far.  Therefore, this class is disabled by default.  Its left intact, though, in case a
 * situation is ever encountered where a custom rule could benefit the conflation of a particualar
 * dataset.  In the case that this happens, moving this logic into hoot-js might end up making more
 * sense.
 */
class PoiPolygonCustomMatchRules
{

public:

  PoiPolygonCustomMatchRules(const ConstOsmMapPtr& map, const set<ElementId>& polyNeighborIds,
                             const set<ElementId>& poiNeighborIds, double distance);

  /**
   * Collects information from the elements passed in and their relationships to other elements
   * around them.  Can be an expensive operation as it potentially loops through many surrounding
   * elements.
   *
   * @param poi a POI element to collect information from
   * @param poly a polygon element to collect information from
   */
  void collectInfo(ConstElementPtr poi, ConstElementPtr poly);

  /**
   * Determines if any custom rule was triggered.
   *
   * @return true if a rule was triggered
   */
  bool ruleTriggered() const
  { return _isRecCenterMatch || _poiNeighborWithAddressContainedInPoly; }

private:

  ConstOsmMapPtr _map;

  set<ElementId> _polyNeighborIds;
  set<ElementId> _poiNeighborIds;

  //distance between the poi and poly
  double _distance;

  int _badGeomCount;

  //a custom recreation center match was found
  bool _isRecCenterMatch;
  //a custom match was found where a POI has an address that matches the address of another POI
  //contained with the polygon its being compared to in collectInfo
  bool _poiNeighborWithAddressContainedInPoly;

};

}

#endif // POIPOLYGONCUSTOMMATCHRULES_H
