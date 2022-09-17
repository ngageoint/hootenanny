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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef ROAD_CROSSING_POLY_MARKER_H
#define ROAD_CROSSING_POLY_MARKER_H

// Hoot
#include <hoot/core/conflate/highway/RoadCrossingPolyRule.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/validation/Validator.h>

namespace hoot
{

/**
 * Marks roads in instances where they cross over polygons and is governed by a set of rules (see
 * RoadCrossingPolyRule and the "Feature Validation" section of the User documentation). By default,
 * such roads are marked for review. If validation is enabled, then validation tags are added
 * instead of reviews.
 *
 * We could extend this to other linear types, like rivers, once we see a need.
 */
class RoadCrossingPolyMarker : public ConstOsmMapOperation, public Configurable, public Validator
{
public:

  static QString className() { return "RoadCrossingPolyMarker"; }

  RoadCrossingPolyMarker();
  ~RoadCrossingPolyMarker() override = default;

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map) override;

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Marking roads crossing polygons..."; }
  QString getCompletedStatusMessage() const override
  {
    return QString("Marked %1 roads crossing polygons out of %2 total roads .")
            .arg(StringUtils::formatLargeNumber(_numAffected), StringUtils::formatLargeNumber(_numRoads));
  }

  QString getDescription() const override { return "Marks roads crossing polygons"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  /**
   * @see Validator
   */
  void enableValidation() override { _addValidationTags = true; }
  /**
   * @see Validator
   */
  void disableValidation() override { _addValidationTags = false; }
  /**
   * @see Validator
   */
  QString getValidationErrorMessage() const override;
  /**
   * @see Validator
   */
  long getNumValidationErrors() const override { return _numAffected; }
  /**
   * @see Validator
   */
  long getNumFeaturesValidated() const override { return _numProcessed; }

private:

  OsmMapPtr _map;

  QString _crossingRulesFile;
  QList<RoadCrossingPolyRule> _crossingRules;
  bool _addValidationTags;

  QSet<ElementId> _markedRoads;
  int _numRoads;

  int _taskStatusUpdateInterval;
};

}

#endif // ROAD_CROSSING_POLY_MARKER_H
