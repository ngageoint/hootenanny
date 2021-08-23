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
#include "NetworkMergerCreator.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/LinearMergerFactory.h>
#include <hoot/core/conflate/merging/MarkForReviewMerger.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/network/NetworkMatch.h>
#include <hoot/core/conflate/network/PartialNetworkMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>


// Standard
#include <typeinfo>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, NetworkMergerCreator)

NetworkMergerCreator::NetworkMergerCreator()
{
  _map = nullptr;
  _minMatchOverlapPercentage = ConfigOptions().getNetworkMergerMinLargeMatchOverlapPercentage();
}

bool NetworkMergerCreator::createMergers(
  const MatchSet& matchesIn, vector<MergerPtr>& mergers) const
{
  LOG_DEBUG("Creating mergers with " << className() << "...");
  LOG_TRACE("Creating mergers for match set: " << matchesIn);

  QString matchesList = "";
  if (hoot::Log::Trace == hoot::Log::getInstance().getLevel())
  {
    for (MatchSet::const_iterator it = matchesIn.begin(); it != matchesIn.end(); ++it)
    {
      const NetworkMatch* match = dynamic_cast<const NetworkMatch*>(it->get());
      if (match)
      {
        matchesList += match->getEdgeMatch()->getUid() + " ";
      }
    }
  }
  LOG_VART(matchesList.size());
  LOG_VART(matchesList);

  MatchSet matches = matchesIn;
  LOG_VART(matches);

  bool result = false;
  assert(matches.size() > 0);
  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(matches.begin()->get());
  if (m)
  {
    const bool matchOverlap = _containsOverlap(matches);
    LOG_VART(matchOverlap);

    if (!matchOverlap)
    {
      // create a merger that can merge multiple partial matches
      QSet<ConstEdgeMatchPtr> edgeMatches;
      int count = 0;
      set<pair<ElementId, ElementId>> pairs;
      foreach (ConstMatchPtr itm, matches)
      {
        const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(itm.get());
        edgeMatches.insert(nm->getEdgeMatch());
        set<pair<ElementId, ElementId>> p = nm->getMatchPairs();
        pairs.insert(p.begin(), p.end());

        count++;
        if (count % 100 == 0)
        {
          PROGRESS_INFO(
            "Added match " << count << " of " << matches.size() << " to partial network merger...");
        }
      }
      mergers.push_back(
        LinearMergerFactory::getMerger(
          pairs, edgeMatches, m->getNetworkDetails(), HighwayMatch::MATCH_NAME));
    }
    else
    {
      // If one match completely contains the rest, use the larger match. This may need to be
      // reverted as we play with more data, but at this point it seems like a reasonable heuristic.
      if (const NetworkMatch* larger = _getLargestContainer(matches))
      {
        LOG_DEBUG("Adding the larger match to the partial network merger...");
        mergers.push_back(
          LinearMergerFactory::getMerger(
            larger->getMatchPairs(),
            QSet<ConstEdgeMatchPtr>() << larger->getEdgeMatch(),
            larger->getNetworkDetails(),
            HighwayMatch::MATCH_NAME));
      }
      else
      {
        const double overlapPercent = _getOverlapPercent(matches);
        if (overlapPercent > _minMatchOverlapPercentage) // Go ahead and merge largest match
        {
          const NetworkMatch* largest = _getLargest(matches);
          LOG_TRACE("Merging largest Match: " << largest->getEdgeMatch()->getUid());
          mergers.push_back(
            LinearMergerFactory::getMerger(
              largest->getMatchPairs(),
              QSet<ConstEdgeMatchPtr>() << largest->getEdgeMatch(),
              largest->getNetworkDetails(),
              HighwayMatch::MATCH_NAME));
        }
        else // Throw a review
        {
          LOG_DEBUG("Marking " << matches.size() << " overlapping matches for review...");
          int count = 0;
          for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
          {
            set<pair<ElementId, ElementId>> s = (*it)->getMatchPairs();
            set<ElementId> eids;
            for (set<pair<ElementId, ElementId>>::const_iterator jt = s.begin(); jt != s.end();
                 ++jt)
            {
              eids.insert(jt->first);
              eids.insert(jt->second);
            }
            LOG_VART(eids);

            const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(it->get());
            mergers.push_back(
              std::make_shared<MarkForReviewMerger>(
                eids,
                "A complex road situation was found with multiple plausible solutions. Please "
                "reference input data/imagery and manually merge or modify as needed.",
                nm->getName(),
                nm->getScore()));

            count++;
            if (count % 100 == 0)
            {
              PROGRESS_INFO(
                "Added match " << count << " of " << matches.size() << " for review...");
            }
          }
        }
      }
    }

    result = true;
  }
  else
  {
    LOG_DEBUG("Match invalid; skipping merge: " << (*matches.begin())->toString());
  }
  LOG_VART(result);

  return result;
}

vector<CreatorDescription> NetworkMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates mergers that merge roads conflated with the Network Algorithm",
    false);
  return result;
}

bool NetworkMergerCreator::_containsOverlap(const MatchSet& matches) const
{
  bool matchOverlap = false;
  assert(matches.size() > 0);
  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(matches.begin()->get());
  if (m)
  {
    for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
    {
      const NetworkMatch* match1 = dynamic_cast<const NetworkMatch*>(it->get());

      MatchSet::const_iterator jt = it;
      for (++jt; jt != matches.end(); ++jt)
      {
        const NetworkMatch* match2 = dynamic_cast<const NetworkMatch*>(jt->get());

        // Sanity check to make sure our matches
        if (!match1 || !match2)
        {
          LOG_VART(*it);
          LOG_VART(*jt);
          throw UnsupportedException(
            "If one match is a network match they should all be network matches.");
        }

        if (match1->getEdgeMatch()->overlaps(match2->getEdgeMatch()))
        {
          matchOverlap = true;
          LOG_TRACE("Overlapping matches:");
          LOG_VART(match1->getEdgeMatch());
          LOG_VART(match2->getEdgeMatch());
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
    const NetworkMatch* match1 = dynamic_cast<const NetworkMatch*>(it->get());

    MatchSet::const_iterator jt = it;
    for (++jt; jt != matches.end(); ++jt)
    {
      const NetworkMatch* match2 = dynamic_cast<const NetworkMatch*>(jt->get());
      LOG_TRACE(match1->getEdgeMatch()->getUid() << ":" << match2->getEdgeMatch()->getUid());
      double percent = _getOverlapPercent(match1, match2);
      LOG_VART(percent);
      count += percent;
      total += 100.0;
    }
  }

  return 100.0 * count / total;
}

// Tricky: lets approach this from the perspective of the smaller match, not the larger
double NetworkMergerCreator::_getOverlapPercent(const NetworkMatch* m1, const NetworkMatch* m2) const
{
  QList<EdgeString::EdgeEntry> m1e1, m1e2, m2e1, m2e2;
  std::shared_ptr<const OsmMap> pMap(_map->shared_from_this());

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

  double count = 0.0;
  double total = 0.0;
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
    pMap = _map->shared_from_this();
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

  return 100.0 * count / total;
}

const NetworkMatch* NetworkMergerCreator::_getLargest(const MatchSet& matches) const
{
  LOG_TRACE("Retrieving largest match...");

  if (matches.size() < 1)
  {
    LOG_TRACE("No largest match found.");
    return nullptr;
  }

  const NetworkMatch* largest = dynamic_cast<const NetworkMatch*>(matches.begin()->get());
  double longestLen = 0.0;
  foreach (ConstMatchPtr m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m.get());

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
  foreach (ConstMatchPtr m, matches)
  {
    const NetworkMatch* nm = dynamic_cast<const NetworkMatch*>(m.get());
    if (nm != largest && largest->getEdgeMatch()->contains(nm->getEdgeMatch()) == false)
    {
      LOG_TRACE("No largest match found.");
      return nullptr;
    }
  }

  LOG_VART(largest);
  return largest;
}

bool NetworkMergerCreator::isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1,
  ConstMatchPtr m2, const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  bool result = false;
  if (dynamic_cast<const NetworkMatch*>(m1.get()) || dynamic_cast<const NetworkMatch*>(m2.get()))
  {
    result = m1->isConflicting(m2, map);
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
