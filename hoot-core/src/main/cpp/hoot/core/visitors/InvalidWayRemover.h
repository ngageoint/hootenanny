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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */

#ifndef INVALID_WAY_REMOVER_H
#define INVALID_WAY_REMOVER_H

// hoot
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

namespace hoot
{

/**
 * Removes ways with no nodes and zero length ways (ways with one node).
 */
class InvalidWayRemover : public ElementOsmMapVisitor, public ConflateInfoCacheConsumer
{

public:

  static QString className() { return "hoot::InvalidWayRemover"; }

  InvalidWayRemover() = default;
  ~InvalidWayRemover() override = default;

  void visit(const std::shared_ptr<Element>& e) override;

  QString getInitStatusMessage() const override
  { return "Removing invalid ways..."; }

  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " invalid ways."; }

  QString getDescription() const override
  { return "Removes ways with no nodes and zero length ways"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

private:

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;
};

}

#endif // INVALID_WAY_REMOVER_H
