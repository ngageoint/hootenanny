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
#include "MultiaryPoiMerger.h"

// hoot
#include <hoot/core/conflate/matching/MatchFactory.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/rnd/conflate/multiary/MultiaryHierarchicalClusterAlgorithm.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, MultiaryPoiMerger)

MultiaryPoiMerger::MultiaryPoiMerger(std::set<std::pair<ElementId, ElementId>>& pairs) :
MergerBase(pairs)
{
}

void MultiaryPoiMerger::apply(const OsmMapPtr& map,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  // find the appropriate match creator for calculating scores between clusters
  std::vector<std::shared_ptr<MatchCreator>> matchCreators = MatchFactory::getInstance().
    getCreators();
  foreach (std::shared_ptr<MatchCreator> mc, matchCreators)
  {
    if (mc->isMatchCandidate(map->getElement(_pairs.begin()->first), map))
    {
      if (_matchCreator.get())
      {
        LOG_VARW(_matchCreator->getDescription());
        LOG_VARW(mc->getDescription());
        throw HootException("Expected only a single match creator to be a candidate for the given "
          "element. This method may need to be more restrictive, or you may need to reduce your "
          "list of match creators.");
      }
      _matchCreator = mc;
    }
  }

  // find the appropriate merger creator for merging elements in clusters
  foreach (const CreatorDescription& d, MergerFactory::getInstance().getAllAvailableCreators())
  {
    // ugh. Magic string. To work around this we'll need to link against hoot-js, or find another
    // way to add that dep.
    if (d.getClassName() == "hoot::ScriptMergerCreator")
    {
      _mergerCreator.reset(Factory::getInstance().constructObject<MergerCreator>(d.getClassName()));
    }
  }

  MultiaryScoreCachePtr score(new MultiaryScoreCache(map, _matchCreator));
  MultiaryPoiMergeCachePtr merge(new MultiaryPoiMergeCache(map, _matchCreator, _mergerCreator));
  MultiaryHierarchicalClusterAlgorithm clusterer(merge, score,
    _matchCreator->getMatchThreshold()->getMissThreshold());

  MultiaryClusterAlgorithm::ClusterList clusters = clusterer.calculateClusters(map, _pairs);
  QList<MultiaryClusterAlgorithm::ClusterLinkPtr> reviews = clusterer.takeReviews();

  _mergeClusters(map, replaced, clusters);

  _createReviews(map, reviews);
}

void MultiaryPoiMerger::_createReviews(const OsmMapPtr& map,
  QList<MultiaryClusterAlgorithm::ClusterLinkPtr> reviews) const
{
  ReviewMarker reviewMarker;
  foreach (MultiaryClusterAlgorithm::ClusterLinkPtr link, reviews)
  {
    // we aren't using the review score in the review mark b/c the review score is almost always
    // 0. We may be able to fudge a value based on the match/miss thresholds, but I'm not sure
    // the score is every really used so I'll just leave it the default value for now.
    reviewMarker.mark(map,
      map->getElement(link->a->mergedElement->getElementId()),
      map->getElement(link->b->mergedElement->getElementId()),
      link->explainText,
      "POI");
  }
}

void MultiaryPoiMerger::_mergeClusters(const OsmMapPtr& map,
  std::vector<std::pair<ElementId, ElementId>>& replaced,
  MultiaryClusterAlgorithm::ClusterList clusters) const
{
  foreach (MultiaryClusterPtr mc, clusters)
  {
    foreach (ConstElementPtr e, *mc)
    {
      if (e->getElementId() != mc->mergedElement->getElementId())
      {
        RecursiveElementRemover(e->getElementId()).apply(map);
        replaced.emplace_back(
            e->getElementId(),
            mc->mergedElement->getElementId());
      }
    }
    // Copy mergedElement so we know this is the only map that contains the element.
    ElementPtr newE(mc->mergedElement->clone());
    // this will replace the old entry.
    map->addElement(newE);
  }
}

}
