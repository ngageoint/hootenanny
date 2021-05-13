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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2014, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/schema/AttributeCoOccurrence.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class TagCompareCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::TagCompareCmd"; }

  TagCompareCmd() = default;

  QString getName() const override { return "tag-compare"; }
  QString getDescription() const override { return "Compares tags between two maps"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

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

    const QString input1 = args[0];
    const QString input2 = args[1];

    LOG_STATUS(
      "Comparing tags for ..." << FileUtils::toLogFormat(input1, 25) << " and ..." <<
      FileUtils::toLogFormat(input2, 25) << "...");

    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, input1, false, Status::Unknown1);
    IoUtils::loadMap(map, input2, false, Status::Unknown2);

    cooccurrence.addToMatrix(map);

    if (tableFormat)
      cout << cooccurrence.printTable();
    else
      cout << cooccurrence.printList();

    LOG_STATUS(
      "Tags compared in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TagCompareCmd)

}

