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
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/algorithms/changeset/NodeDensityTaskGridGenerator.h>
#include <hoot/core/algorithms/changeset/UniformTaskGridGenerator.h>
#include <hoot/core/geometry/GeometryUtils.h>

namespace hoot
{

class TaskGridCmd : public BaseCommand
{

public:

  static std::string className() { return "hoot::TaskGridCmd"; }

  TaskGridCmd() = default;

  virtual QString getName() const override { return "task-grid"; }

  virtual QString getDescription() const override { return "Creates a task grid covering a map"; }

  virtual int runSimple(QStringList& args) override
  {
    if (args.contains("--uniform") && args.contains("--node-density"))
    {
      throw IllegalArgumentException(
        "Only one of --uniform or --node-density may be specified for " + getName());
    }
    else if (!args.contains("--uniform") && !args.contains("--node-density"))
    {
      throw IllegalArgumentException(
        "At least one of --uniform or --node-density must be specified for " + getName());
    }

    bool isUniformGrid = false;
    if (args.contains("--uniform"))
    {
      isUniformGrid = true;
      args.removeAt(args.indexOf("--uniform"));
    }
    else if (args.contains("--node-density"))
    {
      args.removeAt(args.indexOf("--node-density"));
    }

    if (!isUniformGrid && args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters with the --node-density option.").arg(getName()));
    }
    else if (isUniformGrid && args.size() < 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least three parameters with the --uniform option.").arg(getName()));
    }

    QStringList inputs;
    QString bounds;
    const QString input = args[0];
    LOG_VARD(input);
    if (!isUniformGrid)
    {
      if (!input.contains(";"))
      {
        inputs.append(input);
      }
      else
      {
        // multiple inputs
        inputs = input.split(";");
      }
    }
    else
    {
      if (GeometryUtils::isEnvelopeConfigString(input))
      {
        bounds = input;
      }
      else if (!input.contains(";"))
      {
        inputs.append(input);
      }
      else
      {
        // multiple inputs
        inputs = input.split(";");
      }
    }
    LOG_VARD(inputs);
    LOG_VARD(bounds);

    const QString output = args[1];
    if (!output.toLower().endsWith(".geojson") && !output.toLower().endsWith(".osm"))
    {
      throw IllegalArgumentException(
        "Invalid output file format: " + output + ". Only the GeoJSON (.geojson) and OSM " +
        "(.osm) output formats are supported.");
    }
    LOG_VARD(output);

    int gridDimensionSize = 2;
    if (isUniformGrid)
    {
      bool parseSuccess = false;
      gridDimensionSize = args[2].toInt(&parseSuccess);
      if (!parseSuccess || gridDimensionSize < 2)
      {
        throw IllegalArgumentException(
          "Invalid grid dimension size value: " + args[2] + ". Must be >= 2.");
      }
    }

    // optional parameters

    int maxNodesPerCell = 1000;
    double pixelSize = 0.001; // .1km?
    int maxAttempts = 3;
    int maxTimePerAttempt = -1;
    int pixelSizeAutoReductionFactor = 10;
    // TODO: implement random for uniform task grids as well?
    int randomSeed = -1;
    if (!isUniformGrid)
    {
      if (args.contains("--bounds"))
      {
        const int optionNameIndex = args.indexOf("--bounds");
        bounds = args.at(optionNameIndex + 1).trimmed();
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(bounds);

      if (args.contains("--maxNodesPerCell"))
      {
        const int optionNameIndex = args.indexOf("--maxNodesPerCell");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        maxNodesPerCell = optionStrVal.toInt(&parseSuccess);
        if (!parseSuccess || maxNodesPerCell < 1)
        {
          throw IllegalArgumentException(
            "Invalid maximum node count per cell value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(maxNodesPerCell);

      if (args.contains("--pixel-size"))
      {
        const int optionNameIndex = args.indexOf("--pixel-size");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        pixelSize = optionStrVal.toDouble(&parseSuccess);
        if (!parseSuccess || pixelSize <= 0.0)
        {
          throw IllegalArgumentException("Invalid pixel size value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(pixelSize);

      if (args.contains("--maxAttempts"))
      {
        const int optionNameIndex = args.indexOf("--maxAttempts");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        maxAttempts = optionStrVal.toInt(&parseSuccess);
        if (!parseSuccess || maxAttempts < 1)
        {
          throw IllegalArgumentException("Invalid maximum attempts value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(maxAttempts);

      if (args.contains("--maxTimePerAttempt"))
      {
        const int optionNameIndex = args.indexOf("--maxTimePerAttempt");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        maxTimePerAttempt = optionStrVal.toInt(&parseSuccess);
        if (!parseSuccess || maxTimePerAttempt < -1)
        {
          throw IllegalArgumentException("Invalid maximum time per attempt value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(maxTimePerAttempt);

      if (args.contains("--pixelSizeReductionFactor"))
      {
        const int optionNameIndex = args.indexOf("--pixelSizeReductionFactor");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        pixelSizeAutoReductionFactor = optionStrVal.toInt(&parseSuccess);
        if (!parseSuccess || pixelSizeAutoReductionFactor < 1)
        {
          throw IllegalArgumentException(
            "Invalid pixel size automatic reduction factor value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(pixelSizeAutoReductionFactor);

      if (args.contains("--random") && args.contains("--randomSeed"))
      {
        const int optionNameIndex = args.indexOf("--randomSeed");
        bool parseSuccess = false;
        const QString optionStrVal = args.at(optionNameIndex + 1).trimmed();
        randomSeed = optionStrVal.toInt(&parseSuccess);
        if (!parseSuccess || randomSeed < -1)
        {
          throw IllegalArgumentException("Invalid random seed value: " + optionStrVal);
        }
        args.removeAt(optionNameIndex + 1);
        args.removeAt(optionNameIndex);
      }
      LOG_VARD(randomSeed);
    }

    if (!isUniformGrid)
    {
      NodeDensityTaskGridGenerator taskGridGen(inputs, maxNodesPerCell, bounds, output);
      taskGridGen.setReadInputFullThenCrop(false); //??
      taskGridGen.setPixelSize(pixelSize);
      taskGridGen.setPixelSizeRetryReductionFactor(pixelSizeAutoReductionFactor);
      taskGridGen.setMaxNumTries(maxAttempts);
      taskGridGen.setMaxTimePerAttempt(maxTimePerAttempt);
      taskGridGen.setWriteRandomCellOnly(args.contains("--random"));
      taskGridGen.setRandomSeed(randomSeed);
      /*TaskGrid taskGrid =*/ taskGridGen.generateTaskGrid();
    }
    else
    {
      if (!bounds.trimmed().isEmpty())
      {
        /*TaskGrid taskGrid =*/
          UniformTaskGridGenerator(bounds, gridDimensionSize, output).generateTaskGrid();
      }
      else
      {
        /*TaskGrid taskGrid =*/
          UniformTaskGridGenerator(inputs, gridDimensionSize, output).generateTaskGrid();
      }
    }

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, TaskGridCmd)

}
