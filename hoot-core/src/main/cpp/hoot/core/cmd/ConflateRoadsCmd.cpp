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

// GEOS
#include <geos/geom/GeometryFactory.h>

// Hoot
#include <hoot/core/Conflator.h>
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/Settings.h>

// Standard
#include <fstream>

namespace hoot
{
using namespace std;

class ConflateRoadsCmd : public BaseCommand
{
public:
  static string className() { return "hoot::ConflateRoadsCmd"; }

  ConflateRoadsCmd() {}

  virtual QString getName() const { return "conflate-roads"; }

  static QString postOpsKey() { return "conflator.post.ops"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }

    shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, args[0], false, Status::Unknown1);
    loadMap(map, args[1], false, Status::Unknown2);

    // call conflation routine
    Conflator conflator;
    conflator.loadSource(map);
    conflator.conflate();

    shared_ptr<OsmMap> result(new OsmMap(conflator.getBestMap()));

    // Apply any user specified operations.
    NamedOp(ConfigOptions().getConflatorPostOps()).apply(result);

    MapProjector::projectToWgs84(result);

    saveMap(result, args[2]);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConflateRoadsCmd)

}
