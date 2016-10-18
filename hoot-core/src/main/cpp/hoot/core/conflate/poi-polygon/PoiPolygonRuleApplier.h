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
#ifndef POIPOLYGONRULEAPPLIER_H
#define POIPOLYGONRULEAPPLIER_H

// geos
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/OsmMap.h>

namespace hoot
{

class MatchClassification;
class SchemaVertex;

/**
 * A custom set of rules intended to prevent bad matches and reduce unnecessary reviews during
 * poi to poly conflation.
 */
class PoiPolygonRuleApplier
{

public:

  PoiPolygonRuleApplier(const ConstOsmMapPtr& map, const set<ElementId>& areaNeighborIds,
                            const set<ElementId>& poiNeighborIds, double distance,
                            double nameScoreThreshold, bool nameMatch, bool exactNameMatch,
                            double typeScoreThreshold, double typeScore, bool typeMatch,
                            double matchDistance, shared_ptr<Geometry> polyGeom,
                            shared_ptr<Geometry> poiGeom, const QString testUuid);

  bool applyRules(ConstElementPtr poi, ConstElementPtr poly, MatchClassification& matchClass);

private:

  ConstOsmMapPtr _map;

  set<ElementId> _areaNeighborIds;
  set<ElementId> _poiNeighborIds;

  double _distance;
  double _nameScoreThreshold;
  bool _nameMatch;
  bool _exactNameMatch;
  double _typeScoreThreshold;
  double _typeScore;
  bool _typeMatch;
  double _matchDistance;

  static vector<SchemaVertex> _allTags;

  shared_ptr<Geometry> _polyGeom;
  shared_ptr<Geometry> _poiGeom;

  int _badGeomCount;

  QString _testUuid;

  bool _isPark(ConstElementPtr element) const;
  bool _isParkish(ConstElementPtr element) const;
  bool _isRecCenter(ConstElementPtr element) const;
  bool _isRecCenter2(ConstElementPtr element) const;
  bool _isPlayground(ConstElementPtr element) const;
  bool _isPlayArea(ConstElementPtr element) const;
  bool _isSport(ConstElementPtr element) const;
  bool _isBuildingIsh(ConstElementPtr element) const;
  bool _hasMoreThanOneType(ConstElementPtr element) const;

};

}

#endif // POIPOLYGONPARKAPPLIER_H
