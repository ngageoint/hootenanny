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
#include "NetworkMergerCreator.h"

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

#include "NetworkMatch.h"
#include "PartialNetworkMerger.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, NetworkMergerCreator)

NetworkMergerCreator::NetworkMergerCreator()
{
  _map = 0;
  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getHighwaySublineStringMatcher()));

  _minSplitSize = ConfigOptions(conf()).getWayMergerMinSplitSize();
}

bool NetworkMergerCreator::createMergers(const MatchSet& matches,
  vector<Merger*>& mergers) const
{
  bool result = false;
  assert(matches.size() > 0);

  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*matches.begin());

  LOG_VAR(matches);

  if (m)
  {
    set< pair<ElementId, ElementId> > eids;
    bool matchOverlap = false;

    for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
      const NetworkMatch* nmi = dynamic_cast<const NetworkMatch*>(*it);

      MatchSet::const_iterator jt = it;
      for (++jt; jt != matches.end(); ++jt)
      {
        const NetworkMatch* nmj = dynamic_cast<const NetworkMatch*>(*jt);

        if (!nmi || !nmj)
        {
          LOG_VARE(*it);
          LOG_VARE(*jt);
          throw UnsupportedException("If one match is a network match they should all be network "
            "matches.");
        }

        if (nmi->getEdgeMatch()->overlaps(nmj->getEdgeMatch()))
        {
          matchOverlap = true;
        }
      }
    }

    LOG_VAR(matchOverlap);

    // if there are only 2 matches and one completely contains the other, use the larger match.
    // This may need to be reverted as we play with more data, but at this point it seems like a
    // reasonable heuristic.
    if (const NetworkMatch* larger = _getLargestContainer(matches))
    {
      mergers.push_back(new PartialNetworkMerger(larger->getMatchPairs(),
        QSet<ConstEdgeMatchPtr>() << larger->getEdgeMatch(),
        larger->getNetworkDetails()));
    }
    else if (!matchOverlap)
    {
      QSet<ConstEdgeMatchPtr> edgeMatches;
      set< pair<ElementId, ElementId> > pairs;
      // create a merger that can merge multiple partial matches. If any of the partial matches
      // overlap then mark them all for review.
      foreach (const Match* itm, matches)
      {
        const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(itm);
        edgeMatches.insert(nm->getEdgeMatch());
        set< pair<ElementId, ElementId> > p = nm->getMatchPairs();
        pairs.insert(p.begin(), p.end());
      }

      mergers.push_back(new PartialNetworkMerger(pairs, edgeMatches, m->getNetworkDetails()));
    }
    else
    {
      double sum = 0.0;
      QStringList scores;
      LOG_VAR(matches.size());
      // go through all the matches
      for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
      {
        set< pair<ElementId, ElementId> > s = (*it)->getMatchPairs();
        set<ElementId> eids;
        for (set< pair<ElementId, ElementId> >::const_iterator jt = s.begin(); jt != s.end(); ++jt)
        {
          eids.insert(jt->first);
          eids.insert(jt->second);
        }

        const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*it);
        scores << QString::number(m->getProbability());
        sum += m->getScore();

        mergers.push_back(new MarkForReviewMerger(eids, "A complex road situation was found with "
          "multiple plausible solutions. Please reference input data/imagery and manually merge "
          "or modify as needed.",
          m->getMatchName(), m->getScore()));
      }
    }

    result = true;
  }

  return result;
}

vector<MergerCreator::Description> NetworkMergerCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Network Merge Creator", true));

  return result;
}

const NetworkMatch* NetworkMergerCreator::_getLargestContainer(const MatchSet& matches) const
{
  if (matches.size() <= 1)
  {
    return 0;
  }

  const NetworkMatch* largest = dynamic_cast<const NetworkMatch*>(*matches.begin());
  int largestCount = -1;
  foreach (const Match* m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m);
    int count = nm->getEdgeMatch()->getString1()->getCount() +
      nm->getEdgeMatch()->getString2()->getCount();
    if (count > largestCount)
    {
      largestCount = count;
      largest = nm;
    }
  }

  LOG_VAR(largest);

  foreach (const Match* m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m);
    if (nm != largest &&
      largest->getEdgeMatch()->contains(nm->getEdgeMatch()) == false)
    {
      return 0;
    }
  }

  LOG_INFO("Found largest");

  return largest;
}

bool NetworkMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  bool result = false;

  if (dynamic_cast<const NetworkMatch*>(m1) || dynamic_cast<const NetworkMatch*>(m2))
  {
    result = m1->isConflicting(*m2, map);
  }

  return result;
}

bool NetworkMergerCreator::_isConflictingSet(const MatchSet& matches) const
{
  // _map must be set using setOsmMap()
  assert(_map != 0);
  bool conflicting = matches.size() > 1;

  return conflicting;
}

}
