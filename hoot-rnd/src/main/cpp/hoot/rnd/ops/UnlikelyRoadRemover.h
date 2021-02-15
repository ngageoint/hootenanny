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

#ifndef UNLIKELY_ROAD_REMOVER_H
#define UNLIKELY_ROAD_REMOVER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Removes road features that are very likely not roads at all
 */
class UnlikelyRoadRemover : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "hoot::UnlikelyRoadRemover"; }

  UnlikelyRoadRemover();
  virtual ~UnlikelyRoadRemover() = default;

  /**
   * @see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const { return "Removing unlikely roads..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Removed " + StringUtils::formatLargeNumber(_numAffected) + " unlikely roads.";
  }

  virtual QString getDescription() const
  { return "Removes road features that are very likely not roads at all"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

private:

  // TODO
  int _maxWayLength;
  // TODO
  int _numHistogramBins;
  // TODO
  double _headingDelta;
  // TODO
  Meters _sampleDistance;
  // TODO
  double _maxHeadingVariance;

  ElementCriterionPtr _getRemovalCrit(const ConstOsmMapPtr& map);
};

}

#endif // UNLIKELY_ROAD_REMOVER_H
