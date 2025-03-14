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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "RasterComparator.h"

// GEOS
#include <geos/geom/LineString.h>

// Hoot
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryPainter.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/WaysVisitor.h>

// Qt
#include <QImage>
#include <QPainter>

using namespace std;

namespace hoot
{

class PaintVisitor : public ConstElementVisitor
{
public:

  PaintVisitor(OsmMapPtr map, QPainter& pt, QMatrix& m)
    : _map(map), _pt(pt), _m(m)
  { }
  ~PaintVisitor() override = default;

  void visit(const ConstElementPtr& e) override
  {
    vector<ConstWayPtr> ways = WaysVisitor::extractWays(_map, e);

    for (const auto& way : ways)
      GeometryPainter::drawWay(_pt, _map.get(), way.get(), _m);

    _numAffected = static_cast<long>(ways.size());
  }

  QString getDescription() const override { return ""; }
  QString getName() const override { return ""; }
  QString getClassName() const override { return ""; }

private:

  OsmMapPtr _map;
  QPainter& _pt;
  QMatrix& _m;
};

RasterComparator::RasterComparator(const std::shared_ptr<OsmMap>& map1, const std::shared_ptr<OsmMap>& map2)
  : BaseComparator(map1, map2),
    _wayLengthSum(0.0),
    _ignoreFilter(ConfigOptions().getCompareIncludeAllElements())
{
}

double RasterComparator::compareMaps()
{
  LOG_STATUS("Comparing maps using raster scoring...");

  _updateBounds();
  _wayLengthSum = 0.0;

  cv::Mat image1;
  cv::Mat image2;

  _renderImage(_mapP1, image1);
  _renderImage(_mapP2, image2);

  CvMat i1 = image1;
  CvMat i2 = image2;
  double min;
  double max;
  double minTemp;
  double maxTemp;
  CvPoint p1, p2;

  cvMinMaxLoc(&i1, &min, &max, &p1, &p2);
  cvMinMaxLoc(&i2, &minTemp, &maxTemp, &p1, &p2);
  min = std::min(min, minTemp);
  max = std::max(max, maxTemp);

  LOG_STATUS("Calculating error...");
  double error = _calculateError(image1, image2);

  IplImage* diffImage = cvCreateImage( cvSize(_width, _height), IPL_DEPTH_32F, 1);
  cv::Mat diff = cv::cvarrToMat(diffImage);

  LOG_STATUS("Calculating difference...");
  const float* image1Data = image1.ptr<float>(0);
  const float* image2Data = image2.ptr<float>(0);
  float* diffData = diff.ptr<float>(0);
  size_t size = (image1.dataend - image1.datastart) / sizeof(float);
  for (size_t i = 0; i < size; i++)
    diffData[i] = fabs(image1Data[i] - image2Data[i]);

  _saveImage(diff, "test-output/diff.png", max);
  _saveImage(image1, "test-output/image1.png", max);
  _saveImage(image2, "test-output/image2.png", max);

  cvReleaseImage(&diffImage);

  return 1 - error;
}

void RasterComparator::_dumpImage(cv::Mat& image) const
{
  printf("\n");
  for (int y = 0; y < _height; y++)
  {
    const float* row = image.ptr<float>(y);
    for (int x = 0; x < _width; x++)
      printf("%.2g ", row[x]);
    printf("\n");
  }
}

void RasterComparator::_renderImage(const std::shared_ptr<OsmMap>& map, cv::Mat& image) const
{
  QImage qImage(_width, _height, QImage::Format_ARGB32);
  QPainter pt(&qImage);
  pt.setRenderHint(QPainter::Antialiasing, false);
  pt.fillRect(pt.viewport(), Qt::black);
  QPen pen;
  pen.setWidth(0);
  pen.setColor(qRgb(1, 0, 0));
  pt.setPen(pen);

  QMatrix m = GeometryPainter::createMatrix(pt.viewport(), _projectedBounds);

  PaintVisitor pv(map, pt, m);
  if (!_ignoreFilter)
  {
    HighwayCriterion crit(map);
    FilteredVisitor v(crit, pv);
    map->visitRo(v);
  }
  else
    map->visitRo(pv);

  if (pv.getNumFeaturesAffected() < 1)
    throw EmptyMapInputException();

  cv::Mat in(cvSize(_width, _height), CV_32FC1);
  image = cv::Mat(cvSize(_width, _height), CV_32FC1);

  int pixelCtr = 0;
  for (int y = 0; y < _height; y++)
  {
    float* row = in.ptr<float>(y);
    for (int x = 0; x < _width; x++)
    {
      row[x] = static_cast<float>(qRed(qImage.pixel(x, y)) * _pixelSize);

      pixelCtr++;
      if (pixelCtr % (_taskStatusUpdateInterval * 1000) == 0)
      {
        PROGRESS_STATUS(
          "Rendered " << StringUtils::formatLargeNumber(pixelCtr) << " of " <<
          StringUtils::formatLargeNumber(_height * _width) << " pixels.");
      }
    }
  }

  int ks = static_cast<int>(ceil(_sigma / _pixelSize * 3) * 2 + 1);
  cv::GaussianBlur(in, image, cvSize(ks, ks), _sigma / _pixelSize, _sigma / _pixelSize, cv::BORDER_CONSTANT);
}

}
