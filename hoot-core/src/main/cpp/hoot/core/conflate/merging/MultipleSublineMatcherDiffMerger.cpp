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
 * @copyright Copyright (C) 22021 Maxar (http://www.maxar.com/)
 */
#include "MultipleSublineMatcherDiffMerger.h"

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

int MultipleSublineMatcherDiffMerger::_numTimesBackupMatcherUsed = 0;

HOOT_FACTORY_REGISTER(Merger, MultipleSublineMatcherDiffMerger)

MultipleSublineMatcherDiffMerger::MultipleSublineMatcherDiffMerger() :
LinearDiffMerger()
{
}

MultipleSublineMatcherDiffMerger::MultipleSublineMatcherDiffMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher2) :
LinearDiffMerger(pairs, sublineMatcher),
_sublineMatcher2(sublineMatcher2)
{
  // The subline matchers have already been initialized by the conflate script by this point.
}

WaySublineMatchString MultipleSublineMatcherDiffMerger::_matchSubline(ElementPtr e1, ElementPtr e2)
{
  WaySublineMatchString match;
  if (e1->getElementType() != ElementType::Node && e2->getElementType() != ElementType::Node)
  {
    LOG_TRACE(
      "Matching elements for merging: " << e1->getElementId() << ", " <<
      e2->getElementId() << " with " << _sublineMatcher->getName() << " and " <<
      _sublineMatcher->getSublineMatcherName() << "...");
    try
    {
      match = _sublineMatcher->findMatch(_map, e1, e2);
    }
    catch (const RecursiveComplexityException& e)
    {
      // See related note in MultipleSublineMatcherSnapMerger.
      LOG_TRACE(
        e.getWhat() << " Re-matching river sublines for merging: " <<
        e1->getElementId() << ", " <<  e2->getElementId() << " with " <<
        _sublineMatcher2->getName() << " and " << _sublineMatcher2->getSublineMatcherName() <<
        "...");
      match = _sublineMatcher2->findMatch(_map, e1, e2);
      _numTimesBackupMatcherUsed++;
    }
  }
  return match;
}

}
