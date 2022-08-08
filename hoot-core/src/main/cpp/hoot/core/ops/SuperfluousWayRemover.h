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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef SUPERFLUOUSWAYREMOVER_H
#define SUPERFLUOUSWAYREMOVER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

namespace hoot
{

class OsmMap;

/**
 * Removes all ways that are not being used by relations and contain zero nodes or all the nodes
 * are identical.
 */
class SuperfluousWayRemover : public OsmMapOperation, public Configurable,
  public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "SuperfluousWayRemover"; }

  SuperfluousWayRemover() = default;
  ~SuperfluousWayRemover() override = default;

  /**
   * @see OsmMapOperation
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Splits all the ways in the input map and returns the resulting map.
   *
   * @return the number of ways removed
   */
  static long removeWays(std::shared_ptr<OsmMap>& map);

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Removing superfluous ways..."; }
  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " superfluous ways"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Removes ways not in relations or containing zero or all identical nodes"; }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

private:

  // ways with these IDs will never be removed
  QSet<long> _excludeIds;
  // the number of ways that explicitly weren't removed due to configuration
  int _numExplicitlyExcluded;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  void _removeWays(const std::shared_ptr<OsmMap>& map);
};

}

#endif // SUPERFLUOUSWAYREMOVER_H
