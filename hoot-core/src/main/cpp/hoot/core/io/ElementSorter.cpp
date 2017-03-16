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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementSorter.h"

#include <hoot/core/util/Log.h>

// Standard
#include <algorithm>

namespace hoot
{

ElementSorter::ElementSorter(ConstOsmMapPtr source) :
  _nodeIndex(0),
  _wayIndex(0),
  _relationIndex(0)
{
  LOG_DEBUG(
    "Sorting element by element type for map with element count: " << source->getElementCount());

  _source = source;

  for (NodeMap::const_iterator it = _source->getNodes().begin();
    it != _source->getNodes().end(); ++it)
  {
    _nodeIds.push_back(it->first);
  }
  for (WayMap::const_iterator it = _source->getWays().begin();
    it != _source->getWays().end(); ++it)
  {
    _wayIds.push_back(it->first);
  }
  for (RelationMap::const_iterator it = _source->getRelations().begin();
    it != _source->getRelations().end(); ++it)
  {
    _relationIds.push_back(it->first);
  }

  sort(_nodeIds.begin(), _nodeIds.end());
  sort(_wayIds.begin(), _wayIds.end());
  sort(_relationIds.begin(), _relationIds.end());
}

boost::shared_ptr<OGRSpatialReference> ElementSorter::getProjection() const
{
  return _source->getProjection();
}

bool ElementSorter::hasMoreElements()
{
  return _nodeIndex != _nodeIds.size() ||
    _wayIndex != _wayIds.size() ||
    _relationIndex != _relationIds.size();
}

ElementPtr ElementSorter::readNextElement()
{
  ElementPtr result;
  ConstElementPtr cr;

  if (_nodeIndex != _nodeIds.size())
  {
    cr = _source->getNode(_nodeIds[_nodeIndex++]);
  }
  else if (_wayIndex != _wayIds.size())
  {
    cr = _source->getWay(_wayIds[_wayIndex++]);
  }
  else if (_relationIndex != _relationIds.size())
  {
    cr = _source->getRelation(_relationIds[_relationIndex++]);
  }

  if (cr.get())
  {
    result.reset(cr->clone());
  }

  return result;
}

}
