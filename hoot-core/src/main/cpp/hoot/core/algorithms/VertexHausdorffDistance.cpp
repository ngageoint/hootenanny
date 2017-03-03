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
#include "VertexHausdorffDistance.h"

// geos
#include <geos/geom/CoordinateFilter.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/util/TopologyException.h>

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/Log.h>

// Standard
#include <memory.h>
#include <limits>

namespace hoot
{
using namespace geos::geom;
using namespace std;

class VertexHausdorffFilter : public CoordinateFilter
{
public:

  VertexHausdorffFilter(const Geometry& g) :
  _g(g),
  _logWarnCount(0)
  {
    _distance = -1;
  }

  virtual void filter_ro(const Coordinate* c)
  {
    auto_ptr<Point> pt(GeometryFactory::getDefaultInstance()->createPoint(*c));
    double d = _g.distance(pt.get());

    if (d < 0)
    {
      if (_logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("d less than zero. " << _g.toString());
      }
      else if (_logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      _logWarnCount++;
    }

    if (d > _distance || _distance == -1)
    {
      _distance = d;
    }
  }

  double getDistance() const { return _distance; }

private:

  double _distance;
  const Geometry& _g;

  //this should be static, but there's no header file
  unsigned int _logWarnCount;
};

VertexHausdorffDistance::VertexHausdorffDistance(const Geometry& g1, const Geometry& g2)
{
  compute(g1, g2);
}

void VertexHausdorffDistance::compute(const Geometry& g1, const Geometry& g2)
{
  auto_ptr<Geometry> g(GeometryUtils::validateGeometry(&g1));
  VertexHausdorffFilter filter(*g);
  g2.apply_ro(&filter);
  _distance = filter.getDistance();
}

}
