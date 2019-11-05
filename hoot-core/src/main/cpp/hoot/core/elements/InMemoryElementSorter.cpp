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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "InMemoryElementSorter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Standard
#include <algorithm>

using namespace std;

namespace hoot
{

InMemoryElementSorter::InMemoryElementSorter(ConstOsmMapPtr source) :
  _nodeIndex(0),
  _wayIndex(0),
  _relationIndex(0)
{
  if (source)
  {
    // Its possible an empty map was sent back just for obtaining an empty stream...let's not
    // log that.
    if (source->getElementCount() > 0)
    {
      LOG_INFO(
        "Sorting " << StringUtils::formatLargeNumber(source->getElementCount()) <<
        " elements in-memory...");
    }

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

    std::sort(_nodeIds.begin(), _nodeIds.end());
    std::sort(_wayIds.begin(), _wayIds.end());
    std::sort(_relationIds.begin(), _relationIds.end());
  }
}

std::shared_ptr<OGRSpatialReference> InMemoryElementSorter::getProjection() const
{
  return _source->getProjection();
}

bool InMemoryElementSorter::hasMoreElements()
{
  return _nodeIndex != _nodeIds.size() ||
    _wayIndex != _wayIds.size() ||
    _relationIndex != _relationIds.size();
}

ElementPtr InMemoryElementSorter::readNextElement()
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

bool InMemoryElementSorter::_elementCompare(const ConstElementPtr& e1, const ConstElementPtr& e2)
{
  const ElementType::Type type1 = e1->getElementType().getEnum();
  const ElementType::Type type2 = e2->getElementType().getEnum();
  if (type1 == type2)
  {
    return e1->getId() < e2->getId();
  }
  else
  {
    return type1 < type2;
  }
}

void InMemoryElementSorter::sort(std::vector<ElementPtr>& elements)
{
  std::sort(elements.begin(), elements.end(), _elementCompare);
}

}
