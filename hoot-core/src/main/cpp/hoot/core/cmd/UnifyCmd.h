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
#ifndef UNIFYCMD_H
#define UNIFYCMD_H

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/conflate/UnifyingConflator.h>
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>

// Tgs
#include <tgs/System/SystemInfo.h>

// Standard
#include <fstream>

namespace hoot
{
using namespace std;

class UnifyCmd : public BaseCommand
{
public:
  static string className() { return "hoot::UnifyCmd"; }

  UnifyCmd() {}

  QString getHelp() const
  {
    // 80 columns
    //  | <---                                                                      ---> |
    return getName() + " (input1) (input2) (output)\n"
        "  Conflates two input sources into one output using the unifying conflation\n"
        "  approach.\n"
        "  * input1 - First input.\n"
        "  * input2 - Second input.\n"
        "  * output - The output path.";
  }

  virtual QString getName() const { return "unify"; }

  int runSimple(QStringList args)
  {
    {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    LOG_DEBUG(SystemInfo::getMemoryUsageString());
    shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, args[0], false, Status::Unknown1);
    loadMap(map, args[1], false, Status::Unknown2);

    NamedOp(ConfigOptions().getConflatePreOps()).apply(map);

    LOG_DEBUG(SystemInfo::getMemoryUsageString());

//    LOG_WARN("Saving to debug map.");
//    saveMap(map, "tmp/debug.osm");
    {
      // call conflation routine
      UnifyingConflator conflator;
      conflator.apply(map);
      LOG_DEBUG(SystemInfo::getMemoryUsageString());
    }

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getConflatePostOps()).apply(map);

    LOG_DEBUG(SystemInfo::getMemoryUsageString());

    MapProjector::projectToWgs84(map);

    LOG_DEBUG(SystemInfo::getMemoryUsageString());
    saveMap(map, args[2]);

    map.reset();
    }
    LOG_DEBUG(SystemInfo::getMemoryUsageString());

    return 0;
  }
};

}

#endif // UNIFYCMD_H
