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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __MULTIARY_POI_MERGER_H__
#define __MULTIARY_POI_MERGER_H__

// hoot
#include <hoot/core/conflate/Match.h>
#include <hoot/core/conflate/MergerBase.h>

#include "MultiaryClusterAlgorithm.h"

namespace hoot
{

/**
 * Merger operation for idenifying clusters of elements within the provided pairs and only merging
 * the clustered elements. This is most useful when conflating large numbers of input sources or
 * intradataset conflation where there could be n-elements that represent the same object.
 */
class MultiaryPoiMerger : public MergerBase
{
public:

  /**
   * Construct with a subgraph of pairs. The links in the set of pairs are used to determine links
   * between elements so it is important that the pairs are represenetative of the matches.
   */
  MultiaryPoiMerger(std::set< std::pair<ElementId, ElementId> >& pairs);

  /**
   * See parent.
   */
  virtual void apply(const OsmMapPtr& map,
    std::vector< std::pair<ElementId, ElementId> >& replaced);

  virtual QString toString() const { return "MultiaryPoiMerger"; }

protected:

  virtual PairsSet& getPairs() { return _pairs; }
  virtual const PairsSet& getPairs() const { return _pairs; }

private:

  boost::shared_ptr<MatchCreator> _matchCreator;
  boost::shared_ptr<MergerCreator> _mergerCreator;

  std::set< std::pair<ElementId, ElementId> > _pairs;

  /**
   * @brief _createReviews Create review entries for each cluster review in the review list.
   * @param map Add reviews to this map.
   * @param reviews List of links that should be reviewed.
   */
  void _createReviews(const OsmMapPtr& map,
    QList<MultiaryClusterAlgorithm::ClusterLinkPtr> reviews);

  /**
   * Merge all the elements that make up each of the clusters into a single element.
   * @param map The map to modify
   * @param replaced The elements that are replaced will be recorded in this set. (from, to)
   * @param clusters The list of clusters to merge in the map.
   */
  void _mergeClusters(const OsmMapPtr& map,
    std::vector< std::pair<ElementId, ElementId> >& replaced,
    MultiaryClusterAlgorithm::ClusterList clusters);
};

}

#endif // __MULTIARY_POI_MERGER_H__
