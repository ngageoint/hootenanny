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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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

  static std::string className() { return "hoot::RemoveReviewUnknown2Visitor"; }

  virtual void visit(const std::shared_ptr<Element>& e) override;

  virtual QString getDescription() const override
  { return "Removes review relations and their UNKNOWN2 elements"; }

  virtual QString getInitStatusMessage() const
  { return "Removing relations..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Removed " + QString::number(_numAffected) + " review relations and their " +
      QString::number(_numElements) + " UNKNOWN2 elements.";
  }

  virtual std::string getClassName() const { return className(); }

private:

  /** Total number of elements removed along with review relations */
  int _numElements;
};

}

#endif  //  REMOVE_REVIEW_UKNOWN2_VISITOR_H
