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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "MapProjector.h"

// Hoot
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// GEOS
#include <geos/geom/CoordinateFilter.h>

// Standard
#include <iomanip>

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
      LOG_WARN("Source Point, x:" << inx << " y: " << iny);
      LOG_WARN("Source SRS: " << MapProjector::toWkt(_transform->GetSourceCS()));
      LOG_WARN("Target Point, x:" << c->x << " y: " << c->y);
      LOG_WARN("Target SRS: " << MapProjector::toWkt(_transform->GetTargetCS()));
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

bool MapProjector::_angleLessThan(
  const MapProjector::PlanarTestResult& p1, const MapProjector::PlanarTestResult& p2)
{
  return p1.angleError < p2.angleError;
}

Radians MapProjector::_calculateAngle(Coordinate p1, Coordinate p2, Coordinate p3) const
{
  Radians theta1 = atan2(p2.y - p1.y, p2.x - p1.x);
  Radians theta2 = atan2(p3.y - p1.y, p3.x - p1.x);
  return theta1 - theta2;
}

OGRSpatialReferencePtr MapProjector::createAeacProjection(const OGREnvelope& env)
{
  OGRSpatialReferencePtr srs(new OGRSpatialReference());
  srs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  double height = env.MaxY - env.MinY;
  double stdP1 = env.MinY + height * .25;
  double stdP2 = env.MinY + height * .75;
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;

  if (env.MinX == env.MaxX || env.MinY == env.MaxY)
    throw HootException("Min and max bounds values cannot be the same.");

  if (srs->SetACEA(stdP1, stdP2, y, x, 0, 0) != OGRERR_NONE)
    throw HootException("Error creating Albers equal area conic projection.");

  return srs;
}

vector<OGRSpatialReferencePtr> MapProjector::createAllPlanarProjections(const OGREnvelope& env) const
{
  vector<OGRSpatialReferencePtr> result;

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

    OGRSpatialReferencePtr mollweide(new OGRSpatialReference());
    mollweide->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (mollweide->SetFromUserInput("ESRI:54009") == OGRERR_NONE)
      result.push_back(mollweide);

    OGRSpatialReferencePtr eckertVI(new OGRSpatialReference());
    eckertVI->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (eckertVI->SetFromUserInput("ESRI:53010") == OGRERR_NONE)
      result.push_back(eckertVI);

    OGRSpatialReferencePtr sphereBonne(new OGRSpatialReference());
    sphereBonne->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (sphereBonne->SetFromUserInput("ESRI:53024") == OGRERR_NONE)
      result.push_back(sphereBonne);

    OGRSpatialReferencePtr customMercator(new OGRSpatialReference());
    customMercator->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customMercator->SetMercator(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customMercator);

    OGRSpatialReferencePtr customBonne(new OGRSpatialReference());
    customBonne->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customBonne->SetBonne(M_PI_2, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customBonne);

    // Lambert azimuthal equal-area projection
    OGRSpatialReferencePtr customLaea(new OGRSpatialReference());
    customLaea->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customLaea->SetLAEA(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customLaea);

    OGRSpatialReferencePtr customLcc1sp(new OGRSpatialReference());
    customLcc1sp->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customLcc1sp->SetLCC1SP(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customLcc1sp);

    OGRSpatialReferencePtr customRobinson(new OGRSpatialReference());
    customRobinson->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customRobinson->SetRobinson(centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customRobinson);

    // custom transverse mercator
    OGRSpatialReferencePtr customTm(new OGRSpatialReference());
    customTm->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customTm->SetTM(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customTm);

    // Polyconic
    OGRSpatialReferencePtr customPolyconic(new OGRSpatialReference());
    customPolyconic->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customPolyconic->SetPolyconic(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customPolyconic);

    // Two Point Equidistant
    OGRSpatialReferencePtr customTped(new OGRSpatialReference());
    customTped->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customTped->SetTPED(stdP1, centerLon, stdP2, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customTped);

    // Equidistant Conic
    OGRSpatialReferencePtr customEc(new OGRSpatialReference());
    customEc->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customEc->SetEC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customEc);

    // Azimuthal Equidistant
    OGRSpatialReferencePtr customAe(new OGRSpatialReference());
    customAe->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customAe->SetAE(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customAe);

    // Lambert Convformal Conic
    OGRSpatialReferencePtr customLcc(new OGRSpatialReference());
    customLcc->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
    if (customLcc->SetLCC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
      result.push_back(customLcc);
  }

  return result;
}

OGRSpatialReferencePtr MapProjector::createOrthographic(const OGREnvelope& env)
{
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  return createOrthographic(x, y);
}

OGRSpatialReferencePtr MapProjector::createOrthographic(double x, double y)
{
  OGRSpatialReferencePtr srs(new OGRSpatialReference());
  srs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
    throw HootException("Error creating orthographic projection.");
  return srs;
}

OGRSpatialReferencePtr MapProjector::createPlanarProjection(const OGREnvelope& env, Radians maxAngleError,
                                                            Meters maxDistanceError, Meters testDistance,
                                                            bool warnOnFail) const
{
  LOG_TRACE("Selecting best planar projection...");

  //  If the envelope is undefined, return an orthographic projection around (0, 0)
  if (!env.IsInit())
    return createOrthographic(0, 0);

  // If the envelope has zero size, then return an orthographic projection.
  if (env.MaxX == env.MinX || env.MaxY == env.MinY)
    return createOrthographic(env);

  vector<OGRSpatialReferencePtr> projs = createAllPlanarProjections(env);
  LOG_VART(projs.size());

  const QString deg = QChar(0x00B0);

  if (projs.empty())
    throw HootException("No candidate planar projections are available.");

  vector<PlanarTestResult> testResults;
  vector<PlanarTestResult> passingResults;

  size_t index = 0;
  for (const auto& projection : projs)
  {
    PlanarTestResult tr;
    tr.i = index;
    if (_evaluateProjection(env, projection, testDistance, tr.distanceError, tr.angleError))
    {
      // create a score that is weighted by the user's threshold values.
      tr.score = tr.distanceError / maxDistanceError + tr.angleError / maxAngleError;
      LOG_VART(tr.score);
      testResults.push_back(tr);
      if (tr.distanceError <= maxDistanceError && tr.angleError <= maxAngleError)
        passingResults.push_back(tr);
    }
    else
    {
      tr.distanceError = numeric_limits<double>::max();
      tr.angleError = numeric_limits<double>::max();
      tr.score = numeric_limits<double>::max();
      testResults.push_back(tr);
    }
    LOG_TRACE("dis: " << tr.distanceError << "m angle: " << toDegrees(tr.angleError) << deg);
    index++;
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
  if (!passingResults.empty())
  {
    bestIndex = static_cast<int>(_findBestScore(passingResults));
    LOG_VART(bestIndex);
    LOG_VART(toWkt(projs[bestIndex]));
  }
  else if (warnOnFail == false)
    level = Log::Warn;
  else if (!testResults.empty())
  {
    LOG_WARN(errorMessage);
    bestIndex = static_cast<int>(_findBestScore(testResults));
    LOG_VART(bestIndex);
    level = Log::Info;
  }

  LOG_LEVEL(level, "Planar projection has max distance error " << fixed << setprecision(2)
            << testResults[bestIndex].distanceError << "m "
            << "(" << testResults[bestIndex].distanceError / testDistance * 100.0 << "%) "
            << "and max angular error: " << toDegrees(testResults[bestIndex].angleError) << deg
            << " test distance: " << testDistance << "m");
  LOG_LEVEL(level, "Projection: " << toWkt(projs[bestIndex]));

  if (bestIndex == -1)
    throw HootException(errorMessage);

  LOG_VART(toWkt(projs[bestIndex].get()));
  return projs[bestIndex];
}

OGRSpatialReferencePtr MapProjector::createSinusoidalProjection(const OGREnvelope& env)
{
  double centerLon = (env.MaxX + env.MinX) / 2.0;
  OGRSpatialReferencePtr srs(new OGRSpatialReference());
  srs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  if (srs->SetSinusoidal(centerLon, 0.0, 0.0) != OGRERR_NONE)
  {
    throw HootException("Error creating sinusoidal projection.");
  }
  return srs;
}

OGRSpatialReferencePtr MapProjector::createWgs84Projection()
{
  OGRSpatialReferencePtr srs(new OGRSpatialReference());
  srs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  if (srs->importFromEPSG(4326) != OGRERR_NONE)
    throw HootException("Error creating EPSG:4326 projection.");
  return srs;
}

bool MapProjector::_evaluateProjection(const OGREnvelope& env, const OGRSpatialReferencePtr& srs,
                                       Meters testDistance, Meters& maxDistanceError,
                                       Radians& maxAngleError) const
{
  // Disable CPL error messages. They will be re-enabled when the DisableCplErrors object is
  // destructed.
  DisableCplErrors disableErrors;
  OGRSpatialReferencePtr wgs84 = MapProjector::createWgs84Projection();

  std::shared_ptr<OGRCoordinateTransformation> t(OGRCreateCoordinateTransformation(wgs84.get(), srs.get()));
  if (t.get() == nullptr)
    return false;

  if (env.MaxX == env.MinX || env.MaxY == env.MinY)
    throw HootException("Cannot evaluate a projection with an envelope of zero area.");

  double width = env.MaxX - env.MinX;
  double height = env.MaxY - env.MinY;
  double stepSize = max(width, height) / 20.0;
  double stepsX = width / stepSize;
  double stepsY = height / stepSize;
  double stepSizeX = width / stepsX;
  double stepSizeY = height / stepsY;
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
      success = t->Transform(1, &p1.x, &p1.y) && success;

      if (!success)
        return false;

      Coordinate upc = GeometryUtils::calculateDestination(c1, 0.0, testDistance);
      Coordinate up = upc;
      success = t->Transform(1, &up.x, &up.y) && success;

      for (double bearing = 0.0; bearing < 360.0; bearing += 20.0)
      {
        Coordinate c2 = GeometryUtils::calculateDestination(c1, bearing, testDistance);
        Coordinate p2 = c2;
        success = t->Transform(1, &p2.x, &p2.y) && success;

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

size_t MapProjector::_findBestScore(const vector<PlanarTestResult>& results) const
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
                                 const OGRSpatialReferencePtr& srs1,
                                 const OGRSpatialReferencePtr& srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == nullptr)
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());

  LOG_TRACE("Reprojecting map from: " << toWkt(srs1) << " to " << toWkt(srs2) << "...");

  Coordinate result;
  result.x = c.x;
  result.y = c.y;
  ReprojectCoordinateFilter(t).project(&result);

  OGRCoordinateTransformation::DestroyCT(t);

  return result;
}

void MapProjector::project(const std::shared_ptr<OsmMap>& map,
                           const OGRSpatialReferencePtr& ref)
{
  LOG_TRACE("Reprojecting map to: " << toWkt(ref) << "...");

  OGRSpatialReferencePtr sourceSrs = map->getProjection();
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(sourceSrs.get(), ref.get()));

  if (t == nullptr)
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());

  ReprojectCoordinateFilter rcf(t);

  int count = 0;
  const NodeMap& nodes = map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
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
        "Reprojecting " << StringUtils::formatLargeNumber(count) << " of " <<
        StringUtils::formatLargeNumber(nodes.size()));
    }
    count++;
  }

  map->setProjection(ref);

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::project(const std::shared_ptr<Geometry>& g,
                           const OGRSpatialReferencePtr& srs1,
                           const OGRSpatialReferencePtr& srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == nullptr)
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());

  LOG_TRACE("Reprojecting map from: " << toWkt(srs1) << " to " << toWkt(srs1) << "...");

  ReprojectCoordinateFilter filter(t);
  g->apply_rw(&filter);
  //  NOTE: It is very important that the geometry is marked as changed after if it reprojected
  g->geometryChanged();

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::projectToAeac(const std::shared_ptr<OsmMap>& map)
{
  OGRSpatialReferencePtr srs = getInstance().createAeacProjection(CalculateMapBoundsVisitor::getBounds(map));
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
  OGRSpatialReferencePtr srs(new OGRSpatialReference());
  srs->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
    throw HootException("Error creating orthographic projection.");
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
    OGRSpatialReferencePtr srs = MapProjector::createWgs84Projection();
    proj.project(map, srs);
  }
}

bool MapProjector::_scoreLessThan(const MapProjector::PlanarTestResult& p1,
                                  const MapProjector::PlanarTestResult& p2)
{
  return p1.score < p2.score;
}

QString MapProjector::toWkt(const OGRSpatialReference* srs)
{
  char* wkt = nullptr;
  srs->exportToWkt(&wkt);
  QString result = QString::fromUtf8(wkt);
  CPLFree(wkt);
  return result;
}

}
