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
#include "PoiPolygonMergerCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/MarkForReviewMerger.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>

// Standard
#include <typeinfo>

#include "PoiPolygonMatch.h"
#include "PoiPolygonMerger.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, PoiPolygonMergerCreator)

PoiPolygonMergerCreator::PoiPolygonMergerCreator()
{
  _map = 0;
}

Match* PoiPolygonMergerCreator::_createMatch(const ConstOsmMapPtr& map, ElementId eid1,
  ElementId eid2) const
{
  return MatchFactory::getInstance().createMatch(map, eid1, eid2);
}

bool PoiPolygonMergerCreator::createMergers(const MatchSet& matches,
  vector<Merger*>& mergers) const
{
  bool result = false;
  assert(matches.size() > 0);

  bool foundAPoi = false;
  bool foundAPolygon = false;
  QStringList matchTypes;
  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m = *it;
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

  // if there is at least one POI and at least one polygon, then we need to merge things in a
  // special way.
  if (foundAPoi && foundAPolygon)
  {
    set< pair<ElementId, ElementId> > eids;

    // go through all the matches
    for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
      set< pair<ElementId, ElementId> > s = (*it)->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }

    if (_isConflictingSet(matches))
    {
      mergers.push_back(new MarkForReviewMerger(eids, "Conflicting information",
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

vector<MergerCreator::Description> PoiPolygonMergerCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "POI to Polygon Merge Creator", true));

  return result;
}

bool PoiPolygonMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  bool foundAPoi = false;
  bool foundAPolygon = false;
  if (m1->getMatchMembers() & MatchMembers::Poi ||
      m2->getMatchMembers() & MatchMembers::Poi)
  {
    foundAPoi = true;
  }
  if (m1->getMatchMembers() & MatchMembers::Polygon ||
      m2->getMatchMembers() & MatchMembers::Polygon)
  {
    foundAPolygon = true;
  }

  bool result;
  if (foundAPoi && foundAPolygon)
  {
    // get out the matched pairs from the matches
    set< pair<ElementId, ElementId> > p1 = m1->getMatchPairs();
    set< pair<ElementId, ElementId> > p2 = m2->getMatchPairs();
    // we're expecting them to have one match each, more could be handled, but are not necessary at
    // this time.
    assert(p1.size() == 1 && p2.size() == 1);
    pair<ElementId, ElementId> eids1 = *p1.begin();
    pair<ElementId, ElementId> eids2 = *p2.begin();

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
      result = false;
      return result;
    }

    if (eids2.first == sharedEid)
    {
      o2 = eids2.second;
    }
    else
    {
      assert(eids2.second == sharedEid);
      o2 = eids2.first;
    }

    // create POI/Polygon matches and check to see if it is a miss
    auto_ptr<Match> ma(_createMatch(map, o1, o2));

    // return conflict only if it is a miss, a review is ok.
    result = false;
    if (ma.get() == 0 ||
        ma->getType() == MatchType::Miss)
    {
      result = true;
    }
  }
  // if you don't dereference the m1/m2 pointers it always returns Match as the typeid. Odd.
  else if (typeid(*m1) == typeid(*m2))
  {
    result = m1->isConflicting(*m2, map);
  }
  else
  {
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
    for (MatchSet::const_iterator jt = matches.begin(); jt != matches.end(); ++jt)
    {
      const Match* m2 = *jt;

      if (m1 != m2)
      {
        ConstOsmMapPtr map = _map->shared_from_this();
        if (MergerFactory::getInstance().isConflicting(map, m1, m2))
        {
          conflicting = true;
        }
      }
    }
  }

  return conflicting;
}

}
