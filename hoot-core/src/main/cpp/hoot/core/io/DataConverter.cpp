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
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/ProjectToGeographicVisitor.h>
#include <hoot/js/v8Engine.h>
#include <hoot/core/util/StringUtils.h>

// std
#include <vector>

// Qt
#include <QElapsedTimer>

namespace hoot
{

const QString DataConverter::JOB_SOURCE = "Convert";
const int DataConverter::MAX_FILE_PRINT_LENGTH = 30;

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
  boost::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    // We use this init mutex as a bandaid. Hoot uses a lot of problematic
    // singletons that cause issues when you try to multithread stuff.
    QMutexLocker lock(_pInitMutex);
    ogrWriter.reset(new OgrWriter());
    ogrWriter->setScriptPath(_translation);
    ogrWriter->initTranslator();
    ogrWriter->setCache(_pElementCache);
  }

  while (!_pElementQ->empty())
  {
    pNewElement = _pElementQ->dequeue();

    boost::shared_ptr<geos::geom::Geometry> g;
    std::vector<ScriptToOgrTranslator::TranslatedFeature> tf;
    ogrWriter->translateToFeatures(cacheProvider, pNewElement, g, tf);

    { // Mutex Scope
      QMutexLocker lock(_pTransFeaturesQMutex);
      _pTransFeaturesQ->enqueue(std::pair<boost::shared_ptr<geos::geom::Geometry>,
                                std::vector<ScriptToOgrTranslator::TranslatedFeature>>(g, tf));
    }
  }

  { // Mutex Scope
    QMutexLocker lock(_pTransFeaturesQMutex);
    *_pFinishedTranslating = true;
  }

  LOG_DEBUG("Done Translating");
}

void ogrWriterThread::run()
{
  // Messing with these parameters did not improve performance...
  // http://trac.osgeo.org/gdal/wiki/ConfigOptions
  // e.g. using CPLSetConfigOption to set "GDAL_CACHEMAX" to "512"
  // or using CPLSetConfigOption to set "FGDB_BULK_LOAD" to "YES"

  //  Create an isolate for our thread
  v8::Isolate::CreateParams params;
  params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate * threadIsolate = v8::Isolate::New(params);
  threadIsolate->Enter();
  v8::Locker v8Lock(threadIsolate);

  bool done = false;
  std::pair<boost::shared_ptr<geos::geom::Geometry>,
    std::vector<ScriptToOgrTranslator::TranslatedFeature>> feature;

  // Setup writer
  boost::shared_ptr<OgrWriter> ogrWriter;
  { // Mutex Scope
    QMutexLocker lock(_pInitMutex);
    ogrWriter.reset(new OgrWriter());
    ogrWriter->setScriptPath(_translation);
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

  LOG_DEBUG("Done Writing Features");
}

int DataConverter::logWarnCount = 0;

DataConverter::DataConverter() :
_colsArgSpecified(false),
_featureReadLimit(0),
_printLengthMax(ConfigOptions().getProgressVarPrintLengthMax())
{
}

void DataConverter::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);
  setConvertOps(config.getConvertOps());
  LOG_VARD(_convertOps);
}

void DataConverter::convert(const QStringList inputs, const QString output)
{
  _validateInput(inputs, output);

  _progress.setSource(JOB_SOURCE);
  _progress.setState(Progress::JobState::Running);

   LOG_STATUS(
    "Converting " << inputs.join(", ").right(_printLengthMax) << " to " <<
    output.right(_printLengthMax) << "...");

  //We require that a translation be present when converting to OGR.  We may be able to absorb this
  //logic into _convert (see notes below).
  if (inputs.size() == 1 && IoUtils::isSupportedOgrFormat(output, true) && !_translation.isEmpty())
  {
    _convertToOgr(inputs.at(0), output);
  }
  /* We require that a translation be present when converting from OGR.
   * Would like to be absorb some or all of this logic into _convert but not sure its feasible.
   */
  else if (inputs.size() >= 1 && !_translation.isEmpty() &&
           IoUtils::areSupportedOgrFormats(inputs, true))
  {
    _convertFromOgr(inputs, output);
  }
  //If this wasn't a to/from OGR conversion OR no translation was specified, just call the general
  //convert routine (a translation will be applied to non-OGR inputs, if present).
  else
  {
    _convert(inputs, output);
  }

  _progress.set(
    1.0, Progress::JobState::Successful, true,
    "Converted ..." + inputs.join(", ").right(_printLengthMax) + " to: ..." +
    output.right(_printLengthMax));
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

void DataConverter::_fillElementCache(QString inputUrl, ElementCachePtr cachePtr,
                                      QQueue<ElementPtr> &workQ)
{
  // Setup reader
  boost::shared_ptr<OsmMapReader> reader =
    OsmMapReaderFactory::createReader(inputUrl);
  reader->open(inputUrl);
  boost::shared_ptr<ElementInputStream> streamReader =
    boost::dynamic_pointer_cast<ElementInputStream>(reader);

  boost::shared_ptr<OGRSpatialReference> projection = streamReader->getProjection();
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

void DataConverter::_transToOgrMT(QString input, QString output)
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
  QQueue<std::pair<boost::shared_ptr<geos::geom::Geometry>,
         std::vector<ScriptToOgrTranslator::TranslatedFeature>>> transFeaturesQ;
  bool finishedTranslating = false;

  // Read all elements
  // We should figure out a way to make this not-memory bound in the future
  _fillElementCache(input, pElementCache, elementQ);
  LOG_DEBUG("Element Cache Filled");

  // Note the OGR writer is the slowest part of this whole operation,
  // but it's relatively opaque to us as a 3rd party library. So the best we
  // can do right now is try to translate & write in parallel

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

  // Wait for writer to finish
  LOG_DEBUG("Waiting for writer to finish...");
  writerThread.wait();
}

void DataConverter::_convertToOgr(const QString input, const QString output)
{
  LOG_DEBUG("_convertToOgr (formerly known as osm2ogr)");

  if (OsmMapReaderFactory::hasElementInputStream(input) &&
      // TODO: I *believe* this ops size restriction needs to be replaced with a check similar to
      // what is in _convert...but not sure.
      _convertOps.size() == 0 &&
      //none of the convert bounding box supports are able to do streaming I/O at this point
      !ConfigUtils::boundsOptionEnabled())
  {
    _transToOgrMT(input, output);
  }
  else
  {
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
          JOB_SOURCE, Progress::JobState::Running,
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
    boost::shared_ptr<OgrWriter> writer(new OgrWriter());
    writer->setScriptPath(_translation);
    writer->open(output);
    writer->write(map);
    currentStep++;

    LOG_INFO(
      "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
      " elements to output in: " << StringUtils::secondsToDhms(timer.elapsed()) << ".");
  }
}

std::vector<float> DataConverter::_getOgrInputProgressWeights(OgrReader& reader,
                                                              const QString input,
                                                              const QStringList layers)
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
    // cover the case where no feature count available efficiently
    if (featuresPerLayer /*== -1*/ < 1) undefinedCounts++;
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

void DataConverter::_convertFromOgr(const QStringList inputs, const QString output)
{
  LOG_DEBUG("_convertFromOgr (formerly known as ogr2osm)");

  QElapsedTimer timer;
  timer.start();

  _progress.set(0.0, "Loading maps: ..." + inputs.join(",").right(_printLengthMax) + "...");

  OsmMapPtr map(new OsmMap());
  OgrReader reader;
  if (_featureReadLimit > 0)
  {
    reader.setLimit(_featureReadLimit);
  }
  reader.setTranslationFile(_translation);

  // The ordering for these ogr2osm ops matters.
  if (ConfigOptions().getOgr2osmSimplifyComplexBuildings())
  {
    _convertOps.prepend("hoot::BuildingPartMergeOp");
  }
  if (ConfigOptions().getOgr2osmMergeNearbyNodes())
  {
    _convertOps.prepend("hoot::MergeNearbyNodes");
  }
  LOG_VARD(_convertOps);

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
      PROGRESS_STATUS(
        "Reading layer " << i + 1 << " of " << layers.size() << ": " << layers[i] << "...");
      LOG_VART(progressWeights[i]);
      reader.setProgress(
        Progress(
          JOB_SOURCE, Progress::JobState::Running, (float)i / (float)(layers.size() * numTasks),
          progressWeights[i]));
      reader.read(input, layers[i], map);
    }
  }

  if (map->getNodes().size() == 0)
  {
    const QString msg = "After translation the map is empty. Aborting.";
    _progress.set(1.0, Progress::JobState::Failed, true, msg);
    throw HootException(msg);
  }

  LOG_INFO(
    "Read " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements from input in: " << StringUtils::secondsToDhms(timer.elapsed()) << ".");
  currentTask++;

  MapProjector::projectToPlanar(map);
  if (_convertOps.size() > 0)
  {
    NamedOp convertOps(_convertOps);
    convertOps.setProgress(
      Progress(
        JOB_SOURCE, Progress::JobState::Running, (float)(currentTask - 1) / (float)numTasks,
        taskWeight));
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

void DataConverter::_convert(const QStringList inputs, const QString output)
{
  LOG_DEBUG("general convert");

  // This keeps the status and the tags.
  conf().set(ConfigOptions::getReaderUseFileStatusKey(), true);
  conf().set(ConfigOptions::getReaderKeepStatusTagKey(), true);

  //For non OGR conversions, the translation must be passed in as an op.
  if (!_translation.trimmed().isEmpty())
  {
    //a previous check was done to make sure both a translation and export cols weren't specified
    assert(!_colsArgSpecified);
    //a previous check was done to make sure a translation wasn't specified as both a command line
    //input and a convert op
    assert(_convertOps.contains("hoot::TranslationOp") &&
           _convertOps.contains("hoot::TranslationVisitor"));

    _convertOps.prepend("hoot::TranslationOp");
    LOG_VARD(_convertOps);
    conf().set(ConfigOptions::getTranslationScriptKey(), _translation);
    LOG_VART(conf().get(ConfigOptions().getTranslationScriptKey()));
  }

  //check to see if all of the i/o can be streamed
  const bool isStreamable =
    ElementStreamer::areValidStreamingOps(_convertOps) &&
    ElementStreamer::areStreamableIo(inputs, output);

  int numTasks = 0;
  if (isStreamable)
  {
    numTasks = 1;
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
    //Shape file output currently isn't streamable, so we know we won't see export cols here.  If
    //it is ever made streamable, then we'd have to refactor this.
    assert(!_colsArgSpecified);

    //stream the i/o
    ElementStreamer::stream(
      inputs, output, QStringList(),
      Progress(
        JOB_SOURCE, Progress::JobState::Running, (float)(currentTask - 1) / (float)numTasks,
        taskWeight));
    currentTask++;
  }
  else
  {
    Progress inputLoadProgress(JOB_SOURCE, Progress::JobState::Running, 0.0, taskWeight);
    OsmMapPtr map(new OsmMap());
    for (int i = 0; i < inputs.size(); i++)
    {
      inputLoadProgress.setFromRelative(
        (float)i / (float)inputs.size(), Progress::JobState::Running, false,
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
          JOB_SOURCE, Progress::JobState::Running, (float)(currentTask - 1) / (float)numTasks,
          taskWeight));
      convertOps.apply(map);
      currentTask++;
    }

    _progress.set(
      (float)(currentTask - 1) / (float)numTasks,
      "Writing map: ..." + output.right(_printLengthMax) + "...");
    MapProjector::projectToWgs84(map);
    if (output.toLower().endsWith(".shp") && _colsArgSpecified)
    {
      // If the user specified cols, then we want to export them.
      _exportToShapeWithCols(output, _columns, map);
    }
    else
    {
      LOG_DEBUG("General conversion with: _convert (the original convert command)");
      IoUtils::saveMap(map, output);
    }
    currentTask++;
  }
}

void DataConverter::_exportToShapeWithCols(const QString output, const QStringList cols,
                                           OsmMapPtr map)
{
  LOG_DEBUG("_exportToShapeWithCols (formerly known as osm2shp)");

  QElapsedTimer timer;
  timer.start();

  boost::shared_ptr<OsmMapWriter> writer =
    OsmMapWriterFactory::createWriter(output);
  boost::shared_ptr<ShapefileWriter> shapeFileWriter =
    boost::dynamic_pointer_cast<ShapefileWriter>(writer);
  //currently only one shape file writer, and this is it
  assert(shapeFileWriter.get());
  shapeFileWriter->setColumns(cols);
  shapeFileWriter->open(output);
  shapeFileWriter->write(map, output);

  LOG_INFO(
    "Wrote " << StringUtils::formatLargeNumber(map->getElementCount()) <<
    " elements to output in: " << StringUtils::secondsToDhms(timer.elapsed()) << ".");
}

}
