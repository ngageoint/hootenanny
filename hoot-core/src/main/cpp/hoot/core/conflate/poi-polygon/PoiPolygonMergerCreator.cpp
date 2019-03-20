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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonMergerCreator.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMatch.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonMerger.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

// Standard
#include <typeinfo>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, PoiPolygonMergerCreator)

PoiPolygonMergerCreator::PoiPolygonMergerCreator() :
_map(0),
_autoMergeManyPoiToOnePolyMatches(ConfigOptions().getPoiPolygonAutoMergeManyPoiToOnePolyMatches())
{
}

Match* PoiPolygonMergerCreator::_createMatch(const ConstOsmMapPtr& map, ElementId eid1,
  ElementId eid2) const
{
  return MatchFactory::getInstance().createMatch(map, eid1, eid2);
}

bool PoiPolygonMergerCreator::createMergers(const MatchSet& matches, vector<Merger*>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  bool foundAPoi = false;
  bool foundAPolygon = false;
  QStringList matchTypes;
  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m = *it;
    LOG_VART(m->toString());
    if (m->getMatchMembers() & MatchMembers::Poi)
    {
      foundAPoi = true;
    }
    if (m->getMatchMembers() & MatchMembers::Polygon)
    {
      foundAPolygon = true;
    }
    if (matchTypes.contains(m->getMatchName()) == false)
    {
      matchTypes.append(m->getMatchName());
    }
  }
  LOG_VART(foundAPoi);
  LOG_VART(foundAPolygon);
  if (!foundAPoi || !foundAPolygon)
  {
    LOG_TRACE("Match invalid; skipping merge.");
  }

  // if there is at least one POI and at least one polygon, then we need to merge things in a
  // special way.
  if (foundAPoi && foundAPolygon)
  {
    set< pair<ElementId, ElementId> > eids;

    // go through all the matches
    for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
      const Match* m = *it;
      LOG_VART(m->toString());

      //All of the other merger creators check the type of the match and do nothing if it isn't
      //the correct type (in this case a PoiPolygon match).  Adding that logic in here can cause
      //problems where some matches are passed up completely by all mergers, which results in an
      //exception.  If this merger is meant to be catch all for pois/polygons, then this is ok.  If
      //not, then some rework may need to be done here.
      set< pair<ElementId, ElementId> > s = m->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }
    LOG_VART(eids);

    if (_isConflictingSet(matches))
    {
      mergers.push_back(
        new MarkForReviewMerger(
          eids,
          "Conflicting information: multiple features have been matched to the same feature and require review.",
          matchTypes.join(";"), 1));
    }
    else
    {
      mergers.push_back(new PoiPolygonMerger(eids));
    }

    result = true;
  }

  return result;
}

vector<CreatorDescription> PoiPolygonMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(className(), "Generates mergers that merge POIs into polygons", false));
  return result;
}

bool PoiPolygonMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  bool foundAPoi = false;
  bool foundAPolygon = false;
  if (m1->getMatchMembers() & MatchMembers::Poi || m2->getMatchMembers() & MatchMembers::Poi)
  {
    foundAPoi = true;
  }
  if (m1->getMatchMembers() & MatchMembers::Polygon || m2->getMatchMembers() & MatchMembers::Polygon)
  {
    foundAPolygon = true;
  }

  bool result;
  if (foundAPoi && foundAPolygon)
  {
    LOG_TRACE("Found a poi and a polygon...");

    // get out the matched pairs from the matches
    set< pair<ElementId, ElementId> > p1 = m1->getMatchPairs();
    set< pair<ElementId, ElementId> > p2 = m2->getMatchPairs();
    // we're expecting them to have one match each, more could be handled, but are not necessary at
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
    // there should be one elementId that is shared between the matches, find it as well as the
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
    // there are no overlapping ElementIds, this isn't a conflict.
    else
    {
      LOG_TRACE("no conflict");
      result = false;
      return result;
    }
    LOG_VART(sharedEid);
    LOG_VART(o1);

    if (eids2.first == sharedEid)
    {
      o2 = eids2.second;
    }
    else
    {
      assert(eids2.second == sharedEid);
      o2 = eids2.first;
    }
    LOG_VART(o2);

    //We only want the auto-merge to occur here in the situation where two pois are matching
    //against the same poly.  We do not want auto-merges occurring in the situation where the
    //same poi is being reviewed against multiple polys, so those end up as reviews.

    //not passing the tag ignore list here, since it would have already be used when calling
    //these methods from PoiPolygonMatch
    if (_autoMergeManyPoiToOnePolyMatches &&
        _poiCrit.isSatisfied(map->getElement(o1)) &&
        _poiCrit.isSatisfied(map->getElement(o2)) &&
        _polyCrit.isSatisfied(map->getElement(sharedEid)))
    {
      LOG_TRACE("Automatically merging pois: " << o1 << ", " << o2 << " into poly: " << sharedEid);
      return false;
    }

    // create POI/Polygon matches and check to see if it is a miss
    boost::shared_ptr<Match> ma(_createMatch(map, o1, o2));

    // return conflict only if it is a miss, a review is ok.
    result = false;
    if (ma.get() == 0 || ma->getType() == MatchType::Miss)
    {
      LOG_TRACE("miss");
      result = true;
    }
  }
  // if you don't dereference the m1/m2 pointers it always returns Match as the typeid. Odd.
  else if (typeid(*m1) == typeid(*m2))
  {
    LOG_TRACE("type ids match");
    result = m1->isConflicting(*m2, map);
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
  bool conflicting = false;

  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m1 = *it;
    LOG_VART(m1);
    for (MatchSet::const_iterator jt = matches.begin(); jt != matches.end(); ++jt)
    {
      const Match* m2 = *jt;
      LOG_VART(m2);

      if (m1 != m2)
      {
        ConstOsmMapPtr map = _map->shared_from_this();
        if (MergerFactory::getInstance().isConflicting(map, m1, m2))
        {
          LOG_TRACE("conflicting");
          conflicting = true;
        }
      }
    }
  }

  return conflicting;
}

}
