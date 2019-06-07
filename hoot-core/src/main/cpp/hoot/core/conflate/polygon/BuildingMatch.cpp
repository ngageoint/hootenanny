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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "BuildingMatch.h"

// hoot
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/OverlapExtractor.h>
#include <hoot/core/algorithms/extractors/SmallerOverlapExtractor.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/polygon/BuildingRfClassifier.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/OsmUtils.h>

// Qt
#include <QDateTime>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Match, BuildingMatch)

QString BuildingMatch::_matchName = "Building";

BuildingMatch::BuildingMatch() :
Match()
{
}

BuildingMatch::BuildingMatch(const ConstMatchThresholdPtr& mt) :
Match(mt)
{
}

BuildingMatch::BuildingMatch(const ConstOsmMapPtr& map,
                             const std::shared_ptr<const BuildingRfClassifier>& rf,
                             const ElementId& eid1, const ElementId& eid2,
                             const ConstMatchThresholdPtr& mt) :
Match(mt),
_eid1(eid1),
_eid2(eid2),
_rf(rf),
_explainText(""),
_reviewIfSecondaryFeatureNewer(ConfigOptions().getBuildingReviewIfSecondaryNewer()),
_dateTagKey(ConfigOptions().getBuildingDateTagKey()),
_dateFormat(ConfigOptions().getBuildingDateFormat()),
_matchReviewsWithContainment(ConfigOptions().getBuildingForceContainedMatch())
{  
  _p = _rf->classify(map, _eid1, _eid2);

  ConstElementPtr element1 = map->getElement(_eid1);
  ConstElementPtr element2 = map->getElement(_eid2);

  OsmUtils::logElementDetail(element1, map, Log::Trace, "BuildingMatch: e1");
  OsmUtils::logElementDetail(element2, map, Log::Trace, "BuildingMatch: e2");

  MatchType type = getType();
  LOG_VART(type);
  QStringList description;

  if (type != MatchType::Match)
  { 
    // If we have a review and one of the buildings completely contains the other (smaller
    // overlap = 1), then let's convert to a match if the associted config options was enabled.
    const double smallerOverlap = SmallerOverlapExtractor().extract(*map, element1, element2);
    LOG_VART(smallerOverlap);
    if (type == MatchType::Review && _matchReviewsWithContainment && smallerOverlap == 1.0)
    {
      LOG_TRACE(
        "Found building pair: " <<  _eid1 << ", " << _eid2 << " marked for review where one " <<
        "building is completely contained inside of the other. Marking as a match...")
      _p.clear();
      _p.setMatchP(1.0);
    }
    else
    {
      description = _getMatchDescription(map, type, element1, element2);
    }
  }
  else if (_reviewIfSecondaryFeatureNewer)
  {
    description = _createReviewIfSecondaryFeatureNewer(element1, element2);
  }

  //  Join the string descriptions together or generate the default
  if (description.length() > 0)
    _explainText = description.join(" ");
  else
    _explainText = mt->getTypeDetail(_p);
  LOG_VART(toString());
  LOG_VART(_explainText);
}

QStringList BuildingMatch::_createReviewIfSecondaryFeatureNewer(const ConstElementPtr& element1,
                                                                const ConstElementPtr& element2)
{
  LOG_VART(_dateTagKey);
  LOG_VART(_dateFormat);

  QStringList description;

  if (!_dateTagKey.isEmpty() && !_dateFormat.isEmpty())
  {
    ConstElementPtr refBuilding;
    ConstElementPtr secondaryBuilding;
    LOG_VART(element1->getStatus().getEnum());
    LOG_VART(element2->getStatus().getEnum());
    if (element1->getStatus().getEnum() == Status::Unknown2)
    {
      secondaryBuilding = element1;
      refBuilding = element2;
    }
    else
    {
      assert(element2->getStatus().getEnum() == Status::Unknown2);
      secondaryBuilding = element2;
      refBuilding = element1;
    }
    LOG_VART(refBuilding->getId());
    LOG_VART(secondaryBuilding->getId());

    const QString secondaryBuildingDateStrVal =
      secondaryBuilding->getTags().get(_dateTagKey).trimmed();
    const QString refBuildingDateStrVal =
      refBuilding->getTags().get(_dateTagKey).trimmed();
    if (secondaryBuildingDateStrVal.isEmpty() || refBuildingDateStrVal.isEmpty())
    {
      LOG_TRACE("Date tags not found on both buildings.");
      return description;
    }

    const QDateTime secondaryBuildingDate =
      QDateTime::fromString(secondaryBuildingDateStrVal, _dateFormat);
    LOG_VART(secondaryBuildingDate);
    if (!secondaryBuildingDate.isValid())
    {
      throw HootException(
        "Invalid configured building date format: " + secondaryBuildingDate.toString() +
        ".  Expected the form: " + _dateFormat);
    }
    const QDateTime refBuildingDate =
      QDateTime::fromString(refBuildingDateStrVal, _dateFormat);
    LOG_VART(refBuildingDate);
    if (!refBuildingDate.isValid())
    {
      throw HootException(
        "Invalid configured building date format: " + refBuildingDate.toString() +
        ".  Expected the form: " + _dateFormat);
    }

    if (secondaryBuildingDate > refBuildingDate)
    {
      _p.clear();
      _p.setReviewP(1.0);
      const QString msg =
        "Secondary building with timestamp: " + secondaryBuildingDate.toString() +
        " is newer than reference building with timestamp: " + refBuildingDate.toString();
      LOG_TRACE(msg);
      description.append(msg);
    }
  }

  return description;
}

QStringList BuildingMatch::_getMatchDescription(const ConstOsmMapPtr& map, const MatchType& type,
                                                const ConstElementPtr& element1,
                                                const ConstElementPtr& element2)
{
  QStringList description;

  //  Get the overlap
  const double overlap = OverlapExtractor().extract(*map, element1, element2);
  LOG_VART(overlap);

  // If the buildings aren't matched and they overlap at all, then make them be reviewed.
  if (type == MatchType::Miss && overlap > 0.0)
  {
    _p.clear();
    _p.setReviewP(1.0);
    description.append("Unmatched buildings are overlapping.");
  }
  //  Add extra explanation text to reviews
  else if (type == MatchType::Review)
  {
    //  Deal with the overlap first
    if (overlap >= 0.75)        description.append("Large building overlap.");
    else if (overlap >= 0.5)    description.append("Medium building overlap.");
    else if (overlap >= 0.25)   description.append("Small building overlap.");
    else                        description.append("Very little building overlap.");

    //  Next check the Angle Histogram
    const double angle = AngleHistogramExtractor(0.0).extract(*map, element1, element2);
    LOG_VART(angle);
    if (angle >= 0.75)          description.append("Very similar building orientation.");
    else if (angle >= 0.5)      description.append("Similar building orientation.");
    else if (angle >= 0.25)     description.append("Semi-similar building orientation.");
    else                        description.append("Building orientation not similar.");

    //  Finally, the edge distance
    const double edge =
      EdgeDistanceExtractor(
        ValueAggregatorPtr(new QuantileAggregator(0.4))).extract(*map, element1, element2);
    LOG_VART(edge);
    if (edge >= 90)             description.append("Building edges very close to each other.");
    else if (edge >= 70)        description.append("Building edges somewhat close to each other.");
    else                        description.append("Building edges not very close to each other.");
  }

  return description;
}

map<QString, double> BuildingMatch::getFeatures(const ConstOsmMapPtr& m) const
{
  return _rf->getFeatures(m, _eid1, _eid2);
}

set<pair<ElementId, ElementId>> BuildingMatch::getMatchPairs() const
{
  set<pair<ElementId, ElementId>> result;
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
