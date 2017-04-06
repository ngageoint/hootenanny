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

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QFile>

namespace hoot
{

class ApplyRubberSheetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ApplyRubberSheetCmd"; }

  ApplyRubberSheetCmd() { }

  virtual QString getName() const { return "apply-rubber-sheet"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    QString transformPath = args[0];

    boost::shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, args[1], true, Status::Unknown1);
    QString outputPath = args[2];

    // make sure rubber sheeting isn't applied during cleaning.
    QStringList l = ConfigOptions().getMapCleanerTransforms();
    l.removeAll(QString::fromStdString(RubberSheet::className()));
    conf().set(MapCleaner::opsKey(), l);
    MapCleaner().apply(map);

    RubberSheet rs;
    rs.setReference(false);

    QFile fp(transformPath);
    if (fp.open(QIODevice::ReadOnly) == false)
    {
      throw HootException(QString("Error opening %1 for writing").arg(transformPath));
    }
    rs.readTransform1to2(fp);

    rs.applyTransform(map);

    MapProjector::projectToWgs84(map);

    saveMap(map, outputPath);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ApplyRubberSheetCmd)

}

