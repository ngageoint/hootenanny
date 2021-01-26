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
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Removes all consecutive way nodes that are duplicates.
 *
 * This is kind of a bandaid for the issue discovered in #2066.  The source creating the
 * duplicated nodes appears to be in the conflation routines somewhere and should eventually be
 * found and fixed.
 */
class RemoveDuplicateWayNodesVisitor : public ElementVisitor, public OsmMapConsumer
{
public:

  static QString className() { return "hoot::RemoveDuplicateWayNodesVisitor"; }

  RemoveDuplicateWayNodesVisitor() = default;
  virtual ~RemoveDuplicateWayNodesVisitor() = default;

  virtual void visit(const ElementPtr& e);

  /**
   * @see OsmMapConsumer
   */
  virtual void setOsmMap(OsmMap* map) { _map = map->shared_from_this(); }

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
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  OsmMapPtr _map;
};

}

#endif // REMOVEDUPLICATEWAYNODESVISITOR_H
