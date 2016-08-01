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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/scoring/MapComparator.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class IsMatchCmd : public BaseCommand
{
public:

  static string className() { return "hoot::IsMatchCmd"; }

  IsMatchCmd() { }

  virtual QString getName() const { return "is-match"; }

  virtual int runSimple(QStringList args)
  {

    MapComparator mapCompare;

    if (args.contains("--ignore-uuid"))
    {
      args.removeAll("--ignore-uuid");
      mapCompare.setIgnoreUUID();
    }

    if (args.contains("--use-datetime"))
    {
      args.removeAll("--use-datetime");
      mapCompare.setUseDateTime();
    }


    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    shared_ptr<OsmMap> map1(new OsmMap());
    loadMap(map1, args[0], true, Status::Unknown1);
    shared_ptr<OsmMap> map2(new OsmMap());
    loadMap(map2, args[1], true, Status::Unknown1);

    int result;

    if (mapCompare.isMatch(map1, map2))
    {
      result = 0;
    }
    else
    {
      result = 1;
    }

    return result;
  }
};

HOOT_FACTORY_REGISTER(Command, IsMatchCmd)

}

