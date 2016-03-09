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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "GraphComparator.h"

#include "DirectedGraph.h"
#include "ShortestPath.h"

// GEOS
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/operation/distance/DistanceOp.h>
using namespace geos::operation::distance;

// Hoot
#include <hoot/core/GeometryPainter.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/algorithms/WaySplitter.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/conflate/splitter/IntersectionSplitter.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QDebug>
#include <QDir>
#include <QPainter>

// TGS
#include <tgs/Statistics/Random.h>
#include <tgs/ProbablePath/ProbablePathCalculator.h>
using namespace Tgs;

namespace hoot
{

GraphComparator::GraphComparator(shared_ptr<OsmMap> map1, shared_ptr<OsmMap> map2) :
      BaseComparator(map1, map2)
{
  _iterations = 100;
  _init();
}

cv::Mat GraphComparator::_calculateCostDistance(shared_ptr<OsmMap> map, Coordinate c)
{
  // make a copy of the map so we can manipulate it.
  map.reset(new OsmMap(map));

  // find the nearest feature
  long wId = map->getIndex().findNearestWay(c);
  shared_ptr<Way> w = map->getWay(wId);

  // split way at c
  WayLocation wl = LocationOfPoint::locate(map, w, c);
  vector< shared_ptr<Way> > v = WaySplitter::split(map, w, wl);
  wl = LocationOfPoint::locate(map, v[0], c);
  if (wl.isNode() == false)
  {
    // I haven't been able to recreate the case when this happens.
    LOG_WARN("Internal Error: Expected wl to be on a node, but it was this: " << wl);
  }
  assert(wl.isNode() == true);

  // populate graph
  shared_ptr<DirectedGraph> graph(new DirectedGraph());
  graph->deriveEdges(map);

  ShortestPath sp(graph);
  // set cost at c to zero.
  long sourceId = v[0]->getNodeId(wl.getSegmentIndex());
  sp.setNodeCost(sourceId, 0.0);

  // calculate cost
  sp.calculateCost();

  // paint graph onto raster
  //_exportGraphImage(map, *graph, sp, "/home/jason.surratt/tmp/graph.png");
  cv::Mat mat = _paintGraph(map, *graph, sp);

  // calculate cost distance raster
  _calculateRasterCost(mat);

  return mat;
}

void GraphComparator::_calculateRasterCost(cv::Mat& mat)
{
  ProbablePathCalculator ppc;
  ppc.setRandomNoise(0.0);
  ppc.setRandomPatches(0.0, 1);
  vector<float> friction(_width * _height, 5.0 * _pixelSize);
  Tgs::Image<float> cost(_width, _height);

  for (int y = 0; y < _height; y++)
  {
    float* row = mat.ptr<float>(y);
    for (int x = 0; x < _width; x++)
    {
      cost.pixel(x, y) = row[x];
    }
  }

  ppc.setFriction(_height, _width, friction);
  cost = ppc.updateCostSurface(cost);

  for (int y = 0; y < _height; y++)
  {
    float* row = mat.ptr<float>(y);
    for (int x = 0; x < _width; x++)
    {
      row[x] = cost.pixel(x, y);
    }
  }
}

double GraphComparator::compareMaps()
{
  _updateBounds();
  double diffScoreSum = 0.0;

  vector<double> scores;
  int same = 0;
  // sampled standard deviation
  _s = -1;
  // 1.645 for 90% confidence, 1.96 for 95% confidence, and 2.58 for 99% confidence.
  // please update the header file comments if you change this value.
  double zalpha = 1.645;
  _ci = -1;

  // do this a bunch of times
  for (int i = 0; i < _iterations && same < 4; i++)
  {
    // generate a random source point
    _r.x = Random::instance()->generateUniform() * (_projectedBounds.MaxX - _projectedBounds.MinX) +
          _projectedBounds.MinX;
    _r.y = Random::instance()->generateUniform() * (_projectedBounds.MaxY - _projectedBounds.MinY) +
          _projectedBounds.MinY;

    shared_ptr<OsmMap> referenceMap;
    // pick one map as the reference map
    if (Random::instance()->coinToss())
    {
      referenceMap = _mapP1;
    }
    else
    {
      referenceMap = _mapP2;
    }

    _maxGraphCost = 0.0;

    // find the random source point's nearest point on a feature in one of the maps
    _r = _findNearestPointOnFeature(referenceMap, _r);

    // generate a cost distance raster for each map
    cv::Mat image1 = _calculateCostDistance(_mapP1, _r);
    cv::Mat image2 = _calculateCostDistance(_mapP2, _r);

    // take the difference of the two rasters and normalize
    double error = _calculateError(image1, image2);

    if (_debugImages)
    {
      cv::Mat diff(cvSize(_width, _height), CV_32FC1);

      const float* image1Data = image1.ptr<float>(0);
      const float* image2Data = image2.ptr<float>(0);
      float* diffData = diff.ptr<float>(0);
      size_t size = (image1.dataend - image1.datastart) / sizeof(float);
      for (size_t j = 0; j < size; j++)
      {
        diffData[j] = fabs(image1Data[j] - image2Data[j]);
      }

      QDir().mkpath("test-output/route-image");
      QString s1 = QString("test-output/route-image/route-%1-a.png").arg(i, 3, 10, QChar('0'));
      QString s2 = QString("test-output/route-image/route-%1-b.png").arg(i, 3, 10, QChar('0'));
      QString sdiff = QString("test-output/route-image/route-%1-diff.png").arg(i, 3, 10, QChar('0'));
      _saveImage(image1, s1, _maxGraphCost * 3);
      _saveImage(image2, s2, _maxGraphCost * 3);
      _saveImage(diff, sdiff, _maxGraphCost * 3);
    }

    image1.release();
    image2.release();

    // keep a running tally of the differences.
    diffScoreSum += error;

    scores.push_back(1 - error);
    sort(scores.begin(), scores.end());
    _median = scores[scores.size() / 2];
    _mean = 1 - (diffScoreSum / (i + 1));

    if (scores.size() > 1)
    {
      double v = 0;
      for (size_t i = 0; i < scores.size(); i++)
      {
        v += (scores[i] - _mean) * (scores[i] - _mean);
      }
      _s = sqrt(v / (scores.size() - 1));

      _ci = zalpha * _s / sqrt(scores.size());
    }


    if (Log::getInstance().isInfoEnabled())
    {
      cout << i << " / " << _iterations << " mean: " << _mean << "   \r";
      cout.flush();
    }
    //qDebug() << _median << 1 - error << _mean << "+/-" << _ci << "sd: " << _s;
  }

  if (Log::getInstance().isInfoEnabled())
  {
    cout << "                                   \r";
  }

  return _mean;
}

void GraphComparator::drawCostDistance(shared_ptr<OsmMap> map, vector<Coordinate>& c,
                                       QString output)
{
  _updateBounds();
  // make a copy of the map so we can manipulate it.
  map.reset(new OsmMap(map));

  for (size_t i = 0; i < c.size(); i++)
  {
    cout << c[i].x << " " << c[i].y << endl;
    c[i] = _findNearestPointOnFeature(map, c[i]);
    cout << c[i].x << " " << c[i].y << endl;
    // find the nearest feature
    long wId = map->getIndex().findNearestWay(c[i]);
    shared_ptr<Way> w = map->getWay(wId);

    // split way at c
    WayLocation wl = LocationOfPoint::locate(map, w, c[i]);
    vector< shared_ptr<Way> > v = WaySplitter::split(map, w, wl);
    wl = LocationOfPoint::locate(map, v[0], c[i]);
    assert(wl.isNode() == true);
  }

  // populate graph
  shared_ptr<DirectedGraph> graph(new DirectedGraph());
  graph->deriveEdges(map);

  LOG_WARN("Running cost");
  ShortestPath sp(graph);

  for (size_t i = 0; i < c.size(); i++)
  {
    long wId = map->getIndex().findNearestWay(c[i]);
    shared_ptr<Way> w = map->getWay(wId);

    WayLocation wl = LocationOfPoint::locate(map, w, c[i]);

    // set cost at c to zero.
    long sourceId = w->getNodeId(wl.getSegmentIndex());
    sp.setNodeCost(sourceId, 0.0);
  }

  // calculate cost
  sp.calculateCost();
  LOG_WARN("Cost done");

  cv::Mat mat = _paintGraph(map, *graph, sp);

  _saveImage(mat, output, -1.0, false);
  _saveImage(mat, output.replace(".png", "2.png"), -1.0, true);

  shared_ptr<OGRSpatialReference> srs(new OGRSpatialReference());
  srs->importFromEPSG(900913);

  Coordinate c1 = MapProjector::project(Coordinate(_projectedBounds.MinX, _projectedBounds.MinY), map->getProjection(), srs);
  cout << "coord " << c1.x << ", " << c1.y << endl;

  Coordinate c2 = MapProjector::project(Coordinate(_projectedBounds.MaxX, _projectedBounds.MaxY), map->getProjection(), srs);
  cout << "coord2 " << c2.x << ", " << c2.y << endl;

  printf("POSITION_Y=%f\n", (c1.y + c2.y) / 2.0);
  //cout << "POSITION_Y=" << (c1.y + c2.y) / 2.0 << endl;
  printf("POSITION_X=%f\n", (c1.x + c2.x) / 2.0);
  //cout << "POSITION_X=" << (c1.x + c2.x) / 2.0 << endl;
  cout << "M12=0.0" << endl;
  cout << "M11=1.0" << endl;
  cout << "M10=0.0" << endl;
  cout << "M02=0.0" << endl;
  cout << "INITIAL_SCALE=" << (c2.x - c1.x) / (double)_width * 100.0 << endl;
  cout << "M01=0.0" << endl;
  cout << "M00=1.0" << endl;

  // paint graph onto raster
  //_exportGraphImage(map, *graph, sp, output);
}

void GraphComparator::_exportGraphImage(shared_ptr<OsmMap> map, DirectedGraph& /*graph*/,
                                        ShortestPath& sp, QString path)
{
  const NodeMap& nodes = map->getNodeMap();

  double maxCost = 1e-100;
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    double cost = sp.getNodeCost(it->first);
    maxCost = std::max(cost, maxCost);
  }

  cout << _width << " x " << _height << endl;

  QImage image(_width, _height, QImage::Format_ARGB32);
  QPainter pt(&image);
  pt.setRenderHint(QPainter::Antialiasing, true);
  pt.fillRect(pt.viewport(), Qt::black);

  GeometryPainter gp;
  QMatrix m = gp.createMatrix(pt.viewport(), _projectedBounds);

  QPen pen(Qt::white);

  pen.setWidth(7);
  pt.setPen(pen);
  gp.drawPoint(pt, _r.x, _r.y, m);

  pen.setWidth(3);
  QColor c;

  cout << "max cost: " << maxCost << endl;

  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    double cost = sp.getNodeCost(it->first);
    if (cost == 0)
    {
      c.setRgb(0, 255, 0);
    }
    if (cost < 0)
    {
      c.setRgb(0, 0, 255);
    }
    else
    {
      int v = (cost / maxCost) * 255.0;
      c.setRgb(255, v, 0);
    }

    cout << "cost: " << cost << endl;

    pen.setColor(c);
    pt.setPen(pen);
    gp.drawNode(pt, it->second.get(), m);
  }

  image.save(path);
}

void GraphComparator::_init()
{
  // make sure the intersections only lay on end nodes.
  IntersectionSplitter::splitIntersections(_mapP1);
  IntersectionSplitter::splitIntersections(_mapP2);
  _debugImages = false;
}

cv::Mat GraphComparator::_paintGraph(shared_ptr<OsmMap> map, DirectedGraph& graph, ShortestPath& sp)
{
  const WayMap& ways = map->getWays();

  cv::Mat mat(cvSize(_width, _height), CV_32FC1);

  for (int y = 0; y < _height; y++)
  {
    float* row = mat.ptr<float>(y);
    for (int x = 0; x < _width; x++)
    {
      row[x] = -1.0;
    }
  }

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    shared_ptr<Way> w = it->second;
    double cost = sp.getNodeCost(w->getNodeIds()[0]);
    if (cost >= 0)
    {
      double friction = graph.determineCost(w);
      if (friction >= 0)
      {
        double startCost = cost;
        double endCost = sp.getNodeCost(w->getNodeIds()[w->getNodeCount() - 1]);
        _paintWay(mat, map, w, friction, startCost, endCost);
        _maxGraphCost = std::max(startCost, endCost);
      }
    }
  }

  return mat;
}

void GraphComparator::_paintWay(cv::Mat& mat, ConstOsmMapPtr map, shared_ptr<Way> way, double friction,
                                double startCost, double endCost)
{
  LocationOfPoint lop(map, way);
  double length = ElementConverter(map).convertToLineString(way)->getLength();
  // v is the distance along the way in meters
  for (double v = 0.0; v <= length; v += _pixelSize / 2.0)
  {
    double cost = std::min(startCost + v * friction, endCost + (length - v) * friction);
    Coordinate c = lop.locate(v);

    int x = (c.x - _projectedBounds.MinX) / _pixelSize;
    int y = _height - (c.y - _projectedBounds.MinY) / _pixelSize;

    float* row = mat.ptr<float>(y);
    if (row[x] >= 0.0)
    {
      row[x] = std::min((float)cost, row[x]);
    }
    else
    {
      row[x] = cost;
    }
  }
}

}
