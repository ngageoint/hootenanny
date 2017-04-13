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
#ifndef BIGPERTYOP_H
#define BIGPERTYOP_H

// Hoot
#include <hoot/core/ops/Boundable.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/io/Serializable.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/perty/RngConsumer.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class OsmMap;

/**
 * Modifies geometries by moving them by a normally distributed random distance. The distance they
 * are moved is determined by the sigma parameter. All operations are performed in geodesic space.
 *
 * When #2847 is done then this can likely be removed.
 */
class BigPertyOp :
    public OsmMapOperation,
    public Serializable,
    public Boundable,
    public Configurable,
    public RngConsumer
{
public:

  static std::string className() { return "hoot::BigPertyOp"; }

  BigPertyOp();

  virtual ~BigPertyOp();

  virtual void apply(boost::shared_ptr<OsmMap>& map);

  /**
   * Returns the name of the class. Typically just returns the result of className().
   */
  virtual string getClassName() const { return className(); }

  /**
   * Reads the information for this object from the stream.
   */
  virtual void readObject(QDataStream& is);

  virtual void setBounds(const geos::geom::Envelope& bounds) { _bounds = bounds; }

  void setConfiguration(const Settings &conf);

  void setMaxDistance(Meters maxDistance);

  virtual void setRng(boost::minstd_rand& rng) { _rng = &rng; }

  void setSigma(Meters sigma) { _sigma = sigma; }

  /**
   * Writes the data for this object to the stream. This should _not_ include uniquely identifying
   * class information. E.g. the class name.
   */
  virtual void writeObject(QDataStream& os) const;

private:
  geos::geom::Envelope _bounds;
  auto_ptr<boost::minstd_rand> _rngAuto;
  boost::minstd_rand* _rng;
  Meters _sigma;
  Meters _maxDistance;

  void _createOffset(Radians& bearing, Meters& distance);
};

}

#endif // BIGPERTYOP_H
