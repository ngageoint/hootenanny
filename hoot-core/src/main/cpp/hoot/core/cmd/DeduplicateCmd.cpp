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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/elements/ElementDeduplicator.h>

namespace hoot
{

/**
 * TODO
 */
class DeduplicateCmd : public BaseCommand
{
public:

  static std::string className() { return "hoot::DeduplicateCmd"; }

  DeduplicateCmd() {}

  virtual QString getName() const { return "de-duplicate"; }

  virtual QString getDescription() const
  { return "TODO"; }

  virtual int runSimple(QStringList& args) override
  {
    // optional params

    bool interDatasetOnly = false;
    if (args.contains("--interDatasetOnly"))
    {
      interDatasetOnly = true;
      args.removeAll("--interDatasetOnly");
    }
    LOG_VAR(interDatasetOnly);

    bool skipNodes = false;
    if (args.contains("--skipNodes"))
    {
      skipNodes = true;
      args.removeAll("--skipNodes");
    }
    LOG_VAR(skipNodes);

    bool skipWays = false;
    if (args.contains("--skipWays"))
    {
      skipWays = true;
      args.removeAll("--skipWays");
    }
    LOG_VAR(skipWays);

    bool skipRelations = false;
    if (args.contains("--skipRelations"))
    {
      skipRelations = true;
      args.removeAll("--skipRelations");
    }
    LOG_VAR(skipRelations);

    bool favorMoreConnectedWays = false;
    if (args.contains("--favorMoreConnectedWays"))
    {
      favorMoreConnectedWays = true;
      args.removeAll("--favorMoreConnectedWays");
    }
    LOG_VAR(favorMoreConnectedWays);

    if (args.size() != 2 && args.size() != 4)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw HootException(QString("%1 takes two or four parameters.").arg(getName()));
    }

    OsmMapPtr map1(new OsmMap());
    IoUtils::loadMap(map1, args[0], true, Status::Unknown1);
    OsmMapPtr map2;
    if (args.size() > 2)
    {
      map2.reset(new OsmMap());
      IoUtils::loadMap(map2, args[2], true, Status::Unknown2);
    }

    ElementDeduplicator deduper;
    deduper.setDedupeIntraMap(!interDatasetOnly);
    deduper.setDedupeNodes(!skipNodes);
    deduper.setDedupeWays(!skipWays);
    deduper.setDedupeRelations(!skipRelations);
    deduper.setFavorMoreConnectedWays(favorMoreConnectedWays);

    if (map2)
    {
      deduper.dedupe(map1, map2);
      OsmMapWriterFactory::write(map1, args[1]);
      OsmMapWriterFactory::write(map2, args[3]);
    }
    else
    {
      deduper.dedupe(map1);
      OsmMapWriterFactory::write(map1, args[1]);
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, DeduplicateCmd)

}


