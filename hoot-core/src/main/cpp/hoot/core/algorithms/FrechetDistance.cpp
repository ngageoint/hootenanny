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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "FrechetDistance.h"

// geos
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>

//  Hoot
#include <hoot/core/algorithms/DirectionFinder.h>
#include <hoot/core/algorithms/WayHeading.h>
#include <hoot/core/algorithms/linearreference/LocationOfPoint.h>
#include <hoot/core/ops/CopySubsetOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ElementConverter.h>

namespace hoot
{

FrechetDistance::FrechetDistance(const ConstOsmMapPtr &map, const ConstWayPtr &way1, const ConstWayPtr &way2, Radians maxAngle)
  : _matrix(boost::extents[way1->getNodeCount()][way2->getNodeCount()]), _maxAngle(maxAngle)
{
  //  Copy the map and two ways
  _map.reset(new OsmMap());
  CopySubsetOp(map,
               way1->getElementId(),
               way2->getElementId()).apply(_map);
  _w1 = _map->getWay(way1->getId());
  _w2 = _map->getWay(way2->getId());
  //  Convert both ways to line strings
  _ls1 = ElementConverter(_map).convertToLineString(_w1);
  _ls2 = ElementConverter(_map).convertToLineString(_w2);
  //  Precalculate the locations and distances for way1
  LocationOfPoint locatorWay2(_map, _w2);
  for (size_t i = 0; i < _w1->getNodeCount(); i++)
  {
    Coordinate pointOnWay1 = _map->getNode(_w1->getNodeId(i))->toCoordinate();
    WayLocation nearestPointOnWay2 = locatorWay2.locate(pointOnWay1);
    //  Save off the nearest point on way2 and the distance to it
    _locations_w1.push_back(nearestPointOnWay2);
    _distances_w1.push_back(pointOnWay1.distance(nearestPointOnWay2.getCoordinate()));
  }
  //  Precalculate the locations and distances for way2
  LocationOfPoint locatorWay1(_map, _w1);
  for (size_t i = 0; i < _w2->getNodeCount(); i++)
  {
    Coordinate pointOnWay2 = _map->getNode(_w2->getNodeId(i))->toCoordinate();
    WayLocation nearestPointOnWay1 = locatorWay1.locate(pointOnWay2);
    //  Save off the nearest point on way1 and the distance to it
    _locations_w2.push_back(nearestPointOnWay1);
    _distances_w2.push_back(pointOnWay2.distance(nearestPointOnWay1.getCoordinate()));
  }
  //  Precalculate the discreet matrix
  _matrix = calculateMatrix();

}

frechet_matrix FrechetDistance::calculateMatrix()
{
  int rows = _ls1->getNumPoints();
  int cols = _ls2->getNumPoints();
  if (rows < 1 || cols < 1)
    throw HootException("FrechetDistance::calculateMatrix - ways not valid sizes");
  frechet_matrix frechet(boost::extents[rows][cols]);
  //  Iterate the matrix and calculate the distances
  for (int r = 0; r < rows; r++)
  {
    for (int c = 0; c < cols; c++)
      frechet[r][c] = _ls1->getCoordinateN(r).distance(_ls2->getCoordinateN(c));
  }

  return frechet;
}

void FrechetDistance::advanceAndCheckRow(const int rows, const int cols, int& r, int& c, Meters& max_frechet)
{
  //  Only advance to and check the next row
  advanceAndCheck(rows, cols, r, c, max_frechet, true, false);
}

void FrechetDistance::advanceAndCheckColumn(const int rows, const int cols, int& r, int& c, Meters& max_frechet)
{
  //  Only advance to and check the next column
  advanceAndCheck(rows, cols, r, c, max_frechet, false, true);
}

void FrechetDistance::advanceAndCheckBoth(const int rows, const int cols, int& r, int& c, Meters& max_frechet)
{
  //  Advance to and check both the next column and next row
  advanceAndCheck(rows, cols, r, c, max_frechet, true, true);
}

void FrechetDistance::advanceAndCheck(const int rows, const int cols, int& r, int& c, Meters& max_frechet, bool advance_row, bool advance_col)
{
  if (!advance_row && ! advance_col)
    return;
  //  Advance the row
  if (advance_row)
  {
    max_frechet = max(max_frechet, _distances_w1[r]);
    r++;
    //  Don't move off of the edge of the matrix
    if (r < rows)
      max_frechet = max(max_frechet, _distances_w1[r]);
  }
  //  Advance the column
  if (advance_col)
  {
    max_frechet = max(max_frechet, _distances_w2[c]);
    c++;
    //  Don't move off of the edge of the matrix
    if (c < cols)
      max_frechet = max(max_frechet, _distances_w2[c]);
  }
}

Radians FrechetDistance::getHeadingWay1(int index)
{
  //  Return the heading for _w1 at index
  return getHeading(_w1, index);
}

Radians FrechetDistance::getHeadingWay2(int index)
{
  //  Return the heading for _w2 at index
  return getHeading(_w2, index);
}

Radians FrechetDistance::getHeading(WayPtr way, int index)
{
  //  TEST
  return getHeadingAvg(way, index);
  //  END TEST

  //  Setup the indices to check between
  int index1 = index;
  int index2 = index + 1;
  //  Check the previous indices if index is the last node
  if (index == (long)(way->getNodeCount() - 1))
  {
    index1 = index - 1;
    index2 = index;
  }
  //  Get the coordinates of the two nodes and create a line segment
  Coordinate c11 = _map->getNode(way->getNodeId(index1))->toCoordinate();
  Coordinate c12 = _map->getNode(way->getNodeId(index2))->toCoordinate();
  LineSegment ls(c11, c12);
  //  Make sure to ignore any duplicate nodes in the way
  while (ls.p0 == ls.p1)
  {
    //  Ignore duplicates
    if (index2 == (long)(way->getNodeCount() - 1))
      index1--;
    else if (index1 > 0)
      index2++;
    else
      return 0;
    //  Re-get the coordinates of the two nodes and create a line segment
    c11 = _map->getNode(way->getNodeId(index1))->toCoordinate();
    c12 = _map->getNode(way->getNodeId(index2))->toCoordinate();
    ls = LineSegment(c11, c12);
  }
  //  Use the line segment angle if available or use the rough estimate from WayHeading::calculateHeading
  if (ls.p0 != ls.p1)
    return ls.angle();
  else
    return WayHeading::calculateHeading(WayLocation(_map, way, index, 0), 0.5);
}

Radians FrechetDistance::getHeadingAvg(WayPtr way, int index)
{
  //  Get the average of the previous, current, and next headings if possible
  int start = index - 1;
  int stop = index + 2;
  //  No previous exists
  if (start < 0)
    start++;
  //  Can't go next or possibly current
  while (stop >= (int)way->getNodeCount())
  {
    start--;
    stop--;
  }
  //  Don't shift off of the left side of the way
  if (start < 0)
    start = 0;
  //  This case is a way with only one point
  if (stop == 0)
    return WayHeading::calculateHeading(WayLocation(_map, way, 0, 0), 0.5);
  //  Iterate the 3 (hopefully) segments, getting their headings
  Radians heading = 0.0;
  int count = 0;
  for (int i = start; i < stop; i++)
  {
    Coordinate c11 = _map->getNode(way->getNodeId(i))->toCoordinate();
    Coordinate c12 = _map->getNode(way->getNodeId(i + 1))->toCoordinate();
    LineSegment ls = LineSegment(c11, c12);
    //  Sum up the heading
    if (ls.p0 != ls.p1)
      heading += ls.angle();
    else
      heading += WayHeading::calculateHeading(WayLocation(_map, way, i, 0), 0.5);
    //  Increment the denominator
    count++;
  }
  //  Average the heading
  return heading / count;
}

Meters FrechetDistance::distance()
{
  //  Since at least one endpoint in each way points to an endpoint of another a simple
  //  maximum value of all the closest distances is the non-discreet Frechet distance
  return max(*max_element(_distances_w1.begin(), _distances_w1.end()),
             *max_element(_distances_w2.begin(), _distances_w2.end()));
}

frechet_subline FrechetDistance::maxSubline(Meters maxDistance)
{
  frechet_subline best_subline;

  int rows = _ls1->getNumPoints();
  int cols = _ls2->getNumPoints();
  if (rows < 1 || cols < 1)
    throw HootException("FrechetDistance::_calculate - Invalid matrix size");
  //  Get the uninformed Frechet distance as the max
  Meters max_frechet = distance();

  frechet_subline starts;
  //  Check if the max subline is the combination of both lines
  if (max_frechet >= maxDistance)
  {
    //  Need some kind of algorithm here to get a few different starting locations for max sublines
    //  Let's start with an exhaustive search for starting points less than max distance
    for (int r = 0; r < rows; r++)
    {
      for (int c = 0; c < cols; c++)
      {
        //  Only start where the distance is less than max distance
        if (_matrix[r][c] < maxDistance)
        {
          //  Check above and left to eliminate starting points that lie along a path already started
          if (r > 0 && _matrix[r - 1][c] < maxDistance)
            continue;
          if (c > 0 && _matrix[r][c - 1] < maxDistance)
            continue;
          if (r > 0 && c > 0 && _matrix[r][c] < maxDistance)
            continue;
          starts.push_back(vertex_match(r, c));
        }
      }
    }
  }
  else
  {
    //  Max frechet fits inside the max distance
    starts.push_back(vertex_match(0, 0));
  }

  Meters best_frechet = maxDistance;
  //  Iterate all of the valid starting points in order to reduce search
  for (frechet_subline::size_type i = 0; i < starts.size(); i++)
  {
    frechet_subline subline;

    int r = starts[i].first;
    int c = starts[i].second;
    //  Use the starting position and modify it if the ways are reversed
    subline.push_back(vertex_match(r, c));
    //  The beginning frechet distance is between the two starting nodes
    Meters frechet = _matrix[r][c];
    //  Iterate through the matrix from the start position
    while (r != rows && c != cols)
    {
      Meters max_frechet = 0.0;
      //  Check for the next move
      if (r == rows - 1 && c == cols -1)
        break;
      else if (r == rows - 1)
        advanceAndCheckColumn(rows, cols, r, c, max_frechet);
      else if (c == cols - 1)
        advanceAndCheckRow(rows, cols, r, c, max_frechet);
      else if (_matrix[r + 1][c + 1] <= _matrix[r + 1][c] && _matrix[r + 1][c + 1] <= _matrix[r][c + 1])
        advanceAndCheckBoth(rows, cols, r, c, max_frechet);
      else if (_matrix[r][c + 1] <= _matrix[r + 1][c] && _matrix[r][c + 1] <= _matrix[r + 1][c + 1])
        advanceAndCheckColumn(rows, cols, r, c, max_frechet);
      else if (_matrix[r + 1][c] <= _matrix[r][c + 1] && _matrix[r + 1][c] <= _matrix[r + 1][c + 1])
        advanceAndCheckRow(rows, cols, r, c, max_frechet);
      double value = (max_frechet > 0.0 ? min(_matrix[r][c], max_frechet) : _matrix[r][c]);
      //  Check that the distance is less than the max distance in order to include this node
      if (value < maxDistance)
      {
        //  Check to make sure that the headings are within the max angle before adding the subline entry
        Radians h1 = getHeadingWay1(r);
        Radians h2 = getHeadingWay2(c);
        Radians delta = WayHeading::deltaMagnitude(h1, h2);
        if (delta <= _maxAngle)
        {
          subline.push_back(vertex_match(r, c));
          frechet = min(frechet, value);
        }
        else
          break;
      }
      else
        break;
    }
    //  Do some backtracking if the two endpoints are too far away of each other
    while (frechet > maxDistance)
    {
      //  Backtrack
      subline.pop_back();
      if (subline.size() < 1)
        break;
      //  Get the distance between the last two nodes
      vertex_match m = subline[subline.size() - 1];
      frechet = _matrix[m.first][m.second];
    }
    //  Is this subline "better" than the current best?
    if ((best_subline.size() == subline.size() && frechet < best_frechet) ||
        (best_subline.size() < subline.size()))
    {
      best_frechet = frechet;
      best_subline = subline;
    }
  }
  return best_subline;
}

}
