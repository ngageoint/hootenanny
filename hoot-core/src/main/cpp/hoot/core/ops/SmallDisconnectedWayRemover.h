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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef SMALL_DISCONNECTED_WAY_REMOVER_H
#define SMALL_DISCONNECTED_WAY_REMOVER_H

// Hoot
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

/**
 * Removes small and disconnected ways.
 *
 * This is primarily useful for using with incompletely collected data e.g. extracted features.
 */
class SmallDisconnectedWayRemover : public OsmMapOperation, public Configurable, public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "SmallDisconnectedWayRemover"; }

  SmallDisconnectedWayRemover();
  SmallDisconnectedWayRemover(const int maxWayLength, const int maxWayNodeCount);
  ~SmallDisconnectedWayRemover() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Removing small and disconnected ways..."; }
  QString getCompletedStatusMessage() const override
  {
    return "Removed " + StringUtils::formatLargeNumber(_numAffected) + " small and disconnected ways.";
  }

  QString getDescription() const override { return "Removes small and disconnected ways"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

 private:

  OsmMapPtr _map;

  // max length a way can have to be eligible for removal
  double _maxWayLength;
  // max node count a way can have to be eligible for removal
  int _maxWayNodeCount;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;
};

}

#endif // SMALL_DISCONNECTED_WAY_REMOVER_H
