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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WayMatchStringSplitter.h"

// hoot
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

QString WayMatchStringSplitter::_overlyAggressiveMergeReviewText =
  "One or more ways in the merge are being removed. This is likely due to an inconsistent match. "
  "Please review the length of the review for overly aggressive merges and manually merge features "
  "using input data/imagery. There may also be one or more zero length ways at intersections.";

WayMatchStringSplitter::WayMatchStringSplitter()
{
}

void WayMatchStringSplitter::applySplits(OsmMapPtr map,
  vector<pair<ElementId, ElementId> > &replaced,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) throw (NeedsReviewException)
{
  _splitWay1(map, replaced, mappings);
  _splitWay2(map, replaced, mappings);
}

QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> WayMatchStringSplitter::_buildWayIndex1(
  OsmMapPtr map,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> result;

  foreach (WayMatchStringMerger::SublineMappingPtr sm, mappings)
  {
    result.insert(map->getWay(sm->getStart1().getWay()->getElementId()), sm);
  }

  return result;
}

QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> WayMatchStringSplitter::_buildWayIndex2(
  OsmMapPtr map,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> result;

  foreach (WayMatchStringMerger::SublineMappingPtr sm, mappings)
  {
    result.insert(map->getWay(sm->getStart2().getWay()->getElementId()), sm);
  }

  return result;
}

void WayMatchStringSplitter::_splitWay1(OsmMapPtr map,
  vector<pair<ElementId, ElementId> > &replaced,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings)
{
  ElementConverter ec(map);

  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> wayMapping =
    _buildWayIndex1(map, mappings);

  // split each way in the subline mapping.
  foreach (WayPtr w1, wayMapping.uniqueKeys())
  {
    QList<WayMatchStringMerger::SublineMappingPtr> sm = wayMapping.values(w1);
    qStableSort(sm.begin(), sm.end(), WayMatchStringMerger::SublineMappingLessThan1());

    // if there is only one entry in sm and it spans the whole way, don't split anything.
    if (sm.size() == 1 && sm[0]->getStart1().isExtreme(WayLocation::SLOPPY_EPSILON) &&
      sm[0]->getEnd1().isExtreme(WayLocation::SLOPPY_EPSILON))
    {
      sm[0]->newWay1 = w1;
      continue;
    }

    vector<WayLocation> wls;

    // push them all on. This will inevitably create some empty ways, but it is predictable.
    for (int i = 0; i < sm.size(); ++i)
    {
      // make sure the split points are sorted.
      wls.push_back(std::min(sm.at(i)->getStart1(), sm.at(i)->getEnd1()));
      wls.push_back(std::max(sm.at(i)->getStart1(), sm.at(i)->getEnd1()));
    }

    // This can happen if there is an unmatched section in a way
    bool discontiguous = false;
    for (size_t i = 0; i < wls.size() - 1; ++i)
    {
      assert(wls[i] <= wls[i + 1]);
      if (i % 2 == 1 && wls[i] != wls[i + 1])
      {
        discontiguous = true;
      }
    }

    LOG_VAR(w1.get());
    LOG_VAR(wls);
    vector<WayPtr> splits = WaySplitter(map, w1).createSplits(wls);

    assert((int)splits.size() == sm.size() * 2 + 1);

    QList<ElementPtr> newWays;

    int c = 0;
    WayPtr w = splits[c++];
    // if this isn't empty
    if (w && ec.calculateLength(w) > 0.0)
    {
      newWays.append(w);
      replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
    }

    for (int i = 0; i < sm.size(); ++i)
    {
      WayPtr w;
      w = splits[c++];
      if (!w)
      {
        throw NeedsReviewException(_overlyAggressiveMergeReviewText);
      }
      sm.at(i)->newWay1 = w;
      replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
      newWays.append(w);

      w = splits[c++];
      // if this isn't empty
      if (w && ec.calculateLength(w) > 0.0)
      {
        // only the last split should be non-empty
        if (i != sm.size() - 1 && discontiguous == false)
        {
          throw InternalErrorException("Only the last split should be empty.");
        }
        newWays.append(w);
        replaced.push_back(pair<ElementId, ElementId>(w1->getElementId(), w->getElementId()));
      }
    }

    map->replace(w1, newWays);
  }
}


void WayMatchStringSplitter::_splitWay2(OsmMapPtr map,
  vector<pair<ElementId, ElementId> > &replaced,
  QList<WayMatchStringMerger::SublineMappingPtr> mappings)
{
  ElementConverter ec(map);

  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> wayMapping =
    _buildWayIndex2(map, mappings);

  // split each way in the subline mapping.
  foreach (WayPtr w2, wayMapping.uniqueKeys())
  {
    QList<WayMatchStringMerger::SublineMappingPtr> sm = wayMapping.values(w2);
    qStableSort(sm.begin(), sm.end(), WayMatchStringMerger::SublineMappingLessThan2());

    // if there is only one entry in sm and it spans the whole way, don't split anything.
    if (sm.size() == 1 && sm[0]->getStart2().isExtreme(WayLocation::SLOPPY_EPSILON) &&
      sm[0]->getEnd2().isExtreme(WayLocation::SLOPPY_EPSILON))
    {
      sm[0]->setNewWay2(w2);
      continue;
    }

    vector<WayLocation> wls;

    // push them all on. This will inevitably create some empty ways, but it is predictable.
    for (int i = 0; i < sm.size(); ++i)
    {
      // make sure the split points are sorted.
      wls.push_back(std::min(sm.at(i)->getStart2(), sm.at(i)->getEnd2()));
      wls.push_back(std::max(sm.at(i)->getStart2(), sm.at(i)->getEnd2()));
    }

    // This can happen if there is an unmatched section in a way
    bool discontiguous = false;
    for (size_t i = 0; i < wls.size() - 1; ++i)
    {
      assert(wls[i] <= wls[i + 1]);
      if (i % 2 == 1 && wls[i] != wls[i + 1])
      {
        discontiguous = true;
      }
    }

    vector<WayPtr> splits = WaySplitter(map, w2).createSplits(wls);

    assert((int)splits.size() == sm.size() * 2 + 1);

    LOG_VAR(w2.get());
    LOG_VAR(wls);
    LOG_VAR(splits);

    QList<ElementPtr> newWays;

    int c = 0;
    WayPtr w = splits[c++];
    // if this isn't empty
    if (w && ec.calculateLength(w) > 0.0)
    {
      newWays.append(w);
      replaced.push_back(pair<ElementId, ElementId>(w2->getElementId(), w->getElementId()));
    }

    for (int i = 0; i < sm.size(); ++i)
    {
      WayPtr w;
      w = splits[c++];
      if (!w)
      {
        throw NeedsReviewException(_overlyAggressiveMergeReviewText);
      }
      sm.at(i)->setNewWay2(w);
      replaced.push_back(pair<ElementId, ElementId>(w2->getElementId(), w->getElementId()));
      newWays.append(w);

      w = splits[c++];
      LOG_VAR(c);
      LOG_VAR(w);
      // if this isn't empty
      if (w && ec.calculateLength(w) > 0.0)
      {
        // only the last split should be non-empty
        if (i != sm.size() - 1 && discontiguous == false)
        {
          throw InternalErrorException("Only the last split should be empty.");
        }
        newWays.append(w);
        replaced.push_back(pair<ElementId, ElementId>(w2->getElementId(), w->getElementId()));
      }

    }

    map->replace(w2, newWays);
  }
}

}
