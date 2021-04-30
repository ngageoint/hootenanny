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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/ops/MapCleaner.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/StringUtils.h>

// Tgs
#include <tgs/System/Timer.h>

using namespace std;

namespace hoot
{

static const QString JOB_SOURCE = "Clean";

class CleanCmd : public BaseCommand
{
public:

  static QString className() { return "hoot::CleanCmd"; }

  CleanCmd() = default;

  QString getName() const override { return "clean"; }

  QString getDescription() const override { return "Corrects erroneous map data"; }

  int runSimple(QStringList& args) override
  {
    Tgs::Timer timer;

    if (args.size() != 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes two parameters.").arg(getName()));
    }

    Progress progress(
      ConfigOptions().getJobId(),
      JOB_SOURCE,
      Progress::JobState::Running,
      0.0,
      // import, export, and cleaning tasks
      1.0 / 3.0);

    progress.set(0.0, Progress::JobState::Running, "Importing map...");
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, args[0], true, Status::Unknown1);

    progress.set(1.0 / 3.0, Progress::JobState::Running, "Cleaning map...");
    MapCleaner(progress).apply(map);

    progress.set(2.0 / 3.0, Progress::JobState::Running, "Exporting map...");
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, args[1]);

    double totalElapsed = timer.getElapsed();
    progress.set(
      1.0, Progress::JobState::Successful,
      "Cleaning job completed using " +
      QString::number(ConfigOptions().getMapCleanerTransforms().size()) +
      " cleaning operations in " + StringUtils::millisecondsToDhms((qint64)(totalElapsed * 1000)));

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, CleanCmd)

}

