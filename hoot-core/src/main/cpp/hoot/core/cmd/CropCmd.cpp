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
 * @copyright Copyright (C) 2012, 2013, 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/cmd/BoundedCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/MapCropper.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>

// Qt
#include <QElapsedTimer>

using namespace geos::geom;
using namespace std;

namespace hoot
{

class CropCmd : public BoundedCommand
{
public:

  static QString className() { return "CropCmd"; }

  CropCmd() = default;

  QString getName() const override { return "crop"; }
  QString getDescription() const override { return "Crops a map to a bounds"; }

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

    if (!separateOutput && args.size() < 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least three parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }
    else if (separateOutput && args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }

    int boundsIndex;
    if (args.contains("--write-bounds"))
      boundsIndex = args.size() - 2;
    else
      boundsIndex = args.size() - 1;
    _env = GeometryUtils::boundsFromString(args.at(boundsIndex));
    args.removeAt(boundsIndex);
    // This has to be done after we get the envelope.
    BoundedCommand::runSimple(args);

    QString output;
    if (!separateOutput)
    {
      output = args.last();
      args.removeLast();
    }

    // Everything left is an input.
    QStringList inputs;
    if (!recursive)
      inputs = IoUtils::expandInputs(args);
    else
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);

    QElapsedTimer timer;
    timer.start();

    LOG_STATUS(
      "Cropping ..." << FileUtils::toLogFormat(inputs, 25) << " and writing output to ..." <<
      FileUtils::toLogFormat(output, 25) << "...");

    if (!separateOutput)  // combines all inputs and writes them to the same output
      _crop(inputs, output);
    else  // writes a separate output for each input
      _cropSeparateOutput(inputs);

    LOG_STATUS("Map cropped in: " << StringUtils::millisecondsToDhms(timer.elapsed()) + " total.");

    return 0;
  }

private:

  std::shared_ptr<geos::geom::Geometry> _env;

  void _crop(const QStringList& inputs, const QString& output) const
  {
    OsmMapPtr map = std::make_shared<OsmMap>();
    if (inputs.size() == 1)
      IoUtils::loadMap(map, inputs.at(0), true);
    else  // Avoid ID conflicts across multiple inputs.
      IoUtils::loadMaps(map, inputs, false);

    _crop(map);

    IoUtils::saveMap(map, output);
  }

  void _cropSeparateOutput(const QStringList& inputs) const
  {
    for (const auto& input : qAsConst(inputs))
    {
      OsmMapPtr map = std::make_shared<OsmMap>();
      IoUtils::loadMap(map, input, true);

      _crop(map);

      // Write the output to a similarly named path as the input with some text appended to the
      // input name.
      IoUtils::saveMap(map, IoUtils::getOutputUrlFromInput(input, "-cropped"));
    }
  }

  void _crop(OsmMapPtr& map) const
  {
    MapCropper cropper;
    cropper.setBounds(_env);
    cropper.setConfiguration(conf());
    cropper.apply(map);
  }

  void _writeBoundsFile() override
  {
    OsmMapWriterFactory::write(
      GeometryUtils::createMapFromBounds(*(_env->getEnvelopeInternal())),
      ConfigOptions().getBoundsOutputFile());
  }
};

HOOT_FACTORY_REGISTER(Command, CropCmd)

}
