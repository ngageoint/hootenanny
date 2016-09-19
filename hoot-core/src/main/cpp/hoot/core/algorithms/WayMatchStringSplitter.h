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
#ifndef WAYMATCHSTRINGSPLITTER_H
#define WAYMATCHSTRINGSPLITTER_H

#include "WayMatchStringMerger.h"

namespace hoot
{

class WayMatchStringSplitter
{
public:
  WayMatchStringSplitter();

  /**
   * Traverses all mappings, splits ways where appropriate and updates the subline mappings in
   * place.
   */
  void applySplits(OsmMapPtr map, vector<pair<ElementId, ElementId> > &replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) throw (NeedsReviewException);

private:
  static QString _overlyAggressiveMergeReviewText;

  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> _buildWayIndex1(OsmMapPtr map,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;
  QMultiMap<WayPtr, WayMatchStringMerger::SublineMappingPtr> _buildWayIndex2(OsmMapPtr map,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings) const;

  /// @todo there is probably a clever way to make these two functions a single funtion that takes
  /// function pointers or similar
  void _splitWay1(OsmMapPtr map, vector<pair<ElementId, ElementId> > &replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings);
  void _splitWay2(OsmMapPtr map, vector<pair<ElementId, ElementId> > &replaced,
    QList<WayMatchStringMerger::SublineMappingPtr> mappings);
};

}

#endif // WAYMATCHSTRINGSPLITTER_H
