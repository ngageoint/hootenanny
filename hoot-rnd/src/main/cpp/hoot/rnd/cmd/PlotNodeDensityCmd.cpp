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

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/EnvelopeProvider.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QImage>
#include <QElapsedTimer>

// Standard
#include <fstream>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class PlotNodeDensityCmd : public BaseCommand
{

  public:

    static QString className() { return "hoot::PlotNodeDensityCmd"; }

    PlotNodeDensityCmd() = default;

    QString getName() const override { return "plot-node-density"; }
    QString getDescription() const override { return "Creates a node density plot for a map"; }
    QString getType() const { return "rnd"; }

    Envelope getEnvelope(const std::shared_ptr<OsmMapReader>& reader) const
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
        Envelope result;
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
              result = Envelope(n->getX(), n->getX(), n->getY(), n->getY());
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
        OsmMapPtr map(new OsmMap());
        reader->setUseDataSourceIds(true);
        reader->read(map);
        return CalculateMapBoundsVisitor::getGeosBounds(map);
      }
    }

    cv::Mat calculateDensity(const Envelope& envelope, double pixelSize,
                             std::shared_ptr<OsmMapReader> reader) const
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

    int toColorBand(QString c) const
    {
      bool ok;
      int result = c.toInt(&ok);
      if (!ok || result < 0 || result > 255)
      {
        throw HootException("Expected the color to be a number in the range [0-255]");
      }
      return result;
    }

    int toColorPortion(QString c) const
    {
      bool ok;
      double result = c.toDouble(&ok);
      if (!ok || result < 0 || result > 255)
      {
        throw HootException("Expected the color portion to be a number in the range [0-255]");
      }
      return result;
    }

    int runSimple(QStringList& args) override
    {
      QElapsedTimer timer;
      timer.start();

      if (args.size() < 3 || args.size() > 5)
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
      }

      bool ok;
      QString input = args[0];
      QString output = args[1];
      double maxSize = args[2].toDouble(&ok);
      if (!ok || maxSize < 1)
      {
        throw HootException("Expected a number > 0 for max size.");
      }

      LOG_STATUS(
        "Plotting node density for ..." << FileUtils::toLogFormat(input, 25) <<
        " and writing output to ..." << FileUtils::toLogFormat(output, 25) << "...");

      // initialize to black
      QRgb baseColors = qRgba(0, 0, 0, 255);
      if (args.size() >= 4)
      {
        QStringList bs = args[3].split(",");
        if (bs.size() != 4)
        {
          throw HootException("Expected base-colors to be RGBA and comma delimited.");
        }
        baseColors = qRgba(toColorBand(bs[0]), toColorBand(bs[1]), toColorBand(bs[2]),
          toColorBand(bs[3]));
      }
      vector<double> colorMultiplier(4, 255.0);
      colorMultiplier[3] = 0.0;
      if (args.size() >= 5)
      {
        QStringList bs = args[4].split(",");
        if (bs.size() != 4)
        {
          throw HootException("Expected base-colors to be RGBA and comma delimited.");
        }
        colorMultiplier[0] = toColorPortion(bs[0]);
        colorMultiplier[1] = toColorPortion(bs[1]);
        colorMultiplier[2] = toColorPortion(bs[2]);
        colorMultiplier[3] = toColorPortion(bs[3]);
      }

      std::shared_ptr<OsmMapReader> reader =
        OsmMapReaderFactory::createReader(input, true);
      reader->open(input);
      Envelope e = getEnvelope(reader);
      LOG_DEBUG("Envelope: " << GeometryUtils::toString(e));

      double pixelSize;
      if (e.getWidth() > e.getHeight())
      {
        pixelSize = e.getWidth() / maxSize;
      }
      else
      {
        pixelSize = e.getHeight() / maxSize;
      }

      reader = OsmMapReaderFactory::createReader(input, true);
      reader->open(input);
      cv::Mat mat = calculateDensity(e, pixelSize, reader);
      Envelope imageEnvelope(e.getMinX(), e.getMinX() + pixelSize * mat.size().width,
                             e.getMinY(), e.getMinY() + pixelSize * mat.size().height);

      QImage qImage(mat.size().width, mat.size().height, QImage::Format_ARGB32);

      int32_t maxValue = 0;
      for (int y = 0; y < qImage.height(); y++)
      {
        const int32_t* row = mat.ptr<int32_t>(y);
        for (int x = 0; x < qImage.width(); x++)
        {
          maxValue = std::max(row[x], maxValue);
        }
      }
      QRgb rgb;
      for (int y = 0; y < qImage.height(); y++)
      {
        const int32_t* row = mat.ptr<int32_t>(y);
        for (int x = 0; x < qImage.width(); x++)
        {
          double v = log1p(row[x]) / log(maxValue);
          int r = max(0, min<int>(255, v * colorMultiplier[0] + qRed(baseColors)));
          int g = max(0, min<int>(255, v * colorMultiplier[1] + qGreen(baseColors)));
          int b = max(0, min<int>(255, v * colorMultiplier[2] + qBlue(baseColors)));
          int a = max(0, min<int>(255, v * colorMultiplier[3] + qAlpha(baseColors)));
          rgb = qRgba(r, g, b, a);
          qImage.setPixel(x, qImage.height() - y - 1, rgb);
        }
      }
      LOG_VARD(maxValue);

      qImage.save(output);

      // based on the definition here: http://en.wikipedia.org/wiki/World_file
      fstream pngw;
      pngw.exceptions(ios::failbit | ios_base::badbit);
      pngw.open((output + "w").toUtf8().constData(), ios_base::out | ios_base::trunc);
      pngw << pixelSize << endl;
      pngw << 0 << endl;
      pngw << 0 << endl;
      pngw << -pixelSize << endl;
      pngw << e.getMinX() + pixelSize / 2.0 << endl;
      pngw << e.getMinY() + (mat.size().height - 1) * pixelSize - pixelSize / 2.0 << endl;

      pngw.close();

      LOG_STATUS(
        "Node density plotted in " << StringUtils::millisecondsToDhms(timer.elapsed()) <<
         " total.");

      return 0;
    }
};

HOOT_FACTORY_REGISTER(Command, PlotNodeDensityCmd)

}
