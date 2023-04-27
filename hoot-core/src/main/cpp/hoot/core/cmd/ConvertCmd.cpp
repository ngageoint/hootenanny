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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
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

// Tgs
#include <tgs/System/SystemInfo.h>

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

    if (args.contains("--autoconfig-cache"))
    {
      setupAutoConfigCache();
      args.removeAll("--autoconfig-cache");
    }

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

    if (configOpts.getConvertOps().contains(MapCropper::className()) && configOpts.getCropBounds().trimmed().isEmpty())
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
      inputs = IoUtils::expandInputs(args);
    else
      inputs = IoUtils::getSupportedInputsRecursively(args, inputFilters);

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
      for (const auto& input : qAsConst(inputs))
      {
        // Write each output to the format specified by output and a similarly named path as the
        // input with some text appended to the input name. We need to re-init DataConverter here
        // each time since it sets and holds onto conversion operators based on the input type.
        DataConverter converter;
        converter.setConfiguration(conf());
        converter.convert(input, IoUtils::getOutputUrlFromInput(input, "-converted", output));
      }
    }
    LOG_STATUS("Data conversion completed in " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");

    return 0;
  }

  void setupAutoConfigCache() const
  {
    //  These values were found through testing
    const double node_size = 350.0;
    const double way_size = 400.0;
    const double relation_size = 375.0;
    //  Get the amount of memory available
    const long memory_available = Tgs::SystemInfo::getPhysicalMemoryAvailable();
    //  Get the current cache size values from the settings
    Settings& settings = conf();
    ConfigOptions options(settings);
    const long default_cache_nodes = options.getElementCacheSizeNode();
    const long default_cache_ways = options.getElementCacheSizeWay();
    const long default_cache_relations = options.getElementCacheSizeRelation();
    //  Calculate a number of nodes/ways/relations that will "fit" in that memory
    //  Use the element cache memory threshold so that some memory remains
    const double memory_threshold = static_cast<double>(ConfigOptions().getElementCacheSizeAutoThreshold()) / 100.0;
    //  The current defaults are 10M nodes, 2M ways, and 2M relations since most nodes don't have tags which are the most expensive part
    const double available_for_cache = static_cast<double>(memory_available) * memory_threshold;
    const long cache_nodes = static_cast<long>((available_for_cache * 5.0 / 7.0) / node_size);
    const long cache_ways = static_cast<long>((available_for_cache * 1.0 / 7.0) / way_size);
    const long cache_relations = static_cast<long>((available_for_cache * 1.0 / 7.0) / relation_size);
    LOG_DEBUG("Available memory for element cache: " << memory_available << " bytes (threshold: " << memory_threshold << ")");
    //  Update the values of the cache size if smaller than the settings
    if (cache_nodes < default_cache_nodes)
    {
      settings.set(ConfigOptions::getElementCacheSizeNodeKey(), QString::number(cache_nodes));
      LOG_DEBUG("Setting element.cache.size.node=" << cache_nodes);
    }
    if (cache_ways < default_cache_ways)
    {
      settings.set(ConfigOptions::getElementCacheSizeWayKey(), QString::number(cache_ways));
      LOG_DEBUG("Setting element.cache.size.way=" << cache_ways);
    }
    if (cache_relations < default_cache_relations)
    {
      settings.set(ConfigOptions::getElementCacheSizeRelationKey(), QString::number(cache_relations));
      LOG_DEBUG("Setting element.cache.size.relation=" << cache_relations);
    }
  }
};

HOOT_FACTORY_REGISTER(Command, ConvertCmd)

}

