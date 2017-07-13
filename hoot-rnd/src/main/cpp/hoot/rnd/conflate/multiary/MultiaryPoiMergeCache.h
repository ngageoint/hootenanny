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
#ifndef MULTIARYPOIMERGECACHE_H
#define MULTIARYPOIMERGECACHE_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Element.h>

// Qt
#include <QSet>

#include "MultiaryCluster.h"

namespace hoot
{

class MatchCreator;
class MergerCreator;

/**
 * Class for merging two clusters into a single cluster.
 *
 * At this point the class assumes you are merging nodes, but it could be easily expanded to support
 * the other element types.
 *
 * @todo At this point merges are not cached.
 */
class MultiaryPoiMergeCache
{
public:
  /**
   * @param map Map that contains the elements to be merged.
   * @param matchCreator
   * @param mergerCreator
   */
  MultiaryPoiMergeCache(ConstOsmMapPtr map, boost::shared_ptr<MatchCreator> matchCreator,
    boost::shared_ptr<MergerCreator> mergerCreator);

  MultiaryClusterPtr merge(MultiaryClusterPtr c1, MultiaryClusterPtr c2);

private:

  ConstOsmMapPtr _map;
  boost::shared_ptr<MatchCreator> _matchCreator;
  boost::shared_ptr<MergerCreator> _mergerCreator;
};

typedef boost::shared_ptr<MultiaryPoiMergeCache> MultiaryPoiMergeCachePtr;

}

#endif // MULTIARYPOIMERGECACHE_H
