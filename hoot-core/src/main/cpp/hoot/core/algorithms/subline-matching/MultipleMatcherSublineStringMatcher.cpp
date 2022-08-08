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
#include "MultipleMatcherSublineStringMatcher.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/subline-matching/MaximalSubline.h>
#include <hoot/core/algorithms/subline-matching/MaximalSublineMatcher.h>
#include <hoot/core/algorithms/subline-matching/SublineMatcher.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatch.h>
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/ops/CopyMapSubsetOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/WaysVisitor.h>
#include <hoot/core/criterion/MultiLineStringCriterion.h>

// Standard
#include <algorithm>

using namespace std;

namespace hoot
{

int MultipleMatcherSublineStringMatcher::_numTimesBackupMatcherUsed = 0;

HOOT_FACTORY_REGISTER(SublineStringMatcher, MultipleMatcherSublineStringMatcher)

MultipleMatcherSublineStringMatcher::MultipleMatcherSublineStringMatcher() :
SublineStringMatcher()
{
}

MultipleMatcherSublineStringMatcher::MultipleMatcherSublineStringMatcher(
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher1,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher2) :
SublineStringMatcher(),
_sublineMatcher1(sublineMatcher1),
_sublineMatcher2(sublineMatcher2)
{
  // The subline matchers have already been initialized by the conflate script by this point.
  LOG_VART(_sublineMatcher1.get());
  LOG_VART(_sublineMatcher2.get());
}

void MultipleMatcherSublineStringMatcher::setConfiguration(const Settings& conf)
{
  _sublineMatcher1->setConfiguration(conf);
  _sublineMatcher2->setConfiguration(conf);
}

void MultipleMatcherSublineStringMatcher::setMaxRelevantAngle(Radians r)
{
  _sublineMatcher1->setMaxRelevantAngle(r);
  _sublineMatcher2->setMaxRelevantAngle(r);
}

void MultipleMatcherSublineStringMatcher::setMinSplitSize(Meters minSplitSize)
{
  _sublineMatcher1->setMinSplitSize(minSplitSize);
  _sublineMatcher2->setMinSplitSize(minSplitSize);
}

void MultipleMatcherSublineStringMatcher::setHeadingDelta(Meters headingDelta)
{
  _sublineMatcher1->setHeadingDelta(headingDelta);
  _sublineMatcher2->setHeadingDelta(headingDelta);
}

WaySublineMatchString MultipleMatcherSublineStringMatcher::findMatch(
  const ConstOsmMapPtr& map, const ConstElementPtr& e1, const ConstElementPtr& e2,
  Meters maxRelevantDistance) const
{
  WaySublineMatchString match;
  if (e1->getElementType() != ElementType::Node && e2->getElementType() != ElementType::Node)
  {
    LOG_TRACE(
      "Matching elements for merging: " << e1->getElementId() << ", " <<
      e2->getElementId() << " with " << _sublineMatcher1->getSublineMatcherName() << "...");
    try
    {
      match = _sublineMatcher1->findMatch(map, e1, e2, maxRelevantDistance);
    }
    catch (const RecursiveComplexityException& e)
    {
      // If we receive this exception with the more accurate but sometimes slower maximal subline
      // matcher (assuming _sublineMatcher was configured to be Maximal), we'll try again with the
      // Frechet matcher, which is sometimes less accurate but usually much faster (assuming
      // _sublineMatcher2 was configured to be Frechet).
      LOG_TRACE(
        e.getWhat() << " Re-matching sublines for merging: " <<
        e1->getElementId() << ", " <<  e2->getElementId() << " with " <<
        _sublineMatcher2->getSublineMatcherName() << "...");
      match = _sublineMatcher2->findMatch(map, e1, e2, maxRelevantDistance);
      _numTimesBackupMatcherUsed++;
    }
  }
  return match;
}

}

