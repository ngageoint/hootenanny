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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMatchCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/util/ConfPath.h>
#include <hoot/core/util/ConfigOptions.h>

#include "PoiPolygonMatch.h"
#include "visitors/PoiPolygonMatchVisitor.h"


namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, PoiPolygonMatchCreator)

PoiPolygonMatchCreator::PoiPolygonMatchCreator()
{
}

Match* PoiPolygonMatchCreator::createMatch(const ConstOsmMapPtr& map, ElementId eid1,
                                           ElementId eid2)
{
  PoiPolygonMatch* result = 0;

  if (eid1.getType() != eid2.getType())
  {
    ConstElementPtr e1 = map->getElement(eid1);
    ConstElementPtr e2 = map->getElement(eid2);
    const bool foundPoi = PoiPolygonMatch::isPoi(*e1) || PoiPolygonMatch::isPoi(*e2);
    const bool foundPoly = PoiPolygonMatch::isPoly(*e1) || PoiPolygonMatch::isPoly(*e2);

    if (foundPoi && foundPoly)
    {
      result = new PoiPolygonMatch(map, /*eid1, eid2,*/ getMatchThreshold(), _getRf());
      result->setConfiguration(conf());
      result->calculateMatch(eid1, eid2);
    }
  }

  return result;
}

void PoiPolygonMatchCreator::createMatches(const ConstOsmMapPtr& map,
                                           std::vector<const Match*>& matches,
                                           ConstMatchThresholdPtr threshold)
{
  LOG_INFO("Creating matches with: " << className() << "...");
  LOG_VARD(*threshold);

  PoiPolygonMatch::resetMatchDistanceInfo();

  PoiPolygonMatchVisitor v(map, matches, threshold, _getRf());
  map->visitRo(v);

  if (conf().getBool(ConfigOptions::getPoiPolygonPrintMatchDistanceTruthKey()))
  {
    PoiPolygonMatch::printMatchDistanceInfo();
  }
}

std::vector<MatchCreator::Description> PoiPolygonMatchCreator::getAllCreators() const
{
  std::vector<Description> result;
  result.push_back(
    Description(
      className(),
      "POI to Polygon Match Creator",
      //this match creator has two conflatable types, so arbitrarily just picking one of them as
      //the base feature type; stats class will handle the logic to deal with both poi and polygon
      //input types
      MatchCreator::Polygon,
      false));
  return result;
}

bool PoiPolygonMatchCreator::isMatchCandidate(ConstElementPtr element,
                                              const ConstOsmMapPtr& /*map*/)
{
  return element->isUnknown() &&
    (PoiPolygonMatch::isPoi(*element) || PoiPolygonMatch::isPoly(*element));
}

boost::shared_ptr<MatchThreshold> PoiPolygonMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiPolygonMatchThreshold(), config.getPoiPolygonMissThreshold(),
                         config.getPoiPolygonReviewThreshold()));
  }
  return _matchThreshold;
}

boost::shared_ptr<PoiPolygonRfClassifier> PoiPolygonMatchCreator::_getRf()
{
  if (!_rf)
  {
    _rf.reset(new PoiPolygonRfClassifier());
  }
  return _rf;
}

}
