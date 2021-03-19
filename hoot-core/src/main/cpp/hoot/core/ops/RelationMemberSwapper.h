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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RELATION_MEMBER_SWAPPER_H
#define RELATION_MEMBER_SWAPPER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * Swaps all relation members references for one element ID with another
 */
class RelationMemberSwapper : public ConstOsmMapOperation
{
public:

  static QString className() { return "hoot::RelationMemberSwapper"; }

  RelationMemberSwapper();
  RelationMemberSwapper(const ElementId& idToReplace, const ElementId& idToReplaceWith,
                        const bool includeReviewRelations = true);
  ~RelationMemberSwapper() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * Swaps all relation members references for one element ID with another
   *
   * @param idToReplace id of the element to remove from relations
   * @param idToReplaceWith id of the element to replace the element removed from relations
   * @param map map owning the element being removed
   * @param includeReviewRelations if true, review relations are modified
   */
  static void swap(const ElementId& idToReplace, const ElementId& idToReplaceWith,
                   const OsmMapPtr& map, const bool includeReviewRelations = true);

  QString getInitStatusMessage() const override
  {
    return
      "Swapping relation member references for " + _idToReplace.toString() + " with " +
      _idToReplaceWith.toString() + " ...";
  }

  QString getCompletedStatusMessage() const override
  { return "Swapped " + QString::number(_numAffected) + " relation member references"; }

  QString getDescription() const override
  { return "Swaps all relation members references for one element ID with another"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  // id of element being removed from relations
  ElementId _idToReplace;
  // id of the element to replace the element removed from relations
  ElementId _idToReplaceWith;
  // if true, review relations are modified
  bool _includeReviewRelations;
};

}

#endif // RELATION_MEMBER_SWAPPER_H
