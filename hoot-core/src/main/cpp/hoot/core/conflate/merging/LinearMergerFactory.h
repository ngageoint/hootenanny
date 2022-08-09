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
 * A factory for creating a merger capable of merging a feature with a linear geometry. The merger
 * implementation returned may be controlled via the geometry.linear.merger.default configuration
 * option.
 *
 * The class hierarchy for linear mergers has become somewhat complex over time due to the
 * introduction of the Network Algorithm and Attribute Conflation. This attempts to shield some of
 * that complexity from callers. This class should not be confused with Mergerfactory which
 * generates MergerCreators. MergerCreators dealing with linear features call this class to
 * determine which Merger to use.
 */
class LinearMergerFactory
{
public:

  /**
   * Creates a linear feature merger
   *
   * @param eids element IDs of the features to be merged
   * @param sublineMatcher the subline matcher to be used to match the features
   * @param matchedBy optional name of the matcher used to match the features being merged; used to
   * mark a feature with the type of matcher that matched it
   * @param matchedBySubroutine name of the matching subroutine used (e.g. road median matching)
   * @return a merger
   */
  static MergerPtr getMerger(
    const std::set<std::pair<ElementId, ElementId>>& eids,
    const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
    const QString matchedBy = QString(), const QString matchedBySubroutine = QString());

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

private:

  /**
   * @brief _getMedianMerger creates a custom merger for the road median to divided road conflation
   * workflow.
   * @return a merger
   */
  static MergerPtr _getMedianMerger();
};

}

#endif // LINEAR_MERGER_FACTORY_H
