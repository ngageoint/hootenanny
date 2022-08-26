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
#include "MarkForReviewMerger.h"

// hoot
#include <hoot/core/schema/TagComparator.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(Merger, MarkForReviewMerger)

MarkForReviewMerger::MarkForReviewMerger(const set<pair<ElementId, ElementId>>& pairs, const QString& note,
                                         const QString& reviewType, double score)
  : _pairs(pairs),
    _note(note),
    _reviewType(reviewType),
    _score(score)
{
  LOG_VART(reviewType);
}

MarkForReviewMerger::MarkForReviewMerger(const set<ElementId>& eids, const QString& note, const QString& reviewType, double score)
  : _eids(eids),
    _note(note),
    _reviewType(reviewType),
    _score(score)
{
  LOG_VART(reviewType);
}

void MarkForReviewMerger::apply(const OsmMapPtr& map, vector<pair<ElementId, ElementId>>& /*replaced*/)
{
  assert(!(_eids.size() >=1 && _pairs.size() >= 1));

  if (_eids.size() >= 1)
    _reviewMarker.mark(map, _eids, _note, _reviewType, _score);
  else
  {
    for (const auto& id_pair : _pairs)
    {
      ElementPtr e1 = map->getElement(id_pair.first);
      ElementPtr e2 = map->getElement(id_pair.second);
      if (e1.get() && e2.get())
        _reviewMarker.mark(map, e1, e2, _note, _reviewType, _score);
    }
  }
}

set<ElementId> MarkForReviewMerger::getImpactedElementIds() const
{
  set<ElementId> result;

  if (_eids.size() >= 1)
    result = _eids;
  else
  {
    // Make sure the map contains all our elements and they aren't conflated.
    for (const auto& id_pair : _pairs)
    {
      result.insert(id_pair.first);
      result.insert(id_pair.second);
    }
  }

  return result;
}

bool MarkForReviewMerger::isValid(const ConstOsmMapPtr& /*map*/) const
{
  return true;
}

void MarkForReviewMerger::replace(ElementId oldEid, ElementId newEid)
{
  auto it = _pairs.begin();
  while (it != _pairs.end())
  {
    if (it->first == oldEid)
    {
      pair<ElementId, ElementId> newP = *it;
      newP.first = newEid;
      _pairs.insert(newP);
      _pairs.erase(it++);
    }
    else if (it->second == oldEid)
    {
      pair<ElementId, ElementId> newP = *it;
      newP.second = newEid;
      _pairs.insert(newP);
      _pairs.erase(it++);
    }
    else
      ++it;
  }

  set<ElementId>::iterator it2 = _eids.find(oldEid);
  if (it2 != _eids.end())
  {
    _eids.erase(it2);
    _eids.insert(newEid);
  }
}

QString MarkForReviewMerger::toString() const
{
  return QString("MarkForReviewMerger, pairs: ") + hoot::toString(_pairs);
}

}
