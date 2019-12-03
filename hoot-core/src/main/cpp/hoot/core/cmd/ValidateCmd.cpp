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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/JosmMapValidator.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/io/IoUtils.h>

using namespace std;

namespace hoot
{

class ValidateCmd : public BaseCommand
{
public:

  static string className() { return "hoot::ValidateCmd"; }

  ValidateCmd() { }

  virtual QString getName() const override { return "validate"; }

  virtual QString getDescription() const override
  { return "Marks erroneous map data with validation errors using JOSM"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, args[0], true, Status::Unknown1);

    JosmMapValidator().apply(map);

    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, args[1]);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ValidateCmd)


}

