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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEINVALIDREVIEWRELATIONSVISITOR_H
#define REMOVEINVALIDREVIEWRELATIONSVISITOR_H

#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Remove all empty review relations
 */
class RemoveInvalidReviewRelationsVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::RemoveInvalidReviewRelationsVisitor"; }

  RemoveInvalidReviewRelationsVisitor();

  virtual void visit(const ElementPtr& e);

  virtual QString getInitStatusMessage() const
  { return "Removing review relations with no members..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " empty review relations"; }

  virtual QString getDescription() const { return "Removes empty review relations"; }

  virtual std::string getClassName() const { return className(); }
};

}

#endif // REMOVEINVALIDREVIEWRELATIONSVISITOR_H
