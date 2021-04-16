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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MergerBase.h"

using namespace std;

namespace hoot
{

set<ElementId> MergerBase::getImpactedElementIds() const
{
  set<ElementId> result;

  const PairsSet& pairs = _getPairs();
  LOG_VART(hoot::toString(pairs));
  // Make sure the map contains all our elements and they aren't conflated.
  for (set<pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
       it != pairs.end(); ++it)
  {
    LOG_VART(it->first);
    LOG_VART(it->second);

    result.insert(it->first);
    result.insert(it->second);
  }
  LOG_VART(result);

  return result;
}

bool MergerBase::isValid(const ConstOsmMapPtr& map) const
{
  bool result = true;

  const PairsSet& pairs = _getPairs();
  LOG_VART(hoot::toString(pairs));
  // make sure the map contains all our elements and they aren't conflated.
  for (set<pair<ElementId, ElementId>>::const_iterator it = pairs.begin();
       it != pairs.end() && result; ++it)
  {
    LOG_VART(it->first);
    LOG_VART(it->second);

    result &= map->containsElement(it->first);
    result &= map->containsElement(it->second);

    if (result)
    {
      result &= map->getElement(it->first)->getStatus() != Status::Conflated;
      result &= map->getElement(it->second)->getStatus() != Status::Conflated;
    }
  }
  LOG_VART(result);

  return result;
}

void MergerBase::replace(ElementId oldEid, ElementId newEid)
{
  PairsSet& pairs = _getPairs();
  set<pair<ElementId, ElementId>>::iterator it = pairs.begin();
  while (it != pairs.end())
  {
    ElementId eid1 = it->first;
    ElementId eid2 = it->second;
    LOG_VART(eid1);
    LOG_VART(eid2);

    if (it->first == oldEid)
    {
      LOG_TRACE("Replacing " << oldEid << " with " << newEid << "...");
      pair<ElementId, ElementId> newP = *it;
      newP.first = newEid;
      LOG_VART(newP);
      pairs.insert(newP);
      pairs.erase(it++);
    }
    else if (it->second == oldEid)
    {
      LOG_TRACE("Replacing " << oldEid << " with " << newEid << "...");
      pair<ElementId, ElementId> newP = *it;
      newP.second = newEid;
      LOG_VART(newP);
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
