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
#include "BuildingMatch.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/MaxWordSetDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/Soundex.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/polygon/extractors/CentroidDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/CompactnessExtractor.h>
#include <hoot/core/conflate/polygon/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/conflate/polygon/extractors/NameExtractor.h>
#include <hoot/core/conflate/polygon/extractors/OverlapExtractor.h>
#include <hoot/core/conflate/polygon/extractors/AngleHistogramExtractor.h>
#include <hoot/core/schema/TranslateStringDistance.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/index/ElementToRelationMap.h>
#include <hoot/core/index/OsmMapIndex.h>

// Standard
#include <sstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

#include "BuildingRfClassifier.h"

namespace hoot
{

QString BuildingMatch::_matchName = "Building";

BuildingMatch::BuildingMatch(const ConstOsmMapPtr& map, shared_ptr<const BuildingRfClassifier> rf,
  const ElementId& eid1, const ElementId& eid2, ConstMatchThresholdPtr mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2),
  _rf(rf),
  _explainText("")
{
  //LOG_DEBUG("Classifying building pair: " << eid1 << "; " << eid2);

  _p = _rf->classify(map, _eid1, _eid2);

  //If the buildings aren't matched and they overlap at all, then make them be reviewed.
  if (getType() == MatchType::Miss &&
      (OverlapExtractor().extract(*map, map->getElement(eid1), map->getElement(eid2)) > 0.0))
  {
    //LOG_DEBUG("Building miss overlap:");
    //LOG_VARD(eid1);
    //LOG_VARD(eid2);

    _p.clear();
    _p.setReviewP(1.0);
    _explainText = "Unmatched buildings are overlapping.";
  }
  else
  {
    _explainText = mt->getTypeDetail(_p);
  }
  //LOG_DEBUG(toString());
}

map<QString, double> BuildingMatch::getFeatures(const shared_ptr<const OsmMap>& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

set< pair<ElementId, ElementId> > BuildingMatch::getMatchPairs() const
{
  set< pair<ElementId, ElementId> > result;
  result.insert(pair<ElementId, ElementId>(_eid1, _eid2));
  return result;
}

double BuildingMatch::getProbability() const
{
  return _p.getMatchP();
}

bool BuildingMatch::isConflicting(const Match& other, const ConstOsmMapPtr& /*map*/) const
{
  const BuildingMatch* bm = dynamic_cast<const BuildingMatch*>(&other);
  if (bm == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

QString BuildingMatch::toString() const
{
  stringstream ss;
  ss << "BuildingMatch: " << _eid1 << ", " << _eid2 << " p: " << _p.toString();
  //if (getType() == MatchType::Review)
  //{
    //ss << " note: " << _explainText;
  //}
  return QString::fromStdString(ss.str());
}

}
