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

#ifndef DUAL_HIGHWAY_MARKER_H
#define DUAL_HIGHWAY_MARKER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/conflate/ConflateInfoCache.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * Detects and marks divided roads with a custom tag. At a minimum, a oneway=yes tag must be
 * present on a road in order for it to be detected.
 */
class DualHighwayMarker : public ConstOsmMapOperation, public Configurable
{
public:

  static QString className() { return "DualHighwayMarker"; }

  DualHighwayMarker();
  ~DualHighwayMarker() override = default;

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return "Marking roads that appear to be dual highways..."; }
  QString getCompletedStatusMessage() const override
  {
    return "Marked " + StringUtils::formatLargeNumber(_numAffected) + " dual highways.";
  }

  QString getDescription() const override { return "Marks dual highways with a custom tag."; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  void setMinParallelScore(const double score) { _minParallelScore = score; }
  void setMarkCrossingRoads(const bool mark) { _markCrossingRoads = mark; }
  void setMaxCrossingRoadsParallelScore(const double score)
  { _maxCrossingRoadsParallelScore = score; }

 private:

  OsmMapPtr _map;

  // how parallel two roads must be to be considered as part of the same divided highway
  double _minParallelScore;
  // determines whether roads crossing divided roads are also marked
  bool _markCrossingRoads;
  // how perpendicular a road intersecting a divided road must be to be considered as crossing
  // between it
  double _maxCrossingRoadsParallelScore;

  // the number of crossing roads detected
  int _numCrossing;
  // used to cache element distance vals
  std::shared_ptr<ConflateInfoCache> _elementInfo;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  int _taskStatusUpdateInterval;

  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element) const;
  void _createIndex();

  void _markRoadsCrossingDividedRoads(const ConstWayPtr& divRoad1, const ConstWayPtr& divRoad2);
};

}

#endif // DUAL_HIGHWAY_MARKER_H
