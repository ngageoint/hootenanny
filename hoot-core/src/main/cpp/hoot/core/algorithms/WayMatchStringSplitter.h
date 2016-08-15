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
