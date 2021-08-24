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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef NODE_DENSITY_TILE_BOUNDS_CALCULATOR_H
#define NODE_DENSITY_TILE_BOUNDS_CALCULATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QElapsedTimer>

// GDAL
#include <ogr_geometry.h>

namespace hoot
{

class Node;

/**
 * Calculates divisions as the center of pixels and outputs tile boundaries that attempt to
 * even distribute nodes based on a specified maximum node per tile count.
 *
 * WARNING: Under the covers we're using signed 32bit integers to represent the number of nodes
 * in a pixel. At this point there are fewer than 2 billion nodes in the planet file. As long as
 * there are fewer than ~2 billion nodes in a single pixel this shouldn't be a problem. I'm doing
 * this b/c of RAM limitations on my machine. In the future, it may be necessary to change over
 * to 64bit floats, but I imagine that is a long way off. If you do switch then you'll have to
 * change the CV_32SC1 to CV_64FC1 (floating point representation). This will impact the
 * PaintNodes* code as well.
 */
class NodeDensityTileBoundsCalculator
{
public:

  class TileCalcException : public HootException
  {
    public:

      TileCalcException(QString message) :
      HootException(message)
      {
      }
  };

  static QString className() { return "hoot::NodeDensityTileBoundsCalculator"; }

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

    Pixel() = default;
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

    QString toString() const
    {
      return QString("%1,%2,%3,%4").arg(minX).arg(minY).arg(maxX).arg(maxY);
    }
  };

  NodeDensityTileBoundsCalculator();
  virtual ~NodeDensityTileBoundsCalculator() = default;

  /**
   * Calculates a set of boundary tiles
   *
   * @param map the map containing the nodes
   */
  void calculateTiles(const ConstOsmMapPtr& map);

  static QString tilesToString(const std::vector<std::vector<geos::geom::Envelope>>& tiles);

  std::vector<std::vector<geos::geom::Envelope>> getTiles() const { return _tiles; }

  int getTileCount() const { return _tileCount; }

  /**
   * Returns the node counts for each computed tile bounding box
   *
   * @return a grid of node counts
   */
  std::vector<std::vector<long>> getNodeCounts() const { return _nodeCounts; }

  long getMinNodeCountInOneTile() const { return _minNodeCountInOneTile; }
  long getMaxNodeCountInOneTile() const { return _maxNodeCountInOneTile; }

  double getPixelSize() const { return _pixelSize; }
  void setPixelSize(double size) { _pixelSize = size; }

  /**
   * The entire tree will keep growing until all boxes are less than or equal to this number of
   * nodes.
   */
  long getMaxNodesPerTile() const { return _maxNodesPerTile; }
  void setMaxNodesPerTile(long max) { _maxNodesPerTile = max; }

  /**
   * Set the slop. This is the percentage that the line can vary from center. A higher slop will
   * yield slightly better conflation results, but less efficient distribution. The default should
   * be fine in most cases.
   */
  void setSlop(double slop) { _slop = slop; }

  void setPixelSizeRetryReductionFactor(int factor) { _pixelSizeRetryReductionFactor = factor; }
  void setMaxNumTries(int numTries) { _maxNumTries = numTries; }
  void setMaxTimePerAttempt(int seconds) { _maxTimePerAttempt = seconds; }
  void setEnvelope(const OGREnvelope& e) { _envelope = e; }

private:

  static int logWarnCount;

  // used for white box testing.
  friend class NodeDensityTileBoundsCalculatorTest;

  cv::Mat _r1, _r2, _min;

  double _pixelSize;
  OGREnvelope _envelope;
  // target max nodes per tile
  long _maxNodesPerTile;
  double _slop;
  int32_t _maxValue;

  // actual max nodes per tile
  long _maxNodeCountInOneTile;
  // actual min nodes per tile
  long _minNodeCountInOneTile;
  // if multiple attempts are enabled, auto reduce the pixel size by this amount
  int _pixelSizeRetryReductionFactor;
  // allows for multiple calc attempts
  int _maxNumTries;

  // timeout in seconds; -1 is unlimited
  int _maxTimePerAttempt;
  QElapsedTimer _timer;

  std::vector<std::vector<long>> _nodeCounts;
  std::vector<std::vector<geos::geom::Envelope>> _tiles;
  int _tileCount;

  /*
   * Calculates a set of rectangular bounding boxes that at most contain a configured set of nodes;
   * strives to return a set of tiles with the most balanced number of nodes possible
   */
  void _calculateTiles();

  void _renderImage(const ConstOsmMapPtr& map);
  void _renderImage(const ConstOsmMapPtr& map, cv::Mat& r1, cv::Mat& r2);
  void _setImages(const cv::Mat& r1, const cv::Mat& r2);

  void _calculateMin();

  int _calculateSplitX(const PixelBox& b);
  int _calculateSplitY(const PixelBox& b);

  void _countNode(const std::shared_ptr<Node>& n);

  double _evaluateSplitPoint(const PixelBox& pb, const Pixel& p);

  void _exportImage(cv::Mat& r, QString output) const;
  void _exportResult(const std::vector<PixelBox>& boxes, QString output);

  bool _isDone(std::vector<PixelBox>& boxes);

  long _sumPixels(const PixelBox& pb, cv::Mat& r) const;
  long _sumPixels(const PixelBox& pb);

  geos::geom::Envelope _toEnvelope(const PixelBox& pb) const;

  void _checkForTimeout() const;
};

}

#endif // NODE_DENSITY_TILE_BOUNDS_CALCULATOR_H
