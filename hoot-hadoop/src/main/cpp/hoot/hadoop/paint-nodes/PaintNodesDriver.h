/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef PAINTNODESDRIVER_H
#define PAINTNODESDRIVER_H

// Hoot
#include <hoot/core/util/OpenCv.h>
#include <hoot/hadoop/Driver.h>

// Qt
#include <QString>

// geos
#include <geos/geom/Envelope.h>

namespace hoot
{

class PaintNodesDriver : public Driver
{
public:
  PaintNodesDriver();

  /**
   * Calculate the density on the given input. The MapStats will also be calculated and stored back
   * into the input directory as "all.stats".
   * @param input Input directory containing .pbf file(s). This must be a directory.
   */
  const cv::Mat& calculateDensity(const geos::geom::Envelope& e, double pixelSize, QString input);

private:
  /// Number of nodes per pixel
  cv::Mat _c;

  int _width, _height;

  void _loadOutputDir(const QString& output);

  void _loadPartFile(const QString& partPath);
};

}

#endif // PAINTNODESDRIVER_H
