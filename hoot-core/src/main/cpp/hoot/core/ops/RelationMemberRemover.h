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
 * @copyright Copyright (C) 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
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
  virtual ~RelationMemberRemover() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(const OsmMapPtr& map);

  /**
   * Removes all relation members references for an element
   *
   * @param id id of the element to remove from relations
   * @param map map owning the element being removed
   * @param includeReviewRelations if true, review relations are modified
   */
  static void remove(const ElementId& id, const OsmMapPtr& map,
                     const bool includeReviewRelations = true);

  virtual QString getInitStatusMessage() const
  {
    return "Removing relation member references for " + _idToReplace.toString() + "...";
  }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " relation member references"; }

  virtual QString getDescription() const
  { return "Removes all relation members references for an element"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  // id of element being removed from relations
  ElementId _idToReplace;
  // if true, review relations are modified
  bool _includeReviewRelations;
};

}

#endif // RELATION_MEMBER_SWAPPER_H

