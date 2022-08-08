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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "MultiaryHierarchicalClusterAlgorithm.h"

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>

namespace hoot
{

MultiaryHierarchicalClusterAlgorithm::MultiaryHierarchicalClusterAlgorithm(
  MultiaryPoiMergeCachePtr mergeCache, MultiaryScoreCachePtr scoreCache,
  const MatchThreshold& matchThreshold) :
  MultiaryClusterAlgorithm(mergeCache, scoreCache),
  _matchThreshold(matchThreshold)
{
}

void MultiaryHierarchicalClusterAlgorithm::_addChildLinks(MultiaryClusterPtr parent,
  MultiaryClusterPtr child)
{
  foreach (MultiaryClusterPtr other, child->links)
  {
    if (other->valid)
    {
      MatchClassification s = _scoreCache->getScore(parent, other);

      // If the match is a miss, there is no reason to record it as a link.
      if (_matchThreshold.getType(s) != MatchType::Miss &&
          // Only create a new ClusterLink if paren't hasn't already been linked.
          other->links.contains(parent) == false)
      {
        parent->links.append(other);
        other->links.append(parent);

        ClusterLinkPtr l = std::make_shared<ClusterLink>(parent, other, s);
        l->explainText = _scoreCache->getLastExplainText();
        _linkQueue.push(l);
      }
    }
  }
}

MultiaryClusterAlgorithm::ClusterList MultiaryHierarchicalClusterAlgorithm::calculateClusters(
  OsmMapPtr map, std::set<std::pair<ElementId, ElementId>>& pairs)
{
  // initialize the cluster list with one element per cluster.
  // Also initialize all the links between clusters in a priority queue.
  _initializeClusters(map, pairs);

  // while the best match is above our match threshold criteria
  while (_linkQueue.size() > 0 &&
    _matchThreshold.getType(_linkQueue.top()->score) == MatchType::Match)
  {
    // pop off the best match and merge those two clusters
    ClusterLinkPtr cl = _linkQueue.top();
    _linkQueue.pop();

    if (cl->isValid())
    {
      // Do we need to update the replaced vector?
      MultiaryClusterPtr newCluster = _mergeCache->merge(cl->a, cl->b);

      // update the connected links in all clusters to reflect the latest merger
      cl->a->valid = false;
      _clusters.removeAll(cl->a);
      cl->b->valid = false;
      _clusters.removeAll(cl->b);

      _clusters.append(newCluster);

      // calculate the distance from the new cluster to all neighboring clusters and update the
      // heap accordingly
      _addChildLinks(newCluster, cl->a);
      _addChildLinks(newCluster, cl->b);
    }
  }

  return _clusters;
}

QList<MultiaryClusterAlgorithm::ClusterLinkPtr> MultiaryHierarchicalClusterAlgorithm::takeReviews()
{
  // did you call cluster() first?
  assert(_clusters.size() > 0);

  QList<MultiaryClusterAlgorithm::ClusterLinkPtr> result;

  while (!_linkQueue.empty() &&
    (_matchThreshold.getType(_linkQueue.top()->score) == MatchType::Match ||
      _matchThreshold.getType(_linkQueue.top()->score) == MatchType::Review))
  {
    if (_linkQueue.top()->isValid())
    {
      result.append(_linkQueue.top());
    }
    _linkQueue.pop();
  }

  return result;
}

void MultiaryHierarchicalClusterAlgorithm::_initializeClusters(
  OsmMapPtr map, const std::set<std::pair<ElementId, ElementId>>& pairs)
{
  _clusters.clear();
  while (!_linkQueue.empty())
  {
    _linkQueue.pop();
  }

  QSet<ElementId> allElements;

  for (std::set<std::pair<ElementId, ElementId>>::iterator it = pairs.begin();
    it != pairs.end(); ++it)
  {
    allElements.insert(it->first);
    allElements.insert(it->second);
  }

  QHash<ElementId, MultiaryClusterPtr> eToCluster;

  foreach (ElementId eid, allElements)
  {
    MultiaryClusterPtr c = std::make_shared<MultiaryCluster>();
    c->append(map->getElement(eid));
    c->mergedElement = map->getElement(eid);
    _clusters.append(c);
    eToCluster[eid] = c;
  }

  for (std::set<std::pair<ElementId, ElementId>>::iterator it = pairs.begin();
    it != pairs.end(); ++it)
  {
    MatchClassification score = _scoreCache->getScore(eToCluster[it->first],
      eToCluster[it->second]);
    MatchType mt = _matchThreshold.getType(score);
    if (mt == MatchType::Match || mt == MatchType::Review)
    {
      ClusterLinkPtr l =
        std::make_shared<ClusterLink>(eToCluster[it->first], eToCluster[it->second], score);
      l->a->links.append(l->b);
      l->b->links.append(l->a);
      l->explainText = _scoreCache->getLastExplainText();
      _linkQueue.push(l);
    }
  }
}

}
