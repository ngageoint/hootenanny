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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */


#include "MapProjector.h"

// Boost
using namespace boost;

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/visitors/CalculateBoundsVisitor.h>

// GEOS
#include <geos/geom/CoordinateFilter.h>

// Standard
#include <iomanip>
#include <iostream>

namespace hoot
{

shared_ptr<MapProjector> MapProjector::_theInstance;

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
    LOG_WARN("Source Point, x:" << inx << " y: " << iny);
    LOG_WARN("Source SRS: " << MapProjector::toWkt(_transform->GetSourceCS()));
    LOG_WARN("Target Point, x:" << c->x << " y: " << c->y);
    LOG_WARN("Target SRS: " << MapProjector::toWkt(_transform->GetTargetCS()));
    throw IllegalArgumentException(err);
  }
}


MapProjector& MapProjector::getInstance()
{
  if (!_theInstance.get())
  {
    _theInstance.reset(new MapProjector());
  }
  return *_theInstance;
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

shared_ptr<OGRSpatialReference> MapProjector::createAeacProjection(const OGREnvelope& env)
{
  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
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

vector< shared_ptr<OGRSpatialReference> > MapProjector::createAllPlanarProjections(
  const OGREnvelope& env)
{
  vector< shared_ptr<OGRSpatialReference> > result;

  double centerLat = (env.MaxY + env.MinY) / 2.0;
  double centerLon = (env.MaxX + env.MinX) / 2.0;
  double height = env.MaxY - env.MinY;
  double stdP1 = env.MinY + height * .25;
  double stdP2 = env.MinY + height * .75;

  try { result.push_back(createOrthographic(env)); } catch (HootException& e) { }

  if (ConfigOptions().getTestForceOrthographicProjection() == false)
  {
    try { result.push_back(createAeacProjection(env)); } catch (HootException& e) { }
    try { result.push_back(createSinusoidalProjection(env)); } catch (HootException& e) { }

    shared_ptr<OGRSpatialReference> mollweide(new OGRSpatialReference());
    if (mollweide->importFromEPSG(54009) == OGRERR_NONE)
    {
      result.push_back(mollweide);
    }

    shared_ptr<OGRSpatialReference> eckertVI(new OGRSpatialReference());
    if (eckertVI->importFromEPSG(53010) == OGRERR_NONE)
    {
      result.push_back(eckertVI);
    }

    shared_ptr<OGRSpatialReference> sphereBonne(new OGRSpatialReference());
    if (sphereBonne->importFromEPSG(53024) == OGRERR_NONE)
    {
      result.push_back(sphereBonne);
    }

    shared_ptr<OGRSpatialReference> customMercator(new OGRSpatialReference());
    if (customMercator->SetMercator(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customMercator);
    }

    shared_ptr<OGRSpatialReference> customBonne(new OGRSpatialReference());
    if (customBonne->SetBonne(M_PI_2, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customBonne);
    }

    // Lambert azimuthal equal-area projection
    shared_ptr<OGRSpatialReference> customLaea(new OGRSpatialReference());
    if (customLaea->SetLAEA(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLaea);
    }

    shared_ptr<OGRSpatialReference> customLcc1sp(new OGRSpatialReference());
    if (customLcc1sp->SetLCC1SP(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLcc1sp);
    }

    shared_ptr<OGRSpatialReference> customRobinson(new OGRSpatialReference());
    if (customRobinson->SetRobinson(centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customRobinson);
    }

    // custom transverse mercator
    shared_ptr<OGRSpatialReference> customTm(new OGRSpatialReference());
    if (customTm->SetTM(centerLat, centerLon, 1.0, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customTm);
    }

    // Polyconic
    shared_ptr<OGRSpatialReference> customPolyconic(new OGRSpatialReference());
    if (customPolyconic->SetPolyconic(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customPolyconic);
    }

    // Two Point Equidistant
    shared_ptr<OGRSpatialReference> customTped(new OGRSpatialReference());
    if (customTped->SetTPED(stdP1, centerLon, stdP2, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customTped);
    }

    // Equidistant Conic
    shared_ptr<OGRSpatialReference> customEc(new OGRSpatialReference());
    if (customEc->SetEC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customEc);
    }

    // Azimuthal Equidistant
    shared_ptr<OGRSpatialReference> customAe(new OGRSpatialReference());
    if (customAe->SetAE(centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customAe);
    }

    // Lambert Convformal Conic
    shared_ptr<OGRSpatialReference> customLcc(new OGRSpatialReference());
    if (customLcc->SetLCC(stdP1, stdP2, centerLat, centerLon, 0.0, 0.0) == OGRERR_NONE)
    {
      result.push_back(customLcc);
    }
  }

  return result;
}

shared_ptr<OGRSpatialReference> MapProjector::createOrthographic(const OGREnvelope& env)
{
  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating orthographic projection.");
  }
  return srs;
}

shared_ptr<OGRSpatialReference> MapProjector::createPlanarProjection(const OGREnvelope& env,
  Radians maxAngleError, Meters maxDistanceError, Meters testDistance, bool warnOnFail)
{
  vector< shared_ptr<OGRSpatialReference> > projs = createAllPlanarProjections(env);

  QString deg = QChar(0x00B0);

  if (projs.size() == 0)
  {
    throw HootException("No candidate planar projections are available.");
  }

  vector<PlanarTestResult> testResults;
  vector<PlanarTestResult> passingResults;

  // if the envelope has zero size then return an orthographic projection.
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
  QString errorMessage =
      "A projection within the specified error bounds could not be found. This is "
      "likely due to a very large bounds on the data. Please read "
      "'Hootenanny - Distance and Angle Errors Due to Reprojection' for more "
      "information. You may experience poor conflation performance as a result.";
  int bestIndex = -1;
  Log::WarningLevel level = Log::Debug;
  if (passingResults.size() > 0)
  {
    bestIndex = _findBestScore(passingResults);

    char* wkt = 0;
    projs[bestIndex]->exportToWkt(&wkt);
    LOG_DEBUG("Projection: " << wkt)
    OGRFree(wkt);
  }
  else if (warnOnFail == false)
  {
    level = Log::Warn;
  }
  else if (testResults.size() > 0)
  {
    LOG_WARN(errorMessage);
    bestIndex = _findBestScore(testResults);
    level = Log::Info;
  }

  LOG_LEVEL(level, "Planar projection has max distance error " << fixed << setprecision(2)
            << testResults[bestIndex].distanceError << "m "
            << "(" << testResults[bestIndex].distanceError / testDistance * 100.0 << "%) "
            << "and max angular error: " << toDegrees(testResults[bestIndex].angleError) << deg
            << " test distance: " << testDistance << "m");
  LOG_LEVEL(level, "Projection: " << toWkt(projs[bestIndex]));

  if (bestIndex == -1)
  {
    throw HootException(errorMessage);
  }

  return projs[bestIndex];
}


shared_ptr<OGRSpatialReference> MapProjector::createSinusoidalProjection(const OGREnvelope& env)
{
  double centerLon = (env.MaxX + env.MinX) / 2.0;

  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());

  if (srs->SetSinusoidal(centerLon, 0.0, 0.0) != OGRERR_NONE)
  {
    throw HootException("Error creating sinusoidal projection.");
  }

  return srs;
}

shared_ptr<OGRSpatialReference> MapProjector::createWgs84Projection()
{
  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());

  // EPSG 4326 = WGS84
  // if (srs->SetWellKnownGeogCS("WGS84") != OGRERR_NONE)
  if (srs->importFromEPSG(4326) != OGRERR_NONE)
  {
    throw HootException("Error creating EPSG:4326 projection.");
  }

  return srs;
}

bool MapProjector::_evaluateProjection(const OGREnvelope& env,
  shared_ptr<OGRSpatialReference> srs, Meters testDistance, Meters& maxDistanceError,
  Radians& maxAngleError)
{
  // Disable CPL error messages. They will be re-enabled when the DisableCplErrors object is
  // destructed.
  DisableCplErrors disableErrors;
  shared_ptr<OGRSpatialReference> wgs84 = MapProjector::createWgs84Projection();

  auto_ptr<OGRCoordinateTransformation> t(OGRCreateCoordinateTransformation(wgs84.get(),
                                                                            srs.get()));
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
  auto_ptr<geos::geom::Envelope> e(GeometryUtils::toEnvelope(env));

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

  return success;
}

bool MapProjector::_distanceLessThan(const MapProjector::PlanarTestResult& p1,
  const MapProjector::PlanarTestResult& p2)
{
  return p1.distanceError < p2.distanceError;
}

size_t MapProjector::_findBestResult(vector<PlanarTestResult>& results)
{
  vector<PlanarTestResult> orderByAngle = results;
  vector<PlanarTestResult> orderByDistance = results;
  sort(orderByDistance.begin(), orderByDistance.end(), _distanceLessThan);
  sort(orderByAngle.begin(), orderByAngle.end(), _angleLessThan);

  set<size_t> foundInDistance;
  for (size_t i = 0; i < orderByDistance.size(); ++i)
  {
    foundInDistance.insert(orderByDistance[i].i);
    if (foundInDistance.find(orderByAngle[i].i) != foundInDistance.end())
    {
      return orderByAngle[i].i;
    }
  }

  throw InternalErrorException("Unable to find a best result. Bug?");
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

Coordinate MapProjector::project(const Coordinate& c, shared_ptr<OGRSpatialReference> srs1,
                            shared_ptr<OGRSpatialReference> srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  Coordinate result;

  result.x = c.x;
  result.y = c.y;
  ReprojectCoordinateFilter(t).project(&result);

  OGRCoordinateTransformation::DestroyCT(t);

  return result;
}


void MapProjector::project(shared_ptr<OsmMap> map,
                                             shared_ptr<OGRSpatialReference> ref)
{
  shared_ptr<OGRSpatialReference> sourceSrs = map->getProjection();
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(sourceSrs.get(), ref.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  ReprojectCoordinateFilter rcf(t);

  int count = 0;
  const NodeMap& nodes = map->getNodeMap();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    Node* n = it->second.get();
    Coordinate c = n->toCoordinate();
    try
    {
      rcf.project(&c);
    }
    catch(IllegalArgumentException& e)
    {
      LOG_WARN("Failure projecting node: " << n->toString());
      throw e;
    }

    n->setX(c.x);
    n->setY(c.y);

    if (count % 1000 == 0 && Log::getInstance().isInfoEnabled())
    {
      cout << "Reprojecting " << count << " / " << nodes.size() << "         \r";
      cout.flush();
    }
    count++;
  }
  if (Log::getInstance().isInfoEnabled())
  {
    cout << endl;
  }

  map->setProjection(ref);

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::project(const shared_ptr<Geometry>& g,
  const shared_ptr<OGRSpatialReference>& srs1, const shared_ptr<OGRSpatialReference>& srs2)
{
  OGRCoordinateTransformation* t(OGRCreateCoordinateTransformation(srs1.get(), srs2.get()));

  if (t == 0)
  {
    throw HootException(QString("Error creating transformation object: ") + CPLGetLastErrorMsg());
  }

  ReprojectCoordinateFilter filter(t);
  g->apply_rw(&filter);

  OGRCoordinateTransformation::DestroyCT(t);
}

void MapProjector::projectToAeac(shared_ptr<OsmMap> map)
{
  shared_ptr<OGRSpatialReference> srs = getInstance().createAeacProjection(
    CalculateBoundsVisitor::getBounds(map));
  project(map, srs);
}

void MapProjector::projectToOrthographic(shared_ptr<OsmMap> map)
{
  OGREnvelope env = CalculateBoundsVisitor::getBounds(map);
  return projectToOrthographic(map, env);
}

void MapProjector::projectToOrthographic(shared_ptr<OsmMap> map, const OGREnvelope& env)
{
  MapProjector proj;
  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  double x = (env.MinX + env.MaxX) / 2.0;
  double y = (env.MinY + env.MaxY) / 2.0;
  if (srs->SetOrthographic(y, x, 0, 0) != OGRERR_NONE)
  {
    throw HootException("Error creating orthographic projection.");
  }
  proj.project(map, srs);
}

void MapProjector::projectToPlanar(shared_ptr<OsmMap> map)
{
  if (isGeographic(map))
  {
    OGREnvelope env = CalculateBoundsVisitor::getBounds(map);
    projectToPlanar(map, env);
  }
}

void MapProjector::projectToPlanar(shared_ptr<OsmMap> map, const OGREnvelope& env)
{
  if (map->getProjection()->IsProjected() == false)
  {
    shared_ptr<OGRSpatialReference> srs = getInstance().createPlanarProjection(env);
    project(map, srs);
  }
}

void MapProjector::projectToWgs84(shared_ptr<OsmMap> map)
{
  if (isGeographic(map) == false)
  {
    MapProjector proj;
    shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
    //srs->importFromEPSG(4326);
    srs->SetWellKnownGeogCS("WGS84");
    proj.project(map, srs);
  }
}

Coordinate MapProjector::projectFromWgs84(const Coordinate& c,
                                     shared_ptr<OGRSpatialReference> srs)
{
  shared_ptr<OGRSpatialReference> wgs84(new OGRSpatialReference());
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

