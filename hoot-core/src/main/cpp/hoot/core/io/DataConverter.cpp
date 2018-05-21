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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DataConverter.h"

#include <hoot/core/util/Log.h>
#include <hoot/core/OsmMapConsumer.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>

// std
#include <vector>

namespace hoot
{

unsigned int DataConverter::logWarnCount = 0;

DataConverter::DataConverter() :
_colsArgSpecified(false),
_featureReadLimit(0)
{
}

void DataConverter::_validateInput(const QStringList inputs, const QString output)
{
  LOG_VART(IoUtils::areSupportedOgrFormats(inputs, true));
  LOG_VART(IoUtils::isSupportedOsmFormat(inputs.at(0)));
  LOG_VART(IoUtils::isSupportedOsmFormat(output));
  LOG_VART(IoUtils::isSupportedOgrFormat(inputs.at(0), true));
  LOG_VART(IoUtils::isSupportedOgrFormat(output));

  if (inputs.size() > 1 && !IoUtils::areSupportedOgrFormats(inputs, true) &&
      !IoUtils::isSupportedOsmFormat(output))
  {
    throw HootException(
      "Multiple inputs are only allowed when converting from an OGR format to OSM.");
  }

  if (!_translation.trimmed().isEmpty() && !IoUtils::areSupportedOgrFormats(inputs, true) &&
      !IoUtils::isSupportedOgrFormat(output, false))
  {
    throw HootException(
      "A translation can only be specified when converting to/from OGR formats.");
  }

  //TODO: We may be able to relax the restriction here of requiring the input be an OSM
  //format, but since cols were originally only used with osm2shp, let's keep it here for now.
  if (_colsArgSpecified && !output.toLower().endsWith(".shp") &&
      !IoUtils::isSupportedOsmFormat(inputs.at(0)))
  {
    throw HootException(
      "Columns may only be specified when converting from an OSM format to the shape file format.");
  }

  if (!_translation.isEmpty() && _colsArgSpecified)
  {
    throw HootException("Cannot specify both a translation and export columns.");
  }

  //TODO: Can this eventually be removed?
  if (_featureReadLimit > 0 && !IoUtils::areSupportedOgrFormats(inputs, true))
  {
    throw HootException("Limit may only be specified when converting OGR inputs.");
  }
}

void DataConverter::convert(const QStringList inputs, const QString output)
{
  _validateInput(inputs, output);

  LOG_INFO("Converting " << inputs.join(", ").right(100) << " to " << output.right(100) << "...");

  if (output.toLower().endsWith(".shp") && inputs.size() == 1 &&
      IoUtils::isSupportedOsmFormat(inputs.at(0)) && _colsArgSpecified)
  {
    _convertOsmToShp(inputs.at(0), output);
  }
  else if (inputs.size() == 1 && IoUtils::isSupportedOsmFormat(inputs.at(0)) &&
           IoUtils::isSupportedOgrFormat(output) && !_translation.isEmpty())
  {
    _convertOsmToOgr(inputs.at(0), output);
  }
  else if (IoUtils::areSupportedOgrFormats(inputs, true) &&
           IoUtils::isSupportedOsmFormat(output) && !_translation.isEmpty())
  {
    _convertOgrToOsm(inputs, output);
  }
  else
  {
    _generalConvert(inputs.at(0), output);
  }
}

void DataConverter::_convertOsmToShp(const QString input, const QString output)
{
  LOG_TRACE("osm2shp");

  OsmMapPtr map(new OsmMap());
  IoUtils::loadMap(map, input, true);

  ShapefileWriter writer;
  writer.setColumns(_columns);
  writer.write(map, output);
}

void DataConverter::_convertOsmToOgr(const QString input, const QString output)
{
  LOG_TRACE("osm2ogr");

  boost::shared_ptr<OgrWriter> writer(new OgrWriter());
  if (ConfigOptions().getElementCacheSizeNode() > 0 &&
      ConfigOptions().getElementCacheSizeWay() > 0 &&
      ConfigOptions().getElementCacheSizeRelation() > 0)
  {
    writer->setCacheCapacity(
      ConfigOptions().getElementCacheSizeNode(), ConfigOptions().getElementCacheSizeWay(),
      ConfigOptions().getElementCacheSizeRelation());
  }
  writer->setScriptPath(_translation);
  writer->open(output);

  OsmMapReaderFactory readerFactory = OsmMapReaderFactory::getInstance();
  if (readerFactory.hasElementInputStream(input) &&
      ConfigOptions().getConvertOps().size() == 0 &&
      //none of the convert bounding box supports are able to do streaming I/O at this point
      !ConfigUtils::boundsOptionEnabled())
  {
    boost::shared_ptr<OsmMapReader> reader =
      OsmMapReaderFactory::getInstance().createReader(input);
    reader->open(input);
    boost::shared_ptr<ElementInputStream> streamReader =
      boost::dynamic_pointer_cast<ElementInputStream>(reader);
    boost::shared_ptr<ElementOutputStream> streamWriter =
      boost::dynamic_pointer_cast<ElementOutputStream>(writer);

    boost::shared_ptr<OGRSpatialReference> projection = streamReader->getProjection();
    ProjectToGeographicVisitor visitor;
    bool notGeographic = !projection->IsGeographic();

    if (notGeographic)
      visitor.initialize(projection);

    while (streamReader->hasMoreElements())
    {
      ElementPtr element = streamReader->readNextElement();
      if (notGeographic)
        visitor.visit(element);
      streamWriter->writeElement(element);
    }
  }
  else
  {
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, input, true);

    NamedOp(ConfigOptions().getConvertOps()).apply(map);
    MapProjector::projectToWgs84(map);
    writer->write(map);
  }
}

void DataConverter::_convertOgrToOsm(const QStringList inputs, const QString output)
{
  LOG_TRACE("ogr2osm");

  OsmMapPtr map(new OsmMap());
  OgrReader reader;
  if (_featureReadLimit > 0)
  {
    reader.setLimit(_featureReadLimit);
  }
  reader.setTranslationFile(_translation);

  ConfigOptions configOptions;

  //TODO: move to top?
  Progress progress("DataConverter");
  progress.setReportType(configOptions.getProgressReportingFormat());
  progress.setState("Running");

  LOG_INFO("Reading layers...");
  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs[i].trimmed();
    LOG_VART(input);

    if (input.trimmed().isEmpty())
    {
      LOG_WARN("Got an empty layer, skipping.");
      continue;
    }

    QStringList layers;
    if (input.contains(";"))
    {
      QStringList list = input.split(";");
      input = list.at(0);
      layers.append(list.at(1));
    }
    else
    {
      layers = reader.getFilteredLayerNames(input);
      layers.sort();
    }
    LOG_VARD(layers);

    if (layers.size() == 0)
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Could not find any valid layers to read from in " + input + ".");
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(typeid(this).name() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }

    // process the completion status report information first
    long featureCountTotal = 0;
    int undefinedCounts = 0;
    std::vector<float> progressWeights;
    for (int i = 0; i < layers.size(); i++)
    {
      LOG_VART(layers[i]);
      // simply open the file, get the meta feature count value, and close
      int featuresPerLayer = reader.getFeatureCount(input, layers[i]);
      progressWeights.push_back((float)featuresPerLayer);
      // cover the case where no feature count available efficiently
      if (featuresPerLayer == -1) undefinedCounts++;
      else featureCountTotal += featuresPerLayer;
    }

    int definedCounts = layers.size() - undefinedCounts;

    // determine weights for 3 possible cases
    if (undefinedCounts == layers.size())
    {
      for (int i = 0; i < layers.size(); i++) progressWeights[i] = 1. / (float)layers.size();
    }
    else if(definedCounts == layers.size())
    {
      for (int i = 0; i < layers.size(); i++) progressWeights[i] /= (float)featureCountTotal;
    }
    else
    {
      for (int i = 0; i<layers.size(); i++)
        if (progressWeights[i] == -1)
        {
          progressWeights[i] = (1. / (float)definedCounts) * featureCountTotal;
        }
      // reset featurecount total and recalculate
      featureCountTotal = 0;
      for (int i = 0; i < layers.size(); i++) featureCountTotal += progressWeights[i];
      for (int i = 0; i < layers.size(); i++) progressWeights[i] /= (float)featureCountTotal;
    }

    // read each layer's data
    for (int i = 0; i < layers.size(); i++)
    {
      if (Log::getInstance().getLevel() == Log::Info)
      {
        std::cout << ".";
        std::cout.flush();
      }
      LOG_VART(input);
      LOG_VART(layers[i]);
      progress.setTaskWeight(progressWeights[i]);
      reader.read(input, layers[i], map, progress);
    }
  }

  if (map->getNodes().size() == 0)
  {
    progress.set(1.0, "Failed", true, "After translation the map is empty.  Aborting.");
    throw HootException("After translation the map is empty. Aborting.");
  }

  MapProjector::projectToPlanar(map);
  QStringList convertOps = ConfigOptions().getConvertOps();
  //the ordering for these ogr2osm ops may matter
  if (ConfigOptions().getOgr2osmSimplifyComplexBuildings())
  {
    convertOps.prepend("hoot::BuildingPartMergeOp");
  }
  if (ConfigOptions().getOgr2osmMergeNearbyNodes())
  {
    convertOps.prepend("hoot::MergeNearbyNodes");
  }
  NamedOp(convertOps).apply(map);
  MapProjector::projectToWgs84(map);
  IoUtils::saveMap(map, output);

  progress.set(1.0, "Successful", true, "Finished successfully.");
}

void DataConverter::_generalConvert(const QString input, const QString output)
{
  LOG_TRACE("convert");

  // This keeps the status and the tags.
  conf().set(ConfigOptions().getReaderUseFileStatusKey(), true);
  conf().set(ConfigOptions().getReaderKeepStatusTagKey(), true);
  LOG_VART(OsmMapReaderFactory::getInstance().hasElementInputStream(input));

  QString writerName = ConfigOptions().getOsmMapWriterFactoryWriter();
  if (writerName.trimmed().isEmpty())
  {
    writerName = OsmMapWriterFactory::getWriterName(output);
  }
  LOG_VART(writerName);
  LOG_TRACE(OsmMapWriterFactory::getInstance().hasElementOutputStream(output));
  LOG_TRACE(ConfigUtils::boundsOptionEnabled());

  if (OsmMapReaderFactory::getInstance().hasElementInputStream(input) &&
      OsmMapWriterFactory::getInstance().hasElementOutputStream(output) &&
      _areValidStreamingOps(ConfigOptions().getConvertOps()) &&
      //the XML writer can't keep sorted output when streaming, so require an additional config
      //option be specified in order to stream when writing that format
      (writerName != "hoot::OsmXmlWriter" ||
      (writerName == "hoot::OsmXmlWriter" && !ConfigOptions().getWriterXmlSortById())) &&
      //none of the convert bounding box supports are able to do streaming I/O at this point
      !ConfigUtils::boundsOptionEnabled())
  {
    ElementStreamer::stream(input, output);
  }
  else
  {
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(
      map, input, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    NamedOp(ConfigOptions().getConvertOps()).apply(map);
    MapProjector::projectToWgs84(map);
    IoUtils::saveMap(map, output);
  }
}

bool DataConverter::_areValidStreamingOps(const QStringList ops)
{
  // add visitor/criterion operations if any of the convert ops are visitors.
  foreach (QString opName, ops)
  {
    if (!opName.trimmed().isEmpty())
    {
      if (Factory::getInstance().hasBase<ElementCriterion>(opName.toStdString()))
      {
        ElementCriterionPtr criterion(
          Factory::getInstance().constructObject<ElementCriterion>(opName));
        // when streaming we can't provide a reliable OsmMap.
        if (dynamic_cast<OsmMapConsumer*>(criterion.get()) != 0)
        {
          return false;
        }
      }
      else if (Factory::getInstance().hasBase<ElementVisitor>(opName.toStdString()))
      {
        // good, pass
      }
      else
      {
        return false;
      }
    }
  }

  return true;
}

}
