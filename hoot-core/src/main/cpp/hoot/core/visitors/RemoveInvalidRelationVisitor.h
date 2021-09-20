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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef REMOVE_INVALID_RELATION_VISITOR_H
#define REMOVE_INVALID_RELATION_VISITOR_H

//  Hoot
#include <hoot/core/elements/Relation.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>

namespace hoot
{

/**
 * Remove all duplicate linestrings from multilinestring relations and multilinestring relations
 * that have less than two members, thus making them not "multi" linestrings.
 *
 * TODO: split out duplicate review relation vis into its own class and call it
 * RemoveDuplicateReviewRelationMembers OR maybe it can be replaced with
 * RemoveDuplicateRelationMembersVisitor; rename this class to
 * RemoveDuplicateMultilineStringRelationVisitor
 */
class RemoveInvalidRelationVisitor : public ElementOsmMapVisitor
{
public:

  static QString className() { return "RemoveInvalidRelationVisitor"; }

  RemoveInvalidRelationVisitor();
  ~RemoveInvalidRelationVisitor() override = default;

  void visit(const ElementPtr& e) override;

  QString getInitStatusMessage() const override
  { return "Removing invalid and multiline string relations..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numMembersRemoved) + " relation member(s) and " +
    QString::number(_numAffected) + " relation(s)"; }

  QString getDescription() const override
  {
    return "Removes duplicate ways in relations and invalid relations";
  }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  // one of the few OperatStatusInfo implementers recording two stats, so we'll add an extra var
  // to track
  int _numMembersRemoved;

  void _removeDuplicates(const RelationPtr& r);
};

}

#endif // REMOVE_INVALID_RELATION_VISITOR_H
