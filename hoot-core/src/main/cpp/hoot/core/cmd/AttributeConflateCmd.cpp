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
#include <hoot/core/Conflator.h>
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/conflate/MapCleaner.h>
#include <hoot/core/conflate/RubberSheet.h>
#include <hoot/core/manipulators/WayTagMerger.h>
#include <hoot/core/util/Settings.h>

namespace hoot
{

class AttributeConflateCmd : public BaseCommand
{
public:
  static string className() { return "hoot::ReferenceConflateCmd"; }

  AttributeConflateCmd() {}

  virtual QString getName() const { return "attr-conflate"; }

  int runSimple(QStringList args)
  {
    if (args.size() != 3)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three parameters.").arg(getName()));
    }
    QString inputRef = args[0];
    QString inputNonRef = args[1];
    QString output = args[2];

    shared_ptr<OsmMap> map(new OsmMap());
    loadMap(map, inputRef, false, Status::Unknown1);
    loadMap(map, inputNonRef, false, Status::Unknown2);

    // force only attribute conflation.
    conf().set(Conflator::manipulatorsKey(), WayTagMerger::className());
    // tell rubber sheet to move Unknown2 toward Unknown1
    conf().set(RubberSheet::refKey(), true);

    MapCleaner().apply(map);

    // call conflation routine
    Conflator conflator;
    conflator.setSource(map);
    conflator.conflate();
    shared_ptr<OsmMap> conflated(new OsmMap(conflator.getBestMap()));

    // remove any ways in Unknown2 that were not matched. We're creating a copy so we can remove
    // and iterate in one loop.
    const WayMap allWays = conflated->getWays();
    for (WayMap::const_iterator it = allWays.begin(); it != allWays.end(); ++it)
    {
      const shared_ptr<const Way>& way = it->second;

      if (way->getStatus() == Status::Unknown2)
      {
        conflated->removeWay(way);
      }
    }

    MapProjector::projectToWgs84(conflated);
    saveMap(conflated, output);

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, AttributeConflateCmd)

}
