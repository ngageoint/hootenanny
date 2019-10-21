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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "DataConverter.h"

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/ShapefileWriter.h>
#include <hoot/core/io/OgrWriter.h>
#include <hoot/core/io/ElementCacheLRU.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>
#include <hoot/js/v8Engine.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/ops/SchemaTranslationOp.h>
#include <hoot/core/visitors/SchemaTranslationVisitor.h>
#include <hoot/core/ops/BuildingPartMergeOp.h>
#include <hoot/core/ops/MergeNearbyNodes.h>
#include <hoot/core/ops/BuildingOutlineUpdateOp.h>
#include <hoot/core/visitors/WayGeneralizeVisitor.h>
#include <hoot/core/visitors/RemoveDuplicateWayNodesVisitor.h>

// std
#include <vector>

// Qt
#include <QElapsedTimer>

namespace hoot
{

const QString DataConverter::JOB_SOURCE = "Convert";

void elementTranslatorThread::run()
{
  //  Create an isolate for the thread
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate * threadIsolate = v8::Isolate::New(params);
  threadIsolate->Enter();
  v8::Locker v8Lock(threadIsolate);

  ElementPtr pNewElement(NULL);
  ElementProviderPtr cacheProvider(_pElementCache);

  // Setup writer used for translation
  std::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    // We use this init mutex as a bandaid. Hoot uses a lot of problematic
    // singletons that cause issues when you try to multithread stuff.
    QMutexLocker lock(_pInitMutex);
    ogrWriter.reset(new OgrWriter());
    ogrWriter->setSchemaTranslationScript(_translation);
    ogrWriter->initTranslator();
    ogrWriter->setCache(_pElementCache);
  }

  while (!_pElementQ->empty())
  {
    pNewElement = _pElementQ->dequeue();

    std::shared_ptr<geos::geom::Geometry> g;
    std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> tf;
    ogrWriter->translateToFeatures(cacheProvider, pNewElement, g, tf);

    { // Mutex Scope
      QMutexLocker lock(_pTransFeaturesQMutex);
      _pTransFeaturesQ->enqueue(std::pair<std::shared_ptr<geos::geom::Geometry>,
                                std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>>(g, tf));
    }
  }

  { // Mutex Scope
    QMutexLocker lock(_pTransFeaturesQMutex);
    *_pFinishedTranslating = true;
  }
}

void ogrWriterThread::run()
{
  // Messing with these parameters did not improve performance...
  // http://trac.osgeo.org/gdal/wiki/ConfigOptions
  // e.g. using CPLSetConfigOption to set "GDAL_CACHEMAX" to "512" or using CPLSetConfigOption
  // to set "FGDB_BULK_LOAD" to "YES"

  //  Create an isolate for our thread
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate * threadIsolate = v8::Isolate::New(params);
  threadIsolate->Enter();
  v8::Locker v8Lock(threadIsolate);

  bool done = false;
  std::pair<std::shared_ptr<geos::geom::Geometry>,
    std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>> feature;

  // Setup writer
  std::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    QMutexLocker lock(_pInitMutex);
    ogrWriter.reset(new OgrWriter());
    ogrWriter->setSchemaTranslationScript(_translation);
    ogrWriter->open(_output);
  }

  while (!done)
  {
    bool doSleep = false;

    // Get element
    { // Mutex Scope
      QMutexLocker lock(_pTransFeaturesQMutex);
      if (_pTransFeaturesQ->isEmpty())
      {
        doSleep = true;
        if (*_pFinishedTranslating)
        {
          done = true;
        }
      }
      else
      {
        feature = _pTransFeaturesQ->dequeue();
      }
    }

    // Write element or sleep
    if (doSleep)
    {
      doSleep = false;
      msleep(100);
    }
    else
    {
      ogrWriter->writeTranslatedFeature(feature.first, feature.second);
    }
  }
  ogrWriter->close();
}

int DataConverter::logWarnCount = 0;

DataConverter::DataConverter() :
_ogrFeatureReadLimit(0),
_printLengthMax(ConfigOptions().getProgressVarPrintLengthMax())
{
}

void DataConverter::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setConvertOps(config.getConvertOps());
  setOgrFeatureReadLimit(config.getOgrReaderLimit());
  setShapeFileColumns(config.getShapeFileWriterCols());
  setTranslation(config.getSchemaTranslationScript());
  _translationDirection = config.getSchemaTranslationDirection().trimmed().toLower();
  LOG_VARD(_convertOps);
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
    "Converting " + inputs.join(", ").right(_printLengthMax) + " to " +
    output.right(_printLengthMax) + "...");

  // Due to the custom multithreading available for OGR reading, the fact that both OGR reading and
  // writing do their translations inline (don't use SchemaTranslationOp or
  // SchemaTranslationVisitor), and OGR reading support for layer names, conversions involving OGR
  // data must follow a separate logic path from non-OGR data. It would be nice at some point to be
  // able to do everything generically from within the _convert method.

  // We require that a translation be present when converting to OGR, the translation direction be
  // to OGR or unspecified, and that only one input is specified.
  if (inputs.size() == 1 && IoUtils::isSupportedOgrFormat(output, true) &&
      !_translation.isEmpty() &&
      (_translationDirection.isEmpty() || _translationDirection == "toogr"))
  {
    _convertToOgr(inputs.at(0), output);
  }
  // We require that a translation be present when converting from OGR, the translation direction be
  // to OSM or unspecified, and multiple inputs are supported.
  else if (inputs.size() >= 1 && IoUtils::areSupportedOgrFormats(inputs, true) &&
           !_translation.isEmpty() &&
           (_translationDirection.isEmpty() || _translationDirection == "toosm"))
  {
    _convertFromOgr(inputs, output);
  }
  // If this wasn't a to/from OGR conversion, or no translation was specified, or a translation
  // direction different than what was expected for the input/output formats was specified, just
  // call the generic convert routine. If no translation direction was specified, we'll try to guess
  // it and let the user know that we did.
  //
  // Note that it still is possible an OGR format can go in here, if you didn't specify a
  // translation. That seems a little odd and maybe worth rethinking. Most the time you are going to
  // be specifying a translation when dealing with OGR formats, but if you're doing an additional
  // conversion on a data file after an initial one you might not specify a translation.
  else
  {
    _convert(inputs, output);
  }

  _progress.set(
    1.0, Progress::JobState::Successful,
    "Converted ..." + inputs.join(", ").right(_printLengthMax) + " to: ..." +
    output.right(_printLengthMax));
}

void DataConverter::_validateInput(const QStringList& inputs, const QString& output)
{
  LOG_VART(inputs.size());
  LOG_VART(inputs);
  LOG_VART(output);
  LOG_VART(_translation);
  LOG_VART(_shapeFileColumns);
  LOG_VART(_ogrFeatureReadLimit);
  if (inputs.size() > 0)
  {
    LOG_VART(IoUtils::isSupportedOsmFormat(inputs.at(0)));
    LOG_VART(IoUtils::isSupportedOgrFormat(inputs.at(0), true));
  }
  LOG_VART(IoUtils::areSupportedOgrFormats(inputs, true));
  LOG_VART(IoUtils::isSupportedOsmFormat(output));
  LOG_VART(IoUtils::isSupportedOgrFormat(output));

  if (inputs.size() == 0)
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
}

void DataConverter::_fillElementCache(const QString& inputUrl, ElementCachePtr cachePtr,
                                      QQueue<ElementPtr>& workQ)
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

void DataConverter::_transToOgrMT(const QString& input, const QString& output)
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

  // Read all elements
  // TODO: We should figure out a way to make this not-memory bound in the future
  _fillElementCache(input, pElementCache, elementQ);
  LOG_DEBUG("Element Cache Filled");

  // Note the OGR writer is the slowest part of this whole operation, but it's relatively opaque
  // to us as a 3rd party library. So the best we can do right now is try to translate & write in
  // parallel.

  // Setup & start translator thread
  hoot::elementTranslatorThread transThread;
  transThread._translation = _translation;
  transThread._pElementQ = &elementQ;
  transThread._pTransFeaturesQMutex = &transFeaturesMutex;
  transThread._pInitMutex = &initMutex;
  transThread._pTransFeaturesQ = &transFeaturesQ;
  transThread._pFinishedTranslating = &finishedTranslating;
  transThread._pElementCache = pElementCache;
  transThread.start();
  LOG_DEBUG("Translation Thread Started");

  // Setup & start our writer thread
  hoot::ogrWriterThread writerThread;
  writerThread._translation = _translation;
  writerThread._output = output;
  writerThread._pTransFeaturesQMutex = &transFeaturesMutex;
  writerThread._pInitMutex = &initMutex;
  writerThread._pTransFeaturesQ = &transFeaturesQ;
  writerThread._pFinishedTranslating = &finishedTranslating;
  writerThread.start();
  LOG_DEBUG("OGR Writer Thread Started");

  LOG_DEBUG("Waiting for writer to finish...");
  writerThread.wait();
}

void DataConverter::_convertToOgr(const QString& input, const QString& output)
{
  LOG_DEBUG("_convertToOgr (formerly known as osm2ogr)");

  // This code path has always assumed translation to OGR and never reads the direction, but let's
  // warn callers that the opposite direction they specified won't be used.
  if (conf().getString(ConfigOptions::getSchemaTranslationDirectionKey()) == "toosm")
  {
    LOG_INFO(
      "Ignoring specified schema.translation.direction=toosm and using toogr to write to " <<
      "OGR output...");
  }

  // Translation for going to OGR is always required and happens in the writer itself. It is not to
  // be done with convert ops, so let's ignore any translation ops that were specified.
  _convertOps.removeAll(QString::fromStdString(SchemaTranslationOp::className()));
  _convertOps.removeAll(QString::fromStdString(SchemaTranslationVisitor::className()));
  LOG_VARD(_convertOps);

  LOG_VARD(OsmMapReaderFactory::hasElementInputStream(input));
  if (OsmMapReaderFactory::hasElementInputStream(input) &&
      // multithreaded code doesn't support conversion ops. could it?
      // TODO: if we have a single convert op that is a SchemaTranslationOp or
      // SchemaTranslationVisitor should we pop it off and then run multithreaded with that
      // translation?...seems like we should
      _convertOps.size() == 0 &&
      // multithreaded code doesn't support a bounds...not sure if it could be made to at some point
      !ConfigUtils::boundsOptionEnabled())
  {
    _progress.set(0.0, "Loading and translating map: ..." + input.right(_printLengthMax) + "...");
    _transToOgrMT(input, output);
  }
  else
  {
    // The number of task steps here must be updated as you add/remove job steps in the logic.
    int numSteps = 2;
    if (_convertOps.size() > 0)
    {
      numSteps++;
    }
    int currentStep = 1;

    _progress.set(0.0, "Loading map: ..." + input.right(_printLengthMax) + "...");
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, input, true);
    currentStep++;

    if (_convertOps.size() > 0)
    {
      NamedOp convertOps(_convertOps);
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(currentStep - 1) / (float)numSteps, 1.0 / (float)numSteps));
      convertOps.apply(map);
      currentStep++;
    }

    QElapsedTimer timer;
    timer.start();
    _progress.set(
      (float)(currentStep - 1) / (float)numSteps,
      "Writing map: ..." + output.right(_printLengthMax) + "...");
    MapProjector::projectToWgs84(map);
    std::shared_ptr<OgrWriter> writer(new OgrWriter());
    writer->setSchemaTranslationScript(_translation);
    writer->open(output);
    writer->write(map);
    currentStep++;

    LOG_INFO(
      "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
      " elements to output in: " << StringUtils::millisecondsToDhms(timer.elapsed()) << ".");
  }
}

std::vector<float> DataConverter::_getOgrInputProgressWeights(OgrReader& reader,
                                                              const QString& input,
                                                              const QStringList& layers)
{
  std::vector<float> progressWeights;
  LOG_VART(layers.size());

  // process the completion status report information first
  long featureCountTotal = 0;
  int undefinedCounts = 0;
  for (int i = 0; i < layers.size(); i++)
  {
    LOG_VART(layers[i]);
    // simply open the file, get the meta feature count value, and close
    int featuresPerLayer = reader.getFeatureCount(input, layers[i]);
    LOG_VART(featuresPerLayer);
    progressWeights.push_back((float)featuresPerLayer);
    // cover the case where no feature count available efficiently; Despite the documentation
    // saying "-1" should be returned for layers without the number of features calculated, a size
    // of zero has been seen with some layers.
    if (featuresPerLayer < 1) undefinedCounts++;
    else featureCountTotal += featuresPerLayer;
  }
  LOG_VART(featureCountTotal);
  LOG_VART(undefinedCounts);

  int definedCounts = layers.size() - undefinedCounts;
  LOG_VART(definedCounts);

  // determine weights for 3 possible cases
  if (undefinedCounts == layers.size())
  {
    for (int i = 0; i < layers.size(); i++) progressWeights[i] = 1. / (float)layers.size();
  }
  else if (definedCounts == layers.size())
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
    LOG_VART(progressWeights);
    for (int i = 0; i < layers.size(); i++) progressWeights[i] /= (float)featureCountTotal;
  }

  LOG_VART(progressWeights);
  return progressWeights;
}

QStringList DataConverter::_getOgrLayersFromPath(OgrReader& reader, QString& input)
{
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

  return layers;
}

void DataConverter::_setFromOgrOptions(const QStringList& /*inputs*/)
{
  // The ordering for these added ops matters. Let's run them after any user specified convert ops
  // to avoid unnecessary processing time. Also, if any of these ops gets added here, then we never
  // have a streaming OGR read, since they all require a full map...don't love that...but not sure
  // what can be done about it.

  if (ConfigOptions().getOgr2osmMergeNearbyNodes())
  {
    if (!_convertOps.contains(QString::fromStdString(MergeNearbyNodes::className())))
    {
      _convertOps.append(QString::fromStdString(MergeNearbyNodes::className()));
    }
  }

  if (ConfigOptions().getOgr2osmSimplifyComplexBuildings())
  {
    // Building outline updating needs to happen after building part merging, or we can end up with
    // role verification warnings in JOSM.
    if (!_convertOps.contains(QString::fromStdString(BuildingPartMergeOp::className())))
    {
      _convertOps.append(QString::fromStdString(BuildingPartMergeOp::className()));
    }
    if (!_convertOps.contains(QString::fromStdString(BuildingOutlineUpdateOp::className())))
    {
      _convertOps.append(QString::fromStdString(BuildingOutlineUpdateOp::className()));
    }
  }

  // TODO
  if (!_convertOps.contains(QString::fromStdString(RemoveDuplicateWayNodesVisitor::className())))
  {
    _convertOps.append(QString::fromStdString(RemoveDuplicateWayNodesVisitor::className()));
  }

  // TODO
  if (ConfigOptions().getOgr2osmSimplifyWays() &&
      // passes unit tests off (after fixes); ? regression
      //StringUtils::containsSubstring(inputs, "shp") &&
      !_convertOps.contains(QString::fromStdString(WayGeneralizeVisitor::className())))
  {
    // fails unit tests off; ? regression
    conf().set("way.generalizer.criterion", ConfigOptions().getOgr2osmSimplifyWaysCriterion());
    _convertOps.append(QString::fromStdString(WayGeneralizeVisitor::className()));
  }
}

void DataConverter::_convertFromOgr(const QStringList& inputs, const QString& output)
{
  LOG_DEBUG("_convertFromOgr (formerly known as ogr2osm)");

  QElapsedTimer timer;
  timer.start();

  // This code path has always assumed translation to OSM and never reads the direction, but let's
  // warn callers that the opposite direction they specified won't be used.
  if (conf().getString(ConfigOptions::getSchemaTranslationDirectionKey()) == "toogr")
  {
    LOG_INFO(
      "Ignoring specified schema.translation.direction=toogr and using toosm to write to " <<
      "OSM output...");
  }

  _progress.set(0.0, "Loading maps: ..." + inputs.join(",").right(_printLengthMax) + "...");

  OsmMapPtr map(new OsmMap());
  OgrReader reader;
  if (_ogrFeatureReadLimit > 0)
  {
    reader.setLimit(_ogrFeatureReadLimit);
  }
  reader.setSchemaTranslationScript(_translation);

  // see similar note in _convertToOgr
  _convertOps.removeAll(QString::fromStdString(SchemaTranslationOp::className()));
  _convertOps.removeAll(QString::fromStdString(SchemaTranslationVisitor::className()));
  LOG_VARD(_convertOps);

  _setFromOgrOptions(inputs);
  // Inclined to do this: _convertOps.removeDuplicates();, but there could be some workflows where
  // the same op needs to be called more than once.
  //
  LOG_VARD(_convertOps);

  // The number of task steps here must be updated as you add/remove job steps in the logic.
  int numTasks = 2;
  if (_convertOps.size() > 0)
  {
    numTasks++;
  }
  int currentTask = 1;
  const float taskWeight = 1.0 / (float)numTasks;

  for (int i = 0; i < inputs.size(); i++)
  {
    QString input = inputs[i].trimmed();
    LOG_VARD(input);

    if (input.trimmed().isEmpty())
    {
      LOG_WARN("Got an empty layer, skipping.");
      continue;
    }

    const QStringList layers = _getOgrLayersFromPath(reader, input);
    const std::vector<float> progressWeights = _getOgrInputProgressWeights(reader, input, layers);
    // read each layer's data
    for (int i = 0; i < layers.size(); i++)
    {
      PROGRESS_INFO(
        "Reading layer " << i + 1 << " of " << layers.size() << ": " << layers[i] << "...");
      LOG_VART(progressWeights[i]);
      reader.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)i / (float)(layers.size() * numTasks), progressWeights[i]));
      reader.read(input, layers[i], map);
    }
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
  // turn this on for debugging only
  //OsmMapWriterFactory::writeDebugMap(map, "after-convert-from-ogr");
  currentTask++;

  if (_convertOps.size() > 0)
  {
    NamedOp convertOps(_convertOps);
    convertOps.setProgress(
      Progress(
        ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
        (float)(currentTask - 1) / (float)numTasks, taskWeight));
    convertOps.apply(map);
    currentTask++;
  }

  _progress.set(
    (float)(currentTask - 1) / (float)numTasks,
    "Writing map: ..." + output.right(_printLengthMax) + "...");
  MapProjector::projectToWgs84(map);
  IoUtils::saveMap(map, output);
  currentTask++;
}

void DataConverter::_handleGeneralConvertTranslationOpts(const QString& output)
{
  //For non OGR conversions, the translation must be passed in as an op.
  if (!_translation.trimmed().isEmpty())
  {
    //a previous check was done to make sure both a translation and export cols weren't specified
    assert(!_shapeFileColumnsSpecified());

    if (!_convertOps.contains(QString::fromStdString(SchemaTranslationOp::className())) &&
        !_convertOps.contains(QString::fromStdString(SchemaTranslationVisitor::className())))
    {
      // If a translation script was specified but not the translation op, we'll add auto add the op
      // as the first conversion operation. If the caller wants the translation done after some
      // other op, then they need to explicitly add it to the op list. Always adding the visitor
      // instead of the op, bc its streamable. However, if any other ops in the group aren't
      // streamable it won't matter anyway.
      _convertOps.prepend(QString::fromStdString(SchemaTranslationVisitor::className()));
    }
    else if (_convertOps.contains(QString::fromStdString(SchemaTranslationOp::className())))
    {
      // replacing SchemaTranslationOp with SchemaTranslationVisitor for the reason mentioned above
      _convertOps.replaceInStrings(
        QString::fromStdString(SchemaTranslationOp::className()),
        QString::fromStdString(SchemaTranslationVisitor::className()));
    }
    LOG_VARD(_convertOps);

    // If the translation direction wasn't specified, try to guess it.
    if (_translationDirection.isEmpty())
    {
      _translationDirection = _outputFormatToTranslationDirection(output);
      // This gets read by the TranslationVisitor and cannot be empty.
      conf().set(ConfigOptions::getSchemaTranslationDirectionKey(), _translationDirection);
    }
  }
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

void DataConverter::_convert(const QStringList& inputs, const QString& output)
{
  LOG_DEBUG("general convert");

  // This keeps the status and the tags.
  conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
  conf().set(ConfigOptions::getReaderKeepStatusTagKey(), true);

  // see note in convert; an OGR format could still be processed here
  if (IoUtils::anyAreSupportedOgrFormats(inputs, true))
  {
    _setFromOgrOptions(inputs);
  }

  _handleGeneralConvertTranslationOpts(output);

  //check to see if all of the i/o can be streamed
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
    if (_convertOps.size() > 0)
    {
      numTasks++;
    }
  }
  int currentTask = 1;
  const float taskWeight = 1.0 / (float)numTasks;

  if (isStreamable)
  {
    //Shape file output currently isn't streamable, so we know we won't see export cols here. If
    //it is ever made streamable, then we'd have to refactor this.
    assert(!_shapeFileColumnsSpecified());

    //stream the i/o
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
        "Loading map: ..." + inputs.at(i).right(_printLengthMax) + "...");
      IoUtils::loadMap(
        map, inputs.at(i), ConfigOptions().getReaderUseDataSourceIds(),
        Status::fromString(ConfigOptions().getReaderSetDefaultStatus()));
    }
    currentTask++;

    if (_convertOps.size() > 0)
    {
      NamedOp convertOps(_convertOps);
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(currentTask - 1) / (float)numTasks, taskWeight));
      convertOps.apply(map);
      currentTask++;
    }

    _progress.set(
      (float)(currentTask - 1) / (float)numTasks,
      "Writing map: ..." + output.right(_printLengthMax) + "...");
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

void DataConverter::_exportToShapeWithCols(const QString& output, const QStringList& cols,
                                           const OsmMapPtr& map)
{
  LOG_DEBUG("_exportToShapeWithCols (formerly known as osm2shp)");

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

}
