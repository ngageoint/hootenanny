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
#include <hoot/core/conflate/highway/RoadCrossingPolyRule.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * Marks railway features for review which cross each other. Has options to ignore certain rail
 * types and to prevent marking features that are source from the same input.
 *
 * Choosing not to make this a validator since there is already a crossing rails marker in JOSM. The
 * case could be made at some point to implement Validator if there are differences in the
 * implementations.
 *
 * Technically, the current default configured exclude tags could cause problems. For instance,
 * `railway=subway` is part of the config option and is thus always ignored to be marked as
 * crossing. If a `railway=subway` is crossing under another above ground rail, that is fine.
 * However, if it was crossing with another underground rail that should be detected by this
 * class...haven't actually seen that happen yet, though.
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
    return QString("Marked %1 crossing railways out of %2 total railways.")
             .arg(StringUtils::formatLargeNumber(_numAffected), StringUtils::formatLargeNumber(_numRailways));
  }

  QString getDescription() const override { return "Marks crossing railways"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  /**
   * @brief setTagExcludeFilter adds tag kvp's for features which should be excluded from marking
   * @param excludeTags list of tag kvp's to exclude
   */
  void setTagExcludeFilter(const QStringList& excludeTags);
  /**
   * @brief setMarkIntraDatasetCrossings determines whether feature pairs from within the same input
   * dataset may be marked.
   * @param mark true to mark features within the same input dataset; false otherwise
   */
  void setMarkIntraDatasetCrossings(const bool mark) { _markIntraDatasetCrossings = mark; }

private:

  OsmMapPtr _map;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  // skip comparison of railways having these tags
  ChainCriterionPtr _tagExcludeFilter;
  // if true, no feature status check is done; otherwise input1 vs input1 or input2 vs input2 won't
  // be considered
  bool _markIntraDatasetCrossings;

  // set of concatenated feature pair ID strings; e.g. Way(-1);Way(-2)
  QSet<QString> _markedRailways;
  // total number of railways parsed
  int _numRailways;

  int _taskStatusUpdateInterval;

  void _createIndex();
  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element) const;
};

}

#endif // RAILWAYS_CROSSING_MARKER_H
