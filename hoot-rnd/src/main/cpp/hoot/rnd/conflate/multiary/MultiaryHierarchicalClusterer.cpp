#include "MultiaryHierarchicalClusterer.h"

// hoot
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QHash>

namespace hoot
{

MultiaryHierarchicalClusterer::MultiaryHierarchicalClusterer(MultiaryPoiMergeCachePtr mergeCache,
  MultiaryScoreCachePtr scoreCache, const MatchThreshold& matchThreshold) :
  MultiaryClusterer(mergeCache, scoreCache),
  _matchThreshold(matchThreshold)
{
}

void MultiaryHierarchicalClusterer::_addChildLinks(MultiaryClusterPtr parent,
  MultiaryClusterPtr child)
{
  foreach (MultiaryClusterPtr other, child->links)
  {
    if (other->valid)
    {
      MatchClassification s = _scoreCache->getScore(parent, other);

      // if the match is a miss there is no reason to record it as a link.
      if (_matchThreshold.getType(s) != MatchType::Miss)
      {
        // only create a new ClusterLink if paren't hasn't already been linked.
        if (other->links.contains(parent) == false)
        {
          parent->links.append(other);
          other->links.append(parent);

          ClusterLinkPtr l(new ClusterLink(parent, other, s));
          l->explainText = _scoreCache->getLastExplainText();
          _linkQueue.push(l);
        }
      }
    }
  }
}

MultiaryClusterer::ClusterList MultiaryHierarchicalClusterer::cluster(OsmMapPtr map,
  std::set< std::pair<ElementId, ElementId> >& pairs)
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

  // merge each cluster and update the replaced vector as appropriate

  // Create reviews based on the constructed clusters. This will include any matches between two
  // clusters, but we won't include reviews between two clusters for now.

  return _clusters;
}

QList<MultiaryClusterer::ClusterLinkPtr> MultiaryHierarchicalClusterer::takeReviews()
{
  // did you call cluster() first?
  assert(_clusters.size() > 0);

  QList<MultiaryClusterer::ClusterLinkPtr> result;

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

void MultiaryHierarchicalClusterer::_initializeClusters(
  OsmMapPtr map,
  std::set< std::pair<ElementId, ElementId> >& pairs)
{
  _clusters.clear();
  while (!_linkQueue.empty())
  {
    _linkQueue.pop();
  }

  QSet<ElementId> allElements;

  for (std::set< std::pair<ElementId, ElementId> >::iterator it = pairs.begin();
    it != pairs.end(); ++it)
  {
    allElements.insert(it->first);
    allElements.insert(it->second);
  }

  QHash<ElementId, MultiaryClusterPtr> eToCluster;

  foreach (ElementId eid, allElements)
  {
    MultiaryClusterPtr c(new MultiaryCluster());
    c->append(map->getElement(eid));
    c->mergedElement = map->getElement(eid);
    _clusters.append(c);
    eToCluster[eid] = c;
  }

  for (std::set< std::pair<ElementId, ElementId> >::iterator it = pairs.begin();
    it != pairs.end(); ++it)
  {
    MatchClassification score = _scoreCache->getScore(eToCluster[it->first],
      eToCluster[it->second]);
    MatchType mt = _matchThreshold.getType(score);
    if (mt == MatchType::Match || mt == MatchType::Review)
    {
      ClusterLinkPtr l(new ClusterLink(eToCluster[it->first], eToCluster[it->second], score));
      l->a->links.append(l->b);
      l->b->links.append(l->a);
      l->explainText = _scoreCache->getLastExplainText();
      _linkQueue.push(l);
    }
  }
}

}
