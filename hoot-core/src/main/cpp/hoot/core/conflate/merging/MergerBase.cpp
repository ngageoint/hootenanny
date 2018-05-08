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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MergerBase.h"

using namespace std;

namespace hoot
{

set<ElementId> MergerBase::getImpactedElementIds() const
{
  set<ElementId> result;

  const PairsSet& pairs = getPairs();
  // make sure the map contains all our elements and they aren't conflated.
  for (set< pair<ElementId, ElementId> >::const_iterator it = pairs.begin();
    it != pairs.end(); ++it)
  {
    result.insert(it->first);
    result.insert(it->second);
  }

  return result;
}

bool MergerBase::isValid(const ConstOsmMapPtr& map) const
{
  bool result = true;

  const PairsSet& pairs = getPairs();
  // make sure the map contains all our elements and they aren't conflated.
  for (set< pair<ElementId, ElementId> >::const_iterator it = pairs.begin();
    it != pairs.end() && result; ++it)
  {
    result &= map->containsElement(it->first);
    result &= map->containsElement(it->second);

    if (result)
    {
      result &= map->getElement(it->first)->getStatus() != Status::Conflated;
      result &= map->getElement(it->second)->getStatus() != Status::Conflated;
    }
  }

  return result;
}

void MergerBase::replace(ElementId oldEid, ElementId newEid)
{
  PairsSet& pairs = getPairs();

  set< pair<ElementId, ElementId> >::iterator it = pairs.begin();
  while (it != pairs.end())
  {
    if (it->first == oldEid)
    {
      pair<ElementId, ElementId> newP = *it;
      newP.first = newEid;
      pairs.insert(newP);
      pairs.erase(it++);
    }
    else if (it->second == oldEid)
    {
      pair<ElementId, ElementId> newP = *it;
      newP.second = newEid;
      pairs.insert(newP);
      pairs.erase(it++);
    }
    else
    {
      ++it;
    }
  }
}

}
