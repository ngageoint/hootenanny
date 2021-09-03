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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEREVIEWSBYEIDOP_H
#define REMOVEREVIEWSBYEIDOP_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementConsumer.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>

namespace hoot
{

/**
 * Removes all review relations that an element is involved in and optionally the element itself
 */
class RemoveReviewsByEidOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:

  static QString className() { return "RemoveReviewsByEidOp"; }

  /**
   * Constructor
   *
   * It is expected that the eid will be populated with addElement after construction. addElement
   * must be called exactly two times.
   */
  RemoveReviewsByEidOp() = default;
  /**
    Constructor

    @param eid ID of the element being removed from reviews
    @param clearAndRemoveElement If set to true, then the element will be cleared of all attributes and a
   * removal will be attempted. In some cases (e.g. replace can't be complete if you're replacing a
   * node with a way and the node is in a way), the element won't be removed. If this happens, then
   * all tags will be cleared.
    @param removeParentRefs If true, removes all references in parent relations or ways to the
   * element being removed before trying to remove it. If false and clearAndRemoveElement=true, and
   * the element being removed has memberships in a relation or way, the element will not be removed.
   */
  RemoveReviewsByEidOp(
    ElementId eid, bool clearAndRemoveElement = false, bool removeParentRefs = false);
  ~RemoveReviewsByEidOp() = default;

  /**
   * If the elements aren't specified in the constructor, this must be called exactly two times.
   * Once for 'from' and a second time for 'to'.
   */
  void addElement(const ConstElementPtr& e) override;

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  QString getInitStatusMessage() const override
  { return "Removing conflation reviews..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " conflation reviews"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Removes conflation reviews associated with specified element IDs"; }

private:

  ElementId _eid;
  // removes an element after it has been removed from reviews
  bool _clearAndRemove;
  // removes all references in parent elements to the element being removed from reviews
  bool _removeParentRefs;
};

}

#endif // REMOVEREVIEWSBYEIDOP_H
