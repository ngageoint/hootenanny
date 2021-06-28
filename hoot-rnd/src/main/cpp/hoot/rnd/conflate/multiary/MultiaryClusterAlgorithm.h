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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef MULTIARYCLUSTERALGORITHM_H
#define MULTIARYCLUSTERALGORITHM_H

// hoot
#include <hoot/core/conflate/matching/MatchClassification.h>
#include <hoot/rnd/conflate/multiary/MultiaryCluster.h>
#include <hoot/rnd/conflate/multiary/MultiaryPoiMergeCache.h>
#include <hoot/rnd/conflate/multiary/MultiaryScoreCache.h>

// Standard
#include <queue>

namespace hoot
{

/**
 * Given a subgraph of n-inputs, cluster those inputs into consistent clusters that ideally have
 * a high degree of similarity inside the cluster and a low similarity with elements outside the
 * cluster.
 *
 * The exact clustering technique is defined by the inheriting class.
 */
class MultiaryClusterAlgorithm
{
public:

  /**
   * Represents a link between two clusters.
   */
  class ClusterLink
  {
  public:
    /**
     * The two clusters that are linked. Order is not important.
     */
    MultiaryClusterPtr a, b;
    /**
     * The match score between the two clusters.
     */
    MatchClassification score;
    /**
     * Text that describes the link between the two clusters.
     */
    QString explainText;

    ClusterLink(MultiaryClusterPtr a, MultiaryClusterPtr b, const MatchClassification& score)
    {
      this->a = a;
      this->b = b;
      this->score = score;
    }

    /**
     * Returns true if both the clusters that are part of this link are valid.
     */
    bool isValid() const { return a->valid && b->valid; }

    QString toString() const
    {
      return QString("{ 'score': %1, %2'a': %3, 'b': %4%5}").arg(score.toString()).
        arg(isValid() ? "" : "'valid': false, ").
        arg(a->toString(), b->toString()).
        arg(explainText.isEmpty() ? "" : ", 'explainText': '" + explainText + "'");
    }
  };

  using ClusterLinkPtr = std::shared_ptr<ClusterLink>;

  /**
   * A list of clusters that represent a subgraph.
   */
  using ClusterList = QList<MultiaryClusterPtr>;

  /**
   * Comparison operation to determine if one link has a lower score than another. This is used
   * when sorting and should always return a consistent ordering, even if the scores are identical.
   */
  struct ClusterLinkPtrLess
  {
    bool operator()(const ClusterLinkPtr& x, const ClusterLinkPtr& y) const
    {
      bool result;
      if (x->score.getMatchP() == y->score.getMatchP())
      {
        if (x->score.getReviewP() < y->score.getReviewP())
        {
          result = true;
        }
        else if (x->score.getReviewP() > y->score.getReviewP())
        {
          result = false;
        }
        // in the case of an equal score, return a consistent less than value based on the element
        // ID
        else
        {
          result = x->a->at(0)->getElementId() < y->a->at(0)->getElementId();
        }
      }
      else
      {
        result = x->score.getMatchP() < y->score.getMatchP();
      }
      return result;
    }
  };

  virtual ~MultiaryClusterAlgorithm() = default;
  /**
   * @param mergeCache The cache to use when merging POIs. This abstracts the actual merge operation
   *  away from the clustering algorithm.
   * @param scoreCache The cache to use when scoring POIs. This abstracts the actual score operation
   *  away from the clustering algorithm.
   */
  MultiaryClusterAlgorithm(MultiaryPoiMergeCachePtr mergeCache, MultiaryScoreCachePtr scoreCache);

  /**
   * Cluster a subgraph of elements into a list of clusters. This will also populate the list of
   * reviews which can be queried with takeReviews().
   *
   * Only direct pairs will be evaluated for cluster links so the list of pairs should accurately
   * reflect the elements that have high scores.
   *
   * @param map The map that contains all the elements.
   * @param pairs The pairs of elements that make up the subgraph.
   * @return A list of clusters that were found in the 'pairs' subgraph.
   */
  virtual ClusterList calculateClusters(OsmMapPtr map,
    std::set<std::pair<ElementId, ElementId>> &pairs) = 0;

  /**
   * Returns reviews for the current cluster. Must be called after cluster and can only be called
   * once.
   */
  virtual QList<ClusterLinkPtr> takeReviews() = 0;

protected:

  /// List of clusters that have been found as the algorithm runs. This should be updated
  /// dynamically during execution.
  ClusterList _clusters;

  /// Priority queue that is used to store cluster links. High scoring links are at the top.
  using LinkPriorityQueue =
    std::priority_queue<ClusterLinkPtr, std::vector<ClusterLinkPtr>, ClusterLinkPtrLess>;
  LinkPriorityQueue _linkQueue;

  MultiaryPoiMergeCachePtr _mergeCache;
  MultiaryScoreCachePtr _scoreCache;
};

}

#endif // MULTIARYCLUSTERALGORITHM_H
