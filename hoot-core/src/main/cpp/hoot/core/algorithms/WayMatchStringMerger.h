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
#ifndef WAYSTRINGMERGER_H
#define WAYSTRINGMERGER_H

// hoot
#include <hoot/core/algorithms/linearreference/WaySublineMatchString.h>
#include <hoot/core/algorithms/linearreference/WayMatchStringMapping.h>
#include <hoot/core/schema/TagMerger.h>

namespace hoot
{

class WayMatchStringMergerTest;

/**
 * Provides methods for merging features/geometries in one string of ways into another string of
 * ways.
 */
class WayMatchStringMerger
{
public:
  WayMatchStringMerger(const OsmMapPtr& map, WayMatchStringMappingPtr mapping,
    vector< pair<ElementId, ElementId> >& replaced);

  /**
   * Alternative constructor. If this is used operations that modify the map will fail.
   */
  WayMatchStringMerger(const ConstOsmMapPtr& map, WayMatchStringMappingPtr mapping);

  /**
   * Create a subline match string. This is useful if calling a highway classifier.
   */
  WaySublineMatchStringPtr createMatchString() const;

  /**
   * Merge scrapNode into the keeper way.
   */
  void mergeNode(ElementId scrapNode);

  /**
   * Merge all the tags from the scrap into the associated keeper ways.
   */
  void mergeTags();

  /**
   * Replaces all instances of the scrap elements with the associated set of keeper elements.
   */
  void replaceScraps();

  /**
   * Set the status on all the keeper ways.
   */
  void setKeeperStatus(Status s);

  void setTagMerger(ConstTagMergerPtr tagMerger) { _tagMerger = tagMerger; }

private:
  class SublineMapping
  {
  public:
    WayLocation start;
    WayLocation end;
    WayPtr newWay1;
    WaySubline subline2;
    ConstWayPtr way2;

    QString toString() const
    {
      return QString("{start: %1, end: %2, newWay1: %3, way2: %4}")
        .arg(hoot::toString(start))
        .arg(hoot::toString(end))
        .arg(hoot::toString(newWay1 ? newWay1->getElementId() : ElementId()))
        .arg(hoot::toString(way2 ? way2->getElementId() : ElementId()));
    }
  };
  typedef boost::shared_ptr<SublineMapping> SublineMappingPtr;

  class SublineMappingLessThan
  {
  public:
      inline bool operator()(const WayMatchStringMerger::SublineMappingPtr &t1,
        const WayMatchStringMerger::SublineMappingPtr &t2) const
      {
          return min(t1->start, t1->end) < min(t2->start, t2->end);
      }
  };

  // white box testing
  friend class WayMatchStringMergerTest;

  OsmMapPtr _map;
  WayMatchStringMappingPtr _mapping;
  vector< pair<ElementId, ElementId> >& _replaced;
  // Maps way1 to subline mappings.
  QMultiMap<WayPtr, SublineMappingPtr> _sublineMapping;
  QList<SublineMappingPtr> _sublineMappingOrder;
  QList<WayPtr> _scraps1;
  QMap<ElementId, WayLocation> _nodeToWayLocation2;
  ConstTagMergerPtr _tagMerger;

  void _addSublineMapping(SublineMappingPtr sm);

  void _createWayMappings(WayLocation split1, WayLocation split2, WaySubline subline2);

  WayLocation _findNodeLocation2(WayStringPtr ws, ElementId nodeId);

  void _moveNode(ElementId scrapNode, WayLocation wl1);

  void _rebuildWayString1();

  void _splitPrimary();
};

typedef boost::shared_ptr<WayMatchStringMerger> WayMatchStringMergerPtr;
typedef boost::shared_ptr<const WayMatchStringMerger> ConstWayMatchStringMergerPtr;

}

#endif // WAYSTRINGMERGER_H
