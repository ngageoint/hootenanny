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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVE_REVIEW_UKNOWN2_VISITOR_H
#define REMOVE_REVIEW_UKNOWN2_VISITOR_H

//  Hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Removes all review relations and the UNKNOWN2 elements of those relations leaving only
 * geometries from the ref data and no reviews.
 */
class RemoveReviewUnknown2Visitor : public ElementOsmMapVisitor
{
public:

  static QString className() { return "RemoveReviewUnknown2Visitor"; }

  RemoveReviewUnknown2Visitor() = default;
  ~RemoveReviewUnknown2Visitor() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override
  { return "Removing relations..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Removed " + QString::number(_numAffected) + " review relations and their " +
      QString::number(_numElements) + " UNKNOWN2 elements.";
  }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Removes review relations and their UNKNOWN2 elements"; }

private:

  /** Total number of elements removed along with review relations */
  int _numElements;
};

}

#endif  //  REMOVE_REVIEW_UKNOWN2_VISITOR_H
