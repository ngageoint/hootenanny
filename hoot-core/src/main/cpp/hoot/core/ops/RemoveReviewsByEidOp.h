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

// Standard
#include <set>

namespace hoot
{

/**
 * Removes all review relations that an element is involved in and optionally the element itself
 */
class RemoveReviewsByEidOp : public ConstOsmMapOperation, public ConstElementConsumer
{
public:

  static QString className() { return "hoot::RemoveReviewsByEidOp"; }

  /**
   * Constructor
   *
   * It is expected that the eid will be populated with addElement after construction. addElement
   * must be called exactly two times.
   */
  RemoveReviewsByEidOp() = default;
  /**
    Constructor

    @param eid TODO
    @param clearAndRemoveElement TODO
    @param removeParentRefs TODO
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
  bool _clearAndRemove;
  // TODO
  bool _removeParentRefs;
};

}

#endif // REMOVEREVIEWSBYEIDOP_H
