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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MAPCROPPER_H
#define MAPCROPPER_H

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class OsmMap;
class Way;

/**
 * Provides a clean crop at the edges of the map rather than the ragged crop you get from Osmosis.
 * As a result, it introduces new nodes into the data and may split ways up into multiple ways.
 *
 * In the class, outside and inside are referenced. Outside refers to a geometry that is wholly
 * outside the region that will be kept. Inside refers to a geometry that is at least partially
 * inside the region that will be kept.
 *
 * This class works with four pass conflation (used in Hadoop only) as long as all data, bounds and
 * crop geometry are in WGS84. If the data before this operation is in a planar projection then it
 * should be reprojected using ReprojectToGeographicOp.
 */
class MapCropper : public OsmMapOperation, public Serializable, public Boundable,
  public Configurable
{
public:

  static int logWarnCount;

  static std::string className() { return "hoot::MapCropper"; }

  MapCropper();
  MapCropper(const geos::geom::Envelope& envelope);
  MapCropper(const std::shared_ptr<const geos::geom::Geometry>& g);

  virtual void apply(std::shared_ptr<OsmMap>& map);

  virtual void setConfiguration(const Settings& conf) override;

  virtual std::string getClassName() const override { return className(); }

  virtual void readObject(QDataStream& is) override;

  /**
   * Sets the bounds on the nodes that will be removed.
   */
  virtual void setBounds(const geos::geom::Envelope& bounds) override;
  virtual void setBounds(const std::shared_ptr<const geos::geom::Geometry>& g);

  virtual void writeObject(QDataStream& os) const override;

  virtual QString getDescription() const override { return "Crops a map"; }

  virtual QString getInitStatusMessage() const override
  { return "Cropping map..."; }

  virtual QString getCompletedStatusMessage() const override
  {
    return
      "Cropped " + StringUtils::formatLargeNumber(_numAffected) + " / " +
      StringUtils::formatLargeNumber(_numProcessed) + " elements"; }

  void setInvert(bool invert);
  void setKeepEntireFeaturesCrossingBounds(bool keep);
  void setKeepOnlyFeaturesInsideBounds(bool keep);
  void setInclusionCriterion(const ElementCriterionPtr& crit) { _inclusionCrit = crit; }
  void setRemoveSuperflousFeatures(bool remove) { _removeSuperfluousFeatures = remove; }

private:

  friend class MapCropperTest;

  geos::geom::Envelope _envelope;
  std::shared_ptr<const geos::geom::Geometry> _envelopeG;
  bool _invert;
  geos::geom::Envelope _nodeBounds;
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

  int _statusUpdateInterval;

  int _numWaysInBounds;
  int _numWaysOutOfBounds;
  int _numWaysCrossingThreshold;
  int _numCrossingWaysKept;
  int _numCrossingWaysRemoved;
  int _numNodesRemoved;

  void _cropWay(const std::shared_ptr<OsmMap>& map, long wid);

  /**
   * Returns true if the specified envelope is wholly inside the region that will be kept.
   */
  bool _isWhollyInside(const geos::geom::Envelope& e);
  bool _isWhollyInside(const geos::geom::Geometry& e);

  /**
   * Returns true if the specified envelope is wholly outside the region that will be kept.
   */
  bool _isWhollyOutside(const geos::geom::Envelope& e);
  bool _isWhollyOutside(const geos::geom::Geometry& e);
};

}

#endif // MAPCROPPER_H
