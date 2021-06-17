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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef UNLIKELYINTERSECTIONREMOVER_H
#define UNLIKELYINTERSECTIONREMOVER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>
#include <hoot/core/conflate/ConflateInfoCacheConsumer.h>

// Standard
#include <set>
#include <vector>

namespace hoot
{

class OsmMap;
class Way;

/**
 * Locates intersections that are likely mistakes and separates them. This is typically a problem
 * with data ingested into OSM (e.g. government data such as TIGER).
 *
 * For example, a motorway overpass intersecting a residential street at a 90° is considered
 * unlikely and "unsnapped". The geometry location is not modified.
 */
class UnlikelyIntersectionRemover : public OsmMapOperation, public ConflateInfoCacheConsumer
{
public:

  static QString className() { return "hoot::UnlikelyIntersectionRemover"; }

  UnlikelyIntersectionRemover() = default;
  ~UnlikelyIntersectionRemover() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  /**
   * Splits all the ways in the input map and returns the resulting map.
   */
  static void removeIntersections(std::shared_ptr<OsmMap> map);

  QString getInitStatusMessage() const override
  { return "Removing unlikely intersections..."; }

  QString getCompletedStatusMessage() const override
  { return "Removed " + QString::number(_numAffected) + " unlikely intersections"; }

  QString getDescription() const override
  { return "Removes road intersections that are likely mistakes"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  void setConflateInfoCache(const std::shared_ptr<ConflateInfoCache>& cache) override
  { _conflateInfoCache = cache; }

private:

  std::shared_ptr<OsmMap> _result;

  // Existence of this cache tells us that elements must be individually checked to see that they
  // are conflatable given the current configuration before modifying them.
  std::shared_ptr<ConflateInfoCache> _conflateInfoCache;

  void _evaluateAndSplit(long intersectingNode, const std::set<long>& wayIds);
  double _pIntersection(long intersectingNode, const std::shared_ptr<Way>& w1,
                        const std::shared_ptr<Way>& w2) const;
  void _splitIntersection(long intersectingNode, const std::vector<std::shared_ptr<Way>>& g2) const;
};

}


#endif // UNLIKELYINTERSECTIONREMOVER_H
