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
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef RELATION_MERGER_H
#define RELATION_MERGER_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/OsmMapConsumer.h>

namespace hoot
{

/**
 * Merges similar relations together
 *
 * When members within relations do not match between layers, we keep the non-matching members from
 * both layers and attempt to match and combine their containing relations. Since relations may have
 * similar members with different element IDs between reference and secondary layers, the
 * comparisons ignore element IDs and look at the member elements directly.
 */
class RelationMerger : public OsmMapConsumer
{
public:

  static QString className() { return "RelationMerger"; }

  RelationMerger();
  ~RelationMerger() = default;

  /**
   * Merges the members from the relation with the second element ID into the relation with the
   * first element ID, updates element references, and removes the relation with the second element
   * ID if all of its members are copied over (they may not be if a bounds is present).
   *
   * @param elementId1 ID of the first relation to merge
   * @param elementId2 ID of the second relation to merge
   */
  void merge(const ElementId& elementId1, const ElementId& elementId2);

  /**
   * @see OsmMapConsumer
   */
  void setOsmMap(OsmMap* map) override { _map = map->shared_from_this(); }

  void setMergeTags(bool merge) { _mergeTags = merge; }
  void setDeleteRelation2(bool deleteRelation) { _deleteRelation2 = deleteRelation; }

private:

  OsmMapPtr _map;

  // determines whether tags of the two relations are merged
  bool _mergeTags;
  // determines whether the relation whose members were merged into the other relation is deleted;
  // Useful if members from the same relation are to be merged into multiple other relations
  bool _deleteRelation2;

  /*
   * Returns true if all members from the second relation were merged into the frist relation
   */
  bool _mergeMembers(RelationPtr replacingRelation, RelationPtr relationBeingReplaced) const;
};

}

#endif // _RELATION_MERGER_H
