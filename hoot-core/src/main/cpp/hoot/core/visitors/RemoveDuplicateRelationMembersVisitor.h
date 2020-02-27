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

#ifndef REMOVE_DUPLICATE_RELATION_MEMBER_VISITOR_H
#define REMOVE_DUPLICATE_RELATION_MEMBER_VISITOR_H

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

/**
 * This removes duplicated members from a relation.
 *
 * See notes on RelationData::_members.
 */
class RemoveDuplicateRelationMembersVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::RemoveDuplicateRelationMembersVisitor"; }

  RemoveDuplicateRelationMembersVisitor();

  virtual void visit(const ElementPtr& e);

  virtual QString getInitStatusMessage() const { return "Removing duplicate relation members..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " duplicate relation members"; }

  virtual QString getDescription() const { return "Removes duplicate relation members"; }

  virtual std::string getClassName() const { return className(); }
};

}

#endif // REMOVE_DUPLICATE_RELATION_MEMBER_VISITOR_H
