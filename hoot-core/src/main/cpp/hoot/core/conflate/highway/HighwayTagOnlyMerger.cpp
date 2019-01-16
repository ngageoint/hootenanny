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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayTagOnlyMerger.h"

// hoot
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/ReplaceElementOp.h>
#include <hoot/core/schema/TagMergerFactory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HighwayTagOnlyMerger::HighwayTagOnlyMerger(const std::set<std::pair<ElementId, ElementId>>& pairs)
{
  _pairs = pairs;
}

void HighwayTagOnlyMerger::apply(const OsmMapPtr& map,
                                 std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
       it != _pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    if (map->containsElement(eid1) && map->containsElement(eid2))
    {
      _mergePair(map, eid1, eid2, replaced);
    }
  }
}

bool HighwayTagOnlyMerger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_VART(eid1);
  LOG_VART(eid2);

  if (HighwayMergerAbstract::_mergePair(map, eid1, eid2, replaced))
  {
    return true;
  }

  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);

  if (e1 && e2)
  {
    LOG_VART(e1->getStatus());
    LOG_VART(e2->getStatus());

    ElementPtr elementToKeep;
    ElementPtr elementToRemove;
    assert(!(e1->getStatus() == Status::Conflated && e2->getStatus() == Status::Conflated));
    if (e1->getStatus() == Status::Unknown1 || e1->getStatus() == Status::Conflated)
    {
      elementToKeep = e1;
      elementToRemove = e2;
    }
    else if (e1->getStatus() == Status::Unknown2 || e2->getStatus() == Status::Conflated)
    {
      elementToKeep = e2;
      elementToRemove = e1;
    }

    // There actually could be a relation in here, but the default tag merging doesn't use the
    // element type anyway, so not worrying about it for now.
    elementToKeep->setTags(
      TagMergerFactory::mergeTags(
        elementToKeep->getTags(), elementToRemove->getTags(), ElementType::Way));
    elementToKeep->setStatus(Status::Conflated);
    replaced.push_back(
      std::pair<ElementId, ElementId>(
        elementToRemove->getElementId(), elementToKeep->getElementId()));
    // Is this necessary?
    RecursiveElementRemover(elementToRemove->getElementId()).apply(map);
  }

  return false;
}

}

