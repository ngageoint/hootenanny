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
 * @copyright Copyright (C) 2014, 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/scoring/AttributeCoOccurrence.h>

using namespace std;

namespace hoot
{

class AttributeComparisonCmd : public BaseCommand
{
public:

  static string className() { return "hoot::AttributeComparisonCmd"; }

  AttributeComparisonCmd() { }

  virtual QString getName() const { return "attribute-comparison"; }

  virtual QString getDescription() const
  { return "Compares attributes between two maps"; }

  virtual int runSimple(QStringList args)
  {
    bool tableFormat = true;
    AttributeCoOccurrence cooccurrence;

    // Print a table or just a list
    if (args.contains("--no-table"))
    {
      args.removeAll("--no-table");
      tableFormat = false;
    }

    if (args.size() != 2)
    {
      LOG_VARD(args);
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").
                          arg(getName()));
    }

    OsmMapPtr map(new OsmMap());
    loadMap(map, args[0], false, Status::Unknown1);
    loadMap(map, args[1], false, Status::Unknown2);

    cooccurrence.addToMatrix(map);

    if (tableFormat)
      cout << cooccurrence.printTable();
    else
      cout << cooccurrence.printList();

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, AttributeComparisonCmd)

}

