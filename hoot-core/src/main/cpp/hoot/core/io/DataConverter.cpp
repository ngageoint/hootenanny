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
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>
#include <hoot/core/util/Factory.h>

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

void DataConverter::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setConvertOps(config.getConvertOps());
}

void DataConverter::convert(const QStringList inputs, const QString output)
{
  _validateInput(inputs, output);

  LOG_INFO("Converting " << inputs.join(", ").right(100) << " to " << output.right(100) << "...");

  //We require that a translation be present when converting to OGR.  We may be able to absorb this
  //logic into _convert (see notes below).
  if (inputs.size() == 1 && IoUtils::isSupportedOgrFormat(output) && !_translation.isEmpty())
  {
    _convertToOgr(inputs.at(0), output);
  }
  /* We require that a translation be present when converting from OGR.
   *
   * Also, converting to OGR is the only situation where we support multiple inputs.
   *
   * Would like to be absorb some or all of this logic into _convert but not sure its feasible.
   */
  else if (inputs.size() >= 1 && !_translation.isEmpty() &&
           IoUtils::areSupportedOgrFormats(inputs, true))
  {
    _convertFromOgr(inputs, output);
  }
  //If this wasn't a to/from OGR conversion OR no translation was specified, just call the general
  //convert routine (a translation will be applied to non-OGR inputs, if present).
  else if (inputs.size() == 1)
  {
    _convert(inputs.at(0), output);
  }
  else
  {
    //shouldn't ever get here
    throw HootException("Invalid input arguments.");
  }
}

void DataConverter::_validateInput(const QStringList inputs, const QString output)
{
  LOG_VART(inputs.size());
  LOG_VART(inputs);
  LOG_VART(output);
  LOG_VART(_translation);
  LOG_VART(_colsArgSpecified);
  LOG_VART(_columns);
  LOG_VART(_featureReadLimit);
  if (inputs.size() > 0)
  {
    LOG_VART(IoUtils::isSupportedOsmFormat(inputs.at(0)));
    LOG_VART(IoUtils::isSupportedOgrFormat(inputs.at(0), true));
  }
  LOG_VART(IoUtils::areSupportedOgrFormats(inputs, true));
  LOG_VART(IoUtils::isSupportedOsmFormat(output));
  LOG_VART(IoUtils::isSupportedOgrFormat(output));

  //I was tempted to also add an exception that prevents you from trying to convert from one format
  //as input and the same format as output.  We do that in a couple of tests, and while there may
  //be a way to rework the tests so we don't do it anymore, I haven't looked into it. - BDW

  if (inputs.size() == 0)
  {
    throw HootException("No input(s) specified.");
  }

  if (output.trimmed().isEmpty())
  {
    throw HootException("No output specified.");
  }

  //I don't think it would be possible for translation to work along with the export columns
  //specified, as you'd be first changing your column names with the translation and then trying
  //to export old column names.  If this isn't true, then we could remove this.
  if (!_translation.isEmpty() && _colsArgSpecified)
  {
    throw HootException("Cannot specify both a translation and export columns.");
  }

  if (!_translation.isEmpty() && (_convertOps.contains("hoot::TranslationOp") ||
                                  _convertOps.contains("hoot::TranslationVisitor")))
  {
    throw HootException(
      "Cannot specify both a translation as an input parameter as a configuration option.");
  }

  //We may eventually be able to relax the restriction here of requiring the input be an OSM
  //format, but since cols were originally only used with osm2shp, let's keep it here for now.
  if (_colsArgSpecified && !output.toLower().endsWith(".shp"))
  {
    throw HootException(
      "Columns may only be specified when converting to the shape file format.");
  }

  //Should the feature read limit eventually be supported for all types of inputs?
  if (_featureReadLimit > 0 && !IoUtils::areSupportedOgrFormats(inputs, true))
  {
    throw HootException("Read limit may only be specified when converting OGR inputs.");
  }
}

void DataConverter::_convertToOgr(const QString input, const QString output)
{
  LOG_TRACE("_convertToOgr (formerly known as osm2ogr)");

  //This entire method could be replaced by _convert, if refactoring of the way OgrWriter handles
  //translations is done.  Currently, it depends that a translation script is set directly on it
  //(vs using a translation visitor).  See #2416.

  boost::shared_ptr<OgrWriter> writer(new OgrWriter());
  writer->setScriptPath(_translation);
  writer->open(output);

  OsmMapReaderFactory readerFactory = OsmMapReaderFactory::getInstance();
  if (readerFactory.hasElementInputStream(input) &&
      //TODO: this ops restriction needs to be removed and the ops applied during streaming.
      _convertOps.size() == 0 &&
      //none of the convert bounding box supports are able to do streaming I/O at this point
      !ConfigUtils::boundsOptionEnabled())
  {
    boost::shared_ptr<OsmMapReader> reader =
      OsmMapReaderFactory::getInstance().createReader(input);
    reader->open(input);
    boost::shared_ptr<ElementInputStream> streamReader =
      boost::dynamic_pointer_cast<ElementInputStream>(reader);
    //OgrWriter is streamable, so no extra check needed here.
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

    NamedOp(_convertOps).apply(map);
    MapProjector::projectToWgs84(map);
    writer->write(map);
  }
}

void DataConverter::_convertFromOgr(const QStringList inputs, const QString output)
{
  LOG_TRACE("_convertFromOgr (formerly known as ogr2osm)");

  OsmMapPtr map(new OsmMap());
  OgrReader reader;
  if (_featureReadLimit > 0)
  {
    reader.setLimit(_featureReadLimit);
  }
  reader.setTranslationFile(_translation);

  ConfigOptions configOptions;

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
  //the ordering for these ogr2osm ops may matter
  if (ConfigOptions().getOgr2osmSimplifyComplexBuildings())
  {
    _convertOps.prepend("hoot::BuildingPartMergeOp");
  }
  if (ConfigOptions().getOgr2osmMergeNearbyNodes())
  {
    _convertOps.prepend("hoot::MergeNearbyNodes");
  }
  NamedOp(_convertOps).apply(map);
  MapProjector::projectToWgs84(map);
  IoUtils::saveMap(map, output);

  progress.set(1.0, "Successful", true, "Finished successfully.");
}

void DataConverter::_convert(const QString input, const QString output)
{
  LOG_TRACE("general convert");

  // This keeps the status and the tags.
  conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
  conf().set(ConfigOptions::getReaderKeepStatusTagKey(), true);
  LOG_VART(OsmMapReaderFactory::getInstance().hasElementInputStream(input));

  //For non OGR conversions, the translation must be passed in as an op.
  if (!_translation.trimmed().isEmpty())
  {
    //a previous check was done to make sure both a translation and export cols weren't specified
    assert(!_colsArgSpecified);
    //a previous check was done to make sure a translation wasn't specified as both a command line
    //input and a convert op
    assert (_convertOps.contains("hoot::TranslationOp") &&
            _convertOps.contains("hoot::TranslationVisitor"));

    _convertOps.prepend("hoot::TranslationOp");
    LOG_VART(_convertOps);
    conf().set(ConfigOptions::getTranslationScriptKey(), _translation);
    LOG_VART(conf().get(ConfigOptions().getTranslationScriptKey()));
  }

  QString writerName = ConfigOptions().getOsmMapWriterFactoryWriter();
  if (writerName.trimmed().isEmpty())
  {
    writerName = OsmMapWriterFactory::getWriterName(output);
  }
  LOG_VART(writerName);
  LOG_TRACE(OsmMapWriterFactory::getInstance().hasElementOutputStream(output));
  LOG_TRACE(ConfigUtils::boundsOptionEnabled());

  //try to stream the i/o
  if (OsmMapReaderFactory::getInstance().hasElementInputStream(input) &&
      OsmMapWriterFactory::getInstance().hasElementOutputStream(output) &&
      _areValidStreamingOps(_convertOps) &&
      //the XML writer can't keep sorted output when streaming, so require an additional config
      //option be specified in order to stream when writing that format
      (writerName != "hoot::OsmXmlWriter" ||
      (writerName == "hoot::OsmXmlWriter" && !ConfigOptions().getWriterXmlSortById())) &&
      //none of the convert bounding box supports are able to do streaming I/O at this point
      !ConfigUtils::boundsOptionEnabled())
  {
    //Shape file output currently isn't streamable, so we know we won't see export cols here.  If
    //it is ever made streamable, then we'd have to refactor this.
    assert(!_colsArgSpecified);

    ElementStreamer::stream(input, output);
  }
  //can't stream the i/o
  else
  {
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(
      map, input, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));

    NamedOp(_convertOps).apply(map);
    MapProjector::projectToWgs84(map);

    if (output.toLower().endsWith(".shp") && _colsArgSpecified)
    {
      LOG_TRACE("_exportToShapeWithCols (formerly known as osm2shp)");

      //if the user specified cols, then we want to export them
      _exportToShapeWithCols(output, _columns, map);
    }
    else
    {
      LOG_TRACE("General conversion with: _convert (the original convert command)");

      IoUtils::saveMap(map, output);
    }
  }
}

void DataConverter::_exportToShapeWithCols(const QString output, const QStringList cols,
                                           OsmMapPtr map)
{
  boost::shared_ptr<OsmMapWriter> writer =
    OsmMapWriterFactory::getInstance().createWriter(output);
  boost::shared_ptr<ShapefileWriter> shapeFileWriter =
    boost::dynamic_pointer_cast<ShapefileWriter>(writer);
  //currently only one shape file writer, and this is it
  assert(shapeFileWriter.get());
  shapeFileWriter->setColumns(cols);
  shapeFileWriter->open(output);
  shapeFileWriter->write(map, output);
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
