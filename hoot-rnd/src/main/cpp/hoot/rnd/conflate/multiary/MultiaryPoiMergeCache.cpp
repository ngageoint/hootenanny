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
 * @copyright Copyright (C) 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultiaryPoiMergeCache.h"

// hoot
#include <hoot/core/conflate/matching/MatchCreator.h>
#include <hoot/core/conflate/merging/Merger.h>
#include <hoot/core/conflate/merging/MergerCreator.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>

namespace hoot
{

/**
 * Order first by input number, then by element ID. This should always give a consistent ordering
 * even if the elements come from the same input.
 */
struct ElementOrder
{
public:
  bool operator()(const ConstElementPtr& e1, const ConstElementPtr& e2) const
  { 
    if (e1->getStatus().getInput() < e2->getStatus().getInput())
    {
      return true;
    }
    else
    {
      return e1->getElementId() < e2->getElementId();
    }
  }
};

MultiaryPoiMergeCache::MultiaryPoiMergeCache(const ConstOsmMapPtr& map,
    const std::shared_ptr<MatchCreator>& matchCreator,
    const std::shared_ptr<MergerCreator>& mergerCreator) :
  _map(map),
  _matchCreator(matchCreator),
  _mergerCreator(mergerCreator)
{
}

MultiaryClusterPtr MultiaryPoiMergeCache::merge(
  const MultiaryClusterPtr& c1, const MultiaryClusterPtr& c2) const
{
  MultiaryClusterPtr result = std::make_shared<MultiaryCluster>();

  // Pt all the elements from the two input clusters into our output cluster. These are the
  // original, unmerged elements.
  foreach (const ConstElementPtr& e, *c1)
  {
    result->append(e);
  }
  foreach (const ConstElementPtr& e, *c2)
  {
    result->append(e);
  }

  // Sort the elements so the first inputs come first.
  qSort(result->begin(), result->end(), ElementOrder());

  // Create a match set that we can populate with the provided match creator.
  MatchSet ms;

  // go through the 2nd through the last element
  for (int i = 1; i < result->size(); ++i)
  {
    // Create a match between the ith element and the first element.
    MatchPtr m = _matchCreator->createMatch(_map, result->at(0)->getElementId(),
        result->at(i)->getElementId());

    // If the match isn't valid then this isn't a valid cluster the merge.
    if (!m)
    {
      throw InternalErrorException("Expected all elements in two clusters to have minimal matches "
        "when merging clusters.");
    }

    ms.insert(m);
  }

  // create a merge vector
  std::vector<MergerPtr> mergers;

  // Create a merger with the provided merge creator.
  _mergerCreator->createMergers(ms, mergers);

  // This should only create a single merge operation. This way we get a single output merged
  // element. If we get more than one merge operation then this isn't a valid merge.
  if (mergers.size() != 1)
  {
    throw InternalErrorException("Expected to create exactly one merger.");
  }

  // create an environment in which we can call the merge operation without modify the input map.
  std::set<ElementId> eids;
  foreach (const ConstElementPtr& e, *result)
  {
    eids.insert(e->getElementId());
  }

  OsmMapPtr tmp = std::make_shared<OsmMap>(_map->getProjection());
  CopyMapSubsetOp(_map, eids).apply(tmp);

  std::vector<std::pair<ElementId, ElementId>> replaced;

  // apply the merge operation which should result in a single output node.
  // if you want to merge buildings this is the spot in the code that you'll need to modify.
  mergers[0]->apply(tmp, replaced);

  assert(tmp->getNodeCount() == 1);

  // Record the newly merged element as a clone. By cloning we don't have to worry about assigning
  // this element to two maps and creating registerListener problems.
  result->mergedElement = tmp->getNodes().begin()->second->clone();

  return result;
}


}
