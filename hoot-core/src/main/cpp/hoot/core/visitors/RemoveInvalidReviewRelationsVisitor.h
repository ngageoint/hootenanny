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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEINVALIDREVIEWRELATIONSVISITOR_H
#define REMOVEINVALIDREVIEWRELATIONSVISITOR_H

#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Remove all empty review relations
 *
 * This runs in conflate.pre.ops, among other places. Its debatablewhether input conflation data
 * should allow reviews at all, but maybe there are some uses cases for it. If we ever find that
 * there aren't, then maybe we should replace this entry in conflate.pre.ops with a
 * 'RemoveReviewRelationsVisitor' that removes them all.
 */
class RemoveInvalidReviewRelationsVisitor : public ElementOsmMapVisitor
{
public:

  static QString className() { return "hoot::RemoveInvalidReviewRelationsVisitor"; }

  RemoveInvalidReviewRelationsVisitor() = default;
  ~RemoveInvalidReviewRelationsVisitor() = default;

  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Removing review relations with no members..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " empty review relations"; }

  QString getDescription() const override { return "Removes empty review relations"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

}

#endif // REMOVEINVALIDREVIEWRELATIONSVISITOR_H
