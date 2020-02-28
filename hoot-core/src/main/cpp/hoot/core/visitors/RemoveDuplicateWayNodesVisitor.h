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

#ifndef REMOVEDUPLICATEWAYNODESVISITOR_H
#define REMOVEDUPLICATEWAYNODESVISITOR_H

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

/**
 * Removes all way nodes that are duplicates, with the exception of start/end node duplicates
 * (closed ways).
 *
 * This is kind of a bandaid for the issue discovered in #2066.  The source creating the
 * duplicated nodes appears to be in the conflation routines somewhere and should be found and
 * fixed.  If that happens, this visitor could be removed from the post conflation ops.
 */
class RemoveDuplicateWayNodesVisitor : public ElementVisitor
{
public:

  static std::string className() { return "hoot::RemoveDuplicateWayNodesVisitor"; }

  RemoveDuplicateWayNodesVisitor();

  virtual void visit(const ElementPtr& e);

  /**
   * Removes duplicates nodes from a way
   *
   * @param way the way to remove duplicate nodes from
   */
  static void removeDuplicates(const WayPtr& way);

  virtual QString getInitStatusMessage() const { return "Removing duplicate way nodes..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Removed " + QString::number(_numAffected) + " duplicate way nodes"; }

  virtual QString getDescription() const { return "Removes duplicate way nodes"; }

  /**
   * @see FilteredByCriteria
   */
  virtual QStringList getCriteria() const;

  virtual std::string getClassName() const { return className(); }
};

}

#endif // REMOVEDUPLICATEWAYNODESVISITOR_H
