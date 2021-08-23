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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "HighwayMatch.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/SigmaAggregator.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/splitter/MultiLineStringSplitter.h>
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/conflate/highway/HighwayClassifier.h>
#include <hoot/core/conflate/matching/MatchType.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfigOptions.h>

#include <hoot/core/util/Factory.h>
#include <hoot/core/geometry/GeometryToElementConverter.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Match, HighwayMatch)

const QString HighwayMatch::MATCH_NAME = "Highway";
QString HighwayMatch::_noMatchingSubline = "No valid matching subline found.";

HighwayMatch::HighwayMatch(const std::shared_ptr<HighwayClassifier>& classifier,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher, const ConstOsmMapPtr& map,
  const ElementId& eid1, const ElementId& eid2, ConstMatchThresholdPtr mt) :
Match(mt, eid1, eid2),
_classifier(classifier),
_score(0.0),
_sublineMatcher(sublineMatcher),
_minSplitSize(0.0)
{
  assert(_eid1 != _eid2);

  const ConstElementPtr e1 = map->getElement(_eid1);
  const ConstElementPtr e2 = map->getElement(_eid2);

  LOG_VART(_eid1);
  LOG_VART(_eid2);

  try
  {
    // calculated the shared sublines
    _sublineMatch =
      _sublineMatcher->findMatch(map, e1, e2, ConfigOptions().getSearchRadiusHighway());

    if (_sublineMatch.isValid())
    {
      // calculate the match score
      _c = _classifier->classify(map, eid1, eid2, _sublineMatch);

      MatchType type = getType();
      LOG_VART(type);
      QStringList description;
      if (type != MatchType::Match)
      {
        _updateNonMatchDescriptionBasedOnGeometricProperties(description, map, e1, e2);
      }

      if (description.length() > 0)
        _explainText = description.join(" ");
      else
        _explainText = mt->getTypeDetail(_c);
    }
    else
    {
      _c.setMissP(1);
      _explainText = _noMatchingSubline;
    }

    _score = _sublineMatch.getLength() * _c.getMatchP();
  }
  // if this is an unsupported geometry configuration
  catch (const NeedsReviewException& e)
  {
    _c.clear();
    _c.setReviewP(1.0);
    _explainText = e.getWhat();
  }

  LOG_VART(_score);
  LOG_VART(_c);
  LOG_VART(_explainText);
}

void HighwayMatch::_updateNonMatchDescriptionBasedOnGeometricProperties(
  QStringList& description, const ConstOsmMapPtr& map, const ConstElementPtr e1,
  const ConstElementPtr e2) const
{
  //  Check the Angle Histogram
  double angle = AngleHistogramExtractor().extract(*map, e1, e2);
  if (angle >= 0.75)          description.append("Very similar highway orientation.");
  else if (angle >= 0.5)      description.append("Similar highway orientation.");
  else if (angle >= 0.25)     description.append("Semi-similar highway orientation.");
  else                        description.append("Highway orientation not similar.");

  //  Use the average of the edge distance extractors
  double edge =
    (EdgeDistanceExtractor(std::make_shared<RmseAggregator>()).extract(*map, e1, e2) +
     EdgeDistanceExtractor(std::make_shared<SigmaAggregator>()).extract(*map, e1, e2)) / 2.0;

  if (edge >= 90)             description.append("Highway edges very close to each other.");
  else if (edge >= 70)        description.append("Highway edges somewhat close to each other.");
  else                        description.append("Highway edges not very close to each other.");
}

map<QString, double> HighwayMatch::getFeatures(const ConstOsmMapPtr& m) const
{
  map<QString, double> result;
  if (_sublineMatch.isValid())
  {
    result = _classifier->getFeatures(m, _eid1, _eid2, _sublineMatch);
  }
  return result;
}

set<pair<ElementId, ElementId>> HighwayMatch::getMatchPairs() const
{
  set<pair<ElementId, ElementId>> result;
  result.emplace(_eid1, _eid2);
  return result;
}

double HighwayMatch::getProbability() const
{
  return _c.getMatchP();
}

bool HighwayMatch::isConflicting(
  const ConstMatchPtr& other, const ConstOsmMapPtr& map,
  const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  const HighwayMatch* hm = dynamic_cast<const HighwayMatch*>(other.get());
  // if the other match isn't a highway match then this is a conflict.
  if (hm == nullptr)
  {
    return true;
  }
  // if the other match is a highway match
  else
  {
    // See ticket #5272
    if (getClassification().getReviewP() == 1.0 || other->getClassification().getReviewP() == 1.0)
    {
      return true;
    }

    ElementId sharedEid;
    if (_eid1 == hm->_eid1 || _eid1 == hm->_eid2)
    {
      sharedEid = _eid1;
    }

    if (_eid2 == hm->_eid1 || _eid2 == hm->_eid2)
    {
      // both eids should never be the same.
      assert(sharedEid.isNull());
      sharedEid = _eid2;
    }

    // if the matches don't share at least one eid then it isn't a conflict.
    if (sharedEid.isNull())
    {
      return false;
    }

    // assign o1 and o2 to the non-shared eids
    ElementId o1 = _eid1 == sharedEid ? _eid2 : _eid1;
    ElementId o2 = hm->_eid1 == sharedEid ? hm->_eid2 : hm->_eid1;

    QHash<ElementId, bool>::const_iterator cit1 = _conflicts.find(o2);
    if (cit1 != _conflicts.end())
    {
      return cit1.value();
    }
    QHash<ElementId, bool>::const_iterator cit2 = hm->_conflicts.find(o1);
    if (cit2 != hm->_conflicts.end())
    {
      return cit2.value();
    }

    bool result;
    try
    {
      // easy optimization. If the sublines don't touch then they don't conflict
      if (_sublineMatch.touches(hm->_sublineMatch) == false)
      {
        result = false;
      }
      // we need to check for a conflict in two directions. Is it conflicting if we merge the shared
      // EID with this class first, then is it a conflict if we merge with the other EID first.
      // checking for subline match containment first. This is very fast to check and if either one
      // is true then we know it is a conflict.
      else if (_sublineMatch.contains(hm->_sublineMatch) ||
               hm->_sublineMatch.contains(_sublineMatch) ||
               _isOrderedConflicting(map, sharedEid, o1, o2) ||
               hm->_isOrderedConflicting(map, sharedEid, o2, o1))
      {
        result = true;
      }
      else
      {
        result = false;
      }
    }
    catch (const NeedsReviewException&)
    {
      result = true;
    }

    _conflicts[o2] = result;

    return result;
  }
}

bool HighwayMatch::_isOrderedConflicting(
  const ConstOsmMapPtr& map, ElementId sharedEid, ElementId other1, ElementId other2) const
{
  set<ElementId> eids;
  eids.insert(sharedEid);
  eids.insert(other1);
  eids.insert(other2);
  OsmMapPtr copiedMap = std::make_shared<OsmMap>(map->getProjection());
  CopyMapSubsetOp(map, eids).apply(copiedMap);

  WaySublineMatchString match(_sublineMatch, copiedMap);

  // split the shared line based on the matching subline
  ElementPtr matchShared, scrapsShared;
  ElementPtr match2, scraps2;
  WaySublineCollection string1 = match.getSublineString1();
  WaySublineCollection string2 = match.getSublineString2();

  if (sharedEid == _eid1)
  {
    MultiLineStringSplitter().split(copiedMap, string1, match.getReverseVector1(), matchShared,
                                    scrapsShared);
    MultiLineStringSplitter().split(copiedMap, string2, match.getReverseVector2(), match2, scraps2);
  }
  else
  {
    MultiLineStringSplitter().split(copiedMap, string2, match.getReverseVector1(), matchShared,
                                    scrapsShared);
    MultiLineStringSplitter().split(copiedMap, string1, match.getReverseVector2(), match2, scraps2);
  }

  // if the shared element has no scraps then there is a conflict
  if (!scrapsShared)
  {
    return true;
  }

  // check to see if the scraps match other2
  HighwayMatch m0(
    _classifier, _sublineMatcher, copiedMap, scrapsShared->getElementId(), other2, _threshold);
  if (m0.getType() == MatchType::Match)
  {
    return false;
  }

  // we didn't find any valid matches, so this is a conflict.
  return true;
}

QString HighwayMatch::toString() const
{
  stringstream ss;
  ss << "HighwayMatch " << _eid1 << " " << _eid2 << " P: " << _c.toString();
  return QString::fromUtf8(ss.str().c_str());
}

}
