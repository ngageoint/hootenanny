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
 * A factory for creating a merger capable of merging a feature with a linear geometry
 *
 * The class hierarchy for linear mergers has become unnecessarily complex over time due to the
 * introduction of the Network Algorithm and Attribute Conflation. This attempts to shield some of
 * that complexity from callers. This class should not be confused with Mergerfactory which
 * generates MergerCreators. MergerCreators dealing with linear features call this class to
 * determine which Merger to use.
 *
 * We don't need to worry about handling diff conflate specifically in this factory, as diff
 * conflate will handle creating its own LinearDiffMerger for merging partial matches internally and
 * won't use this class at all to determine the merger for merging complete matches, as it doesn't
 * merge them at all.
 */
class LinearMergerFactory
{
public:

  /**
   * Creates a linear feature merger matched by the Unifying Algorithm.
   *
   * @param eids element IDs of the features to be merged
   * @param sublineMatcher the subline matcher to be used to match the features
   * @param matchedBy optional text to mark a feature with the type of matcher that matched it
   * @return a merger
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const QString matchedBy = QString());

  /**
   * Creates a linear feature merger matched by the Network Algorithm.
   *
   * @param eids element IDs of the features to be merged
   * @param edgeMatches network edge matches involved in the matching of the features
   * @param details details of the network the features belong to
   * @param matchedBy optional text to mark a feature with the type of matcher that matched it
   * @return a merger
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const QSet<ConstEdgeMatchPtr>& edgeMatches, const ConstNetworkDetailsPtr& details,
    const QString matchedBy = QString());

  /**
   * Creates a linear feature merger matched by the Unifying Algorithm and using two subline
   * matchers
   *
   * @param eids element IDs of the features to be merged
   * @param sublineMatcher1 the primary subline matcher to be used to match the features
   * @param sublineMatcher2 a secondary subline matcher to be used to match the features if the
   * first results in a poor runtime
   * @param matchedBy optional text to mark a feature with the type of matcher that matched it
   * @return a merger
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher1,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher2,
    const QString matchedBy = QString());
};

}

#endif // LINEAR_MERGER_FACTORY_H
