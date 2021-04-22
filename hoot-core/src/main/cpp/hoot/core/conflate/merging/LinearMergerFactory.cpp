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
#include "LinearMergerFactory.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/network/PartialNetworkMerger.h>
#include <hoot/core/conflate/merging/LinearTagOnlyMerger.h>
#include <hoot/core/conflate/merging/MultipleSublineMatcherSnapMerger.h>
#include <hoot/core/conflate/merging/LinearAverageMerger.h>
#include <hoot/core/conflate/merging/LinearDiffMerger.h>

namespace hoot
{

MergerPtr LinearMergerFactory::getMerger(
  const std::set<std::pair<ElementId, ElementId>>& eids,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher, const QString matchedBy)
{
  MergerPtr merger(
    Factory::getInstance().constructObject<Merger>(
      ConfigOptions().getGeometryLinearMergerDefault()));
  std::shared_ptr<LinearMergerAbstract> linearMerger =
    std::dynamic_pointer_cast<LinearMergerAbstract>(merger);
  if (!linearMerger)
  {
    throw IllegalArgumentException(
      "Invalid linear merger class name: " + ConfigOptions().getGeometryLinearMergerDefault());
  }
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
  const bool isDiffConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearDiffMerger::className();
  if (isAttributeConflate)
  {
    merger.reset(
      new LinearTagOnlyMerger(
        eids,
        std::shared_ptr<PartialNetworkMerger>(
          new PartialNetworkMerger(eids, edgeMatches, details))));
    std::shared_ptr<LinearMergerAbstract> linearMerger =
      std::dynamic_pointer_cast<LinearMergerAbstract>(merger);
    linearMerger->setMatchedBy(matchedBy);
  }
  else if (isAverageConflate || isDiffConflate)
  {
    return getMerger(eids, std::shared_ptr<SublineStringMatcher>(), matchedBy);
  }
  else
  {
    merger.reset(new PartialNetworkMerger(eids, edgeMatches, details));
  }

  return merger;
}

MergerPtr LinearMergerFactory::getMerger(
  const std::set<std::pair<ElementId, ElementId>>& eids,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher1,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher2, const QString matchedBy)
{
  if (matchedBy != "Waterway" && matchedBy != "Line")
  {
    throw IllegalArgumentException(
      "Invalid matcher for MultipleSublineMatcherSnapMerger: " + matchedBy);
  }
  // Use of LinearTagOnlyMerger for geometries signifies that we're doing Attribute Conflation.
  const bool isAttributeConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearTagOnlyMerger::className();
  // Use of LinearAverageMerger for geometries signifies that we're doing Average Conflation.
  const bool isAverageConflate =
    ConfigOptions().getGeometryLinearMergerDefault() == LinearAverageMerger::className();
  if (isAttributeConflate || isAverageConflate)
  {
    throw IllegalArgumentException(
      "MultipleSublineMatcherSnapMerger may not be used with Attribute or Average Conflation.");
  }

  std::shared_ptr<LinearMergerAbstract> merger(
    new MultipleSublineMatcherSnapMerger(eids, sublineMatcher1, sublineMatcher2));
  merger->setMatchedBy(matchedBy);
  return merger;
}

}

