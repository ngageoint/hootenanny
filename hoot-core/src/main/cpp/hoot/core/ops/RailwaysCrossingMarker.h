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

#ifndef RAILWAYS_CROSSING_MARKER_H
#define RAILWAYS_CROSSING_MARKER_H

// Hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/conflate/highway/RoadCrossingPolyRule.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * TODO
 *
 * Choosing not to make this a validator since there is already a crossing rails marker in JOSM.
 *
 * Technically, the default configured exclude tags aren't completely accurate. For instance,
 * `railway=subway` is always ignored to be marked as crossing. If its crossing under another above
 * ground rail that is fine, but if it was crossing with another underground rail that should be
 * detected...haven't seen that happen yet, though.
 *
 * @todo abstract RoadCrossingPolyMarker and this into a base class?
 */
class RailwaysCrossingMarker : public ConstOsmMapOperation, public Configurable
{
public:

  static QString className() { return "RailwaysCrossingMarker"; }

  RailwaysCrossingMarker();
  ~RailwaysCrossingMarker() override = default;

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override
  { return "Marking rails that cross each other..."; }
  QString getCompletedStatusMessage() const override
  {
    return
      "Marked " + StringUtils::formatLargeNumber(_numAffected) + " crossing railways " +
      "out of " + StringUtils::formatLargeNumber(_numRailways) + " total railways .";
  }

  QString getDescription() const override { return "Marks crossing railways"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

private:

  OsmMapPtr _map;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  // skip comparison of railways having these tags
  ChainCriterionPtr _tagExcludeFilter;
  // TODO
  bool _markIntraDatasetCrossings;

  /*QSet<ElementId>*/QSet<QString> _markedRailways;
  int _numRailways;

  int _taskStatusUpdateInterval;

  void _createIndex();
  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element) const;
  void _createTagExcludeFilter(const QStringList& excludeTags);
};

}

#endif // RAILWAYS_CROSSING_MARKER_H
