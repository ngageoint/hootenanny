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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

// Hoot
#include <hoot/core/algorithms/changeset/NodeDensityTaskGridGenerator.h>
#include <hoot/core/algorithms/changeset/UniformTaskGridGenerator.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QElapsedTimer>

namespace hoot
{

class TaskGridCmd : public BaseCommand
{

public:

  static QString className() { return "hoot::TaskGridCmd"; }

  TaskGridCmd() = default;

  QString getName() const override { return "task-grid"; }
  QString getDescription() const override { return "Creates a task grid covering a map"; }

  int runSimple(QStringList& args) override
  {
    LOG_VARD(args);
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

    QElapsedTimer timer;
    timer.start();

    if (!isUniformGrid)
    {
      _processNodeDensityGrid(args);
    }
    else
    {
      _processUniformGrid(args);
    }

    LOG_STATUS(
      "Task grid generated in " << StringUtils::millisecondsToDhms(timer.elapsed()) << " total.");

    return 0;
  }

private:

  void _processUniformGrid(QStringList& args) const
  {
    if (args.size() < 3)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least three parameters with the --uniform option.").arg(getName()));
    }
    LOG_VARD(args);

    int gridDimensionSize = 2;
    bool parseSuccess = false;
    const int gridDimensionSizeIndex = args.size() - 1;
    gridDimensionSize = args[gridDimensionSizeIndex].toInt(&parseSuccess);
    if (!parseSuccess || gridDimensionSize < 2)
    {
      throw IllegalArgumentException(
        "Invalid grid dimension size value: " + args[2] + ". Must be >= 2.");
    }
    args.removeLast();
    LOG_VARD(gridDimensionSize);
    LOG_VARD(gridDimensionSizeIndex);

    const int outputIndex = args.size() - 1;
    const QString output = args[outputIndex];
    _validateOutput(output);
    args.removeLast();
    LOG_VARD(output);

    LOG_VARD(args);

    // Everything left is an input.
    QString bounds;
    QStringList inputs;
    if (args.size() == 1 && GeometryUtils::isEnvelopeString(args[0]))
    {
      bounds = args[0];
    }
    else
    {
      inputs = IoUtils::expandInputs(args);
    }

    if (!bounds.trimmed().isEmpty())
    {
      LOG_STATUS(
        "Generating uniform task grid over " << bounds << " and writing output to ..." <<
        output << "...");
      /*TaskGrid taskGrid =*/
        UniformTaskGridGenerator(bounds, gridDimensionSize, output).generateTaskGrid();
    }
    else
    {
      LOG_STATUS(
        "Generating uniform task grid for " << inputs.size() <<
        " inputs and writing output to ..." << output << "...");
      /*TaskGrid taskGrid =*/
        UniformTaskGridGenerator(inputs, gridDimensionSize, output).generateTaskGrid();
    }
  }

  void _processNodeDensityGrid(QStringList& args) const
  {
    // optional parameters

    int maxNodesPerCell = 1000;
    double pixelSize = 0.001; // .1km?
    int maxAttempts = 3;
    int maxTimePerAttempt = -1;
    int pixelSizeAutoReductionFactor = 10;
    QString bounds;

    // TODO: implement random for uniform task grids as well?
    int randomSeed = -1;
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
    if (args.contains("--pixelSize"))
    {
      const int optionNameIndex = args.indexOf("--pixelSize");
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

    // Being lenient here in case --randomSeed was specified but --random was forgotten.
    const bool random = args.contains("--random") || args.contains("--randomSeed");
    if (args.contains("--randomSeed"))
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
    if (args.contains("--random"))
    {
      args.removeAt(args.indexOf("--random"));
    }
    LOG_VARD(randomSeed);

    if (args.size() < 2)
    {
      std::cout << getHelp() << std::endl << std::endl;
      throw IllegalArgumentException(
        QString("%1 takes at least two parameters. You provided %2: %3")
          .arg(getName())
          .arg(args.size())
          .arg(args.join(",")));
    }
    LOG_VARD(args);

    const int outputIndex = args.size() - 1;
    const QString output = args[outputIndex];
    args.removeAt(outputIndex);
    _validateOutput(output);

    // Everything left is an input.
    const QStringList inputs = IoUtils::expandInputs(args);

    LOG_STATUS(
      "Generating node density based task grid for " << inputs.size() << " inputs and " <<
      "writing output to ..." << output << "...");

    NodeDensityTaskGridGenerator taskGridGen(inputs, maxNodesPerCell, bounds, output);
    taskGridGen.setReadInputFullThenCrop(false); //??
    taskGridGen.setPixelSize(pixelSize);
    taskGridGen.setPixelSizeRetryReductionFactor(pixelSizeAutoReductionFactor);
    taskGridGen.setMaxNumTries(maxAttempts);
    taskGridGen.setMaxTimePerAttempt(maxTimePerAttempt);
    taskGridGen.setWriteRandomCellOnly(random);
    taskGridGen.setRandomSeed(randomSeed);
    /*TaskGrid taskGrid =*/ taskGridGen.generateTaskGrid();
  }

  void _validateOutput(const QString& output) const
  {
    if (!output.toLower().endsWith(".geojson") && !output.toLower().endsWith(".osm"))
    {
      throw IllegalArgumentException(
        "Invalid output file format: " + output + ". Only the GeoJSON (.geojson) and OSM " +
        "(.osm) output formats are supported.");
    }
  }
};

HOOT_FACTORY_REGISTER(Command, TaskGridCmd)

}
