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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "GeometryUtils.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

// hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/util/Float.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QString>
#include <QStringList>

// Standard
#include <stdint.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int GeometryUtils::logWarnCount = 0;

union DoubleCast
{
  uint64_t v;
  double d;
};

double GeometryUtils::MEAN_EARTH_RADIUS = 6371009;

Coordinate GeometryUtils::calculateDestination(const Coordinate& start, Degrees bearing,
  Meters d)
{
  Radians theta = toRadians(bearing);
  Radians lambda1 = toRadians(start.x);
  Radians phi1 = toRadians(start.y);
  Meters R = MEAN_EARTH_RADIUS;

  Radians phi2 = asin(sin(phi1) * cos(d / R) + cos(phi1) * sin(d / R) * cos(theta));
  Radians lambda2 = lambda1 + atan2(sin(theta) * sin(d / R) * cos(phi1),
    cos(d / R) - sin(phi1) * sin(phi2));

  Coordinate result;
  result.x = toDegrees(lambda2);
  result.y = toDegrees(phi2);

  return result;
}

double GeometryUtils::haversine(const Coordinate &c1, const Coordinate &c2)
{
  if (c1 == c2)
  {
    return 0.0;
  }

  // use long double instead of double. With GCC this gives 128 bit double and can discern down
  // to about 2mm at the equator using haversine. Using a regular double only gives ~9cm.
  Float128 c1LatAsRadians = (Float128)c1.y / 180.0 * M_PI;
  Float128 c1LngAsRadians = (Float128)c1.x / 180.0 * M_PI;
  Float128 c2LatAsRadians = (Float128)c2.y / 180.0 * M_PI;
  Float128 c2LngAsRadians = (Float128)c2.x / 180.0 * M_PI;

  Float128 v = cosl(c1LatAsRadians) * cosl(c2LatAsRadians)
      * cosl(c1LngAsRadians - c2LngAsRadians) + sinl(c1LatAsRadians)
      * sinl(c2LatAsRadians);
  if (v > 1.0)
  {
    v = 1.0;
  }
  Float128 dsigma = acosl(v);
  return dsigma * MEAN_EARTH_RADIUS;
}

bool GeometryUtils::highPrecisionHaversine()
{
#if HOOT_HAVE_FLOAT128
  return true;
#else
  return false;
#endif
}

Envelope* GeometryUtils::toEnvelope(const OGREnvelope& e)
{
  return new Envelope(e.MinX, e.MaxX, e.MinY, e.MaxY);
}

Envelope* GeometryUtils::toEnvelopeFromHex(const QString& s)
{
  QStringList aEnvStr = s.split(",");
  if (aEnvStr.size() != 4)
  {
    throw HootException("Invalid Envelope: Expected 4 bounds.");
  }

  double b[4];
  for (int j = 0; j < 4; j++)
  {
    bool ok;
    DoubleCast f;
    f.v = aEnvStr.at(j).toULongLong(&ok, 16);
    b[j] = f.d;
    if (ok == false)
    {
      throw HootException("Invalid Envelope: Unable to parse double.");
    }
  }
  return new Envelope(b[0], b[1], b[2], b[3]);
}

QString GeometryUtils::toHexString(const Envelope& e)
{
  DoubleCast minx, maxx, miny, maxy;
  minx.d = e.getMinX();
  maxx.d = e.getMaxX();
  miny.d = e.getMinY();
  maxy.d = e.getMaxY();
  return QString("%1,%2,%3,%4").
      arg(minx.v, 0, 16).
      arg(maxx.v, 0, 16).
      arg(miny.v, 0, 16).
      arg(maxy.v, 0, 16);
}

OGREnvelope* GeometryUtils::toOGREnvelope(const geos::geom::Envelope& e)
{
  OGREnvelope* result = new OGREnvelope();
  result->MinX = e.getMinX();
  result->MaxX = e.getMaxX();
  result->MinY = e.getMinY();
  result->MaxY = e.getMaxY();
  return result;
}

QString GeometryUtils::toString(const Envelope& e)
{
  const int precision = ConfigOptions().getWriterPrecision();
  return QString("%1,%2,%3,%4").
      arg(e.getMinX(), 0, 'f', precision).
      arg(e.getMaxX(), 0, 'f', precision).
      arg(e.getMinY(), 0, 'f', precision).
      arg(e.getMaxY(), 0, 'f', precision);
}

QString GeometryUtils::toConfigString(const Envelope& e)
{
  const int precision = ConfigOptions().getWriterPrecision();
  return QString("%1,%2,%3,%4").
      arg(e.getMinX(), 0, 'f', precision).
      arg(e.getMinY(), 0, 'f', precision).
      arg(e.getMaxX(), 0, 'f', precision).
      arg(e.getMaxY(), 0, 'f', precision);
}

Geometry* GeometryUtils::validateGeometry(const Geometry* g)
{
  switch (g->getGeometryTypeId())
  {
  case GEOS_POINT:
    return GeometryFactory::getDefaultInstance()->createGeometry(g);
  case GEOS_GEOMETRYCOLLECTION:
  case GEOS_MULTIPOINT:
  case GEOS_MULTILINESTRING:
  case GEOS_MULTIPOLYGON:
    return validateGeometryCollection(dynamic_cast<const GeometryCollection*>(g));
  case GEOS_LINESTRING:
    return validateLineString(dynamic_cast<const LineString*>(g));
  case GEOS_POLYGON:
    return validatePolygon(dynamic_cast<const Polygon*>(g));
  default:
    const int logWarnMessageLimit = ConfigOptions().getLogWarnMessageLimit();
    if (logWarnCount < logWarnMessageLimit)
    {
      LOG_WARN("Got an unrecognized geometry. " << g->getGeometryTypeId());
    }
    else if (logWarnCount == logWarnMessageLimit)
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
    return GeometryFactory::getDefaultInstance()->createGeometry(g);
  }
}

Geometry* GeometryUtils::validateGeometryCollection(
  const GeometryCollection *gc)
{
  Geometry* result = GeometryFactory::getDefaultInstance()->createEmptyGeometry();

  for (size_t i = 0; i < gc->getNumGeometries(); i++)
  {
    std::shared_ptr<Geometry> geometry(validateGeometry(gc->getGeometryN(i)));
    Geometry* tmp = result->Union(geometry.get());
    delete result;
    result = tmp;
  }
  return result;
}

Geometry* GeometryUtils::validateLineString(const LineString* ls)
{
  // See JTS Secrets for details: http://2007.foss4g.org/presentations/view.php?abstract_id=115
  std::shared_ptr<Point> p(
    GeometryFactory::getDefaultInstance()->createPoint(ls->getCoordinateN(0)));
  return ls->Union(p.get());
}

LinearRing* GeometryUtils::validateLinearRing(const LinearRing* lr)
{
  return dynamic_cast<LinearRing*>(GeometryFactory::getDefaultInstance()->createGeometry(lr));
}

Geometry* GeometryUtils::validatePolygon(const Polygon* p)
{
  Geometry* result;

  // if the input polygon isn't valid.
  if (p->isValid() == false)
  {
    std::shared_ptr<Geometry> tmp;
    // buffer it by zero to attempt to fix topology errors and store the result in an
    // std::shared_ptr that will self delete.
    tmp.reset(p->buffer(0));
    // run the new geometry through the whole routine again just in case the type changed.
    result = validateGeometry(tmp.get());
  }
  else if (p->getArea() == 0.0)
  {
    result = GeometryFactory::getDefaultInstance()->createEmptyGeometry();
  }
  else
  {
    const LineString* oldShell = p->getExteriorRing();
    std::shared_ptr<LinearRing> oldLinearRing(
      GeometryFactory::getDefaultInstance()->createLinearRing(oldShell->getCoordinates()));
    LinearRing* shell = validateLinearRing(oldLinearRing.get());
    std::vector<Geometry*>* holes = new vector<Geometry*>();
    holes->reserve(p->getNumInteriorRing());

    for (size_t i = 0; i < p->getNumInteriorRing(); i++)
    {
      const LineString* ls = p->getInteriorRingN(i);

      if (ls->getGeometryTypeId() == GEOS_LINEARRING)
      {
        const LinearRing* lr = dynamic_cast<const LinearRing*>(ls);

        // keep any holes with three or more points.
        if (lr->getNumPoints() >= 3)
        {
          holes->push_back(validateLinearRing(lr));
        }
      }
      else
      {
        LOG_TRACE("Why isn't it a linear ring?");
        holes->push_back(validateGeometry(ls));
      }
    }

    result = GeometryFactory::getDefaultInstance()->createPolygon(shell, holes);
  }

  return result;
}

Envelope GeometryUtils::envelopeFromConfigString(const QString& boundsStr)
{
  LOG_VART(boundsStr);
  if (boundsStr.trimmed().isEmpty())
  {
    return Envelope();
  }

  const QString errorMsg = "Invalid envelope string: " + boundsStr;
  const QRegExp boundsRegEx("(-*\\d+\\.*\\d*,){3}-*\\d+\\.*\\d*");
  if (!boundsRegEx.exactMatch(boundsStr))
  {
    throw HootException(errorMsg);
  }
  const QStringList boundsParts = boundsStr.split(",");
  assert(boundsParts.size() == 4);
  if ((boundsParts.at(2).toDouble() <= boundsParts.at(0).toDouble()) ||
       boundsParts.at(3).toDouble() <= boundsParts.at(1).toDouble())
  {
    throw HootException(errorMsg);
  }
  return
    Envelope(boundsParts.at(0).toDouble(), boundsParts.at(2).toDouble(),
      boundsParts.at(1).toDouble(), boundsParts.at(3).toDouble());
}

QString GeometryUtils::envelopeToConfigString(const Envelope& bounds)
{
  return QString::number(bounds.getMinX()) + "," +
    QString::number(bounds.getMinY()) + "," +
    QString::number(bounds.getMaxX()) + "," +
    QString::number(bounds.getMaxY());
}

OsmMapPtr GeometryUtils::createMapFromBounds(const geos::geom::Envelope& bounds)
{
  OsmMapPtr boundaryMap(new OsmMap());
  createBoundsInMap(boundaryMap, bounds);
  return boundaryMap;
}

ElementId GeometryUtils::createBoundsInMap(const OsmMapPtr& map, const geos::geom::Envelope& bounds)
{
  NodePtr lowerLeft(
    new Node(
      Status::Unknown1,
      map->createNextNodeId(),
      geos::geom::Coordinate(bounds.getMinX(), bounds.getMinY())));
  map->addNode(lowerLeft);
  NodePtr upperRight(
    new Node(
      Status::Unknown1,
      map->createNextNodeId(),
      geos::geom::Coordinate(bounds.getMaxX(), bounds.getMaxY())));
  map->addNode(upperRight);
  NodePtr upperLeft(
    new Node(
      Status::Unknown1,
      map->createNextNodeId(),
      geos::geom::Coordinate(bounds.getMinX(), bounds.getMaxY())));
  map->addNode(upperLeft);
  NodePtr lowerRight(
    new Node(
      Status::Unknown1,
      map->createNextNodeId(),
      geos::geom::Coordinate(bounds.getMaxX(), bounds.getMinY())));
  map->addNode(lowerRight);

  WayPtr bbox(new Way(Status::Unknown1, map->createNextWayId()));
  bbox->addNode(lowerLeft->getId());
  bbox->addNode(upperLeft->getId());
  bbox->addNode(upperRight->getId());
  bbox->addNode(lowerRight->getId());
  bbox->addNode(lowerLeft->getId());
  map->addWay(bbox);

  return bbox->getElementId();
}

QString GeometryUtils::geometryTypeIdToString(const geos::geom::GeometryTypeId& geometryTypeId)
{
  switch (geometryTypeId)
  {
    case GEOS_POINT:
      return "point";
    case GEOS_MULTIPOINT:
      return "multipoint";
    case GEOS_LINESTRING:
      return "linestring";
    case GEOS_LINEARRING:
      return "linearring";
    case GEOS_MULTILINESTRING:
      return "multilinestring";
    case GEOS_POLYGON:
      return "polygon";
    case GEOS_MULTIPOLYGON:
      return "multipolygon";
    case GEOS_GEOMETRYCOLLECTION:
      return "geometrycollection";
    default:
      return "unknown";
  }
}

}
