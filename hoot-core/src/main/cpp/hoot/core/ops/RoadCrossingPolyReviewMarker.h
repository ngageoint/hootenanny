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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef ROAD_CROSSING_POLY_REVIEW_MARKER_H
#define ROAD_CROSSING_POLY_REVIEW_MARKER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/ConstOsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/highway/RoadCrossingPolyRule.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QSet>

namespace hoot
{

/**
 * TODO
 */
class RoadCrossingPolyReviewMarker : public ConstOsmMapOperation, public Configurable
{
public:

  static std::string className() { return "hoot::RoadCrossingPolyReviewMarker"; }

  RoadCrossingPolyReviewMarker();

  /**
   * @see ConstOsmMapOperation
   */
  void apply(const OsmMapPtr& map);

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings& conf);

  virtual QString getInitStatusMessage() const
  { return "Marking roads crossing polygons for review..."; }

  virtual QString getCompletedStatusMessage() const
  {
    return
      "Marked " + StringUtils::formatLargeNumber(_numAffected) + " / " +
      StringUtils::formatLargeNumber(_numProcessed) + " roads crossing polygons.";
  }

  virtual QString getDescription() const
  { return "Marks roads crossing polygons for review during conflation"; }

  virtual std::string getClassName() const { return className(); }

 private:

  OsmMapPtr _map;
  QString _crossingRulesFile;
  QList<RoadCrossingPolyRule> _crossingRules;
  QSet<ElementId> _markedRoads;
};

}

#endif // ROAD_CROSSING_POLY_REVIEW_MARKER_H
