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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/DataConverter.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

using namespace std;

namespace hoot
{

class ConvertCmd : public BoundedCommand
{
public:

  static QString className() { return "ConvertCmd"; }

  ConvertCmd() = default;

  QString getName() const override { return "convert"; }
  QString getDescription() const override { return "Converts between map formats"; }

  int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    LOG_VART(args.size());
    LOG_VART(args);

    BoundedCommand::runSimple(args);

    bool separateOutput = false;
    if (args.contains("--separate-output"))
    {
      separateOutput = true;
      args.removeAt(args.indexOf("--separate-output"));
    }

    bool recursive = false;
    const QStringList inputFilters = _parseRecursiveInputParameter(args, recursive);

    if (args.size() < 2)
    { 
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    ConfigOptions configOpts(conf());

    if (configOpts.getConvertOps().contains(MapCropper::className()) &&
        configOpts.getCropBounds().trimmed().isEmpty())
    {
      throw IllegalArgumentException(
        "When using " + MapCropper::className() + " with the convert command, the " +
        ConfigOptions::getCropBoundsKey() + " option must be specified.");
    }

    // Output is the last param.
    const int outputIndex = args.size() - 1;
    const QString output = args[outputIndex];
    args.removeAt(outputIndex);
    LOG_VARD(output);

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

    ConfigUtils::checkForDuplicateElementCorrectionMismatch(ConfigOptions().getConvertOps());

    if (!separateOutput)
    {
      // combines all inputs and writes them to the same output
      DataConverter converter;
      converter.setConfiguration(conf());
      converter.convert(inputs, output);
    }
    else
    {
      // writes a separate output for each input
      for (int i = 0; i < inputs.size(); i++)
      {
        const QString input = inputs.at(i);
        // Write each output to the format specified by output and a similarly named path as the
        // input with some text appended to the input name. We need to re-init DataConverter here
        // each time since it sets and holds onto conversion operators based on the input type.
        DataConverter converter;
        converter.setConfiguration(conf());
        converter.convert(input, IoUtils::getOutputUrlFromInput(input, "-converted", output));
      }
    }
    LOG_STATUS(
      "Data conversion completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

