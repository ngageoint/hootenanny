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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

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
 * @copyright Copyright (C) 2012, 2013 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/SuperfluousWayRemover.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QStringList>

namespace hoot
{

class CropMapCmd : public BaseCommand
{
public:
  static string className() { return "hoot::CropMapCmd"; }

  CropMapCmd() {}

  virtual QString getName() const { return "crop-map"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    QString in = args[0];
    QString out = args[1];
    QString bounds = args[2];

    bool allOk = true;
    bool ok;
    QStringList boundsArr = bounds.split(",");
    double left = boundsArr[0].toDouble(&ok);
    allOk &= ok;
    double bottom = boundsArr[1].toDouble(&ok);
    allOk &= ok;
    double right = boundsArr[2].toDouble(&ok);
    allOk &= ok;
    double top = boundsArr[3].toDouble(&ok);
    allOk &= ok;

    if (allOk == false)
    {
      throw HootException("Invalid bounds format.");
    }

    Envelope env(left, right, bottom, top);

    boost::shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, in, true);

    MapCropper::crop(map, env);
    SuperfluousWayRemover::removeWays(map);
    SuperfluousNodeRemover().apply(map);

    saveMap(map, out);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CropMapCmd)

}
