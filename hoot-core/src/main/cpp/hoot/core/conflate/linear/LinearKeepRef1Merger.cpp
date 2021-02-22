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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "LinearKeepRef1Merger.h"

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/algorithms/splitter/WaySublineRemover.h>

namespace hoot
{

// ONLY ENABLE THIS DURING DEBUGGING; We don't want to tie it to debug.maps.write, as it may
// produce a very large number of output files.
const bool LinearKeepRef1Merger::WRITE_DETAILED_DEBUG_MAPS = false;

HOOT_FACTORY_REGISTER(Merger, LinearKeepRef1Merger)

int LinearKeepRef1Merger::logWarnCount = 0;

LinearKeepRef1Merger::LinearKeepRef1Merger() :
LinearMergerAbstract()
{
}

LinearKeepRef1Merger::LinearKeepRef1Merger(
  const std::set<std::pair<ElementId, ElementId>>& pairs,
  const std::shared_ptr<SublineStringMatcher>& sublineMatcher) :
_sublineMatcher(sublineMatcher)
{
  _pairs = pairs;
  LOG_VART(_pairs);
}

void LinearKeepRef1Merger::apply(
  const OsmMapPtr& map, std::vector<std::pair<ElementId, ElementId>>& replaced)
{
  LOG_TRACE("Applying LinearKeepRef1Merger...");
  LOG_VART(hoot::toString(_pairs));
  LOG_VART(hoot::toString(replaced));

  for (std::set<std::pair<ElementId, ElementId>>::const_iterator it = _pairs.begin();
       it != _pairs.end(); ++it)
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;

    for (size_t i = 0; i < replaced.size(); i++)
    {
      LOG_VART(eid1);
      LOG_VART(eid2);
      LOG_VART(replaced[i].first);
      LOG_VART(replaced[i].second);

      LOG_TRACE("e1 before replacement check: " << map->getElement(eid1));
      LOG_TRACE("e2 before replacement check: " << map->getElement(eid2));

      if (eid1 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid1 << " to " << replaced[i].second << "...");
        eid1 = replaced[i].second;
      }
      if (eid2 == replaced[i].first)
      {
        LOG_TRACE("Changing " << eid2 << " to " << replaced[i].second << "...");
        eid2 = replaced[i].second;
      }

      LOG_TRACE("e1 after replacement check: " << map->getElement(eid1));
      LOG_TRACE("e2 after replacement check: " << map->getElement(eid2));
    }

    _mergePair(map, eid1, eid2, replaced);
  }
}

bool LinearKeepRef1Merger::_mergePair(const OsmMapPtr& map, ElementId eid1, ElementId eid2,
  std::vector<std::pair<ElementId, ElementId>>& /*replaced*/)
{
  ElementPtr e1 = map->getElement(eid1);
  ElementPtr e2 = map->getElement(eid2);

  WaySublineMatchString match;
  try
  {
    match = _sublineMatcher->findMatch(map, e1, e2);
  }
  catch (const NeedsReviewException& e)
  {
    LOG_VART(e.getWhat());
    // TODO: fix
    /*_markNeedsReview(result, e1, e2, e.getWhat(), HighwayMatch::getHighwayMatchName());
    return true;*/
  }

  if (!match.isValid())
  {
    // TODO: fix
    /*LOG_TRACE("Complex conflict causes an empty match");
    _markNeedsReview(result, e1, e2, "Complex conflict causes an empty match",
                       HighwayMatch::getHighwayMatchName());
    return true;*/
  }

  // TODO: explain
  // TODO: update partial match removal for non-highway linear types and for Network alg
//    if (!ConfigOptions().getDifferentialRemovePartialMatchesAsWhole() &&
//        match->getName() == HighwayMatch::MATCH_NAME &&
//        match->getMatchPairs().size() == 1) // TODO: size check necessary?
//    {
//      LOG_DEBUG(
//        "Removing matching partial portion from: " << element->getElementId() <<
//        " involved in match of type: " << matchType << "...");

//      std::shared_ptr<const HighwayMatch> highwayMatch =
//        std::dynamic_pointer_cast<const HighwayMatch>(match);
//      LOG_VARD(highwayMatch->getSublineMatch());

//      // Remove only the portion of the way that matched.
//      WayPtr way = std::dynamic_pointer_cast<Way>(element);
//      if (way)
//      {
//        WaySublineMatchString::MatchCollection matches =
//          highwayMatch->getSublineMatch().getMatches();
//        LOG_VARD(matches.size());
//        WaySubline subline1 = matches.at(0).getSubline1();
//        LOG_VARD(subline1.getWay() == way);
//        LOG_VARD(subline1.getWay()->getElementId() == way->getElementId());
//        WaySubline subline2 = matches.at(0).getSubline2();
//        LOG_VARD(subline2.getWay() == way);
//        LOG_VARD(subline2.getWay()->getElementId() == way->getElementId());
//        //if (subline1.getWay() == way)
//        if (subline1.getWay()->getElementId() == way->getElementId())
//        {
//          LOG_DEBUG("Removing subline 1 from " << way->getElementId() << "...");
//          WayLocation start(subline1.getStart());
//          WayLocation end(subline1.getEnd());
//          WaySublineRemover::remove(way, start, end, _pMap);
//        }
//        //if (subline2.getWay() == way)
//        if (subline2.getWay()->getElementId() == way->getElementId())
//        {
//          LOG_DEBUG("Removing subline 2 from " << way->getElementId() << "...");
//          WayLocation start(subline2.getStart());
//          WayLocation end(subline2.getEnd());
//          WaySublineRemover::remove(way, start, end, _pMap);
//        }
//      }
//    }
//    else
//    {
//      LOG_DEBUG(
//        "Removing entire element: " << element->getElementId() << " involved in match of type: " <<
//         matchType << "...");
//      RecursiveElementRemover(element->getElementId()).apply(_pMap);
  //}

//  if (WRITE_DETAILED_DEBUG_MAPS)
//  {
//    OsmMapWriterFactory::writeDebugMap(
//      _pMap, "after-removing-match-" + element->getElementId().toString());
//  }

  return false;
}

}
