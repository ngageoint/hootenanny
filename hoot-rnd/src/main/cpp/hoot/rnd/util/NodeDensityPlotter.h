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
#ifndef NODE_DENSITY_PLOTTER_H
#define NODE_DENSITY_PLOTTER_H

// hoot
#include <hoot/core/util/OpenCv.h>

// Qt
#include <QImage>

namespace hoot
{

class OsmMapReader;

/**
 * Plots nodes to a raster iamge
 */
class NodeDensityPlotter
{
public:

  NodeDensityPlotter();
  ~NodeDensityPlotter() = default;

  /**
   * Writes a node density plot to a raster image output
   *
   * @param input path to the input map containing the nodes to plot
   * @param output path to a raster output image
   */
  void plot(const QString& input, const QString& output) const;

  /**
   * Converts the string representation of a number to a numeric color band
   *
   * @param c the string to convert
   * @return a numeric color band value in the range of 0 to 255
   */
  int toColorBand(const QString& c) const;

  /**
   * Converts the string representation of a number to a numeric color portion
   *
   * @param c the string to convert
   * @return a numeric color portion value in the range of 0 to 255
   */
  int toColorPortion(const QString& c) const;

  void setBaseColors(const QRgb& colors) { _baseColors = colors; }
  void setColorMultiplier(const std::vector<double>& multiplier) { _colorMultiplier = multiplier; }
  void setMaxSize(const double size) { _maxSize = size; }

private:

  QRgb _baseColors;
  std::vector<double> _colorMultiplier;
  double _maxSize;

  double _getPixelSize(const geos::geom::Envelope& env) const;
  geos::geom::Envelope _getEnvelope(const std::shared_ptr<OsmMapReader>& reader) const;

  cv::Mat _calculateDensity(
    const geos::geom::Envelope& envelope, const double pixelSize,
    const std::shared_ptr<OsmMapReader>& reader) const;

  std::shared_ptr<QImage> _createImage(const cv::Mat& mat) const;
  void _writeImage(
    const std::shared_ptr<QImage>& image, const double pixelSize, const geos::geom::Envelope& env,
    const cv::Mat& mat, const QString& output) const;
};

}

#endif // NODE_DENSITY_PLOTTER_H
