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
#ifndef MULTIARYHIERARCHICALCLUSTERALGORITHM_H
#define MULTIARYHIERARCHICALCLUSTERALGORITHM_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/rnd/conflate/multiary/MultiaryClusterAlgorithm.h>

namespace hoot
{

/**
 * An implementation of hierarchical clustering for clustering elements. This is a greedy algorithm
 * and will not identify very complex clusters.
 *
 * Reviews are only generated for the final clusters. Merge operations may suppress some reviews,
 * but that may be desirable.
 */
class MultiaryHierarchicalClusterAlgorithm : public MultiaryClusterAlgorithm
{
public:

  ~MultiaryHierarchicalClusterAlgorithm() = default;

  /**
   * Constructor.
   * @param mergeCache Cache to use when merging elements.
   * @param scoreCache Cache to use for scoring elements.
   * @param matchThreshold The threshold at which a score is called a match/miss/review.
   */
  MultiaryHierarchicalClusterAlgorithm(MultiaryPoiMergeCachePtr mergeCache,
    MultiaryScoreCachePtr scoreCache, const MatchThreshold& matchThreshold);

  /**
   * See parent class.
   */
  ClusterList calculateClusters(OsmMapPtr map,
    std::set<std::pair<ElementId, ElementId>> &pairs) override;

  /**
   * See parent class.
   */
  QList<ClusterLinkPtr> takeReviews() override;

protected:
  MatchThreshold _matchThreshold;

  /**
   * Use the links in the child cluster to populate links in the parent cluster and also create
   * the appropriate new ClusterLink objects.
   */
  void _addChildLinks(MultiaryClusterPtr parent, MultiaryClusterPtr child);

  /**
   * Initialize the algorithm with a single cluster for each element and a ClusterLink for each
   * of the pairs.
   * @param map Map that contains the pairs.
   * @param pairs Pairs that represent links in the subgraph.
   */
  void _initializeClusters(OsmMapPtr map,
    std::set<std::pair<ElementId, ElementId>>& pairs);

};

}

#endif // MULTIARYHIERARCHICALCLUSTERALGORITHM_H
