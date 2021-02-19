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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
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
 * Removes road features that are very likely not roads
 *
 * This is primarily useful for using with incompletely collected data (e.g. extracted features) or
 * data that has been poorly merged during conflation due to its complexity.
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
  { return "Removed " + StringUtils::formatLargeNumber(_numAffected) + " unlikely roads."; }

  virtual QString getDescription() const
  { return "Removes road features that are very likely not roads"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  virtual QStringList getCriteria() const;

private:

  // max length a way can have to be eligible for removal
  int _maxWayLength;
  // number of histogram bins to use with the angle extractor used to calc heading variance
  int _numHistogramBins;
  // heading delta to use with the angle extractor used to calc heading variance; see
  // SampledAngleHistogramExtractor
  double _headingDelta;
  // sample distance to use with the angle extractor used to calc heading variance; see
  // SampledAngleHistogramExtractor
  Meters _sampleDistance;
  // heading variance at or above which a way may be eligible for removal
  double _maxHeadingVariance;

  ElementCriterionPtr _getRemovalCrit(const ConstOsmMapPtr& map);
};

}

#endif // UNLIKELY_ROAD_REMOVER_H
