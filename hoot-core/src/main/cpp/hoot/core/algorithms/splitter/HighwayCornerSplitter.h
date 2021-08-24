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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef HIGHWAYCORNERSPLITTER_H
#define HIGHWAYCORNERSPLITTER_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>

// Qt
#include <QMultiHash>

namespace hoot
{

class OsmMap;

/**
 * @brief The HighwayCornerSplitter class given an OsmMap, splits ways at sharp (or rounded, if
 * desired) corners.
 *
 * This can help when conflating data that is mostly major roads with data that contains a lot of
 * neighborhood-level data.
 */
class HighwayCornerSplitter : public OsmMapOperation, public Configurable
{
public:

  static QString className() { return "hoot::HighwayCornerSplitter"; }

  HighwayCornerSplitter();
  HighwayCornerSplitter(const std::shared_ptr<OsmMap>& map);
  ~HighwayCornerSplitter() = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  static void splitCorners(const std::shared_ptr<OsmMap>& map);
  void splitCorners();

  /**
   * Set the configuration for this object.
   */
  void setConfiguration(const Settings& conf) override;

  QString getInitStatusMessage() const override { return "Splitting sharp road corners..."; }
  QString getCompletedStatusMessage() const override
  { return "Split " + QString::number(_numAffected) + " road corners"; }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Splits sharp road corners"; }

private:

  /**
   * @brief _splitRoundedCorners splits rounded corners in the middle just like a non-rounded
   * corner.
   */
  void _splitRoundedCorners();
  /**
   * @brief _splitWay splits the way at the given node, using the WaySplitter, then process the
   * results.
   * @param wayId Index of way to split
   * @param nodeIdx Index of node to split at
   * @param nodeId ID of the node to split at
   * @param sharpCorner True if being called from the sharp corner splitting code
   * @return True if the way was actually split
   */
  bool _splitWay(long wayId, long nodeIdx, long nodeId, bool sharpCorner = true);
  /** Pointer to the OsmMap */
  std::shared_ptr<OsmMap> _map;
  /** Vector of ways that are yet to be processed */
  std::vector<long> _todoWays;
  /** Threshold in degrees for splitting sharp corners */
  double _cornerThreshold;
  /** Flag to turn on/off rounded corner splitting */
  bool _splitRounded;
  /** Threshold in degrees for splitting rounded corners */
  double _roundedThreshold;
  /** Maximum number of nodes to consider while checking for a rounded corner*/
  int _roundedMaxNodeCount;
};

}

#endif // HIGHWAYCORNERSPLITTER_H
