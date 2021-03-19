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

#ifndef RELATION_MEMBER_REMOVER_H
#define RELATION_MEMBER_REMOVER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * Removes all relation members references for an element
 */
class RelationMemberRemover : public ConstOsmMapOperation
{
public:

  static QString className() { return "hoot::RelationMemberRemover"; }

  RelationMemberRemover();
  RelationMemberRemover(const ElementId& idToReplace, const bool includeReviewRelations = true);
  ~RelationMemberRemover() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * Removes all relation members references for an element
   *
   * @param id id of the element to remove from relations
   * @param map map owning the element being removed
   * @param includeReviewRelations if true, review relations are modified
   */
  static void remove(const ElementId& id, const OsmMapPtr& map,
                     const bool includeReviewRelations = true);

  QString getInitStatusMessage() const override
  {
    return "Removing relation member references for " + _idToReplace.toString() + "...";
  }

  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " relation member references"; }

  QString getDescription() const override
  { return "Removes all relation members references for an element"; }

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

private:

  // id of element being removed from relations
  ElementId _idToReplace;
  // if true, review relations are modified
  bool _includeReviewRelations;
};

}

#endif // RELATION_MEMBER_SWAPPER_H

