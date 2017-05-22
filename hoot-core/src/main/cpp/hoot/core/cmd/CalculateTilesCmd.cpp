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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/EnvelopeProvider.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/OpenCv.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QImage>

// Standard
#include <fstream>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class CalculateTilesCmd : public BaseCommand
{

  public:

    static string className() { return "hoot::CalculateTilesCmd"; }

    CalculateTilesCmd() { }

    virtual QString getName() const { return "calculate-tiles"; }

    virtual int runSimple(QStringList args)
    {
      if (args.size() != 2)
      {
        cout << getHelp() << endl << endl;
        throw HootException(QString("%1 takes two parameters.").arg(getName()));
      }

      QString input = args[0];
      QString output = args[1];

//      boost::shared_ptr<OsmMapReader> reader =
//        OsmMapReaderFactory::getInstance().createReader(input, true);
//      reader->open(input);
//      Envelope e = getEnvelope(reader);
//      LOG_INFO("Envelope: " << GeometryUtils::toString(e));

//      double pixelSize;
//      if (e.getWidth() > e.getHeight())
//      {
//        pixelSize = e.getWidth() / maxSize;
//      }
//      else
//      {
//        pixelSize = e.getHeight() / maxSize;
//      }

//      reader = OsmMapReaderFactory::getInstance().createReader(input, true);
//      reader->open(input);
//      cv::Mat mat = calculateDensity(e, pixelSize, reader);

      return 0;
    }
};

HOOT_FACTORY_REGISTER(Command, CalculateTilesCmd)

}
