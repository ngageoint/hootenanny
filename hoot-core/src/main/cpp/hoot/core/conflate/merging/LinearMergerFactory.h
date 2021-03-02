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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LINEAR_MERGER_FACTORY_H
#define LINEAR_MERGER_FACTORY_H

// hoot
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/NetworkDetails.h>

namespace hoot
{

/**
 * A factory for creating a linear feature merger.
 *
 * @todo
 */
class LinearMergerFactory
{
public:

  /**
   * TODO
   *
   * @param eids
   * @param sublineMatcher
   * @param matchedBy
   * @return
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const QString matchedBy = QString());

  /**
   * TODO
   *
   * @param eids
   * @param edgeMatches
   * @param details
   * @param matchedBy
   * @return
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const QSet<ConstEdgeMatchPtr>& edgeMatches, const ConstNetworkDetailsPtr& details,
    const QString matchedBy = QString());

  /**
   * TODO
   *
   * @param eids
   * @param sublineMatcher1
   * @param sublineMatcher2
   * @param matchedBy
   * @return
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher1,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2,
    const QString matchedBy = QString());
};

}

#endif // LINEAR_MERGER_FACTORY_H
