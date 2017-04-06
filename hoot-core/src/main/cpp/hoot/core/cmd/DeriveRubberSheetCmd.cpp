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
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/OsmMap.h>

// Qt
#include <QFile>

namespace hoot
{

class DeriveRubberSheetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveRubberSheetCmd"; }

  DeriveRubberSheetCmd() { }

  virtual QString getName() const { return "derive-rubber-sheet"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 5)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }

    boost::shared_ptr<OsmMap> map(new OsmMap());
    bool ref = false;
    if (args[0] == "--ref")
    {
      ref = true;
      args.pop_front();
    }
    loadMap(map, args[0], false, Status::Unknown1);
    loadMap(map, args[1], false, Status::Unknown2);
    QString transform2to1Path = args[2];
    QString transform1to2Path;
    if (args.size() == 4)
    {
      transform1to2Path = args[3];
    }
    else if (ref == false)
    {
      throw HootException(QString("You must specify a transform1to2.rs argument if --ref isn't "
        "specified."));
    }

    // make sure rubber sheeting isn't applied during cleaning.
    QStringList l = ConfigOptions().getMapCleanerTransforms();
    l.removeAll(QString::fromStdString(RubberSheet::className()));
    conf().set(MapCleaner::opsKey(), l);
    MapCleaner().apply(map);

    RubberSheet rs;
    rs.setReference(ref);
    rs.calculateTransform(map);

    QFile fp2to1(transform2to1Path);
    if (fp2to1.open(QIODevice::WriteOnly) == false)
    {
      throw HootException(QString("Error opening %1 for writing").arg(transform2to1Path));
    }
    rs.writeTransform2to1(fp2to1);
    // if --ref is specified then this will just write an identity transform
    if (transform1to2Path != "")
    {
      QFile fp1to2(transform1to2Path);
      if (fp1to2.open(QIODevice::WriteOnly) == false)
      {
        throw HootException(QString("Error opening %1 for writing").arg(transform1to2Path));
      }
      rs.writeTransform1to2(fp1to2);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DeriveRubberSheetCmd)

}

