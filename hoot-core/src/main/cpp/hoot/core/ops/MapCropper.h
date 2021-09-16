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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef MAPCROPPER_H
#define MAPCROPPER_H

// GEOS
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class OsmMap;

/**
 * Provides a clean crop at the edges of the map rather than the ragged crop you get from Osmosis.
 * As a result, it introduces new nodes into the data and may split ways up into multiple ways.
 */
class MapCropper : public OsmMapOperation, public Boundable, public Configurable
{
public:

  static int logWarnCount;

  static QString className() { return "MapCropper"; }

  MapCropper();
  ~MapCropper() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  void setConfiguration(const Settings& conf) override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Crops a map"; }

  QString getInitStatusMessage() const override;
  QString getCompletedStatusMessage() const override
  {
    return
      "Cropped " + StringUtils::formatLargeNumber(_numAffected) + " / " +
      StringUtils::formatLargeNumber(_numProcessed) + " elements";
  }

  void setInvert(bool invert);
  void setKeepEntireFeaturesCrossingBounds(bool keep);
  void setKeepOnlyFeaturesInsideBounds(bool keep);
  void setInclusionCriterion(const ElementCriterionPtr& crit) { _inclusionCrit = crit; }
  void setRemoveSuperflousFeatures(bool remove) { _removeSuperfluousFeatures = remove; }
  void setRemoveMissingElements(bool remove) { _removeMissingElements = remove; }
  void setLogWarningsForMissingElements(bool log) { _logWarningsForMissingElements = log; }

private:

  friend class MapCropperTest;

  // if false data outside of the boundary is removed; if true, data inside of the boundary is
  // removed
  bool _invert;
  // If true, won't split apart features straddling the specified bounds.
  bool _keepEntireFeaturesCrossingBounds;
  // If true, will only keep features falling completely inside the specified bounds. This overrides
  // _keepEntireFeaturesCrossingBounds and sets it to false;
  bool _keepOnlyFeaturesInsideBounds;
  // Any way meeting this criterion and all of its way nodes will not be cropped out of the map
  ElementCriterionPtr _inclusionCrit;
  // tracks all ways satisfying _inclusionCrit
  std::set<long> _explicitlyIncludedWayIds;
  // removes useful features created after cropping
  bool _removeSuperfluousFeatures;
  // removes references to missing elements after cropping occurs
  bool _removeMissingElements;

  int _statusUpdateInterval;

  int _numWaysInBounds;
  int _numWaysOutOfBounds;
  int _numWaysCrossingThreshold;
  int _numCrossingWaysKept;
  int _numCrossingWaysRemoved;
  int _numNodesRemoved;

  // determines whether missing elements trigger a warning
  bool _logWarningsForMissingElements;

  void _cropWay(const std::shared_ptr<OsmMap>& map, long wid);

  /**
   * Returns true if the specified envelope is wholly inside the region that will be kept.
   */
  bool _isWhollyInside(const geos::geom::Envelope& e) const;
  bool _isWhollyInside(const geos::geom::Geometry& e) const;

  /**
   * Returns true if the specified envelope is wholly outside the region that will be kept.
   */
  bool _isWhollyOutside(const geos::geom::Envelope& e) const;
  bool _isWhollyOutside(const geos::geom::Geometry& e) const;
};

}

#endif // MAPCROPPER_H
