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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef MAPCROPPER_H
#define MAPCROPPER_H

// GEOS
#include <geos/geom/Envelope.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class OsmMap;
class Way;

  using namespace geos::geom;

/**
 * Provides a clean crop at the edges of the map rather than the ragged crop you get from Osmosis.
 * As a result it introduces new nodes into the data and may split ways up into multiple ways.
 *
 * In the class outside and inside are referenced. Outside refers to a geometry that is wholly
 * outside the region that will be kept. Inside refers to a geometry that is at least partially
 * inside the region that will be kept.
 *
 * This class works with four pass as long as all data, bounds and crop geometry are in WGS84.
 * If the data before this operation is in a planar projection then it should be reprojected using
 * ReprojectToGeographicOp.
 */
class MapCropper : public OsmMapOperation, public Serializable, public Boundable,
  public Configurable
{
public:

  static string className() { return "hoot::MapCropper"; }

  MapCropper();

  MapCropper(const Envelope& envelope);

  MapCropper(const boost::shared_ptr<const Geometry>& g, bool invert);

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual void setConfiguration(const Settings& conf);

  static void crop(boost::shared_ptr<OsmMap> map, const Envelope& envelope);

  static void crop(boost::shared_ptr<OsmMap> map, const boost::shared_ptr<const Geometry>& g,
    bool invert);

  virtual string getClassName() const { return className(); }

  virtual void readObject(QDataStream& is);

  /**
   * Sets the bounds on the nodes that will be removed. This is only useful in fourpass.
   * This value will not be serialized.
   */
  virtual void setBounds(const Envelope &bounds) { _nodeBounds = bounds; }

  virtual void writeObject(QDataStream& os) const;

private:
  Envelope _envelope;
  boost::shared_ptr<const Geometry> _envelopeG;
  bool _invert;
  bool _removeNodes;
  Envelope _nodeBounds;

  void _cropWay(boost::shared_ptr<OsmMap> map, long wid);


  /**
   * Finds the node with coordinate c. Throws an exception if multiple nodes are found with the
   * same coordinate. If no node is found then numeric_limits<long>::max() is returned.
   */
  long _findNodeId(boost::shared_ptr<const OsmMap> map, boost::shared_ptr<const Way> w, const Coordinate& c);

  /**
   * Returns true if the specified envelope is wholly inside the region that will be kept. If
   * it is not known exactly then false will be returned.
   *
   * The operation is very quick.
   */
  bool _isWhollyInside(const Envelope& e);

  /**
   * Returns true if the specified envelope is wholly outside the region that will be kept. If
   * it is not known exactly then false will be returned.
   *
   * The operation is very quick.
   */
  bool _isWhollyOutside(const Envelope& e);

  boost::shared_ptr<Way> _reintroduceWay(boost::shared_ptr<OsmMap> map, boost::shared_ptr<const Way> w,
    const LineString* ls);

  friend class MapCropperTest;
};

}

#endif // MAPCROPPER_H
