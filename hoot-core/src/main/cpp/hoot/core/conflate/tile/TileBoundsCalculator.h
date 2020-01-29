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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef TILEBOUNDSCALCULATOR_H
#define TILEBOUNDSCALCULATOR_H


// Hoot
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QString>

// Std
#include <vector>

// geos
#include <geos/geom/Envelope.h>

// GDAL
#include <ogr_geometry.h>

namespace hoot
{

class TileBoundsCalculatorTest;
class OsmMap;
class Node;

/**
 * Caculates divisions as the center of pixels.
 *
 * WARNING: Under the covers we're using signed 32bit integers to represent the number of nodes
 * in a pixel. At this point there are fewer than 2 billion nodes in the planet file. As long as
 * there are fewer than ~2 billion nodes in a single pixel this shouldn't be a problem. I'm doing
 * this b/c of RAM limitations on my machine. In the future it may be necessary to change over
 * to 64bit floats, but I imagine that is a long way off. If you do switch then you'll have to
 * change the CV_32SC1 to CV_64FC1 (floating point representation). This will impact the
 * PaintNodes* code as well.
 */
class TileBoundsCalculator
{
public:

  static std::string className() { return "hoot::TileBoundsCalculator"; }

  static int logWarnCount;

  /**
   * Specifies the index of a pixel. When specifying a bounding box this represents the lower left
   * pixel relative to the bounding box.
   */
  class Pixel
  {
  public:

    int x;
    int y;

    Pixel(int x, int y) { this->x = x; this->y = y; }

    Pixel() {}
  };

  /**
   * Defines the inclusive box of pixels
   */
  class PixelBox
  {
  public:

    int minX;
    int minY;
    int maxX;
    int maxY;

    PixelBox(const PixelBox& pb)
    {
      minX = pb.minX;
      maxX = pb.maxX;
      minY = pb.minY;
      maxY = pb.maxY;
    }

    PixelBox()
    {
      minX = -1;
      maxX = -1;
      minY = -1;
      maxY = -1;
    }

    PixelBox(int minX, int maxX, int minY, int maxY)
    {
      this->minX = minX;
      this->maxX = maxX;
      this->minY = minY;
      this->maxY = maxY;
    }

    PixelBox getColumnBox(int col) const { return PixelBox(col, col, minY, maxY); }

    int getHeight() const { return maxY - minY + 1; }

    PixelBox getRowBox(int row) const { return PixelBox(minX, maxX, row, row); }

    int getWidth() const { return maxX - minX + 1; }

    PixelBox& operator=(const PixelBox& pb)
    {
      minX = pb.minX;
      maxX = pb.maxX;
      minY = pb.minY;
      maxY = pb.maxY;
      return *this;
    }

    QString toString() const
    {
      return QString("%1,%2,%3,%4").arg(minX).arg(minY).arg(maxX).arg(maxY);
    }
  };

  TileBoundsCalculator(double pixelSize);

  /**
   * Calculates a set of rectangular bounding boxes that at most contain a configured set of nodes;
   * strives to return a set of tiles with the most balanced number of nodes possible
   *
   * @return a grid of tile bounding boxes
   */
  std::vector<std::vector<geos::geom::Envelope>> calculateTiles();

  /**
   * Returns the node counts for each computed tile bounding box
   *
   * @return a grid of node counts
   */
  std::vector<std::vector<long>> getNodeCounts() const { return _nodeCounts; }

  void renderImage(const std::shared_ptr<OsmMap>& map);

  void renderImage(const std::shared_ptr<OsmMap>& map, cv::Mat& r1, cv::Mat& r2);

  void setEnvelope(const OGREnvelope& e) { _envelope = e; }

  void setImages(const cv::Mat& r1, const cv::Mat& r2);

  static QString tilesToString(const std::vector<std::vector<geos::geom::Envelope>>& tiles);

  /**
   * The entire tree will keep growing until all boxes are less than or equal to this number of
   * nodes.
   */
  void setMaxNodesPerBox(long max) { _maxNodesPerBox = max; }

  /**
   * Set the slop. This is the percentage that the line can vary from center. A higher slop will
   * slightly better conflation results, but less efficient distribution. The default should be
   * fine in most cases.
   */
  void setSlop(double slop) { _slop = slop; }

  long getMinNodeCountInOneTile() const { return _minNodeCountInOneTile; }
  long getMaxNodeCountInOneTile() const { return _maxNodeCountInOneTile; }

private:

  // used for white box testing.
  friend class TileBoundsCalculatorTest;

  cv::Mat _r1, _r2, _min;

  double _pixelSize;
  OGREnvelope _envelope;
  long _maxNodesPerBox;
  double _slop;
  int32_t _maxValue;
  long _maxNodeCountInOneTile;
  long _minNodeCountInOneTile;
  std::vector<std::vector<long>> _nodeCounts;

  void _calculateMin();

  int _calculateSplitX(PixelBox& b);

  int _calculateSplitY(const PixelBox& b);

  void _countNode(const std::shared_ptr<Node>& n);

  double _evaluateSplitPoint(const PixelBox& pb, const Pixel& p);

  void _exportImage(cv::Mat& r, QString output);

  void _exportResult(const std::vector<PixelBox>& boxes, QString output);

  bool _isDone(std::vector<PixelBox>& boxes);

  long _sumPixels(const PixelBox& pb, cv::Mat& r);

  long _sumPixels(const PixelBox& pb) { return _sumPixels(pb, _r1) + _sumPixels(pb, _r2); }

  geos::geom::Envelope _toEnvelope(const PixelBox& pb);
};

}

#endif // TILEBOUNDSCALCULATOR_H
