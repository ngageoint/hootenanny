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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultiLineStringSplitter.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/FindNodesInWayFactory.h>
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/algorithms/linearreference/WaySubline.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/elements/ElementGeometryUtils.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/schema/MetadataTags.h>

using namespace std;

namespace hoot
{

MultiLineStringSplitter::MultiLineStringSplitter(const bool markAddedMultilineStringRelations)
  : _markAddedMultilineStringRelations(markAddedMultilineStringRelations)
{
}

std::shared_ptr<FindNodesInWayFactory> MultiLineStringSplitter::_createNodeFactory(const WaySublineCollection& string) const
{
  std::shared_ptr<FindNodesInWayFactory> nfPtr;

  set<ConstWayPtr, WayPtrCompare> ways;
  for (const auto& subline : string.getSublines())
    ways.insert(subline.getWay());

  nfPtr = std::make_shared<FindNodesInWayFactory>();
  // Add all the ways to the FindNodesInWayFactory/
  for (auto const& way : ways)
    nfPtr->addWay(way);

  return nfPtr;
}

ElementPtr MultiLineStringSplitter::createSublines(const OsmMapPtr& map, const WaySublineCollection& string,
                                                   const vector<bool>& reverse,
                                                   std::shared_ptr<GeometryToElementConverter::NodeFactory> nf) const
{
  assert(reverse.size() == string.getSublines().size());
  LOG_VART(string.getSublines().size());
  // if there were no matches then the result will be null
  ElementPtr result;
  vector<WayPtr> matches;

  if (!nf)
    nf = _createNodeFactory(string);

  // Extract all the sublines into ways.
  for (size_t i = 0; i < string.getSublines().size(); i++)
  {
    const WaySubline& subline = string.getSublines()[i];
    //LOG_VART(subline);
    WayPtr w = subline.toWay(map, nf);
    w->setPid(subline.getElementId().getId());
    LOG_VART(w->getElementId());
    if (reverse[i])
      w->reverseOrder();
    if (ElementGeometryUtils::calculateLength(w, map) > 0)
    {
      matches.push_back(w);
      LOG_TRACE("Adding " << w->getElementId() << " to map...");
      map->addElement(w);
    }
  }
  LOG_VART(matches.size());

  // If there was one match then just return the way.
  if (matches.size() == 1)
    result = matches[0];
  else if (matches.size() > 1)  // If there were multiple matches then create a relation to contain the matches.
  {
    RelationPtr r =
      std::make_shared<Relation>(
        matches[0]->getStatus(), map->createNextRelationId(), matches[0]->getCircularError(),
        MetadataTags::RelationMultilineString());
    for (const auto& match : matches)
    {
      LOG_TRACE(
        "multilinestring: adding multiple matches to relation with ID: " << r->getId() <<
        " member: " << match->getElementId());
      LOG_VART(match);
      r->addElement("", match);
      if (_markAddedMultilineStringRelations)
        r->getTags().set(MetadataTags::HootMultilineString(), "yes");
    }
    result = r;
    LOG_TRACE("Multilinestring split relation: " << result);
    map->addElement(r);
  }
  return result;
}

void MultiLineStringSplitter::split(const OsmMapPtr& map, const WaySublineCollection& string,
                                    const vector<bool>& reverse, ElementPtr& match, ElementPtr& scraps,
                                    std::shared_ptr<GeometryToElementConverter::NodeFactory> nf) const
{
  LOG_TRACE("Splitting " << string.toString().left(100) << "...");

  if (!nf)
    nf = _createNodeFactory(string);
  // Rename the matches to the positive subline string.
  const WaySublineCollection& positive = string;
  // Create an inversion of the WaySublineCollection.
  WaySublineCollection negative = string.invert();
  // Create all the sublines that fall within the positive WaySublineCollection and put them into
  // the match element.
  match = createSublines(map, positive, reverse, nf);
  // Create all the sublines that fall within the negative WaySublineCollection and put them into
  // the scraps element.
  vector<bool> reverseNegative(negative.getSublines().size(), false);
  scraps = createSublines(map, negative, reverseNegative, nf);
}

void MultiLineStringSplitter::split(const OsmMapPtr& map, const MultiLineStringLocation& splitPoint,
                                    ElementPtr& match) const
{
  vector<bool> reverse(splitPoint.getWaySublineString().getSublines().size(), false);
  ElementPtr scraps;
  split(map, splitPoint.getWaySublineString(), reverse, match, scraps);
  //delete what's in scraps from the map
  RecursiveElementRemover(scraps->getElementId()).apply(map);
}

}
