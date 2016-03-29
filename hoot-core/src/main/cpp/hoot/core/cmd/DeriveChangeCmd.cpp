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
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/OsmChangeWriter.h>
#include <hoot/core/io/OsmChangeWriterSql.h>

// Qt
#include <QUrl>

namespace hoot
{

/**
 */
class DeriveChangeCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveChangeCmd"; }

  DeriveChangeCmd() { }

  virtual QString getHelp() const
  {
    return "derive-change (from) (to) (output)";
  }

  virtual QString getName() const { return "derive-change"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    shared_ptr<OsmMap> map1(new OsmMap());
    loadMap(map1, args[0], true);

    shared_ptr<OsmMap> map2(new OsmMap());
    loadMap(map2, args[1], true);

    ElementSorterPtr sorted1(new ElementSorter(map1));
    ElementSorterPtr sorted2(new ElementSorter(map2));

    ChangesetDeriverPtr delta(new ChangesetDeriver(sorted1, sorted2));

    if (args[2].endsWith(".osc"))
    {
      OsmChangeWriter writer;

      writer.write(args[2], delta);
    }
    else
    {
      OsmChangeWriterSql writer(QUrl(""));

      writer.write(args[2], delta);
    }

    return 0;
  }
};

//HOOT_FACTORY_REGISTER(Command, DeriveChangeCmd)


}


