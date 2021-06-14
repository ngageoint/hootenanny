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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "NodeDensityPlotter.h"

// Hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/EnvelopeProvider.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QElapsedTimer>

// Standard
#include <fstream>

namespace hoot
{

NodeDensityPlotter::NodeDensityPlotter() :
_maxSize(-1.0)
{
}

void NodeDensityPlotter::plot(const QString& input, const QString& output) const
{
  if (_maxSize < 1.0)
  {
    throw IllegalArgumentException("Expected a number greater 1.0 for max size.");
  }

  QElapsedTimer timer;
  timer.start();

  LOG_STATUS(
    "Plotting node density for ..." << FileUtils::toLogFormat(input, 25) <<
    " and writing output to ..." << FileUtils::toLogFormat(output, 25) << "...");

  std::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(input, true);
  reader->open(input);
  geos::geom::Envelope env = _getEnvelope(reader);
  LOG_DEBUG("Envelope: " << GeometryUtils::toString(env));

  const double pixelSize = _getPixelSize(env);

  reader = OsmMapReaderFactory::createReader(input, true);
  reader->open(input);
  cv::Mat mat = _calculateDensity(env, pixelSize, reader);

  _writeImage(_createImage(mat), pixelSize, env, mat, output);

  LOG_STATUS(
    "Node density plotted in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");
}

void NodeDensityPlotter::_writeImage(
  const std::shared_ptr<QImage>& image, const double pixelSize, const geos::geom::Envelope& env,
  const cv::Mat& mat, const QString& output) const
{
  image->save(output);

  // based on the definition here: http://en.wikipedia.org/wiki/World_file
  std::fstream pngw;
  pngw.exceptions(std::ios::failbit | std::ios_base::badbit);
  pngw.open((output + "w").toUtf8().constData(), std::ios_base::out | std::ios_base::trunc);
  pngw << pixelSize << std::endl;
  pngw << 0 << std::endl;
  pngw << 0 << std::endl;
  pngw << -pixelSize << std::endl;
  pngw << env.getMinX() + pixelSize / 2.0 << std::endl;
  pngw << env.getMinY() + (mat.size().height - 1) * pixelSize - pixelSize / 2.0 << std::endl;

  pngw.close();
}

std::shared_ptr<QImage> NodeDensityPlotter::_createImage(const cv::Mat& mat) const
{
  std::shared_ptr<QImage> qImage =
    std::make_shared<QImage>(mat.size().width, mat.size().height, QImage::Format_ARGB32);

  int32_t maxValue = 0;
  for (int y = 0; y < qImage->height(); y++)
  {
    const int32_t* row = mat.ptr<int32_t>(y);
    for (int x = 0; x < qImage->width(); x++)
    {
      maxValue = std::max(row[x], maxValue);
    }
  }
  QRgb rgb;
  for (int y = 0; y < qImage->height(); y++)
  {
    const int32_t* row = mat.ptr<int32_t>(y);
    for (int x = 0; x < qImage->width(); x++)
    {
      double v = log1p(row[x]) / log(maxValue);
      int r = std::max(0, std::min<int>(255, std::static_cast<int>(v * _colorMultiplier[0] + qRed(_baseColors))));
      int g = std::max(0, std::min<int>(255, std::static_cast<int>(v * _colorMultiplier[1] + qGreen(_baseColors))));
      int r = std::max(0, std::min<int>(255, std::static_cast<int>(v * _colorMultiplier[2] + qBlue(_baseColors))));
      int a = std::max(0, std::min<int>(255, std::static_cast<int>(v * _colorMultiplier[3] + qAlpha(_baseColors))));
      rgb = qRgba(r, g, b, a);
      qImage->setPixel(x, qImage->height() - y - 1, rgb);
    }
  }
  LOG_VARD(maxValue);

  return qImage;
}

double NodeDensityPlotter::_getPixelSize(const geos::geom::Envelope& env) const
{
  double pixelSize;
  if (env.getWidth() > env.getHeight())
  {
    pixelSize = env.getWidth() / _maxSize;
  }
  else
  {
    pixelSize = env.getHeight() / _maxSize;
  }
  return pixelSize;
}

geos::geom::Envelope NodeDensityPlotter::_getEnvelope(
  const std::shared_ptr<OsmMapReader>& reader) const
{
  std::shared_ptr<EnvelopeProvider> ep =
    std::dynamic_pointer_cast<EnvelopeProvider>(reader);
  std::shared_ptr<PartialOsmMapReader> r =
    std::dynamic_pointer_cast<PartialOsmMapReader>(reader);

  if (ep)
  {
    return ep->calculateEnvelope();
  }
  else if (r)
  {
    long nodeCount = 0;
    geos::geom::Envelope result;
    reader->setUseDataSourceIds(true);

    while (r->hasMoreElements())
    {
      ElementPtr e = r->readNextElement();

      if (e->getElementType() == ElementType::Node)
      {
        nodeCount++;
        NodePtr n = std::dynamic_pointer_cast<Node>(e);
        if (result.isNull())
        {
          result = geos::geom::Envelope(n->getX(), n->getX(), n->getY(), n->getY());
        }
        else
        {
          result.expandToInclude(n->getX(), n->getY());
        }
      }
    }
    LOG_VARD(nodeCount);

    return result;
  }
  else
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    reader->setUseDataSourceIds(true);
    reader->read(map);
    return CalculateMapBoundsVisitor::getGeosBounds(map);
  }
}

cv::Mat NodeDensityPlotter::_calculateDensity(
  const geos::geom::Envelope& envelope, const double pixelSize,
  const std::shared_ptr<OsmMapReader>& reader) const
{
  std::shared_ptr<PartialOsmMapReader> r =
    std::dynamic_pointer_cast<PartialOsmMapReader>(reader);
  r->setUseDataSourceIds(true);

  int width = ceil(envelope.getWidth() / pixelSize);
  int height = ceil(envelope.getHeight() / pixelSize);

  cv::Mat c(cvSize(width, height), CV_32SC1, 0.0);

  long count = 0;
  while (r->hasMoreElements())
  {
    ElementPtr e = r->readNextElement();

    if (e.get() && e->getElementType() == ElementType::Node)
    {
      NodePtr n = std::dynamic_pointer_cast<Node>(e);
      int px = int((n->getX() - envelope.getMinX()) / pixelSize);
      int py = int((n->getY() - envelope.getMinY()) / pixelSize);
      px = std::min(width - 1, std::max(0, px));
      py = std::min(height - 1, std::max(0, py));

      c.ptr<int32_t>(py)[px] = c.ptr<int32_t>(py)[px] + 1;
      count++;
    }
  }

  LOG_VARD(count);

  return c;
}

int NodeDensityPlotter::toColorBand(const QString& c) const
{
  bool ok;
  int result = c.toInt(&ok);
  if (!ok || result < 0 || result > 255)
  {
    throw IllegalArgumentException("Expected the color to be a number in the range [0-255]");
  }
  return result;
}

int NodeDensityPlotter::toColorPortion(const QString& c) const
{
  bool ok;
  double result = c.toDouble(&ok);
  if (!ok || result < 0 || result > 255)
  {
    throw IllegalArgumentException("Expected the color portion to be a number in the range [0-255]");
  }
  return result;
}

}
