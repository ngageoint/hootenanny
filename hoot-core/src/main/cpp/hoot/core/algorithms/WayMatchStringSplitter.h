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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef WAYMATCHSTRINGSPLITTER_H
#define WAYMATCHSTRINGSPLITTER_H

#include <hoot/core/algorithms/WayMatchStringMerger.h>

namespace hoot
{

class WayMatchStringSplitter
{
public:

  WayMatchStringSplitter() = default;

  /**
   * Traverses all mappings, splits ways where appropriate and updates the subline mappings in
   * place.
   *
   * @throws NeedsReviewException
   */
  void applySplits(
    OsmMapPtr map, std::vector<std::pair<ElementId, ElementId>>& replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;

private:

  static QString _overlyAggressiveMergeReviewText;

  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> _buildWayIndex(
    WayNumber wn, OsmMapPtr map, QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;

  void _splitWay(
    WayNumber wn, OsmMapPtr map, std::vector<std::pair<ElementId, ElementId>>& replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;
};

}

#endif // WAYMATCHSTRINGSPLITTER_H
