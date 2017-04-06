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
#include "BigPertyOp.h"

// boost
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/OsmMap.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, BigPertyOp)

BigPertyOp::BigPertyOp()
{
  _rngAuto.reset(new boost::minstd_rand);
  _rng = _rngAuto.get();
  setConfiguration(conf());
}

BigPertyOp::~BigPertyOp()
{

}

void BigPertyOp::apply(boost::shared_ptr<OsmMap>& map)
{
  MapProjector::projectToWgs84(map);

  const QString pertiedStr(MetadataTags::HootPertied());
  const QString trueStr("true");

  const NodeMap& nm = map->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    NodePtr n = it->second;
    Coordinate nc = n->toCoordinate();

    if ((_bounds.isNull() || _bounds.contains(nc)) &&
        n->getTags().isTrue(pertiedStr) == false)
    {
      Radians bearing;
      Meters distance;
      _createOffset(bearing, distance);

      Coordinate c = GeometryUtils::calculateDestination(n->toCoordinate(), toDegrees(bearing),
        distance);
      n->setX(c.x);
      n->setY(c.y);

      if (_bounds.isNull() == false)
      {
        n->getTags().set(pertiedStr, trueStr);
      }
    }
  }
}

void BigPertyOp::_createOffset(Radians& bearing, Meters& distance)
{
  boost::normal_distribution<> nd;
  boost::variate_generator<boost::minstd_rand&, boost::normal_distribution<> > N(*_rng, nd);

  bool done = false;
  int count = 0;
  while (!done)
  {
    if (count > 100)
    {
      throw InternalErrorException("We haven't found a valid distance after 100 iterations. "
                                   "This is extraordinarily unlikely and likely points to an "
                                   "internal error.");
    }

    Meters dx = N() * _sigma;
    Meters dy = N() * _sigma;

    bearing = atan2(dy, dx);
    distance = sqrt(dx * dx + dy * dy);

    count++;
    if (_maxDistance < 0 || distance <= _maxDistance)
    {
      done = true;
    }
  }
}

void BigPertyOp::readObject(QDataStream& is)
{
  Meters s;
  is >> s;
  setSigma(s);
  is >> s;
  setMaxDistance(s);
}

void BigPertyOp::setConfiguration(const Settings &conf)
{
  ConfigOptions configOptions(conf);
  setSigma(configOptions.getBigPertyOpSigma());
  setMaxDistance(configOptions.getBigPertyOpMaxDistance());
}

void BigPertyOp::setMaxDistance(Meters maxDistance)
{
  if (maxDistance < _sigma && maxDistance != -1)
  {
    QString err = QString("Max distance must be -1 or greater than or equal to sigma "
                          "(max distance: %1, sigma: %2)").arg(maxDistance).arg(_sigma);
    throw IllegalArgumentException(err);
  }
  _maxDistance = maxDistance;
}

void BigPertyOp::writeObject(QDataStream& os) const
{
  os << _sigma;
  os << _maxDistance;
}

}
