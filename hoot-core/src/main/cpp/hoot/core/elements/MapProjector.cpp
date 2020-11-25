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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "MapProjector.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>
#include <hoot/core/util/StringUtils.h>

// GEOS
#include <geos/geom/CoordinateFilter.h>

// Standard
#include <iomanip>
#include <iostream>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int MapProjector::logWarnCount = 0;

class DisableCplErrors
{
public:

  CPLErrorHandler oldHandler;

  DisableCplErrors()
  {
    oldHandler = CPLSetErrorHandler(emptyErrorHandler);
  }

  ~DisableCplErrors()
  {
    // reset to default.
    CPLSetErrorHandler(oldHandler);
  }

  static void emptyErrorHandler(CPLErr, int, const char *) { }
};

ReprojectCoordinateFilter::ReprojectCoordinateFilter(OGRCoordinateTransformation* t)
{
  _transform = t;
}

void ReprojectCoordinateFilter::filter_rw(Coordinate* c) const
{
  project(c);
}

void ReprojectCoordinateFilter::project(Coordinate* c) const
{
  double inx = c->x;
  double iny = c->y;
  if (_transform->Transform(1, &c->x, &c->y) == FALSE)
  {
    QString err = QString("Error projecting point. Is the point outside of the projection's "
                          "bounds?");
    const int logWarnMessageLimit = ConfigOptions().getLogWarnMessageLimit();
    if (MapProjector::logWarnCount < logWarnMessageLimit)
    {
      LOG_WARN(err);
      LOG_TRACE("Source Point, x:" << inx << " y: " << iny);
      LOG_TRACE("Source SRS: " << MapProjector::toWkt(_transform->GetSourceCS()));
      LOG_TRACE("Target Point, x:" << c->x << " y: " << c->y);
      LOG_TRACE("Target SRS: " << MapProjector::toWkt(_transform->GetTargetCS()));
      MapProjector::logWarnCount++;
    }
    else if (MapProjector::logWarnCount == logWarnMessageLimit)
    {
      LOG_WARN(MapProjector::className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    throw IllegalArgumentException(err);
  }
}

MapProjector& MapProjector::getInstance()
{
  //  Local static singleton instance
  static MapProjector instance;
  return instance;
}

bool MapProjector::_angleLessThan(const MapProjector::PlanarTestResult& p1,
  const MapProjector::PlanarTestResult& p2)
{
  return p1.angleError < p2.angleError;
}

Radians MapProjector::_calculateAngle(Coordinate p1, Coordinate p2, Coordinate p3)
{
  Radians theta1 = atan2(p2.y - p1.y, p2.x - p1.x);
  Radians theta2 = atan2(p3.y - p1.y, p3.x - p1.x);
  return theta1 - theta2;
}

std::shared_ptr<OGRSpatialReference> MapProjector::createAeacProjection(const OGREnvelope& env)
{
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  double height = env.MaxY - env.MinY;
  double stdP1 = env.MinY + height * .25;
  double stdP2 = env.MinY + height * .75;
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;

  if (env.MinX == env.MaxX || env.MinY == env.MaxY)
  {
    throw HootException("Min and max bounds values cannot be the same.");
  }

  if (srs->SetACEA(stdP1, stdP2, y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating Albers equal area conic projection.");
  }

  return srs;
}

vector<std::shared_ptr<OGRSpatialReference>> MapProjector::createAllPlanarProjections(
  const OGREnvelope& env)
{
  vector<std::shared_ptr<OGRSpatialReference>> result;

  double centerLat = (env.MaxY + env.MinY) / 2.0;
  double centerLon = (env.MaxX + env.MinX) / 2.0;
  double height = env.MaxY - env.MinY;
  double stdP1 = env.MinY + height * .25;
  double stdP2 = env.MinY + height * .75;

  try { result.push_back(createOrthographic(env)); } catch (const HootException&) { }

  if (ConfigOptions().getTestForceOrthographicProjection() == false)
  {
    try { result.push_back(createAeacProjection(env)); } catch (const HootException&) { }
    try { result.push_back(createSinusoidalProjection(env)); } catch (const HootException&) { }

    std::shared_ptr<OGRSpatialReference> mollweide(new OGRSpatialReference());
    if (mollweide->importFromEPSG(54009) == OGRERR_NONE)
    {
      result.push_back(mollweide);
    }

    std::shared_ptr<OGRSpatialReference> eckertVI(new OGRSpatialReference());
    if (eckertVI->importFromEPSG(53010) == OGRERR_NONE)
    {
      result.push_back(eckertVI);
    }

    std::shared_ptr<OGRSpatialReference> sphereBonne(new OGRSpatialReference());
    if (sphereBonne->importFromEPSG(53024) == OGRERR_NONE)
    {
      result.push_back(sphereBonne);
    }

    std::shared_ptr<OGRSpatialReference> customMercator(new OGRSpatialReference());
    if (customMercator->SetMercator(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customMercator);
    }

    std::shared_ptr<OGRSpatialReference> customBonne(new OGRSpatialReference());
    if (customBonne->SetBonne(M_PI_2, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customBonne);
    }

    // Lambert azimuthal equal-area projection
    std::shared_ptr<OGRSpatialReference> customLaea(new OGRSpatialReference());
    if (customLaea->SetLAEA(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLaea);
    }

    std::shared_ptr<OGRSpatialReference> customLcc1sp(new OGRSpatialReference());
    if (customLcc1sp->SetLCC1SP(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLcc1sp);
    }

    std::shared_ptr<OGRSpatialReference> customRobinson(new OGRSpatialReference());
    if (customRobinson->SetRobinson(centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customRobinson);
    }

    // custom transverse mercator
    std::shared_ptr<OGRSpatialReference> customTm(new OGRSpatialReference());
    if (customTm->SetTM(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customTm);
    }

    // Polyconic
    std::shared_ptr<OGRSpatialReference> customPolyconic(new OGRSpatialReference());
    if (customPolyconic->SetPolyconic(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customPolyconic);
    }

    // Two Point Equidistant
    std::shared_ptr<OGRSpatialReference> customTped(new OGRSpatialReference());
    if (customTped->SetTPED(stdP1, centerLon, stdP2, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customTped);
    }

    // Equidistant Conic
    std::shared_ptr<OGRSpatialReference> customEc(new OGRSpatialReference());
    if (customEc->SetEC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customEc);
    }

    // Azimuthal Equidistant
    std::shared_ptr<OGRSpatialReference> customAe(new OGRSpatialReference());
    if (customAe->SetAE(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customAe);
    }

    // Lambert Convformal Conic
    std::shared_ptr<OGRSpatialReference> customLcc(new OGRSpatialReference());
    if (customLcc->SetLCC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLcc);
    }
  }

  return result;
}

std::shared_ptr<OGRSpatialReference> MapProjector::createOrthographic(const OGREnvelope& env)
{
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating orthographic projection.");
  }
  return srs;
}

std::shared_ptr<OGRSpatialReference> MapProjector::createOrthographic(double x, double y)
{
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating orthographic projection.");
  }
  return srs;
}

std::shared_ptr<OGRSpatialReference> MapProjector::createPlanarProjection(const OGREnvelope& env,
  Radians maxAngleError, Meters maxDistanceError, Meters testDistance, bool warnOnFail)
{
  LOG_TRACE("Selecting best planar projection...");

  vector<std::shared_ptr<OGRSpatialReference>> projs = createAllPlanarProjections(env);
  LOG_VART(projs.size());

  QString deg = QChar(0x00B0);

  if (projs.size() == 0)
  {
    throw HootException("No candidate planar projections are available.");
  }

  vector<PlanarTestResult> testResults;
  vector<PlanarTestResult> passingResults;

  // If the envelope has zero size, then return an orthographic projection.
  if (env.MaxX == env.MinX || env.MaxY == env.MinY)
  {
    return createOrthographic(env);
  }

  for (size_t i = 0; i < projs.size(); ++i)
  {
    PlanarTestResult tr;
    tr.i = i;
    if (_evaluateProjection(env, projs[i], testDistance, tr.distanceError, tr.angleError))
    {
      // create a score that is weighted by the user's threshold values.
      tr.score = tr.distanceError / maxDistanceError + tr.angleError / maxAngleError;
      LOG_VART(tr.score);
      testResults.push_back(tr);
      if (tr.distanceError <= maxDistanceError && tr.angleError <= maxAngleError)
      {
        passingResults.push_back(tr);
      }
    }
    else
    {
      tr.distanceError = numeric_limits<double>::max();
      tr.angleError = numeric_limits<double>::max();
      tr.score = numeric_limits<double>::max();
      testResults.push_back(tr);
    }
    LOG_TRACE("dis: " << tr.distanceError << "m angle: " << toDegrees(tr.angleError) << deg);
  }

  //  |<---                       80 cols                                         -->|
  const QString errorMessage =
    "A projection within the specified error bounds could not be found. This is "
    "likely due to a very large bounds on the data. Please read "
    "'Hootenanny - Distance and Angle Errors Due to Reprojection' for more "
    "information. You may experience poor conflation performance as a result.";
  int bestIndex = -1;
  Log::WarningLevel level = Log::Trace;
  LOG_VART(passingResults.size());
  LOG_VART(testResults.size());
  if (passingResults.size() > 0)
  {
    bestIndex = _findBestScore(passingResults);
    LOG_VART(bestIndex);
    LOG_VART(toWkt(projs[bestIndex]));
  }
  else if (warnOnFail == false)
  {
    level = Log::Warn;
  }
  else if (testResults.size() > 0)
  {
    LOG_WARN(errorMessage);
    bestIndex = _findBestScore(testResults);
    LOG_VART(bestIndex);
    level = Log::Info;
  }

  LOG_LEVEL(level, "Planar projection has max distance error " << fixed << setprecision(2)
            << testResults[bestIndex].distanceError << "m "
            << "(" << testResults[bestIndex].distanceError / testDistance * 100.0 << "%) "
            << "and max angular error: " << toDegrees(testResults[bestIndex].angleError) << deg
            << " test distance: " << testDistance << "m");
  LOG_LEVEL(level, "Projection: " << toWkt(projs[bestIndex]));
//  LOG_DEBUG("Planar projection has max distance error " << fixed << setprecision(2)
//            << testResults[bestIndex].distanceError << "m "
//            << "(" << testResults[bestIndex].distanceError / testDistance * 100.0 << "%) "
//            << "and max angular error: " << toDegrees(testResults[bestIndex].angleError) << deg
//            << " test distance: " << testDistance << "m");
//  LOG_DEBUG("Projection: " << toWkt(projs.at(bestIndex)));

  if (bestIndex == -1)
  {
    throw HootException(errorMessage);
  }

  LOG_VART(toWkt(projs[bestIndex].get()));
  return projs[bestIndex];
}

std::shared_ptr<OGRSpatialReference> MapProjector::createSinusoidalProjection(const OGREnvelope& env)
{
  double centerLon = (env.MaxX + env.MinX) / 2.0;
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  if (srs->SetSinusoidal(centerLon, 0.0, 0.0) != OGRERR_NONE)
  {
    throw HootException("Error creating sinusoidal projection.");
  }
  return srs;
}

std::shared_ptr<OGRSpatialReference> MapProjector::createWgs84Projection()
{
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  if (srs->importFromEPSG(4326) != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }
  return srs;
}

bool MapProjector::_evaluateProjection(const OGREnvelope& env,
  const std::shared_ptr<OGRSpatialReference>& srs, Meters testDistance, Meters& maxDistanceError,
  Radians& maxAngleError)
{
  // Disable CPL error messages. They will be re-enabled when the DisableCplErrors object is
  // destructed.
  DisableCplErrors disableErrors;
  std::shared_ptr<OGRSpatialReference> wgs84 = MapProjector::createWgs84Projection();

  std::shared_ptr<OGRCoordinateTransformation> t(
    OGRCreateCoordinateTransformation(wgs84.get(), srs.get()));
  if (t.get() == 0)
  {
    return false;
  }

  if (env.MaxX == env.MinX || env.MaxY == env.MinY)
  {
    throw HootException("Cannot evaluate a projection with an envelope of zero area.");
  }

  double width = env.MaxX - env.MinX;
  double height = env.MaxY - env.MinY;
  double stepSize = max(width, height) / 20.0;
  int stepsX = (width) / stepSize;
  int stepsY = (height) / stepSize;
  double stepSizeX = (width) / (double)stepsX;
  double stepSizeY = (height) / (double)stepsY;
  std::shared_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));

  bool success = true;

  maxDistanceError = 0.0;
  maxAngleError = 0.0;

  for (double x = env.MinX; x <= env.MaxX; x += stepSizeX)
  {
    for (double y = env.MinY; y <= env.MaxY; y += stepSizeY)
    {
      Coordinate c1(x, y);
      Coordinate p1 = c1;
      success &= t->TransformEx(1, &p1.x, &p1.y);

      if (!success)
      {
        return false;
      }

      Coordinate upc = GeometryUtils::calculateDestination(c1, 0.0, testDistance);
      Coordinate up = upc;
      success &= t->TransformEx(1, &up.x, &up.y);

      for (double bearing = 0.0; bearing < 360.0; bearing += 20.0)
      {
        Coordinate c2 = GeometryUtils::calculateDestination(c1, bearing, testDistance);
        Coordinate p2 = c2;
        success &= t->TransformEx(1, &p2.x, &p2.y);

        if (e->contains(c2))
        {
          Meters d = fabs(testDistance - p1.distance(p2));
          maxDistanceError = max(maxDistanceError, d);

          if (e->contains(upc))
          {
            Radians theta = _calculateAngle(p1, up, p2);
            Radians delta = WayHeading::deltaMagnitude(toRadians(bearing), theta);
            maxAngleError = max(maxAngleError, delta);
          }
        }
      }
    }
  }

  LOG_VART(maxDistanceError);
  LOG_VART(maxAngleError);
  LOG_VART(success);

  return success;
}

bool MapProjector::_distanceLessThan(const MapProjector::PlanarTestResult& p1,
  const MapProjector::PlanarTestResult& p2)
{
  return p1.distanceError < p2.distanceError;
}

size_t MapProjector::_findBestScore(vector<PlanarTestResult>& results)
{
  vector<PlanarTestResult> orderByScore = results;
  sort(orderByScore.begin(), orderByScore.end(), _scoreLessThan);
  return orderByScore[0].i;
}

bool MapProjector::isGeographic(const ConstElementProviderPtr& provider)
{
  return provider->getProjection()->IsGeographic();
}

Coordinate MapProjector::project(const Coordinate& c,
                                 const std::shared_ptr<OGRSpatialReference>& srs1,
                                 const std::shared_ptr<OGRSpatialReference>& srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  LOG_TRACE("Reprojecting map from: " << toWkt(srs1) << " to " << toWkt(srs2) << "...");

  Coordinate result;
  result.x = c.x;
  result.y = c.y;
  ReprojectCoordinateFilter(t).project(&result);

  OGRCoordinateTransformation::DestroyCT(t);

  return result;
}

void MapProjector::project(const std::shared_ptr<OsmMap>& map,
                           const std::shared_ptr<OGRSpatialReference>& ref)
{
  LOG_TRACE("Reprojecting map to: " << toWkt(ref) << "...");

  std::shared_ptr<OGRSpatialReference> sourceSrs = map->getProjection();
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(sourceSrs.get(), ref.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  ReprojectCoordinateFilter rcf(t);

  int count = 0;
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    Node* n = it->second.get();
    Coordinate c = n->toCoordinate();
    try
    {
      rcf.project(&c);
    }
    catch(const IllegalArgumentException&)
    {
      const int logWarnMessageLimit = ConfigOptions().getLogWarnMessageLimit();
      if (logWarnCount < logWarnMessageLimit)
      {
        LOG_WARN("Failure projecting node: " << n->toString());
      }
      else if (logWarnCount == logWarnMessageLimit)
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      throw;
    }

    n->setX(c.x);
    n->setY(c.y);

    if (count % 10000 == 0)
    {
      PROGRESS_TRACE(
        "Reprojecting " << StringUtils::formatLargeNumber(count) << " / " <<
        StringUtils::formatLargeNumber(nodes.size()));
    }
    count++;
  }

  map->setProjection(ref);

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::project(const std::shared_ptr<Geometry>& g,
  const std::shared_ptr<OGRSpatialReference>& srs1,
  const std::shared_ptr<OGRSpatialReference>& srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  LOG_TRACE("Reprojecting map from: " << toWkt(srs1) << " to " << toWkt(srs1) << "...");

  ReprojectCoordinateFilter filter(t);
  g->apply_rw(&filter);

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::projectToAeac(const std::shared_ptr<OsmMap>& map)
{
  std::shared_ptr<OGRSpatialReference> srs = getInstance().createAeacProjection(
    CalculateMapBoundsVisitor::getBounds(map));
  project(map, srs);
}

void MapProjector::projectToOrthographic(const std::shared_ptr<OsmMap>& map)
{
  OGREnvelope env = CalculateMapBoundsVisitor::getBounds(map);
  return projectToOrthographic(map, env);
}

void MapProjector::projectToOrthographic(const std::shared_ptr<OsmMap>& map, const OGREnvelope& env)
{
  LOG_TRACE("Projecting to orthographic...");
  MapProjector proj;
  std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating orthographic projection.");
  }
  proj.project(map, srs);
}

void MapProjector::projectToPlanar(const std::shared_ptr<OsmMap>& map)
{
  if (isGeographic(map))
  {
    OGREnvelope env = CalculateMapBoundsVisitor::getBounds(map);
    projectToPlanar(map, env);
  }
}

void MapProjector::projectToPlanar(const std::shared_ptr<OsmMap>& map, const OGREnvelope& env)
{
  if (map->getProjection()->IsProjected() == false)
  {
    LOG_TRACE("Projecting to planar...");
    LOG_VART(GeometryUtils::toEnvelope(env)->toString());
    project(map, getInstance().createPlanarProjection(env));
  }
}

void MapProjector::projectToWgs84(const std::shared_ptr<OsmMap>& map)
{
  if (isGeographic(map) == false)
  {
    MapProjector proj;
    std::shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
    srs->SetWellKnownGeogCS("WGS84");
    proj.project(map, srs);
  }
}

Coordinate MapProjector::projectFromWgs84(const Coordinate& c,
                                          const std::shared_ptr<OGRSpatialReference>& srs)
{
  LOG_TRACE("Projecting from WGS84...");
  std::shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
  wgs84->importFromEPSG(4326);
  return project(c, wgs84, srs);
}

bool MapProjector::_scoreLessThan(const MapProjector::PlanarTestResult& p1,
  const MapProjector::PlanarTestResult& p2)
{
  return p1.score < p2.score;
}

QString MapProjector::toWkt(OGRSpatialReference* srs)
{
  char* wkt = 0;
  srs->exportToWkt(&wkt);
  QString result = QString::fromUtf8(wkt);
  OGRFree(wkt);
  return result;
}

}

