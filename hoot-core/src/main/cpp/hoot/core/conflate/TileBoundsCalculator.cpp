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

#include "TileBoundsCalculator.h"

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/visitors/CalculateBoundsVisitor.h>

// Qt
#include <QImage>
#include <QPainter>

namespace hoot
{

TileBoundsCalculator::TileBoundsCalculator(double pixelSize)
{
  _pixelSize = pixelSize;
  _slop = 0.1;
  setMaxNodesPerBox(1000);
}

void TileBoundsCalculator::_calculateMin()
{
  int w = ceil((_envelope.MaxX - _envelope.MinX) / _pixelSize) + 1;
  int h = ceil((_envelope.MaxY - _envelope.MinY) / _pixelSize) + 1;

  assert(h == _r1.rows && w == _r1.cols);
  assert(_r1.rows == _r2.rows && _r1.cols == _r2.cols);

  _min = cv::Mat(cvSize(w, h), CV_32SC1);

  _maxValue = 1.0;
  for (int py = 0; py < h; py++)
  {
    int32_t* row1 = _r1.ptr<int32_t>(py);
    int32_t* row2 = _r2.ptr<int32_t>(py);
    int32_t* rowM = _min.ptr<int32_t>(py);

    for (int px = 0; px < w; px++)
    {
      rowM[px] = std::min(row1[px], row2[px]);
      _maxValue = std::max(_maxValue, std::max(row1[px], row2[px]));
    }
  }
}

vector< vector<Envelope> >  TileBoundsCalculator::calculateTiles()
{
  size_t width = 1;
  vector<PixelBox> boxes;
  boxes.resize(1);
  boxes[0] = PixelBox(0, _r1.cols - 1, 0, _r1.rows - 1);

  double nodeCount = _sumPixels(boxes[0]);
  LOG_INFO("w: " << _r1.cols << " h: " << _r1.rows);
  LOG_INFO("Total node count: " << nodeCount);

  while (!_isDone(boxes))
  {
    width *= 2;

    vector<PixelBox> nextLayer;
    nextLayer.resize(width * width);

    //LOG_INFO("width: " << width);
    for (size_t i = 0; i < boxes.size(); i++)
    {
      PixelBox& b = boxes[i];
      double splitX = _calculateSplitX(b);
      int tx = i % (width / 2);
      int ty = i / (width / 2);
      //LOG_INFO("  i: " << i << " tx: " << tx << " ty: " << ty);

      double splitYLeft = _calculateSplitY(PixelBox(b.minX, splitX, b.minY, b.maxY));
      nextLayer[(tx * 2 + 0) + (ty * 2 + 0) * width] = PixelBox(b.minX, splitX, b.minY,
        splitYLeft);
      nextLayer[(tx * 2 + 0) + (ty * 2 + 1) * width] = PixelBox(b.minX, splitX, splitYLeft + 1,
        b.maxY);

      double splitYRight = _calculateSplitY(PixelBox(splitX + 1, b.maxX, b.minY, b.maxY));
      nextLayer[(tx * 2 + 1) + (ty * 2 + 0) * width] = PixelBox(splitX + 1, b.maxX, b.minY,
        splitYRight);
      nextLayer[(tx * 2 + 1) + (ty * 2 + 1) * width] = PixelBox(splitX + 1, b.maxX, splitYRight + 1,
        b.maxY);
    }

    boxes = nextLayer;
  }

  vector< vector<Envelope> > result;

  long maxNodeCount = 0;
  result.resize(width);
  for (size_t tx = 0; tx < width; tx++)
  {
    result[tx].resize(width);
    for (size_t ty = 0; ty < width; ty++)
    {
      PixelBox& pb = boxes[tx + ty * width];
      maxNodeCount = std::max(maxNodeCount, _sumPixels(pb));
      if (pb.getWidth() < 3 || pb.getHeight() < 3)
      {
        throw HootException("PixelBox must be at least 3 pixels wide and tall.");
      }
      result[tx][ty] = _toEnvelope(pb);
    }
  }
  LOG_INFO("Max node count in one tile: " << maxNodeCount);
  _exportResult(boxes, "tmp/result.png");

  return result;
}

int TileBoundsCalculator::_calculateSplitX(PixelBox& b)
{
  double total = _sumPixels(b);

  double left = _sumPixels(b.getColumnBox(b.minX));

  int best = (b.maxX + b.minX) / 2;
  double bestSum = numeric_limits<double>::max();

  double thisSlop = _slop + 1.0 / (double)(b.maxX - b.minX);

  if (b.getWidth() < 6)
  {
    throw HootException("The input box must be at least six pixels high.");
  }

  for (int c = b.minX + 2; c < b.maxX - 2; c++)
  {
    double colSum = _sumPixels(b.getColumnBox(c));
    double colSumMin = _sumPixels(b.getColumnBox(c), _min) +
      _sumPixels(b.getColumnBox(c + 1), _min);
    left += colSum;

    double slop = abs(0.5 - left / total);
    if ((slop < thisSlop) && colSumMin < bestSum)
    {
      best = c;
      bestSum = colSumMin;
    }
  }

  if (bestSum == numeric_limits<double>::max())
  {
    LOG_WARN("bestSum isn't valid. " << b.toString());
  }

  return best;
}

int TileBoundsCalculator::_calculateSplitY(const PixelBox& b)
{
  double total = _sumPixels(b);

  double bottom = _sumPixels(b.getRowBox(b.minY));

  int best = (b.maxY + b.minY) / 2;
  double bestSum = numeric_limits<double>::max();

  double thisSlop = _slop + 1.0 / (double)(b.maxY - b.minY);

  if (b.getHeight() < 6)
  {
    throw HootException("The input box must be at least six pixels high.");
  }

  for (int r = b.minY + 2; r < b.maxY - 2; r++)
  {
    double rowSum = _sumPixels(b.getRowBox(r));
    double rowSumMin = _sumPixels(b.getRowBox(r), _min) + _sumPixels(b.getRowBox(r + 1), _min);
    bottom += rowSum;

    double slop = abs(0.5 - bottom / total);
    if ((slop < thisSlop) && rowSumMin < bestSum)
    {
      best = r;
      bestSum = rowSumMin;
    }
  }

  if (bestSum == numeric_limits<double>::max())
  {
    LOG_WARN("bestSum isn't valid. " << b.toString() << " total: " << total << " size: " <<
             b.maxY - b.minY);
  }

  return best;
}

void TileBoundsCalculator::_countNode(const shared_ptr<Node>& n)
{
  double x = n->getX();
  double y = n->getY();

  int px = (x - _envelope.MinX) / _pixelSize;
  int py = (y - _envelope.MinY) / _pixelSize;

  if (px < 0 || px >= _r1.cols || py < 0 || py >= _r1.rows)
  {
    throw HootException("Pixel out of bounds.");
  }
  else
  {
    int32_t* row;
    if (n->getStatus() == Status::Unknown1)
    {
      row = _r1.ptr<int32_t>(py);
      row[px] += 1.0;
    }
    else if (n->getStatus() == Status::Unknown2)
    {
      row = _r2.ptr<int32_t>(py);
      row[px] += 1.0;
    }
  }
}

double TileBoundsCalculator::_evaluateSplitPoint(const PixelBox& pb, const Pixel& p)
{
  // This function has two goals:
  // * minimize the number of nodes intersected by a split
  // * minimize the difference between quadrant counts, or ignore the quadrant counts if all
  //   the quadrants are below _maxNodesPerBox
  //
  // Smaller scores are better.

  double llSum = _sumPixels(PixelBox(pb.minX, p.x, pb.minY, p.y));
  double ulSum = _sumPixels(PixelBox(pb.minX, p.x, p.y + 1, pb.maxY));
  double lrSum = _sumPixels(PixelBox(p.x + 1, pb.maxX, pb.minY, p.y));
  double urSum = _sumPixels(PixelBox(p.x + 1, pb.maxX, p.y + 1, pb.maxY));

  double total = llSum + ulSum + lrSum + urSum;
# ifdef DEBUG
    assert(fabs(total - _sumPixels(pb)) < 0.1);
# endif

  double avg = total / 4.0;

  double slop = fabs(llSum - avg) / avg;
  slop += fabs(ulSum - avg) / avg;
  slop += fabs(lrSum - avg) / avg;
  slop += fabs(urSum - avg) / avg;
  slop /= 4.0;

  double slopMultiplier;
  if (slop > _slop)
  {
    slopMultiplier = 2.0 + slop;
  }
  else
  {
    slopMultiplier = 1.0 + slop;
  }

  double intersects = 0.0;
  intersects += _sumPixels(PixelBox(p.x, p.x, pb.minY, pb.maxY));
  intersects += _sumPixels(PixelBox(pb.minX, pb.maxX, p.y, p.y));

  return intersects * slopMultiplier;
}

void TileBoundsCalculator::_exportImage(cv::Mat &r, QString output)
{
  QImage qImage(r.cols, r.rows, QImage::Format_RGB16);
  if (qImage.isNull())
  {
    throw HootException(QString("Unable to allocate image of size %1x%2").arg(r.cols).
      arg(r.rows));
  }
  QPainter pt(&qImage);
  pt.setRenderHint(QPainter::Antialiasing, false);
  pt.fillRect(pt.viewport(), Qt::black);
  QPen pen;
  pen.setWidth(0);
  pen.setColor(qRgb(1, 0, 0));
  pt.setPen(pen);

  LOG_INFO("max value: " << _maxValue);

  for (int y = 0; y < r.rows; y++)
  {
    int32_t* row = r.ptr<int32_t>(y);
    for (int x = 0; x < r.cols; x++)
    {
      double l;
      if (row[x] == 0)
      {
        l = 0.0;
      }
      else
      {
        l = log(row[x]) / log(_maxValue);
      }
      int v = l * 255;
      qImage.setPixel(x, r.rows - y - 1, qRgb(v, v, 50));
    }
  }

  qImage.save(output);
}

void TileBoundsCalculator::_exportResult(const vector<PixelBox>& boxes, QString output)
{
  QImage qImage(_r1.cols, _r1.rows, QImage::Format_RGB16);
  if (qImage.isNull())
  {
    throw HootException(QString("Unable to allocate image of size %1x%2").arg(_r1.cols).
      arg(_r1.rows));
  }
  QPainter pt(&qImage);
  pt.setRenderHint(QPainter::Antialiasing, false);
  pt.fillRect(pt.viewport(), Qt::black);
  QPen pen;
  pen.setWidth(0);
  pen.setColor(qRgb(1, 0, 0));
  pt.setPen(pen);

  LOG_INFO("max value: " << _maxValue);

  for (int y = 0; y < _r1.rows; y++)
  {
    int32_t* row1 = _r1.ptr<int32_t>(y);
    int32_t* row2 = _r2.ptr<int32_t>(y);
    for (int x = 0; x < _r1.cols; x++)
    {
      double l1 = row1[x] <= 0 ? 0.0 : log(row1[x]) / log(_maxValue);
      double l2 = row2[x] <= 0 ? 0.0 : log(row2[x]) / log(_maxValue);

      qImage.setPixel(x, _r1.rows - y - 1, qRgb(l1 * 255, l2 * 255, 0));
    }
  }

  pt.setPen(QPen(QColor(0, 0, 255, 100)));
  for (size_t i = 0; i < boxes.size(); i++)
  {
    const PixelBox& b = boxes[i];
    pt.drawRect(b.minX, _r1.rows - b.maxY - 1, b.maxX - b.minX, b.maxY - b.minY);
  }

  qImage.save(output);
}

bool TileBoundsCalculator::_isDone(vector<PixelBox> &boxes)
{
  bool smallEnough = true;
  bool minSize = false;

  for (size_t i = 0; i < boxes.size(); i++)
  {
    PixelBox& b = boxes[i];
    if (b.getWidth() == 1 || b.getHeight() == 1)
    {
      minSize = true;
    }

    if (_sumPixels(b) > _maxNodesPerBox)
    {
      smallEnough = false;
    }
  }

  if (minSize == true && smallEnough == false)
  {
    throw HootException("Could not find a solution. Try reducing the pixel size or increasing the "
      "max nodes per pixel value.");
  }
  else
  {
    return smallEnough;
  }
}

void TileBoundsCalculator::renderImage(shared_ptr<OsmMap> map)
{
  _envelope = CalculateBoundsVisitor::getBounds(map);

  renderImage(map, _r1, _r2);

  _calculateMin();

  _exportImage(_r1, "tmp/r1.png");
  _exportImage(_r2, "tmp/r2.png");
  _exportImage(_min, "tmp/min.png");
}

void TileBoundsCalculator::renderImage(shared_ptr<OsmMap> map, cv::Mat& r1, cv::Mat& r2)
{
  _envelope = CalculateBoundsVisitor::getBounds(map);

  int w = ceil((_envelope.MaxX - _envelope.MinX) / _pixelSize) + 1;
  int h = ceil((_envelope.MaxY - _envelope.MinY) / _pixelSize) + 1;

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

  const NodeMap& nm = map->getNodeMap();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); it++)
  {
    const shared_ptr<Node>& n = it->second;
    _countNode(n);
  }

  r1 = _r1;
  r2 = _r2;
}

void TileBoundsCalculator::setImages(const cv::Mat& r1, const cv::Mat& r2)
{
  _r1 = r1;
  _r2 = r2;

  _calculateMin();

  _exportImage(_r1, "tmp/r1.png");
  _exportImage(_r2, "tmp/r2.png");
  _exportImage(_min, "tmp/min.png");
}

long TileBoundsCalculator::_sumPixels(const PixelBox& pb, cv::Mat& r)
{
  long sum = 0.0;

  for (int py = pb.minY; py <= pb.maxY; py++)
  {
    int32_t* row = r.ptr<int32_t>(py);

    for (int px = pb.minX; px <= pb.maxX; px++)
    {
      sum += row[px];
    }
  }

  return sum;
}

Envelope TileBoundsCalculator::_toEnvelope(const PixelBox& pb)
{
  return Envelope(_envelope.MinX + pb.minX * _pixelSize,
    _envelope.MinX + (pb.maxX + 1) * _pixelSize,
    _envelope.MinY + pb.minY * _pixelSize,
    _envelope.MinY + (pb.maxY + 1) * _pixelSize);
}

}
