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
 * @copyright Copyright (C) 2013, 2014 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Log.h>
#include <hoot/hadoop/stats/MapStatsDriver.h>
#include <hoot/hadoop/paint-nodes/PaintNodesDriver.h>

// Qt
#include <QImage>
#include <QString>
#include <QStringList>

namespace hoot
{

class BigPaintNodesCmd : public BaseCommand
{
public:

  static string className() { return "hoot::BigPaintNodesCmd"; }

  BigPaintNodesCmd() { }

  virtual QString getName() const { return "big-paint-nodes"; }

  //int BigPaintNodesCmd::run(char *argv[], int argc)
  virtual int runSimple(QStringList args)
  {
    //if (argc != 5)
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes exactly three parameters.").arg(getName()));
    }
    //QString in = argv[2];
    //QString out = argv[4];
    QString in = args[0];
    QString out = args[2];

    bool ok;
    //double pixelSize = QString(argv[3]).toDouble(&ok);
    double pixelSize = QString(args[1]).toDouble(&ok);
    if (ok == false)
    {
      throw HootException("Unable to parse pixelSize.");
    }
    if (pixelSize <= 0)
    {
      throw HootException("pixelSize must be positive.");
    }

    cout << Log::getInstance().getLevel() << endl;

    // calculate the envelope of the map.
    MapStats stats = MapStatsDriver().calculateStats(in);
    Envelope e = stats.getEnvelope();

    PaintNodesDriver driver;
    const cv::Mat& mat = driver.calculateDensity(e, pixelSize, in);

    cout << "Got matrix" << endl;
    cout.flush();

    QImage qImage(mat.size().width, mat.size().height, QImage::Format_ARGB32);

    int32_t max = 0;
    for (int y = 0; y < qImage.height(); y++)
    {
      const int32_t* row = mat.ptr<int32_t>(y);
      for (int x = 0; x < qImage.width(); x++)
      {
        max = std::max(row[x], max);
      }
    }
    QRgb rgb;
    for (int y = 0; y < qImage.height(); y++)
    {
      const int32_t* row = mat.ptr<int32_t>(y);
      for (int x = 0; x < qImage.width(); x++)
      {
        int c = (255 * log(row[x] + 1)) / log(max);
        rgb = qRgb(c, c, c);
        qImage.setPixel(x, qImage.height() - y - 1, rgb);
      }
    }
    LOG_VAR(max);

    qImage.save(out);

    return 0;
  }

};

HOOT_FACTORY_REGISTER(Command, BigPaintNodesCmd)

}
