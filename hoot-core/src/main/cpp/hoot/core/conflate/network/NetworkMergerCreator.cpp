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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NetworkMergerCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>

// Standard
#include <typeinfo>

#include "NetworkMatch.h"
#include "PartialNetworkMerger.h"

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, NetworkMergerCreator)

NetworkMergerCreator::NetworkMergerCreator()
{
  _map = 0;
}

bool NetworkMergerCreator::createMergers(const MatchSet& matchesIn, vector<Merger*>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");
  LOG_TRACE("Creating mergers for match set: " << matchesIn);

  QString matchesList = "";
  if (hoot::Log::Trace == hoot::Log::getInstance().getLevel())
  {
    for (MatchSet::const_iterator it = matchesIn.begin(); it != matchesIn.end(); ++it)
    {
      const NetworkMatch* nmi = dynamic_cast<const NetworkMatch*>(*it);
      if (nmi)
      {
        matchesList += nmi->getEdgeMatch()->getUid() + " ";
      }
    }
  }
  LOG_TRACE(matchesList.size());
  LOG_TRACE(matchesList);

  MatchSet matches = matchesIn;
  _removeDuplicates(matches);
  LOG_VART(matches);

  bool result = false;
  assert(matches.size() > 0);
  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*matches.begin());
  if (m)
  {
    bool matchOverlap = _containsOverlap(matches);
    LOG_VART(matchOverlap);

    if (!matchOverlap)
    {
      // create a merger that can merge multiple partial matches
      LOG_TRACE("Adding the match to the partial network merger...");
      QSet<ConstEdgeMatchPtr> edgeMatches;
      set< pair<ElementId, ElementId> > pairs;
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
      // If one match completely contains the rest, use the larger match.
      // This may need to be reverted as we play with more data, but at this point it seems like a
      // reasonable heuristic.
      if (const NetworkMatch* larger = _getLargestContainer(matches))
      {
        LOG_TRACE("Adding the larger match to the partial network merger...");
        mergers.push_back(
          new PartialNetworkMerger(
            larger->getMatchPairs(),
            QSet<ConstEdgeMatchPtr>() << larger->getEdgeMatch(),
            larger->getNetworkDetails()));
      }
      else
      {
        double overlapPercent = _getOverlapPercent(matches);
        if (overlapPercent > 80.0) // Go ahead and merge largest match
        {
          const NetworkMatch* largest = _getLargest(matches);
          LOG_TRACE("Merging largest Match: " << largest->getEdgeMatch()->getUid());
          mergers.push_back(
            new PartialNetworkMerger(
              largest->getMatchPairs(),
              QSet<ConstEdgeMatchPtr>() << largest->getEdgeMatch(),
              largest->getNetworkDetails()));
        }
        else // Throw a review
        {
          LOG_TRACE("Marking " << matches.size() << " overlapping matches for review...");
          for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
          {
            set< pair<ElementId, ElementId> > s = (*it)->getMatchPairs();
            set<ElementId> eids;
            for (set< pair<ElementId, ElementId> >::const_iterator jt = s.begin(); jt != s.end(); ++jt)
            {
              eids.insert(jt->first);
              eids.insert(jt->second);
            }
            LOG_VART(eids);

            const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*it);
            mergers.push_back(
              new MarkForReviewMerger(
                eids,
                "A complex road situation was found with multiple plausible solutions. Please "
                "reference input data/imagery and manually merge or modify as needed.",
                m->getMatchName(),
                m->getScore()));
          }
        }
      }
    }

    result = true;
  }
  else
  {
    LOG_TRACE("Match invalid; skipping merge: " << (*matches.begin())->toString());
  }
  LOG_VART(result);

  return result;
}

vector<MergerCreator::Description> NetworkMergerCreator::getAllCreators() const
{
  vector<Description> result;
  result.push_back(Description(className(), "Network Merger Creator", true));
  return result;
}

bool NetworkMergerCreator::_containsOverlap(const MatchSet& matches) const
{
  bool matchOverlap = false;
  assert(matches.size() > 0);
  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*matches.begin());
  if (m)
  {
    for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
      const NetworkMatch* nmi = dynamic_cast<const NetworkMatch*>(*it);

      MatchSet::const_iterator jt = it;
      for (++jt; jt != matches.end(); ++jt)
      {
        const NetworkMatch* nmj = dynamic_cast<const NetworkMatch*>(*jt);

        // Sanity check to make sure our matches
        if (!nmi || !nmj)
        {
          LOG_VART(*it);
          LOG_VART(*jt);
          throw UnsupportedException(
                "If one match is a network match they should all be network matches.");
        }

        if (nmi->getEdgeMatch()->overlaps(nmj->getEdgeMatch()))
        {
          matchOverlap = true;
          LOG_TRACE("Overlapping matches:");
          LOG_VART(nmi->getEdgeMatch());
          LOG_VART(nmj->getEdgeMatch());
          break;
        }
      }
    }
  }

  return matchOverlap;
}

double NetworkMergerCreator::_getOverlapPercent(const MatchSet& matches) const
{
  assert(matches.size() > 0);

  double count = 0;
  double total = 0;

  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const NetworkMatch* nmi = dynamic_cast<const NetworkMatch*>(*it);

    MatchSet::const_iterator jt = it;
    for (++jt; jt != matches.end(); ++jt)
    {
      const NetworkMatch* nmj = dynamic_cast<const NetworkMatch*>(*jt);
      LOG_TRACE(nmi->getEdgeMatch()->getUid() << ":" << nmj->getEdgeMatch()->getUid());
      double percent = _getOverlapPercent(nmi, nmj);
      LOG_TRACE(percent);
      count += percent;
      total += 100.0;
    }
  }

  return 100.0*count / total;
}

// Tricky: lets approach this from the perspective of the smaller match, not the larger
double NetworkMergerCreator::_getOverlapPercent(const NetworkMatch* m1, const NetworkMatch* m2) const
{
  QList<EdgeString::EdgeEntry> m1e1, m1e2, m2e1, m2e2;
  boost::shared_ptr<const OsmMap> pMap(_map->shared_from_this());

  // We want m1e1 to be the shorter match set
  if (m1->getEdgeMatch()->getString1()->calculateLength(pMap)
      <= m2->getEdgeMatch()->getString1()->calculateLength(pMap))
  {
    m1e1 = m1->getEdgeMatch()->getString1()->getAllEdges();
    m2e1 = m2->getEdgeMatch()->getString1()->getAllEdges();
  }
  else
  {
    m1e1 = m2->getEdgeMatch()->getString1()->getAllEdges();
    m2e1 = m1->getEdgeMatch()->getString1()->getAllEdges();
  }

  // We want m1e2 to be the shorter match set
  if (m1->getEdgeMatch()->getString2()->calculateLength(pMap)
      <= m2->getEdgeMatch()->getString2()->calculateLength(pMap))
  {
    m1e2 = m1->getEdgeMatch()->getString2()->getAllEdges();
    m2e2 = m2->getEdgeMatch()->getString2()->getAllEdges();
  }
  else
  {
    m1e2 = m2->getEdgeMatch()->getString2()->getAllEdges();
    m2e2 = m1->getEdgeMatch()->getString2()->getAllEdges();
  }

  double count = 0;
  double total = 0;
  foreach (EdgeString::EdgeEntry ee, m1e1)
  {
    Meters edgeLen = ee.getEdge()->calculateLength(pMap);
    if (m2e1.contains(ee))
    {
      count += edgeLen;
    }
    else // try the reverse
    {
      ee.reverse();
      if (m2e1.contains(ee))
        count += edgeLen;
      ee.reverse(); // reverse back to be nice
    }
    total += edgeLen;
  }

  foreach (EdgeString::EdgeEntry ee, m1e2)
  {
    boost::shared_ptr<const OsmMap> pMap(_map->shared_from_this());
    Meters edgeLen = ee.getEdge()->calculateLength(pMap);
    if (m2e2.contains(ee))
    {
      count += edgeLen;
    }
    else // try the reverse
    {
      ee.reverse();
      if (m2e2.contains(ee))
        count += edgeLen;
      ee.reverse(); // reverse back to be nice
    }
    total += edgeLen;
  }

  return 100.0*count / total;
}

const NetworkMatch* NetworkMergerCreator::_getLargest(const MatchSet& matches) const
{
  LOG_TRACE("Retrieving largest match...");

  if (matches.size() < 1)
  {
    LOG_TRACE("No largest match found.");
    return 0;
  }

  const NetworkMatch* largest = dynamic_cast<const NetworkMatch*>(*matches.begin());
  double longestLen = 0.0;
  foreach (const Match* m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m);

    double testLen = nm->getEdgeMatch()->getString1()->calculateLength(_map->shared_from_this());
    if (testLen > longestLen)
    {
      longestLen = testLen;
      largest = nm;
    }
  }

  return largest;
}

const NetworkMatch* NetworkMergerCreator::_getLargestContainer(const MatchSet& matches) const
{
  LOG_TRACE("Retrieving largest container...");

  const NetworkMatch* largest = _getLargest(matches);

  // Figure out if the largest contains the rest
  foreach (const Match* m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m);
    if (nm != largest && largest->getEdgeMatch()->contains(nm->getEdgeMatch()) == false)
    {
      LOG_TRACE("No largest match found.");
      return 0;
    }
  }

  LOG_VART(largest);
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

// I dislike the nested for loop here - but whatcha gonna do? Maybe not create
// duplicate matches in the first place
void NetworkMergerCreator::_removeDuplicates(MatchSet& matches) const
{
  LOG_TRACE("Removing duplicate matches...");

  for (MatchSet::iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const NetworkMatch* nmi = dynamic_cast<const NetworkMatch*>(*it);
    MatchSet::iterator jt = it;

    for (++jt; jt != matches.end(); ++jt)
    {
      const NetworkMatch* nmj = dynamic_cast<const NetworkMatch*>(*jt);

      if (nmi && nmj)
      {
        if (nmi->isVerySimilarTo(nmj))
          LOG_TRACE(nmi->getEdgeMatch()->getUid() << " is very similar to " << nmj->getEdgeMatch()->getUid());

        if (nmi->contains(nmj))
          LOG_TRACE(nmi->getEdgeMatch()->getUid() << " contains " << nmj->getEdgeMatch()->getUid());

        if (nmi->isVerySimilarTo(nmj))
        {
          MatchSet::iterator tmp = jt;
          ++tmp;
          matches.erase(jt);
          jt = tmp;
        } // end if very similar
      } // end if valid pointers
    } // inner for
  } // outer for
}

}
