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

#ifndef REMOVEDUPLICATEWAYNODESVISITOR_H
#define REMOVEDUPLICATEWAYNODESVISITOR_H

// Hoot
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/OsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

namespace hoot
{

/**
 * Removes all consecutive way nodes that are duplicates.
 *
 * This is kind of a bandaid for the issue discovered in #2066.  The source creating the
 * duplicated nodes appears to be in the conflation routines somewhere and should eventually be
 * found and fixed.
 */
class RemoveDuplicateWayNodesVisitor : public ElementVisitor, public OsmMapConsumer,
  public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "RemoveDuplicateWayNodesVisitor"; }

  RemoveDuplicateWayNodesVisitor() = default;
  ~RemoveDuplicateWayNodesVisitor() override = default;

  void visit(const ElementPtr& e) override;

  /**
   * @see OsmMapConsumer
   */
  void setOsmMap(OsmMap* map) override { _map = map->shared_from_this(); }

  /**
   * Removes duplicates nodes from a way
   *
   * @param way the way to remove duplicate nodes from
   */
  static void removeDuplicates(const WayPtr& way);

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getInitStatusMessage() const override { return "Removing duplicate way nodes..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " duplicate way nodes"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes duplicate way nodes"; }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

private:

  OsmMapPtr _map;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;
};

}

#endif // REMOVEDUPLICATEWAYNODESVISITOR_H
