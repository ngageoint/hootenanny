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
 * @copyright Copyright (C) 2023 Maxar (http://www.maxar.com/)
 */

#include "ParallelApiJob.h"

//  Geos
#include <geos/geom/GeometryFactory.h>

namespace hoot
{

bool BoundedParallelApiJob::_boundsIsPolygon = false;
std::shared_ptr<geos::geom::Geometry> BoundedParallelApiJob::_boundingPoly;

std::vector<ParallelApiJobPtr> BoundedParallelApiJob::CreateInitialJob()
{
  //  Split the envelope if it is bigger than the prescribed max
  int lon_div = 1;
  int lat_div = 1;
  if (_envelope.getWidth() > _coordGridSize)
    lon_div = (int)std::ceil(_envelope.getWidth() / _coordGridSize);
  if (_envelope.getHeight() > _coordGridSize)
    lat_div = (int)std::ceil(_envelope.getHeight() / _coordGridSize);
  //  Record the number of envelopes to start with
  std::vector<ParallelApiJobPtr> jobs;
  //  Setup the envelopes to query in a grid
  for (int i = 0; i < lon_div; ++i)
  {
    double lon = _envelope.getMinX() + _coordGridSize * i;
    double lon2 = std::min(lon + _coordGridSize, _envelope.getMaxX());
    for (int j = 0; j < lat_div; ++j)
    {
      double lat = _envelope.getMinY() + _coordGridSize * j;
      double lat2 = std::min(lat + _coordGridSize, _envelope.getMaxY());
      //  Start at the upper right corner and create boxes left to right, top to bottom
      jobs.push_back(std::make_shared<BoundedParallelApiJob>(_query, geos::geom::Envelope(lon, lon2, lat, lat2), _coordGridSize));
    }
  }
  return jobs;
}

std::vector<ParallelApiJobPtr> BoundedParallelApiJob::SplitJob()
{
  //  Split the envelope in quarters and push them all back on the queue
  double lon1 = _envelope.getMinX();
  double lon2 = _envelope.getMinX() + _envelope.getWidth() / 2.0f;
  double lon3 = _envelope.getMaxX();

  double lat1 = _envelope.getMinY();
  double lat2 = _envelope.getMinY() + _envelope.getHeight() / 2.0f;
  double lat3 = _envelope.getMaxY();
  //  Create the four envelopes
  std::vector<geos::geom::Envelope> envelopes(
    {
      geos::geom::Envelope(lon1, lon2, lat1, lat2),
      geos::geom::Envelope(lon2, lon3, lat1, lat2),
      geos::geom::Envelope(lon1, lon2, lat2, lat3),
      geos::geom::Envelope(lon2, lon3, lat2, lat3)
    });

  std::vector<ParallelApiJobPtr> splits;
  if (_boundsIsPolygon)
  {
    //  Split the envelope and remove any sub-envelopes that don't intersect the original polygon
    const geos::geom::GeometryFactory* factory = geos::geom::GeometryFactory::getDefaultInstance();
    //  Convert all of the envelopes
    for (const auto& e : envelopes)
    {
      //  Convert the envelope to a geometry
      std::unique_ptr<geos::geom::Geometry> g = factory->toGeometry(&e);
      //  Don't push envelopes that don't intersect the original geometry
      if (g->intersects(_boundingPoly.get()))
        splits.push_back(std::make_shared<BoundedParallelApiJob>(_query, e, _coordGridSize));
    }
  }
  else
  {
    //  Split the boxes into quads and push them all onto the queue
    for (const auto& e : envelopes)
      splits.push_back(std::make_shared<BoundedParallelApiJob>(_query, e, _coordGridSize));
  }
  return splits;
}

std::vector<ParallelApiJobPtr> QueryParallelApiJob::CreateInitialJob()
{
  std::vector<ParallelApiJobPtr> jobs;
  jobs.push_back(this->shared_from_this());
  return jobs;
}

std::vector<ParallelApiJobPtr> QueryParallelApiJob::SplitJob()
{
  std::vector<ParallelApiJobPtr> splits;
  splits.push_back(this->shared_from_this());
  return splits;
}

}
