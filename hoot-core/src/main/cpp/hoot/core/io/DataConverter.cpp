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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "DataConverter.h"

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/ElementTranslatorThread.h>
#include <hoot/core/io/OgrWriterThread.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/schema/ScriptToOgrSchemaTranslator.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/DuplicateNodeRemover.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/ops/SchemaTranslationOp.h>
#include <hoot/core/schema/SchemaUtils.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ElementVisitor.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateWayNodesVisitor.h>
#include <hoot/core/visitors/SchemaTranslationVisitor.h>

// std
#include <vector>

// Qt
#include <QElapsedTimer>
#include <QMutex>

namespace hoot
{

const QString DataConverter::JOB_SOURCE = "Convert";

int DataConverter::logWarnCount = 0;

DataConverter::DataConverter() :
_translateMultithreaded(false),
_ogrFeatureReadLimit(0),
_printLengthMax(ConfigOptions().getProgressVarPrintLengthMax())
{
}

void DataConverter::setTranslation(const QString& translation)
{
  if (!translation.isEmpty())
  {
    SchemaUtils::validateTranslationUrl(translation);
    _translation = translation;
  }
}

void DataConverter::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  // The ordering of these setters matter to DataConvertTest.
  setConvertOps(config.getConvertOps());
  setOgrFeatureReadLimit(config.getOgrReaderLimit());
  setShapeFileColumns(config.getShapeFileWriterCols());
  const QString translation = config.getSchemaTranslationScript();
  if (!translation.isEmpty())
  {
    setTranslation(config.getSchemaTranslationScript());
  }
  _translationDirection = config.getSchemaTranslationDirection().trimmed().toLower();
  _translateMultithreaded = config.getConvertTranslateMultithreaded();
}

void DataConverter::convert(const QString& input, const QString& output)
{
  convert(QStringList(input), output);
}

void DataConverter::convert(const QStringList& inputs, const QString& output)
{
  _validateInput(inputs, output);

  _progress.setJobId(ConfigOptions().getJobId());
  _progress.setSource(JOB_SOURCE);
  _progress.setState(Progress::JobState::Running);

  _progress.set(
    0.0,
    "Converting ..." + FileUtils::toLogFormat(inputs, _printLengthMax) + " to ..." +
    FileUtils::toLogFormat(output, _printLengthMax) + "...");

  // OGR format I/O is handled a little different compared to other formats, so we'll pick the
  // most appropriate I/O logic path here based on the formats involved. This has been simplfied
  // several times but there still seems to be some logic duplication, and this can probably still
  // be simplified more.

  // If we're writing to an OGR format and multi-threaded processing was specified or if both input
  // and output formats are OGR formats, we'll need to run the memory bounded _convertToOgr method.
  if ((IoUtils::isSupportedOgrFormat(output, true) && _translateMultithreaded) ||
      (IoUtils::areSupportedOgrFormats(inputs, true) &&
       IoUtils::isSupportedOgrFormat(output, true)))
  {
    _convertToOgr(inputs, output);
  }
  else if (IoUtils::areSupportedOgrFormats(inputs, true))
  {
    // We require that a translation be present when converting from OGR, since OgrReader is tightly
    // coupled to the translation logic. If we have a gdb or a dir as input and no translation is
    // present, we'll add a quick and dirty translation script here.
    if (_translation.isEmpty() &&
        (StringUtils::endsWithAny(inputs, ".gdb") || FileUtils::anyAreDirs(inputs)))
    {
      _translation = "translations/quick.js";
    }
    _convertFromOgr(inputs, output);
  }
  // If none of the above conditions was satisfied, we'll call the generic convert routine. Note
  // that _convert may still be passed some OGR formats, which is a bit confusing and further
  // refactoring might change that.
  else
  {
    _convert(inputs, output);
  }

  _progress.set(
    1.0, Progress::JobState::Successful,
    "Converted ..." + FileUtils::toLogFormat(inputs, _printLengthMax) + " to: ..." +
    FileUtils::toLogFormat(output, _printLengthMax));
}

void DataConverter::_validateInput(const QStringList& inputs, const QString& output) const
{
  LOG_VART(inputs.size());
  LOG_VART(inputs);
  LOG_VART(output);
  LOG_VART(_translation);
  LOG_VART(_shapeFileColumns);
  LOG_VART(_ogrFeatureReadLimit);
  if (!inputs.empty())
  {
    LOG_VART(IoUtils::isSupportedOsmFormat(inputs.at(0)));
    LOG_VART(IoUtils::isSupportedOgrFormat(inputs.at(0), true));
  }
  LOG_VART(IoUtils::areSupportedOgrFormats(inputs, true));
  LOG_VART(IoUtils::isSupportedOsmFormat(output));
  LOG_VART(IoUtils::isSupportedOgrFormat(output));

  if (inputs.empty())
  {
    throw HootException("No input(s) specified.");
  }

  if (output.trimmed().isEmpty())
  {
    throw HootException("No output specified.");
  }

  // I don't think it would be possible for translation to work along with the export columns
  // specified, as you'd be first changing your column names with the translation and then trying
  // to export old column names.  If this isn't true, then we could remove this check and allow it.
  if (!_translation.isEmpty() && _shapeFileColumnsSpecified())
  {
    throw HootException("Cannot specify both a translation and export columns.");
  }

  // We may eventually be able to relax the restriction here of requiring the input be an OSM
  // format, but since cols were originally only used with osm2shp and there's no evidence of a need
  // for anything other than an OSM input whe converting to shape, let's keep this check here for
  // now.
  if (_shapeFileColumnsSpecified() && !output.toLower().endsWith(".shp"))
  {
    throw HootException(
      "Columns may only be specified when converting to the shape file format.");
  }

  // Feature read limit currently is only implemented for OGR inputs.
  if (_ogrFeatureReadLimit > 0 && !IoUtils::areSupportedOgrFormats(inputs, true))
  {
    throw HootException("Read limit may only be specified when converting OGR inputs.");
  }

  if (!IoUtils::isUrl(output))
  {
    // write the output dir now so we don't get a nasty surprise at the end of a long job that it
    // can't be written
    IoUtils::writeOutputDir(output);
  }
}

void DataConverter::_convertToOgr(const QStringList& inputs, const QString& output)
{
  LOG_DEBUG("_convertToOgr");

  // This code path has always assumed translation to OGR and never reads the direction, but let's
  // warn callers that the opposite direction they specified won't be used.
  if (conf().getString(ConfigOptions::getSchemaTranslationDirectionKey()) == "toosm")
  {
    LOG_INFO(
      "Ignoring specified schema.translation.direction=toosm and using toogr to write to " <<
      "OGR output...");
  }

  // Set a config option so the translation script knows what the output format is. For this,
  // output format == file extension. We are going to grab everything after the last "." in the
  // output file name and use it as the file extension.
  QString outputFormat = "";
  if (output.lastIndexOf(".") > -1)
  {
    outputFormat = output.right(output.size() - output.lastIndexOf(".") - 1).toLower();
  }
  conf().set(ConfigOptions::getOgrOutputFormatKey(), outputFormat);

  LOG_DEBUG(conf().getString(ConfigOptions::getOgrOutputFormatKey()));

  // Translation for going to OGR is always required and happens in the writer itself. It is not to
  // be done with convert ops, so let's ignore any translation ops that were specified.
  _convertOps.removeAll(SchemaTranslationOp::className());
  _convertOps.removeAll(SchemaTranslationVisitor::className());
  LOG_VARD(_convertOps);

  // Check to see if all of the i/o can be streamed.
  LOG_VARD(OsmMapReaderFactory::hasElementInputStream(inputs));
  if (OsmMapReaderFactory::hasElementInputStream(inputs) &&
      // Multi-threaded code doesn't support conversion ops. Could it?
      _convertOps.empty() &&
      // Multi-threaded code doesn't support a bounds...not sure if it could be made to at some
      // point.
      !ConfigUtils::boundsOptionEnabled())
  {
    _progress.set(0.0, "Loading and translating maps: ...");
    _transToOgrMT(inputs, output);
  }
  else
  {
    // The number of task steps here must be updated as you add/remove job steps in the logic.
    int numTasks = 2;
    if (!_convertOps.empty())
    {
      numTasks++;
    }
    int currentTask = 1;
    const float taskWeight = 1.0 / (float)numTasks;

    Progress inputLoadProgress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running, 0.0, taskWeight);
    OsmMapPtr map(new OsmMap());
    for (int i = 0; i < inputs.size(); i++)
    {
      inputLoadProgress.setFromRelative(
        (float)i / (float)inputs.size(), Progress::JobState::Running,
        "Loading map: ..." + FileUtils::toLogFormat(inputs.at(i), _printLengthMax) + "...");
      IoUtils::loadMap(
        map, inputs.at(i), ConfigOptions().getReaderUseDataSourceIds(),
        Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    }
    currentTask++;

    if (!_convertOps.empty())
    {
      QElapsedTimer timer;
      timer.start();
      OpExecutor convertOps(_convertOps);
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(currentTask - 1) / (float)numTasks, 1.0 / (float)numTasks));
      convertOps.apply(map);
      currentTask++;
      LOG_STATUS(
        "Convert operations ran in " + StringUtils::millisecondsToDhms(timer.elapsed()) <<
        " total.");
    }

    QElapsedTimer timer;
    timer.start();
    _progress.set(
      (float)(currentTask - 1) / (float)numTasks,
      "Writing map: ..." + FileUtils::toLogFormat(output, _printLengthMax) + "...");
    MapProjector::projectToWgs84(map);
    std::shared_ptr<OgrWriter> writer(new OgrWriter());
    writer->setSchemaTranslationScript(_translation);
    writer->open(output);
    writer->write(map);
    writer->close();
    currentTask++;

    LOG_INFO(
      "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
      " elements to output in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  }
}

void DataConverter::_convertFromOgr(const QStringList& inputs, const QString& output)
{
  LOG_DEBUG("_convertFromOgr");

  QElapsedTimer timer;
  timer.start();

  // This code path has always assumed translation to OSM and never reads the direction, but let's
  // warn callers that if they specified the opposite direction it won't be used.
  if (conf().getString(ConfigOptions::getSchemaTranslationDirectionKey()) == "toogr")
  {
    LOG_INFO(
      "Ignoring specified schema.translation.direction=toogr and using toosm to write to " <<
      "OSM output...");
  }

  _progress.set(0.0, "Loading maps: ..." + FileUtils::toLogFormat(inputs, _printLengthMax) + "...");

  // See similar note in _convertToOgr.
  _convertOps.removeAll(SchemaTranslationOp::className());
  _convertOps.removeAll(SchemaTranslationVisitor::className());
  LOG_VARD(_convertOps);

  _setFromOgrOptions();
  // Inclined to add _convertOps.removeDuplicates() here, but there could be some workflows where
  // the same op needs to be called more than once.
  LOG_VARD(_convertOps);

  // The number of task steps here must be updated as you add/remove job steps in the logic.
  int numTasks = 2;
  if (!_convertOps.empty())
  {
    numTasks++;
  }

  int currentTask = 1;
  const float taskWeight = 1.0 / (float)numTasks;

  OsmMapPtr map(new OsmMap());
  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs[i].trimmed();
    LOG_VARD(input);

    if (input.trimmed().isEmpty())
    {
      LOG_WARN("Got an empty layer, skipping.");
      continue;
    }

    // Pass in the job and task info here so progress gets tracked.
    IoUtils::loadMap(
      map, input, ConfigOptions().getReaderUseDataSourceIds(),
      Status::fromString(ConfigOptions().getReaderSetDefaultStatus()), _translation,
      _ogrFeatureReadLimit, JOB_SOURCE, numTasks);
  }

  if (map->getNodes().size() == 0)
  {
    const QString msg = "After translation the map is empty. Aborting.";
    _progress.set(1.0, Progress::JobState::Failed, msg);
    throw HootException(msg);
  }

  LOG_INFO(
    "Read " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements from input in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  currentTask++;

  if (!_convertOps.empty())
  {
    QElapsedTimer timer2;
    timer2.start();
    OpExecutor convertOps(_convertOps);
    convertOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        (float)(currentTask - 1) / (float)numTasks, taskWeight));
    convertOps.apply(map);
    currentTask++;
    LOG_STATUS(
      "Convert operations ran in " + StringUtils::millisecondsToDhms(timer2.elapsed()) <<
      " total.");
  }

  _progress.set(
    (float)(currentTask - 1) / (float)numTasks,
    "Writing map: ..." + FileUtils::toLogFormat(output, _printLengthMax) + "...");
  MapProjector::projectToWgs84(map);
  IoUtils::saveMap(map, output);
  currentTask++;
}

void DataConverter::_convert(const QStringList& inputs, const QString& output)
{
  LOG_DEBUG("general convert");

  // This keeps the status and the tags.
  conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
  conf().set(ConfigOptions::getReaderKeepStatusTagKey(), true);

  // See note in convert. An OGR format could still be processed here.
  if (IoUtils::anyAreSupportedOgrFormats(inputs, true))
  {
    _setFromOgrOptions();
  }

  // If the translation direction wasn't specified, try to guess it.
  if (!_translation.trimmed().isEmpty() && _translationDirection.isEmpty())
  {
    _translationDirection = _outputFormatToTranslationDirection(output);
    // This gets read by the TranslationVisitor and cannot be empty.
    conf().set(ConfigOptions::getSchemaTranslationDirectionKey(), _translationDirection);
  }

  LOG_VARD(IoUtils::isSupportedOgrFormat(output, true));
  if (IoUtils::isSupportedOgrFormat(output, true))
  {
    _setToOgrOptions(output);
  }
  else
  {
    if (!_translation.trimmed().isEmpty())
    {
      _handleNonOgrOutputTranslationOpts();
    }
  }

  LOG_VARD(_shapeFileColumnsSpecified());

  // Check to see if all of the i/o can be streamed.
  LOG_VARD(ElementStreamer::areValidStreamingOps(_convertOps));
  LOG_VARD(ElementStreamer::areStreamableIo(inputs, output));
  const bool isStreamable =
    ElementStreamer::areValidStreamingOps(_convertOps) &&
    ElementStreamer::areStreamableIo(inputs, output);

  // The number of steps here must be updated as you add/remove job steps in the logic.
  int numTasks = 0;
  if (isStreamable)
  {
    numTasks = 1;   // Streaming combines reading/writing into a single step.
  }
  else
  {
    numTasks = 2;
    if (!_convertOps.empty())
    {
      numTasks++;
    }
  }
  int currentTask = 1;
  const float taskWeight = 1.0 / (float)numTasks;

  if (isStreamable)
  {
    // Shape file output currently isn't streamable, so we know we won't see export cols here. If
    // it is ever made streamable, then we'd have to refactor this and remove the assertion.
    assert(!_shapeFileColumnsSpecified());

    // stream the i/o
    ElementStreamer::stream(
      inputs, output, _convertOps,
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        (float)(currentTask - 1) / (float)numTasks, taskWeight));
    currentTask++;
  }
  else
  {
    Progress inputLoadProgress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running, 0.0, taskWeight);
    OsmMapPtr map(new OsmMap());
    for (int i = 0; i < inputs.size(); i++)
    {
      inputLoadProgress.setFromRelative(
        (float)i / (float)inputs.size(), Progress::JobState::Running,
        "Loading map: ..." + FileUtils::toLogFormat(inputs.at(i), _printLengthMax) + "...");
      IoUtils::loadMap(
        map, inputs.at(i), ConfigOptions().getReaderUseDataSourceIds(),
        Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    }
    currentTask++;

    if (!_convertOps.empty())
    {
      QElapsedTimer timer;
      timer.start();
      OpExecutor convertOps(_convertOps);
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(currentTask - 1) / (float)numTasks, taskWeight));
      convertOps.apply(map);
      currentTask++;
      LOG_STATUS(
        "Convert operations ran in " + StringUtils::millisecondsToDhms(timer.elapsed()) <<
        " total.");
    }

    _progress.set(
      (float)(currentTask - 1) / (float)numTasks,
      "Writing map: ..." + FileUtils::toLogFormat(output, _printLengthMax) + "...");
    MapProjector::projectToWgs84(map);
    if (output.toLower().endsWith(".shp") && _shapeFileColumnsSpecified())
    {
      // If the user specified cols, then we want to export them. This requires a separate logic
      // path from the generic convert logic.
      _exportToShapeWithCols(output, _shapeFileColumns, map);
    }
    else
    {
      LOG_DEBUG("General conversion with: _convert (the original convert command)");
      IoUtils::saveMap(map, output);
    }
    currentTask++;
  }
}

void DataConverter::_exportToShapeWithCols(
  const QString& output, const QStringList& cols, const OsmMapPtr& map) const
{
  LOG_DEBUG("_exportToShapeWithCols");

  QElapsedTimer timer;
  timer.start();

  std::shared_ptr<OsmMapWriter> writer =
    OsmMapWriterFactory::createWriter(output);
  std::shared_ptr<ShapefileWriter> shapeFileWriter =
    std::dynamic_pointer_cast<ShapefileWriter>(writer);
  //currently only one shape file writer, and this is it
  assert(shapeFileWriter.get());
  shapeFileWriter->setColumns(cols);
  shapeFileWriter->open(output);
  shapeFileWriter->write(map, output);

  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements to output in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
}

void DataConverter::_fillElementCacheMT(
  const QString& inputUrl, ElementCachePtr cachePtr, QQueue<ElementPtr>& workQ) const
{
  // Setup reader
  std::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(inputUrl);
  reader->open(inputUrl);
  std::shared_ptr<ElementInputStream> streamReader =
    std::dynamic_pointer_cast<ElementInputStream>(reader);

  std::shared_ptr<OGRSpatialReference> projection = streamReader->getProjection();
  ProjectToGeographicVisitor visitor;
  bool notGeographic = !projection->IsGeographic();

  if (notGeographic)
  {
    visitor.initialize(projection);
  }

  while (streamReader->hasMoreElements())
  {
    ElementPtr pNewElement = streamReader->readNextElement();
    if (!pNewElement)
      continue;
    if (notGeographic)
    {
      visitor.visit(pNewElement);
    }

    workQ.enqueue(pNewElement);
    ConstElementPtr constElement(pNewElement);
    cachePtr->addElement(constElement);
  }

  LOG_DEBUG("Done Reading");
}

void DataConverter::_transToOgrMT(const QStringList& inputs, const QString& output) const
{
  LOG_DEBUG("_transToOgrMT");

  QQueue<ElementPtr> elementQ;
  ElementCachePtr pElementCache(
    new ElementCacheLRU(
      ConfigOptions().getElementCacheSizeNode(),
      ConfigOptions().getElementCacheSizeWay(),
      ConfigOptions().getElementCacheSizeRelation()));
  QMutex initMutex;
  QMutex transFeaturesMutex;
  QQueue<std::pair<std::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>> transFeaturesQ;
  bool finishedTranslating = false;

  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs.at(i).trimmed();

    LOG_DEBUG("Reading: " << input);

    // Read all elements from an input
    _fillElementCacheMT(input, pElementCache, elementQ);
  }

  LOG_DEBUG("Element Cache Filled");

  // Note the OGR writer is the slowest part of this whole operation, but it's relatively opaque
  // to us as a 3rd party library. So the best we can do right now is try to translate & write in
  // parallel.

  // Setup & start translator thread
  ElementTranslatorThread transThread;
  transThread.setTranslation(_translation);
  transThread.setElementQueue(&elementQ);
  transThread.setTransFeaturesQueueMutex(&transFeaturesMutex);
  transThread.setInitMutex(&initMutex);
  transThread.setTransFeaturesQueue(&transFeaturesQ);
  transThread.setFinishedTranslating(&finishedTranslating);
  transThread.setElementCache(pElementCache);
  transThread.start();
  LOG_DEBUG("Translation Thread Started");

  // Setup & start our writer thread
  OgrWriterThread writerThread;
  writerThread.setTranslation(_translation);
  writerThread.setOutput(output);
  writerThread.setTransFeaturesQueueMutex(&transFeaturesMutex);
  writerThread.setInitMutex(&initMutex);
  writerThread.setTransFeaturesQueue(&transFeaturesQ);
  writerThread.setFinishedTranslating(&finishedTranslating);
  writerThread.start();
  LOG_DEBUG("OGR Writer Thread Started");

  LOG_DEBUG("Waiting for writer to finish...");
  writerThread.wait();
}

void DataConverter::_setFromOgrOptions()
{
  // The ordering for these added ops matters. Let's run them after any user specified convert ops
  // to avoid unnecessary processing time. Also, if any of these ops gets added here, then we never
  // have a streaming OGR read, since they all require a full map...don't love that...but not sure
  // what can be done about it.

  // Nodes that are very close together but with different IDs present a problem from OGR sources,
  // so let's merge them together.
  if (ConfigOptions().getOgr2osmMergeNearbyNodes())
  {
    if (!_convertOps.contains(DuplicateNodeRemover::className()))
    {
      _convertOps.append(DuplicateNodeRemover::className());
      // also run dupe way node removal
      _convertOps.append(RemoveDuplicateWayNodesVisitor::className());
    }
  }

  // Complex building simplification is primarily meant for UFD buildings, commonly read from OGR
  // sources.
  if (ConfigOptions().getOgr2osmSimplifyComplexBuildings())
  {
    // Building outline updating needs to happen after building part merging, or we can end up with
    // role verification warnings in JOSM.
    if (!_convertOps.contains(BuildingPartMergeOp::className()))
    {
      _convertOps.append(BuildingPartMergeOp::className());
    }
    if (!_convertOps.contains(BuildingOutlineUpdateOp::className()))
    {
      _convertOps.append(BuildingOutlineUpdateOp::className());
    }
  }
}

void DataConverter::_setToOgrOptions(const QString& output)
{
  // This code path has always assumed translation to OGR and never reads the direction, but let's
  // warn callers that the opposite direction they specified won't be used.
  if (_translationDirection == "toosm")
  {
    LOG_INFO(
      "Ignoring specified schema.translation.direction=toosm and using toogr to write to " <<
      "OGR output...");
  }

  // Set a config option so the translation script knows what the output format is. For this,
  // output format == file extension. We are going to grab everything after the last "." in the
  // output file name and use it as the file extension.
  QString outputFormat = "";
  if (output.lastIndexOf(".") > -1)
  {
    outputFormat = output.right(output.size() - output.lastIndexOf(".") - 1).toLower();
  }
  conf().set(ConfigOptions::getOgrOutputFormatKey(), outputFormat);

  LOG_DEBUG(conf().getString(ConfigOptions::getOgrOutputFormatKey()));

  // Translation for going to OGR is always required and happens in the writer itself. It is not to
  // be done with convert ops, so let's ignore any translation ops that were specified.
  _convertOps.removeAll(SchemaTranslationOp::className());
  _convertOps.removeAll(SchemaTranslationVisitor::className());
}

QString DataConverter::_outputFormatToTranslationDirection(const QString& output) const
{
  if (IoUtils::isSupportedOgrFormat(output, true))
  {
    LOG_INFO("No translation direction specified. Assuming 'toogr' based on output format...");
    return "toogr";
  }
  else if (IoUtils::isSupportedOsmFormat(output))
  {
    LOG_INFO("No translation direction specified. Assuming 'toosm' based on output format...");
    return "toosm";
  }
  else
  {
    LOG_INFO("No translation direction specified. Using 'toosm'...");
    return "toosm";
  }
}

void DataConverter::_handleNonOgrOutputTranslationOpts()
{
  //a previous check was done to make sure both a translation and export cols weren't specified
  assert(!_shapeFileColumnsSpecified());

  // For non OGR conversions, the translation must be passed in as an operator.

  if (!_convertOps.contains(SchemaTranslationOp::className()) &&
      !_convertOps.contains(SchemaTranslationVisitor::className()))
  {
    // If a translation script was specified but not the translation op, we'll add auto add the op
    // as the first conversion operation. If the caller wants the translation done after some
    // other op, then they need to explicitly add it to the op list. Always adding the visitor
    // instead of the op, bc its streamable. However, if any other ops in the group aren't
    // streamable it won't matter anyway.
    _convertOps.prepend(SchemaTranslationVisitor::className());
  }
  else if (_convertOps.contains(SchemaTranslationOp::className()))
  {
    // replacing SchemaTranslationOp with SchemaTranslationVisitor for the reason mentioned above
    _convertOps.replaceInStrings(
      SchemaTranslationOp::className(), SchemaTranslationVisitor::className());
  }
  LOG_VARD(_convertOps);
}

}
