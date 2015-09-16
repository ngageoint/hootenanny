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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapReprojector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/util/Settings.h>

// Qt
#include <QFile>

namespace hoot
{

class DeriveRubberSheetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveRubberSheetCmd"; }

  DeriveRubberSheetCmd() { }

  virtual QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " [--ref] (input1) (input2) (transform2to1.rs) \n"
        "  [transform1to2.rs]\n"
        "\n"
        "  Creates a transform file for rubber sheeting inputs.\n"
        "  * --ref - If specified, treat input1 as a reference layer and only produce \n"
        "    one output transform.\n"
        "  * input1 - Input 1 (e.g. .osm file).\n"
        "  * input2 - Input 2 (e.g. .osm file).\n"
        "  * transform2to1.rs - Output transform rubber sheet spec. provides a transform\n"
        "    for moving input 2 towards input 1.\n"
        "  * transform1to2.rs - Output transform rubber sheet spec. Provides a transform\n"
        "    for moving input 1 towards input 2. Required if --ref is not specified.\n";
  }

  virtual QString getName() const { return "derive-rubber-sheet"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() < 3 || args.size() > 5)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three to five parameters.").arg(getName()));
    }

    shared_ptr<OsmMap> map(new OsmMap());
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
    QStringList l = conf().getList(MapCleaner::opsKey(), "");
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

