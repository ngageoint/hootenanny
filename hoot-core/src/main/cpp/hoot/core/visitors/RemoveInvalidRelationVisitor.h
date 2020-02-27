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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
 * TODO: split out duplicate review relation vis into its own class; rename this class to
 * RemoveDuplicateMultilineStringRelationVisitor
 */
class RemoveInvalidRelationVisitor : public ElementOsmMapVisitor
{
public:

  static std::string className() { return "hoot::RemoveInvalidRelationVisitor"; }

  RemoveInvalidRelationVisitor();

  virtual void visit(const ElementPtr& e);

  virtual QString getInitStatusMessage() const
  { return "Removing invalid and multiline string relations..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numMembersRemoved) + " relation members and " +
    QString::number(_numAffected) + " relations"; }

  virtual QString getDescription() const
  {
    return "Removes duplicate ways in relations and invalid relations";
  }

  virtual std::string getClassName() const { return className(); }

private:

  // one of the few OperatStatusInfo implementers recording two stats, so we'll add an extra var
  // to track
  int _numMembersRemoved;

  void _removeDuplicates(const RelationPtr& r);
};

}

#endif // REMOVE_INVALID_RELATION_VISITOR_H
