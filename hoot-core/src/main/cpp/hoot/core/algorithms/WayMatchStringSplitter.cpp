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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "WayMatchStringSplitter.h"

// hoot
#include <hoot/core/algorithms/splitter/WaySplitter.h>
#include <hoot/core/io/OsmJsonWriter.h>
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/util/Log.h>

using namespace std;

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
  QList<WayMatchStringMerger::SublineMappingPtr> mappings)
{
  LOG_TRACE("Applying way splits...");
  _splitWay(WayNumber::Way1, map, replaced, mappings);
  _splitWay(WayNumber::Way2, map, replaced, mappings);
}

QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> WayMatchStringSplitter::_buildWayIndex(
  WayNumber wn, OsmMapPtr map, QList<WayMatchStringMerger::SublineMappingPtr> mappings) const
{
  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> result;

  foreach (WayMatchStringMerger::SublineMappingPtr sm, mappings)
    result.insert(map->getWay(sm->getStart(wn).getWay()->getElementId()), sm);

  return result;
}

void WayMatchStringSplitter::_splitWay(WayNumber wn, OsmMapPtr map, vector<pair<ElementId, ElementId> > &replaced,
                                        QList<WayMatchStringMerger::SublineMappingPtr> mappings)
{
  LOG_TRACE(QString("Splitting way %1...").arg((int)wn));

  ElementConverter ec(map);

  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> wayMapping =
    _buildWayIndex(wn, map, mappings);

  // split each way in the subline mapping.
  foreach (WayPtr way, wayMapping.uniqueKeys())
  {
    LOG_VART(way->getElementId());
    LOG_VART(way->getStatus());

    QList<WayMatchStringMerger::SublineMappingPtr> sm = wayMapping.values(way);
    qStableSort(sm.begin(), sm.end(), WayMatchStringMerger::SublineMappingLessThan(wn));

    // if there is only one entry in sm and it spans the whole way, don't split anything.
    if (sm.size() == 1 && sm[0]->getStart(wn).isExtreme(WayLocation::SLOPPY_EPSILON) &&
      sm[0]->getEnd(wn).isExtreme(WayLocation::SLOPPY_EPSILON))
    {
      LOG_TRACE(
        QString("Skipping splitting way %1 due to single entry in subline spanning the entire way...").arg((int)wn));
      sm[0]->setNewWay(wn, way);
      continue;
    }

    vector<WayLocation> wls;

    // push them all on. This will inevitably create some empty ways, but it is predictable.
    for (int i = 0; i < sm.size(); ++i)
    {
      // make sure the split points are sorted.
      wls.push_back(std::min(sm.at(i)->getStart(wn), sm.at(i)->getEnd(wn)));
      wls.push_back(std::max(sm.at(i)->getStart(wn), sm.at(i)->getEnd(wn)));
    }

    // This can happen if there is an unmatched section in a way
    bool discontiguous = false;
    for (size_t i = 0; i < wls.size() - 1; ++i)
    {
      if (i % 2 == 1 && wls[i] != wls[i + 1])
        discontiguous = true;
    }

    vector<WayPtr> splits = WaySplitter(map, way).createSplits(wls);

    assert((int)splits.size() == sm.size() * 2 + 1);

    LOG_VART(wls);
    LOG_VART(splits);
    QList<ElementPtr> newWays;

    int c = 0;
    WayPtr w = splits[c++];
    // if this isn't empty
    if (w && ec.calculateLength(w) > 0.0)
    {
      newWays.append(w);
      replaced.push_back(pair<ElementId, ElementId>(way->getElementId(), w->getElementId()));
    }

    for (int i = 0; i < sm.size(); ++i)
    {
      WayPtr w;
      w = splits[c++];
      if (!w)
        throw NeedsReviewException(_overlyAggressiveMergeReviewText);

      sm.at(i)->setNewWay(wn, w);
      replaced.push_back(pair<ElementId, ElementId>(way->getElementId(), w->getElementId()));
      newWays.append(w);

      w = splits[c++];
      // if this isn't empty
      if (w && ec.calculateLength(w) > 0.0)
      {
        // only the last split should be non-empty
        if (i != sm.size() - 1 && discontiguous == false)
          throw InternalErrorException("Only the last split should be empty.");

        newWays.append(w);
        replaced.push_back(pair<ElementId, ElementId>(way->getElementId(), w->getElementId()));
      }
    }

    LOG_TRACE("Replacing: " << way->getElementId() << " with: " << newWays);
    map->replace(way, newWays);
  }
}

}
