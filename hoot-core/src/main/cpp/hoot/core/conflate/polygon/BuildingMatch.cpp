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
#include "BuildingMatch.h"

// hoot
#include <hoot/core/util/Factory.h>
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
#include <hoot/core/conflate/MatchClassification.h>

// Standard
#include <sstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

#include "BuildingRfClassifier.h"

namespace hoot
{

QString BuildingMatch::_matchName = "Building";

BuildingMatch::BuildingMatch(const ConstOsmMapPtr& map, boost::shared_ptr<const BuildingRfClassifier> rf,
  const ElementId& eid1, const ElementId& eid2, ConstMatchThresholdPtr mt) :
  Match(mt),
  _eid1(eid1),
  _eid2(eid2),
  _rf(rf),
  _explainText("")
{
  _p = _rf->classify(map, _eid1, _eid2);

  MatchType type = getType();
  QStringList description;
  if (type != MatchType::Match)
  {
    ConstElementPtr element1 = map->getElement(eid1);
    ConstElementPtr element2 = map->getElement(eid2);
    //  Get the overlap
    double overlap = OverlapExtractor().extract(*map, element1, element2);

    //If the buildings aren't matched and they overlap at all, then make them be reviewed.
    if (getType() == MatchType::Miss && overlap > 0.0)
    {
      _p.clear();
      _p.setReviewP(1.0);
      description.append("Unmatched buildings are overlapping.");
    }
    //  Add extra explanation text to reviews
    else if (getType() == MatchType::Review)
    {
      //  Deal with the overlap first
      if (overlap >= 0.75)        description.append("Large building overlap.");
      else if (overlap >= 0.5)    description.append("Medium building overlap.");
      else if (overlap >= 0.25)   description.append("Small building overlap.");
      else                        description.append("Very little building overlap.");
      //  Next check the Angle Histogram
      double angle = AngleHistogramExtractor(0.0).extract(*map, element1, element2);
      if (angle >= 0.75)          description.append("Very similar building orientation.");
      else if (angle >= 0.5)      description.append("Similar building orientation.");
      else if (angle >= 0.25)     description.append("Semi-similar building orientation.");
      else                        description.append("Building orientation not similar.");
      //  Finally the edge distance
      double edge = EdgeDistanceExtractor(new QuantileAggregator(0.4)).extract(*map, element1, element2);
      if (edge >= 90)             description.append("Building edges very close to each other.");
      else if (edge >= 70)        description.append("Building edges somewhat close to each other.");
      else                        description.append("Building edges not very close to each other.");
    }
  }
  //  Join the string descriptions together or generate the default
  if (description.length() > 0)
    _explainText = description.join(" ");
  else
    _explainText = mt->getTypeDetail(_p);
  LOG_VART(toString());
}

map<QString, double> BuildingMatch::getFeatures(const boost::shared_ptr<const OsmMap>& m) const
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
  return QString::fromStdString(ss.str());
}

}
