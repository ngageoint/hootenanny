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
#ifndef POIPOLYGONPARKRULEAPPLIER_H
#define POIPOLYGONPARKRULEAPPLIER_H


// Hoot
//#include <hoot/core/elements/Element.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

class MatchClassification;

/**
 * A custom set of rules intended to reduce unnecessary reviews when matching park related features.
 */
class PoiPolygonParkRuleApplier
{

public:

  PoiPolygonParkRuleApplier(const ConstOsmMapPtr& map, const set<ElementId>& areaNeighborIds,
                            const set<ElementId>& poiNeighborIds, double distance,
                            double nameScoreThreshold, bool exactNameMatch, const QString testUuid);

  bool applyRules(ConstElementPtr poi, ConstElementPtr poly, MatchClassification& matchClass);

private:

  ConstOsmMapPtr _map;

  set<ElementId> _areaNeighborIds;
  set<ElementId> _poiNeighborIds;

  double _distance;
  double _nameScoreThreshold;
  bool _exactNameMatch;

  QString _testUuid;

  bool _elementIsPark(ConstElementPtr element) const;
  bool _elementIsParkish(ConstElementPtr element) const;
  bool _elementIsRecCenter(ConstElementPtr element) const;
  bool _elementIsPlayground(ConstElementPtr element) const;
  bool _elementIsPlayArea(ConstElementPtr element) const;
  bool _elementIsSport(ConstElementPtr element) const;

};

}

#endif // POIPOLYGONPARKRULEAPPLIER_H
