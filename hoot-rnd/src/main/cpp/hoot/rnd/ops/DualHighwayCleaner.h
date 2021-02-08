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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DUAL_HIGHWAY_CLEANER_H
#define DUAL_HIGHWAY_CLEANER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * TODO
 */
class DualHighwayMarker : public ConstOsmMapOperation, public Configurable
{
public:

  static QString className() { return "hoot::DualHighwayMarker"; }

  DualHighwayMarker();
  virtual ~DualHighwayMarker() = default;

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const { return "Marking dual highways..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return "Marked " + StringUtils::formatLargeNumber(_numAffected) + " dual highways.";
  }

  virtual QString getDescription() const { return "Marks dual highways with a custom tag."; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

 private:

  OsmMapPtr _map;

  // TODO
  double _minParallelScore;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  int _taskStatusUpdateInterval;

  Meters _getSearchRadius(const ConstElementPtr& e) const;
  bool _isMatchCandidate(ConstElementPtr element) const;
  void _createIndex();
};

}

#endif // DUAL_HIGHWAY_CLEANER_H
