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

// Qt
#include <QElapsedTimer>

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
    bool separateOutput = false;
    if (args.contains("--separate-output"))
    {
      separateOutput = true;
      args.removeAt(args.indexOf("--separate-output"));
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);
    LOG_VARD(inputFilters);

    if (!separateOutput && args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }
    else if (separateOutput && args.size() < 1)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least one parameter. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    QString output;
    if (!separateOutput)
    {
      // Output is the last param.
      const int outputIndex = args.size() - 1;
      output = args[outputIndex];
      args.removeAt(outputIndex);
    }

    // Everything that's left is an input.
    QStringList inputs;
    if (!recursive)
    {
      inputs = IoUtils::expandInputs(args);
    }
    else
    {
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);
    }

    QElapsedTimer timer;
    timer.start();
    std::shared_ptr<Progress> progress = std::make_shared<Progress>();

    if (!separateOutput)
    {
      // combines all inputs and writes them to the same output
      _clean(inputs, output, progress);
    }
    else
    {
      // writes a separate output for each input
      _cleanSeparateOutput(inputs, progress);
    }

    LOG_VARD(progress.get());
    progress->set(
      1.0, Progress::JobState::Successful,
      "Cleaning job completed using " +
      QString::number(ConfigOptions().getMapCleanerTransforms().size()) +
      " cleaning operations in " + StringUtils::millisecondsToDhms(timer.elapsed()));

    return 0;
  }

private:

  void _clean(
    const QStringList& inputs, const QString& output, std::shared_ptr<Progress> progress) const
  {
    progress =
      std::make_shared<Progress>(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running, 0.0,
        // import, export, and cleaning tasks
        1.0f / 3.0f);

    progress->set(
      0.0, Progress::JobState::Running,
      "Importing " + QString::number(inputs.size()) + " map(s)...");
    OsmMapPtr map = std::make_shared<OsmMap>();
    // We don't try to stream here, b/c there are generally always going to be non-streamable
    // cleaning ops (possibly not, though, if someone drastically changed the default cleaning
    // config...unlikely). If we only have one input, then we'll retain the source IDs to keep the
    // output as consistent with the input as possible. With more than one input there could be ID
    // conflicts, so we won't retain the originals.
    if (inputs.size() == 1)
    {
      IoUtils::loadMap(map, inputs.at(0), true, Status::Unknown1);
    }
    else
    {
      IoUtils::loadMaps(map, inputs, false, Status::Unknown1);
    }

    progress->set(1.0f / 3.0f, Progress::JobState::Running, "Cleaning map(s)...");
    MapCleaner(*progress).apply(map);

    progress->set(
      2.0f / 3.0f, Progress::JobState::Running,
      "Exporting map: ..." + FileUtils::toLogFormat(output, 25) + "...");
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, output);
  }

  void _cleanSeparateOutput(const QStringList& inputs, std::shared_ptr<Progress> progress) const
  {
    // (import, export, and cleaning tasks) * number of inputs
    const float numTasks = 3.0f * (float)inputs.size();
    LOG_VARD(numTasks);
    progress =
      std::make_shared<Progress>(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        0.0f, 1.0f / numTasks);

    float currentTask = 0.0;
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs.at(i);

      progress->set(
        currentTask / numTasks, Progress::JobState::Running,
        "Importing: ..." + FileUtils::toLogFormat(input, 25) + "...");
      OsmMapPtr map = std::make_shared<OsmMap>();
      // See note in _clean about why we don't stream this input.
      IoUtils::loadMap(map, input, true, Status::Unknown1);
      currentTask++;

      progress->set(
        currentTask / numTasks, Progress::JobState::Running,
        "Cleaning: ..." + FileUtils::toLogFormat(input, 25) + "...");
      MapCleaner(*progress).apply(map);
      currentTask++;

      // Write the output to a similarly named path as the input with some text appended to the
      // input name.
      const QString output = IoUtils::getOutputUrlFromInput(input, "-cleaned");
      progress->set(
        currentTask / numTasks, Progress::JobState::Running,
        "Exporting: ..." + FileUtils::toLogFormat(output, 25) + "...");
      MapProjector::projectToWgs84(map);
      IoUtils::saveMap(map, output);
      currentTask++;
    }
  }
};

HOOT_FACTORY_REGISTER(Command, CleanCmd)

}

