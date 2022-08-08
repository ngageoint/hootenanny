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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "LinearMergerFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/highway/MedianToDividedRoadClassifier.h>
#include <hoot/core/conflate/network/PartialNetworkMerger.h>
#include <hoot/core/conflate/merging/LinearTagOnlyMerger.h>
#include <hoot/core/conflate/merging/LinearAverageMerger.h>
#include <hoot/core/schema/SelectiveOverwriteTagMerger.h>

namespace hoot
{

MergerPtr LinearMergerFactory::getMerger(
  const std::set<std::pair<ElementId, ElementId>>& eids,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher, const QString matchedBy,
  const QString matchedBySubroutine)
{
  MergerPtr merger;
  LOG_VART(matchedBy);
  LOG_VART(matchedBySubroutine);
  if (matchedBy == HighwayMatch::MATCH_NAME &&
      matchedBySubroutine == MedianToDividedRoadClassifier::MEDIAN_MATCHED_SUBROUTINE_NAME)
  {
    merger = _getMedianMerger();
  }
  else
  {
    merger =
      Factory::getInstance().constructObject<Merger>(
        ConfigOptions().getGeometryLinearMergerDefault());
  }
  std::shared_ptr<LinearMergerAbstract> linearMerger =
    std::dynamic_pointer_cast<LinearMergerAbstract>(merger);
  if (!linearMerger)
  {
    throw IllegalArgumentException(
      "Invalid linear merger class name: " + ConfigOptions().getGeometryLinearMergerDefault());
  }
  LOG_VART(merger->getClassName());
  linearMerger->setMatchedBy(matchedBy);
  linearMerger->setPairs(eids);
  linearMerger->setSublineMatcher(sublineMatcher);
  return merger;
}

MergerPtr LinearMergerFactory::getMerger(
  const std::set<std::pair<ElementId, ElementId>>& eids,
  const QSet<ConstEdgeMatchPtr>& edgeMatches, const ConstNetworkDetailsPtr& details,
  const QString matchedBy)
{
  MergerPtr merger;

  // Use of LinearTagOnlyMerger for geometries signifies that we're doing Attribute Conflation.
  const bool isAttributeConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearTagOnlyMerger::className();
  // Use of LinearAverageMerger for geometries signifies that we're doing Average Conflation.
  const bool isAverageConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearAverageMerger::className();
  if (isAttributeConflate)
  {
    // This is messy, but we'll need some refactoring to get rid of it.
    merger =
      std::make_shared<LinearTagOnlyMerger>(
        eids, std::make_shared<PartialNetworkMerger>(eids, edgeMatches, details));
    std::shared_ptr<LinearMergerAbstract> linearMerger =
      std::dynamic_pointer_cast<LinearMergerAbstract>(merger);
    linearMerger->setMatchedBy(matchedBy);
  }
  else if (isAverageConflate)
  {
    // Average Conflation doesn't use a subline matcher.
    return getMerger(eids, std::shared_ptr<SublineStringMatcher>(), matchedBy);
  }
  else
  {
    // Reference or Differential Network Conflation; The Network algorithm doesn't support
    // Differential Conflation partial match merging yet, so we don't use it.
    merger = std::make_shared<PartialNetworkMerger>(eids, edgeMatches, details);
  }

  return merger;
}

MergerPtr LinearMergerFactory::_getMedianMerger()
{
  std::shared_ptr<LinearTagOnlyMerger> tagOnlyMerger = std::make_shared<LinearTagOnlyMerger>();
  std::shared_ptr<SelectiveOverwriteTag1Merger> tagMerger =
    std::dynamic_pointer_cast<SelectiveOverwriteTag1Merger>(
      std::make_shared<SelectiveOverwriteTag1Merger>());
  tagMerger->setTagKeys(ConfigOptions().getHighwayMedianToDualHighwayTransferKeys());
  tagOnlyMerger->setTagMerger(tagMerger);
  return tagOnlyMerger;
}

}

