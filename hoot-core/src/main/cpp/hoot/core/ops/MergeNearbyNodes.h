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

#ifndef MERGENEARBYNODES_H
#define MERGENEARBYNODES_H

// hoot
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Units.h>

// GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

class OsmMap;

using namespace geos::geom;

/**
 * This class works with four pass as long as distance is less than the four pass buffer.
 *
 * The input map can be in either a planar or geographic projection.
 */
class MergeNearbyNodes : public OsmMapOperation, public Serializable, public Boundable
{
public:
  static std::string className() { return "hoot::MergeNearbyNodes"; }

  static QString distanceKey() { return "merge.nearby.nodes.distance"; }

  MergeNearbyNodes(Meters distance = -1);

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  virtual string getClassName() const { return className(); }

  static void mergeNodes(boost::shared_ptr<OsmMap> map, Meters distance = -1);

  virtual void readObject(QDataStream& is);

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  virtual void writeObject(QDataStream& os) const;

protected:

  boost::shared_ptr<OsmMap> _map;
  Meters _distance;
  Envelope _bounds;
};

}

#endif // MERGENEARBYNODES_H
