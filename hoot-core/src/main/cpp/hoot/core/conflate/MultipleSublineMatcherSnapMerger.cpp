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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultipleSublineMatcherSnapMerger.h"

// hoot
#include <hoot/core/algorithms/subline-matching/SublineStringMatcher.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, MultipleSublineMatcherSnapMerger)

MultipleSublineMatcherSnapMerger::MultipleSublineMatcherSnapMerger() :
HighwaySnapMerger()
{
}

MultipleSublineMatcherSnapMerger::MultipleSublineMatcherSnapMerger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher2) :
HighwaySnapMerger(pairs, sublineMatcher),
_sublineMatcher2(sublineMatcher2)
{
  // The subline matchers have already been initialized by the conflate script by this point.
}

WaySublineMatchString MultipleSublineMatcherSnapMerger::_matchSubline(
  OsmMapPtr map, ElementPtr e1, ElementPtr e2)
{
  WaySublineMatchString match;
  if (e1->getElementType() != ElementType::Node && e2->getElementType() != ElementType::Node)
  {
    LOG_TRACE(
      "Matching river sublines for merging: " << e1->getElementId() << ", " <<
       e2->getElementId() << "...");
    try
    {
      match = _sublineMatcher->findMatch(map, e1, e2);
    }
    catch (const RecursiveComplexityException&)
    {
      // If we receive this exception with the more accurate but sometimes slower maximal subline
      // matcher (assuming _sublineMatcher was configured to be Maximal), we'll try again with the
      // Frechet matcher, which is sometimes less accurate but usually much faster (assuming
      // _sublineMatcher2 was configured to be Frechet).
      LOG_TRACE(
        "Encountered max recursive complexity. Re-matching river sublines for merging: " <<
        e1->getElementId() << ", " <<  e2->getElementId() << "...");
      match = _sublineMatcher2->findMatch(map, e1, e2);
    }
  }
  return match;
}

}
