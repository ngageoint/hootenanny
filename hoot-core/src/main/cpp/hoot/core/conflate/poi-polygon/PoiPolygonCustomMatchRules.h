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

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

class MatchClassification;
class SchemaVertex;

/**
 * A custom set of rules intended to reduce the number of unnecesary reviews between POI's
 * and polygons with the goal of never causing a miss where there should be a match.  Any rule
 * that does so over the course of time testing against different datasets should be removed
 * from this class.
 */
class PoiPolygonCustomMatchRules
{

public:

  PoiPolygonCustomMatchRules(const ConstOsmMapPtr& map, const set<ElementId>& polyNeighborIds,
                             const set<ElementId>& poiNeighborIds, double distance,
                             shared_ptr<Geometry> polyGeom, shared_ptr<Geometry> poiGeom);

  void collectInfo(ConstElementPtr poi, ConstElementPtr poly);

  bool isRecCenterMatch() const { return _isRecCenterMatch; }
  bool poiNeighborWithAddressContainedInPoly() const
  { return _poiNeighborWithAddressContainedInPoly; }
  bool ruleTriggered() const
  { return _isRecCenterMatch || _poiNeighborWithAddressContainedInPoly; }

private:

  ConstOsmMapPtr _map;

  set<ElementId> _polyNeighborIds;
  set<ElementId> _poiNeighborIds;

  double _distance;

  shared_ptr<Geometry> _polyGeom;
  shared_ptr<Geometry> _poiGeom;

  int _badGeomCount;

  bool _isRecCenterMatch;
  bool _poiNeighborWithAddressContainedInPoly;

};

}

#endif // POIPOLYGONCUSTOMMATCHRULES_H
