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
#include <hoot/core/ops/FindIntersectionsOp.h>


namespace hoot
{



class FindIntersectionsCmd : public BaseCommand
{
public:

  static string className() { return "hoot::FindIntersectionsCmd"; }

  FindIntersectionsCmd() { }

  virtual QString getName() const { return "find-intersections"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    boost::shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, args[0], false, Status::Unknown1);

//    MapCleaner().apply(map);
    FindIntersectionsOp().apply(map);

    MapProjector::projectToWgs84(map);

    saveMap(map, args[1]);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, FindIntersectionsCmd)

}
