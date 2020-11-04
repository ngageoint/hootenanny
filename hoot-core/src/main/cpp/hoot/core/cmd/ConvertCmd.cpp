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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/DataConverter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/ops/MapCropper.h>

// Qt
#include <QElapsedTimer>
#include <QStringList>

using namespace std;

namespace hoot
{

class ConvertCmd : public BoundedCommand
{
public:

  static string className() { return "hoot::ConvertCmd"; }

  ConvertCmd() = default;

  virtual QString getName() const override { return "convert"; }

  virtual QString getDescription() const override { return "Converts between map formats"; }

  virtual int runSimple(QStringList& args) override
  {
    QElapsedTimer timer;
    timer.start();

    LOG_VART(args.size());
    LOG_VART(args);

    BoundedCommand::runSimple(args);

    if (args.size() < 2)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes at least two parameters.").arg(getName()));
    }

    ConfigOptions configOpts(conf());

    if (configOpts.getConvertOps().contains(QString::fromStdString(MapCropper::className())) &&
        configOpts.getCropBounds().trimmed().isEmpty())
    {
      throw IllegalArgumentException(
        "When using " + QString::fromStdString(MapCropper::className()) +
        " with the convert command, the " + configOpts.getCropBoundsKey() +
        " option must be specified.");
    }

    QStringList inputs;
    QString output;
    int argIndex = 0;
    for (int i = 0; i < args.size(); i++)
    {
      const QString arg = args[i];
      LOG_VART(arg);
      // Formerly, "--" options existed and were required to all be at the end of the command, so
      // you could break here once you reached them, and you knew you were done parsing
      // inputs/outputs. Now, the command doesn't take any command line options except
      // --write-bounds, which is parsed out beforehand. After that, it uses all configuration
      // options. So, let's throw if we see a command line option at this point.
      if (arg.startsWith("--"))
      {
        //break;
        throw IllegalArgumentException(
          QString("The convert command takes no inline options starting with '--'. All options ") +
          QString("are passed in as configuration options (-D)."));
      }
      argIndex++;
      inputs.append(arg);
    }
    LOG_VART(inputs.size());
    LOG_VART(argIndex);
    output = inputs.at(argIndex - 1);
    inputs.removeAt(argIndex - 1);
    LOG_VART(inputs.size());
    LOG_VART(inputs);
    LOG_VART(output);  

    ConfigUtils::checkForDuplicateElementCorrectionMismatch(ConfigOptions().getConvertOps());

    DataConverter converter;
    converter.setConfiguration(conf());
    converter.convert(inputs, output);

    LOG_STATUS(
      "Data conversion completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

