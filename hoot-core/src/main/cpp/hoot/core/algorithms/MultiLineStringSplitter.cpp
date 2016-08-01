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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiLineStringSplitter.h"

// geos
#include <geos/geom/LineString.h>

// hoot
#include <hoot/core/algorithms/linearreference/MultiLineStringLocation.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/FindNodesInWayFactory.h>

namespace hoot
{

MultiLineStringSplitter::MultiLineStringSplitter()
{
}

auto_ptr<FindNodesInWayFactory> MultiLineStringSplitter::_createNodeFactory(
    const WaySublineCollection& string) const
{
  auto_ptr<FindNodesInWayFactory> nfPtr;

  set<ConstWayPtr, WayPtrCompare> ways;
  for (size_t i = 0; i < string.getSublines().size(); i++)
  {
    ways.insert(string.getSublines()[i].getWay());
  }

  nfPtr.reset(new FindNodesInWayFactory());

  // add all the ways to the FindNodesInWayFactory
  for (set<ConstWayPtr, WayPtrCompare>::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    nfPtr->addWay(*it);
  }

  return nfPtr;
}

ElementPtr MultiLineStringSplitter::createSublines(const OsmMapPtr& map,
  const WaySublineCollection& string, const vector<bool> &reverse, GeometryConverter::NodeFactory *nf)
  const
{
  assert(reverse.size() == string.getSublines().size());
  // if there were no matches then the result will be null
  ElementPtr result;
  vector<WayPtr> matches;

  auto_ptr<FindNodesInWayFactory> nfPtr;
  if (nf == 0)
  {
    nfPtr = _createNodeFactory(string);
    nf = nfPtr.get();
  }

  // extract all the sublines into ways.
  for (size_t i = 0; i < string.getSublines().size(); i++)
  {
    WayPtr w = string.getSublines()[i].toWay(map, nf);
    if (reverse[i])
    {
      w->reverseOrder();
    }
    if (ElementConverter(map).calculateLength(w) > 0)
    {
      matches.push_back(w);
      map->addElement(w);
    }
  }

  // if there was one match then just return the way.
  if (matches.size() == 1)
  {
    result = matches[0];
  }
  // if there were multiple matches then create a relation to contain the matches.
  else if (matches.size() > 1)
  {
    RelationPtr r(new Relation(matches[0]->getStatus(), map->createNextRelationId(),
      matches[0]->getCircularError(), Relation::MULTILINESTRING));

    for (size_t i = 0; i < matches.size(); i++)
    {
      r->addElement("", matches[i]);
    }
    result = r;
    map->addElement(r);
  }

  return result;
}


void MultiLineStringSplitter::split(const OsmMapPtr& map, const WaySublineCollection& string,
  const vector<bool> &reverse, ElementPtr& match, ElementPtr &scraps,
  GeometryConverter::NodeFactory *nf) const
{
  auto_ptr<FindNodesInWayFactory> nfPtr;
  if (nf == 0)
  {
    nfPtr = _createNodeFactory(string);
    nf = nfPtr.get();
  }

  // rename the matches to the positive subline string
  const WaySublineCollection& positive = string;
  // create an inversion of the WaySublineCollection
  WaySublineCollection negative = string.invert();

  // create all the sublines that fall within the positive WaySublineCollection and put them into
  // the match element.
  match = createSublines(map, positive, reverse, nf);

  // create all the sublines that fall within the negative WaySublineCollection and put them into
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

