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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonMergerCreator.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Standard
#include <typeinfo>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, PoiPolygonMergerCreator)

PoiPolygonMergerCreator::PoiPolygonMergerCreator()
  : _map(nullptr),
    _autoMergeManyPoiToOnePolyMatches(ConfigOptions().getPoiPolygonAutoMergeManyPoiToOnePolyMatches()),
    _allowCrossConflationMerging(ConfigOptions().getPoiPolygonAllowCrossConflationMerging())
{
  LOG_VARD(_allowCrossConflationMerging);
}

MatchPtr PoiPolygonMergerCreator::_createMatch(const ConstOsmMapPtr& map, ElementId eid1, ElementId eid2) const
{
  return MatchFactory::getInstance().createMatch(map, eid1, eid2);
}

bool PoiPolygonMergerCreator::createMergers(const MatchSet& matches, vector<MergerPtr>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  bool foundAPoi = false;
  bool foundAPolygon = false;
  QStringList matchTypes;
  // go through all the matches
  for (const auto& m : matches)
  {
    LOG_VART(m->toString());
    LOG_VART(m->getName());
    // This could be a generic PointPolygon match, which we don't process here.
    if (m->getName() != PoiPolygonMatch::getPoiPolygonMatchName())
      continue;
    if (m->getMatchMembers() & MatchMembers::Poi)
      foundAPoi = true;
    if (m->getMatchMembers() & MatchMembers::Polygon)
      foundAPolygon = true;
    if (matchTypes.contains(m->getName()) == false)
      matchTypes.append(m->getName());
  }
  LOG_VART(foundAPoi);
  LOG_VART(foundAPolygon);

  // If there is at least one POI and at least one polygon, then we need to merge things in a
  // special way.
  if (foundAPoi && foundAPolygon)
  {
    set<pair<ElementId, ElementId>> eids;

    // go through all the matches
    for (const auto& m : matches)
    {
      LOG_VART(m->toString());
      // All of the other merger creators check the type of the match and do nothing if it isn't
      // the correct type (in this case a PoiPolygon match).  Adding that logic in here can cause
      // problems where some matches are passed up completely by all mergers, which results in an
      // exception.  If this merger is meant to be catch all for pois/polygons, then this is ok.  If
      // not, then some rework may need to be done here.
      set<pair<ElementId, ElementId>> s = m->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }
    LOG_VART(eids);

    if (_isConflictingSet(matches))
    {
      mergers.emplace_back(
        std::make_shared<MarkForReviewMerger>(
          eids,
          "Conflicting information: multiple features have been matched to the same feature and "
          "require review.",
          matchTypes.join(";"),
          1));
      LOG_TRACE("Pushed back review merger for : " << eids);
    }
    else
    {
      mergers.emplace_back(std::make_shared<PoiPolygonMerger>(eids));
      LOG_TRACE("Pushed back merger for : " << eids);
    }
    result = true;
  }
  else
  {
    LOG_TRACE("Match invalid; skipping merge.");
  }
  return result;
}

vector<CreatorDescription> PoiPolygonMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates mergers that merge POIs into polygons",
    false);
  return result;
}

bool PoiPolygonMergerCreator::isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1,
                                            ConstMatchPtr m2, const QHash<QString,
                                            ConstMatchPtr>& /*matches*/) const
{
  LOG_VART(m1);
  LOG_VART(m2);

  bool foundAPoi = false;
  bool foundAPolygon = false;
  if (m1->getMatchMembers() & MatchMembers::Poi || m2->getMatchMembers() & MatchMembers::Poi)
    foundAPoi = true;
  if (m1->getMatchMembers() & MatchMembers::Polygon || m2->getMatchMembers() & MatchMembers::Polygon)
    foundAPolygon = true;

  bool result;
  if (foundAPoi && foundAPolygon)
  {
    LOG_TRACE("Found a poi and a polygon...");

    // get out the matched pairs from the matches
    set<pair<ElementId, ElementId>> p1 = m1->getMatchPairs();
    set<pair<ElementId, ElementId>> p2 = m2->getMatchPairs();
    // We're expecting them to have one match each. More could be handled, but are not necessary at
    // this time.
    LOG_VART(p1.size());
    LOG_VART(p2.size());
    assert(p1.size() == 1 && p2.size() == 1);
    pair<ElementId, ElementId> eids1 = *p1.begin();
    pair<ElementId, ElementId> eids2 = *p2.begin();
    LOG_VART(eids1);
    LOG_VART(eids2);

    LOG_VART(eids1.first);
    LOG_VART(eids2.first);
    LOG_VART(eids1.second);
    LOG_VART(eids2.second);
    // There should be one element id that is shared between the matches. Find it as well as the
    // other elements.
    ElementId sharedEid, o1, o2;
    if (eids1.first == eids2.first || eids1.first == eids2.second)
    {
      sharedEid = eids1.first;
      o1 = eids1.second;
    }
    else if (eids1.second == eids2.first || eids1.second == eids2.second)
    {
      sharedEid = eids1.second;
      o1 = eids1.first;
    }
    // There are no overlapping element ids, so this isn't a conflict.
    else
    {
      LOG_TRACE("no conflict");
      result = false;
      return result;
    }
    LOG_VART(sharedEid);
    LOG_VART(o1);

    if (eids2.first == sharedEid)
      o2 = eids2.second;
    else
      o2 = eids2.first;
    LOG_VART(o2);

    // We only want the auto-merge to occur here in the situation where two pois are matching
    // against the same poly. We do not want auto-merges occurring in the situation where the
    // same poi is being reviewed against multiple polys, so those end up as reviews.

    // not passing the tag ignore list here, since it would have already be used when calling
    // these methods from PoiPolygonMatch
    if (_autoMergeManyPoiToOnePolyMatches &&
        _poiCrit.isSatisfied(map->getElement(o1)) &&
        _poiCrit.isSatisfied(map->getElement(o2)) &&
        _polyCrit.isSatisfied(map->getElement(sharedEid)))
    {
      LOG_TRACE("Automatically merging pois: " << o1 << ", " << o2 << " into poly: " << sharedEid);
      return false;
    }

    // Create POI/Polygon matches and check to see if it is a miss.
    std::shared_ptr<Match> ma(_createMatch(map, o1, o2));

    // Return conflict only if it is a miss, a review is ok.
    result = false;
    if (ma.get() == nullptr || ma->getType() == MatchType::Miss)
    {
      LOG_TRACE("miss");
      result = true;
    }
  }
  else
  {
    LOG_TRACE("no conflict");
    result = false;
  }

  return result;
}

bool PoiPolygonMergerCreator::_isConflictingSet(const MatchSet& matches) const
{
  // _map must be set using setOsmMap()
  assert(_map != 0);
  LOG_VART(matches.size());
  LOG_VART(matches);
  ConstOsmMapPtr map = _map->shared_from_this();

  for (const auto& m1 : matches)
  {
    LOG_VART(m1);
    for (const auto& m2 : matches)
    {
      LOG_VART(m2);
      if (m1 != m2)
      {
        const bool conflicting = MergerFactory::getInstance().isConflicting(map, m1, m2);
        // if one of the mergers returned a conflict and cross conflation merging is enabled
        if (conflicting)
        {
          if (_allowCrossConflationMerging)
          {
            const bool oneIsPoiPolyMatch =
              m1->toString().contains("PoiPolygonMatch") ||
              m2->toString().contains("PoiPolygonMatch");
            LOG_VART(oneIsPoiPolyMatch);
            const bool oneIsBuildingMatch =
              m1->toString().contains("BuildingMatch") || m2->toString().contains("BuildingMatch");
            LOG_VART(oneIsBuildingMatch);
            LOG_VART(m1->getType());
            LOG_VART(m2->getType());
            // if we have exactly one poi/poly match, exactly one building match, and both have a
            // status of matched
            if (oneIsPoiPolyMatch && oneIsBuildingMatch && m1->getType() == MatchType::Match &&
                m2->getType() == MatchType::Match)
            {
              // We'll ignore the conflict, so all the matches will merge together.
              LOG_TRACE(
                "Allowing cross conflation Building/PoiPoly auto-merge for: " << m1 << " and " <<
                m2 << "...");
              return false;
            }
            else
            {
              LOG_TRACE("conflicting: " << m1 << " and " << m2);
              return true;
            }
          }

          LOG_TRACE("conflicting: " << m1 << " and " << m2);
          return true;
        }
      }
    }
  }

  return false;
}

void PoiPolygonMergerCreator::convertSharedMatchesToReviews(MatchSetVector& matchSets,
                                                            std::vector<MergerPtr>& mergers,
                                                            const QStringList& matchNameFilter)
{
  LOG_DEBUG("Marking POI/Polygon matches overlapping with POI/POI matches as reviews for " <<
            StringUtils::formatLargeNumber(matchSets.size()) << " match sets...");

  // Get a mapping of all the IDs of elements belonging to both POI/Polygon and a match with one
  // of the specified types.
  QStringList matchNameFilterToUse = matchNameFilter;
  matchNameFilterToUse.append(PoiPolygonMatch::MATCH_NAME);
  QMultiHash<ElementId, QString> elementIdsToMatchTypes;
  for (const auto& matchSet : matchSets)
  {
    for (const auto& match : matchSet)
    {
      const QString matchName = match->getName();
      if (matchNameFilterToUse.contains(matchName))
      {
        const std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
        for (const auto& elementPair : matchPairs)
        {
          if (!elementIdsToMatchTypes.contains(elementPair.first, matchName))
            elementIdsToMatchTypes.insert(elementPair.first, matchName);
          if (!elementIdsToMatchTypes.contains(elementPair.second, matchName))
            elementIdsToMatchTypes.insert(elementPair.second, matchName);
        }
      }
    }
  }
  LOG_VARD(elementIdsToMatchTypes.size());
  if (elementIdsToMatchTypes.isEmpty())
  {
    return;
  }

  // Find all elements involved in matches of multiple types.
  QSet<ElementId> elementIdsInvolvedInOverlappingMatch;
  const QList<ElementId> elementIds = elementIdsToMatchTypes.keys();
  for (const auto& elementId : elementIds)
  {
    if (elementIdsToMatchTypes.values(elementId).size() > 1)
      elementIdsInvolvedInOverlappingMatch.insert(elementId);
  }
  LOG_VARD(elementIdsInvolvedInOverlappingMatch.size());
  if (elementIdsInvolvedInOverlappingMatch.isEmpty())
    return;

  // For all elements found to be in overlapping POI/Polygon matches, add a review merger for the
  // associated match pairs, and exclude the entire match set that match pair is in from the output
  // match set so the match set doesn't get processed more than once.
  MatchSetVector filteredMatchSets;
  for (const auto& matchSet : matchSets)
  {
    MatchSet filteredMatchSet;
    for (const auto& match : matchSet)
    {
      const std::set<std::pair<ElementId, ElementId>> matchPairs = match->getMatchPairs();
      for (const auto& elementPair : matchPairs)
      {
        if (match->getName() == PoiPolygonMatch::MATCH_NAME &&
            (elementIdsInvolvedInOverlappingMatch.contains(elementPair.first) ||
             elementIdsInvolvedInOverlappingMatch.contains(elementPair.second)))
        {
          LOG_TRACE("Adding review for POI to Polygon match conflicting with another POI/POI match and " <<
                    "removing; ids: " << matchPairs << "...");
          mergers.push_back(
            std::make_shared<MarkForReviewMerger>(
              matchPairs, "Inter-matcher overlapping matches", match->getName(), 1.0));
        }
        else
          filteredMatchSet.insert(match);
      }
    }
    if (filteredMatchSet.size() != 0)
      filteredMatchSets.push_back(matchSet);
  }
  LOG_VARD(mergers.size());
  LOG_VARD(matchSets.size());
  LOG_VARD(filteredMatchSets.size());
  matchSets.swap(filteredMatchSets);
}

}
