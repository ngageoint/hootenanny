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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "NodeDensityTileBoundsCalculator.h"

// hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QImage>
#include <QPainter>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int NodeDensityTileBoundsCalculator::logWarnCount = 0;

NodeDensityTileBoundsCalculator::NodeDensityTileBoundsCalculator()
  : _pixelSize(0.001),
    _maxNodesPerTile(1000),
    _slop(0.1),
    _maxNodeCountInOneTile(0),
    _minNodeCountInOneTile(LONG_MAX),
    _pixelSizeRetryReductionFactor(10),
    _maxNumTries(3),
    _maxTimePerAttempt(-1),
    _tileCount(0)
{
}

void NodeDensityTileBoundsCalculator::calculateTiles(const ConstOsmMapPtr& map)
{  
  if (!map)
    throw IllegalArgumentException("Invalid map passed to node density tile calculator.");
  else if (map->getNodeCount() == 0)
    throw IllegalArgumentException("Empty map passed to node density tile calculator.");
  else if (_maxNodesPerTile == 0)
  {
    throw IllegalArgumentException(
      QString("Invalid maximum nodes per tile requirement equal to zero passed to node density " ) +
      QString("tile calculator."));
  }

  LOG_VARD(map->getNodeCount());
  if (map->getNodeCount() <= _maxNodesPerTile)
  {
    LOG_STATUS(
      "Node count " << StringUtils::formatLargeNumber(map->getNodeCount()) << " is less than "
      "specified maximum node count per tile. Returning a single tile covering all of the input "
      "data...");

    const geos::geom::Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(map);
    std::vector<geos::geom::Envelope> subTiles;
    subTiles.push_back(bounds);
    _tiles.push_back(subTiles);
    std::vector<long> subCounts;
    subCounts.push_back(map->getNodeCount());
    _nodeCounts.push_back(subCounts);
    _minNodeCountInOneTile = map->getNodeCount();
    _maxNodeCountInOneTile = map->getNodeCount();
    _tileCount = 1;
  }
  else
  {  
    if (_maxTimePerAttempt > 0)
      _timer.restart();

    int tryCtr = 0;
    LOG_VARD(_tiles.size());
    LOG_VARD(tryCtr);
    LOG_VARD(_maxNumTries);
    while (_tiles.empty() && tryCtr < _maxNumTries)
    {
      tryCtr++;

      QString msg =
        "Running node density tiles calculation attempt " + QString::number(tryCtr) + " / " +
         QString::number(_maxNumTries) + " with pixel size: " + QString::number(_pixelSize) +
         ", max allowed nodes: " + StringUtils::formatLargeNumber(_maxNodesPerTile) +
         ", total input node size: " + StringUtils::formatLargeNumber(map->getNodeCount());
      if (_maxTimePerAttempt == -1)
        msg += ", and with no timeout...";
      else
        msg += ", and with a timeout of " + QString::number(_maxTimePerAttempt) + " seconds...";
      LOG_STATUS(msg);

      cv::Mat r1, r2;
      _renderImage(map, r1, r2);
      // We're calculating for unknown1 only, so fill the second matrix with all zeroes.
      cv::Mat zeros = cv::Mat::zeros(r1.size(), r1.type());
      _setImages(r1, zeros);

      try
      {
        _calculateTiles();
      }
      catch (const TileCalcException& e)
      {
        msg = "Tile calculation attempt " + QString::number(tryCtr) + " / " +
          QString::number(_maxNumTries) + " failed with error: \"" + e.getWhat() + "\"";
        if (tryCtr == _maxNumTries)
        {
          msg += " Aborting calculation.";
          LOG_ERROR(msg);
          //  Rethrow original exception
          throw;
        }
        else
        {
          _tiles.clear();
          msg += " Retrying calculation...";
          LOG_STATUS(msg);
          if (_pixelSizeRetryReductionFactor != -1)
            _pixelSize -= _pixelSize * (_pixelSizeRetryReductionFactor / 100.0);
          LOG_VARD(_tiles.size());
          LOG_VARD(tryCtr);
          LOG_VARD(_maxNumTries);
        }
      }
    }
  }
}

void NodeDensityTileBoundsCalculator::_calculateMin()
{
  int w = static_cast<int>(ceil((_envelope.MaxX - _envelope.MinX) / _pixelSize)) + 1;
  LOG_VART(w);
  int h = static_cast<int>(ceil((_envelope.MaxY - _envelope.MinY) / _pixelSize)) + 1;
  LOG_VART(h);

  assert(h == _r1.rows && w == _r1.cols);
  assert(_r1.rows == _r2.rows && _r1.cols == _r2.cols);

  _min = cv::Mat(cvSize(w, h), CV_32SC1);
  LOG_VART(_min);

  _maxValue = 1.0;
  for (int py = 0; py < h; py++)
  {
    const int32_t* row1 = _r1.ptr<int32_t>(py);
    const int32_t* row2 = _r2.ptr<int32_t>(py);
    int32_t* rowM = _min.ptr<int32_t>(py);

    for (int px = 0; px < w; px++)
    {
      rowM[px] = std::min(row1[px], row2[px]);
      _maxValue = std::max(_maxValue, std::max(row1[px], row2[px]));
    }
  }
}

QString NodeDensityTileBoundsCalculator::tilesToString(const vector<vector<Envelope>>& tiles)
{
  QString str;
  for (size_t tx = 0; tx < tiles.size(); tx++)
  {
    for (size_t ty = 0; ty < tiles[tx].size(); ty++)
      str += GeometryUtils::toConfigString(tiles[tx][ty]) + "\n";
  }
  str.chop(1);
  return str;
}

void NodeDensityTileBoundsCalculator::_checkForTimeout() const
{
  if (_maxTimePerAttempt > 0 && (_timer.elapsed() / 1000) > _maxTimePerAttempt)
    throw TileCalcException("Calculation timed out at " + QString::number(_timer.elapsed() / 1000) + " seconds.");
}

void NodeDensityTileBoundsCalculator::_calculateTiles()
{
  LOG_INFO("Calculating tiles...");

  size_t width = 1;
  vector<PixelBox> boxes;
  boxes.resize(1);
  boxes[0] = PixelBox(0, _r1.cols - 1, 0, _r1.rows - 1);

  double nodeCount = static_cast<double>(_sumPixels(boxes[0]));
  LOG_DEBUG("w: " << _r1.cols << " h: " << _r1.rows);
  LOG_DEBUG("Total node count: " << nodeCount);
  LOG_VARD(_maxNodesPerTile);

  while (!_isDone(boxes))
  {
    width *= 2;

    vector<PixelBox> nextLayer;
    nextLayer.resize(width * width);

    LOG_TRACE("width: " << width);
    for (size_t i = 0; i < boxes.size(); i++)
    {
      const PixelBox& b = boxes[i];
      int splitX = _calculateSplitX(b);
      int tx = static_cast<int>(i % (width / 2));
      int ty = static_cast<int>(i / (width / 2));
      LOG_TRACE("  i: " << i << " tx: " << tx << " ty: " << ty);

      int splitYLeft = _calculateSplitY(PixelBox(b.minX, splitX, b.minY, b.maxY));
      nextLayer[(tx * 2 + 0) + (ty * 2 + 0) * width] =
        PixelBox(b.minX, splitX, b.minY, splitYLeft);
      nextLayer[(tx * 2 + 0) + (ty * 2 + 1) * width] =
        PixelBox(b.minX, splitX, splitYLeft + 1, b.maxY);

      int splitYRight = _calculateSplitY(PixelBox(splitX + 1, b.maxX, b.minY, b.maxY));
      nextLayer[(tx * 2 + 1) + (ty * 2 + 0) * width] =
        PixelBox(splitX + 1, b.maxX, b.minY, splitYRight);
      nextLayer[(tx * 2 + 1) + (ty * 2 + 1) * width] =
        PixelBox(splitX + 1, b.maxX, splitYRight + 1, b.maxY);
    }

    boxes = nextLayer;

    _checkForTimeout();
  }

  _tiles.clear();
  _maxNodeCountInOneTile = 0;
  _minNodeCountInOneTile = LONG_MAX;
  LOG_VARD(width);
  _tiles.resize(width);
  _nodeCounts.clear();
  _nodeCounts.resize(width);

  for (size_t tx = 0; tx < width; tx++)
  {
    _tiles[tx].resize(width);
    _nodeCounts[tx].resize(width);
    for (size_t ty = 0; ty < width; ty++)
    {
      const PixelBox& pb = boxes[tx + ty * width];
      LOG_VART(pb.getWidth());
      LOG_VART(pb.getHeight());

      if (pb.getWidth() < 3 || pb.getHeight() < 3)
      {
        throw TileCalcException(
          "Node density tiles pixel box must be at least three pixels wide and tall. Try "
          "reducing the pixel size or increasing the maximum allowed nodes per tile. Current "
          "pixel box width: " + QString::number(pb.getWidth()) + "; height: " +
          QString::number(pb.getHeight()));
      }

      const long count = _sumPixels(pb);
      _nodeCounts[tx][ty] = count;
      _maxNodeCountInOneTile = std::max(_maxNodeCountInOneTile, count);
      _minNodeCountInOneTile = std::min(_minNodeCountInOneTile, count);

      _tiles[tx][ty] = _toEnvelope(pb);
    }

    _checkForTimeout();
  }
  _tileCount = static_cast<int>(width * width);

  if (_maxNodeCountInOneTile == 0)
  {
    throw TileCalcException(
      "The maximum node density tiles node count in one tile is zero. Try reducing the pixel "
      "size or increasing the maximum allowed nodes per tile.");
  }
  LOG_TRACE("Tiles: " + tilesToString(_tiles));

  _exportResult(boxes, "tmp/result.png");
}

int NodeDensityTileBoundsCalculator::_calculateSplitX(const PixelBox& b)
{
  double total = static_cast<double>(_sumPixels(b));
  LOG_VART(total);

  double left = static_cast<double>(_sumPixels(b.getColumnBox(b.minX)));
  LOG_VART(left);

  int best = (b.maxX + b.minX) / 2;
  double bestSum = numeric_limits<double>::max();

  double thisSlop = _slop + 1.0 / static_cast<double>(b.maxX - b.minX);
  LOG_VART(thisSlop);

  LOG_VART(b.getWidth());
  if (b.getWidth() < 6)
  {
    throw TileCalcException(
      "Node density tiles pixel box must be at least six pixels wide. Try reducing the input "
      "pixel size or increasing the maximum nodes allowed per tile. Current pixel box width: " +
      QString::number(b.getWidth()));
  }

  for (int c = b.minX + 2; c < b.maxX - 2; c++)
  {
    double colSum = static_cast<double>(_sumPixels(b.getColumnBox(c)));
    LOG_VART(colSum);
    double colSumMin = static_cast<double>(_sumPixels(b.getColumnBox(c), _min)) +
                       static_cast<double>(_sumPixels(b.getColumnBox(c + 1), _min));
    LOG_VART(colSumMin);
    left += colSum;

    double slop = abs(0.5 - left / total);
    if ((slop < thisSlop) && colSumMin < bestSum)
    {
      best = c;
      bestSum = colSumMin;
    }
  }
  LOG_VART(left);
  LOG_VART(best);
  LOG_VART(bestSum);

  if (bestSum == numeric_limits<double>::max())
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Node density tiles bestSum isn't valid. " << b.toString());
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  LOG_VART(best);
  return best;
}

int NodeDensityTileBoundsCalculator::_calculateSplitY(const PixelBox& b)
{
  double total = static_cast<double>(_sumPixels(b));
  LOG_VART(total);

  double bottom = static_cast<double>(_sumPixels(b.getRowBox(b.minY)));
  LOG_VART(bottom);

  int best = (b.maxY + b.minY) / 2;
  double bestSum = numeric_limits<double>::max();

  double thisSlop = _slop + 1.0 / static_cast<double>(b.maxY - b.minY);
  LOG_VART(thisSlop);

  if (b.getHeight() < 6)
  {
    throw TileCalcException(
      "Node density tiles pixel box must be at least six pixels high. Try reducing the input "
      "pixel size or increasing the maximum nodes allowed per tile. Current pixel box height: " +
      QString::number(b.getHeight()));
  }

  for (int r = b.minY + 2; r < b.maxY - 2; r++)
  {
    double rowSum = static_cast<double>(_sumPixels(b.getRowBox(r)));
    LOG_VART(rowSum);
    double rowSumMin = static_cast<double>(_sumPixels(b.getRowBox(r), _min)) +
                       static_cast<double>(_sumPixels(b.getRowBox(r + 1), _min));
    LOG_VART(rowSumMin);
    bottom += rowSum;

    double slop = abs(0.5 - bottom / total);
    if ((slop < thisSlop) && rowSumMin < bestSum)
    {
      best = r;
      bestSum = rowSumMin;
    }
  }
  LOG_VART(bottom);
  LOG_VART(best);
  LOG_VART(bestSum);

  if (bestSum == numeric_limits<double>::max())
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN(
        "Node density tiles bestSum isn't valid. " << b.toString() << " total: " << total <<
        " size: " << b.maxY - b.minY);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  LOG_VART(best);
  return best;
}

void NodeDensityTileBoundsCalculator::_countNode(const std::shared_ptr<Node> &n)
{
  double x = n->getX();
  double y = n->getY();

  int px = static_cast<int>((x - _envelope.MinX) / _pixelSize);
  LOG_VART(px);
  int py = static_cast<int>((y - _envelope.MinY) / _pixelSize);
  LOG_VART(py);

  if (px < 0 || px >= _r1.cols || py < 0 || py >= _r1.rows)
    throw HootException("Node density tiles pixel out of bounds.");
  else
  {
    int32_t* row;
    if (n->getStatus() == Status::Unknown1)
    {
      row = _r1.ptr<int32_t>(py);
      row[px] += 1;
    }
    else if (n->getStatus() == Status::Unknown2)
    {
      row = _r2.ptr<int32_t>(py);
      row[px] += 1;
    }
  }
}

void NodeDensityTileBoundsCalculator::_exportImage(cv::Mat& r, QString output) const
{
  QImage qImage(r.cols, r.rows, QImage::Format_RGB16);
  if (qImage.isNull())
  {
    throw HootException(
      QString("Node density tiles: Unable to allocate image of size %1x%2")
        .arg(r.cols)
        .arg(r.rows));
  }
  QPainter pt(&qImage);
  pt.setRenderHint(QPainter::Antialiasing, false);
  pt.fillRect(pt.viewport(), Qt::black);
  QPen pen;
  pen.setWidth(0);
  pen.setColor(qRgb(1, 0, 0));
  pt.setPen(pen);

  LOG_VART(_maxValue);

  LOG_VART(r.cols); //18k
  for (int y = 0; y < r.rows; y++)
  {
    const int32_t* row = r.ptr<int32_t>(y);
    for (int x = 0; x < r.cols; x++)
    {
      double l;
      if (row[x] == 0)
        l = 0.0;
      else
        l = log(row[x]) / log(_maxValue);
      int v = l * 255;
      qImage.setPixel(x, r.rows - y - 1, qRgb(v, v, 50));
    }
    _checkForTimeout();
  }

  qImage.save(output);
}

void NodeDensityTileBoundsCalculator::_exportResult(const vector<PixelBox>& boxes, QString output)
{
  QImage qImage(_r1.cols, _r1.rows, QImage::Format_RGB16);
  if (qImage.isNull())
  {
    throw HootException(
      QString("Node density tiles: Unable to allocate image of size %1x%2")
        .arg(_r1.cols)
        .arg(_r1.rows));
  }
  QPainter pt(&qImage);
  pt.setRenderHint(QPainter::Antialiasing, false);
  pt.fillRect(pt.viewport(), Qt::black);
  QPen pen;
  pen.setWidth(0);
  pen.setColor(qRgb(1, 0, 0));
  pt.setPen(pen);

  LOG_TRACE("max value: " << _maxValue);

  LOG_VART(_r1.rows); //14k
  LOG_VART(_r1.cols); //18k
  for (int y = 0; y < _r1.rows; y++)
  {
    const int32_t* row1 = _r1.ptr<int32_t>(y);
    const int32_t* row2 = _r2.ptr<int32_t>(y);
    for (int x = 0; x < _r1.cols; x++)
    {
      double l1 = row1[x] <= 0 ? 0.0 : log(row1[x]) / log(_maxValue);
      double l2 = row2[x] <= 0 ? 0.0 : log(row2[x]) / log(_maxValue);

      qImage.setPixel(x, _r1.rows - y - 1, qRgb(l1 * 255, l2 * 255, 0));
    }

    _checkForTimeout();
  }

  pt.setPen(QPen(QColor(0, 0, 255, 100)));
  for (const auto& b : boxes)
  {
    pt.drawRect(b.minX, _r1.rows - b.maxY - 1, b.maxX - b.minX, b.maxY - b.minY);
    _checkForTimeout();
  }

  qImage.save(output);
}

bool NodeDensityTileBoundsCalculator::_isDone(vector<PixelBox>& boxes)
{
  LOG_VART(boxes.size());

  bool smallEnough = true;
  bool minSize = false;

  for (const auto& b : boxes)
  {
    if (b.getWidth() == 1 || b.getHeight() == 1)
      minSize = true;
    if (_sumPixels(b) > _maxNodesPerTile)
      smallEnough = false;
  }

  if (minSize == true && smallEnough == false)
  {
    throw TileCalcException(
      "Could not find a node density tiles solution. Try reducing the pixel size or "
      "increasing the maximum nodes allowed per tile.");
  }
  else
    return smallEnough;
}

void NodeDensityTileBoundsCalculator::_renderImage(const ConstOsmMapPtr& map)
{
  _envelope = CalculateMapBoundsVisitor::getBounds(map);

  _renderImage(map, _r1, _r2);

  _calculateMin();

  _exportImage(_r1, "tmp/r1.png");
  _exportImage(_r2, "tmp/r2.png");
  _exportImage(_min, "tmp/min.png");
}

void NodeDensityTileBoundsCalculator::_renderImage(const ConstOsmMapPtr& map, cv::Mat& r1,
                                                   cv::Mat& r2)
{
  LOG_INFO("Rendering images...");

  _envelope = CalculateMapBoundsVisitor::getBounds(map);
  if (Log::getInstance().getLevel() <= Log::Debug)
  {
    std::shared_ptr<geos::geom::Envelope> tempEnv(GeometryUtils::toEnvelope(_envelope));
    LOG_VART(tempEnv->toString());
  }
  LOG_VART(_pixelSize);
  LOG_VART(StringUtils::formatLargeNumber(map->getNodeCount()));

  int w = ceil((_envelope.MaxX - _envelope.MinX) / _pixelSize) + 1;
  LOG_VART(w);
  int h = ceil((_envelope.MaxY - _envelope.MinY) / _pixelSize) + 1;
  LOG_VART(h)

  _r1 = cv::Mat(cvSize(w, h), CV_32SC1);
  _r2 = cv::Mat(cvSize(w, h), CV_32SC1);

  for (int py = 0; py < h; py++)
  {
    int32_t* row1 = _r1.ptr<int32_t>(py);
    int32_t* row2 = _r2.ptr<int32_t>(py);

    for (int px = 0; px < w; px++)
    {
      row1[px] = 0.0;
      row2[px] = 0.0;
    }
  }

  const NodeMap& nm = map->getNodes();
  LOG_VART(nm.size());
  long nodeCtr = 0;
  const int statusUpdateInterval = ConfigOptions().getTaskStatusUpdateInterval();
  for (auto it = nm.begin(); it != nm.end(); ++it)
  {
    const NodePtr& n = it->second;
    _countNode(n);

    nodeCtr++;
    if (nodeCtr % statusUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Counted " << StringUtils::formatLargeNumber(nodeCtr) << " of " <<
        StringUtils::formatLargeNumber(nm.size()) << " nodes.");
    }
  }

  r1 = _r1;
  r2 = _r2;
}

void NodeDensityTileBoundsCalculator::_setImages(const cv::Mat& r1, const cv::Mat& r2)
{
  LOG_INFO("Exporting images...");

  _r1 = r1;
  _r2 = r2;

  _calculateMin();

  _exportImage(_r1, "tmp/r1.png");
  _exportImage(_r2, "tmp/r2.png");
  _exportImage(_min, "tmp/min.png");
}

long NodeDensityTileBoundsCalculator::_sumPixels(const PixelBox& pb) const
{
  return _sumPixels(pb, _r1) + _sumPixels(pb, _r2);
}

long NodeDensityTileBoundsCalculator::_sumPixels(const PixelBox& pb, const cv::Mat& r) const
{
  LOG_VART(pb.minY);
  LOG_VART(pb.maxY);
  LOG_VART(pb.minX);
  LOG_VART(pb.maxX);

  long sum = 0.0;
  for (int py = pb.minY; py <= pb.maxY; py++)
  {
    const int32_t* row = r.ptr<int32_t>(py);

    for (int px = pb.minX; px <= pb.maxX; px++)
      sum += row[px];
  }
  LOG_VART(sum);
  return sum;
}

Envelope NodeDensityTileBoundsCalculator::_toEnvelope(const PixelBox& pb) const
{
  return Envelope(_envelope.MinX + pb.minX * _pixelSize,
                  _envelope.MinX + (pb.maxX + 1) * _pixelSize,
                  _envelope.MinY + pb.minY * _pixelSize,
                  _envelope.MinY + (pb.maxY + 1) * _pixelSize);
}

}
