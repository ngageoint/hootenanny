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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ChangesetCreator.h"

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/ExternalMergeElementSorter.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/ops/SuperfluousNodeRemover.h>
#include <hoot/core/ops/RecursiveSetTagValueOp.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementTypeCriterion.h>

//GEOS
#include <geos/geom/Envelope.h>

namespace hoot
{

const QString ChangesetCreator::JOB_SOURCE = "Derive Changeset";

ChangesetCreator::ChangesetCreator(const bool printStats, const QString osmApiDbUrl) :
_osmApiDbUrl(osmApiDbUrl),
_numTotalTasks(0),
_currentTaskNum(0),
_printStats(printStats),
_singleInput(false)
{
}

void ChangesetCreator::create(const QString& output, const QString& input1, const QString& input2)
{
  if (!_isSupportedOutputFormat(output))
  {
    throw HootException("Unsupported changeset output format: " + output);
  }
  else if (output.endsWith(".osc.sql") && _osmApiDbUrl.isEmpty())
  {
    throw IllegalArgumentException(
      "Output to SQL changeset requires an OSM API database URL be specified.");
  }

  _singleInput = input2.trimmed().isEmpty();
  LOG_VARD(_singleInput);
  // both inputs must support streaming to use streaming I/O
  const bool useStreamingIo =
    _inputIsStreamable(input1) && (_singleInput || _inputIsStreamable(input2));
  LOG_VARD(useStreamingIo);

  // The number of steps here must be updated as you add/remove job steps in the logic.
  _numTotalTasks = 2;
  // for non-streamable convert ops and other inline ops that occur when not streaming
  if (!useStreamingIo)
  {
    // For non-streaming I/O we can divide each data conversion task into a separate step, hence
    // the larger number of steps. With streaming I/O that isn't possible since all the data
    // conversion operations are executed inline at the same time the data is read in.
    _numTotalTasks += 4;
    if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())
    {
      _numTotalTasks++;
    }
    if (ConfigOptions().getConvertOps().size() > 0)
    {
      // Convert ops get a single task, which NamedOp will break down into sub-tasks during
      // progress reporting.
      _numTotalTasks++;
      if (!ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()))
      {
        // Have the extra work of combining and separating data inputs when any of the convert
        // ops aren't streamable.
        _numTotalTasks++;
      }
    }
  }

  _currentTaskNum = 1;
  Progress progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running);
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();

  progress.set(
    0.0,
    "Deriving output changeset: ..." + output.right(maxFilePrintLength) + " from inputs: ..." +
    input1.right(maxFilePrintLength) + " and ..." + input2.right(maxFilePrintLength) + "...");

  // Allow for a buffer around the AOI where the changeset derivation is to occur, if there is an
  // AOI.
  _parseBuffer();

  //sortedElements1 is the former state of the data
  ElementInputStreamPtr sortedElements1;
  //sortedElements2 is the newer state of the data
  ElementInputStreamPtr sortedElements2;

  // If we have two inputs, we'll determine the difference between them as the changeset.
  // Otherwise, we're passing all the input data through to the output changeset, so put it in
  // the sortedElements2 newer data and leave the first one empty. The result will be a changeset
  // made up completely of what is in the single input.

  if (!useStreamingIo)
  {
    // In the case that not all input formats or convert ops are streamable or the
    // user chose to force in memory streaming by not specifying a sort buffer size, let's
    // use memory bound sorting.

    // read both inputs completely
    OsmMapPtr map1(new OsmMap());
    OsmMapPtr map2(new OsmMap());
    _readInputsFully(input1, input2, map1, map2, progress);

    // TODO: There need to be checks here to only sort if the input isn't already sorted like
    // there are for the external sorting (e.g. pre-sorted PBF file).

    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
      "Sorting input elements; task #" + QString::number(_currentTaskNum) + "...");
    if (!_singleInput)
    {
      sortedElements1 = _sortElementsInMemory(map1);
      assert(map2.get());
      sortedElements2 = _sortElementsInMemory(map2);
    }
    else
    {
      sortedElements1 = _getEmptyInputStream();
      sortedElements2 = _sortElementsInMemory(map1);
    }
    _currentTaskNum++;
  }
  else
  {
    // If external sorting is enabled and the input and convert ops are streamable, externally
    // sort the elements to avoid potential memory issues.

    if (!_singleInput)
    {
      sortedElements1 = _getExternallySortedElements(input1, progress);
      sortedElements2 = _getExternallySortedElements(input2, progress);
    }
    else
    {

      sortedElements1 = _getEmptyInputStream();
      sortedElements2 = _getExternallySortedElements(input1, progress);
    }
    _currentTaskNum++;
  }

  // write out the changeset file
  assert(sortedElements1.get() && sortedElements2.get());
  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Writing changeset...");
  _streamChangesetOutput(sortedElements1, sortedElements2, output);
  _currentTaskNum++;

  progress.set(
    1.0, Progress::JobState::Successful,
    "Changeset written to: ..." + output.right(maxFilePrintLength));
}

void ChangesetCreator::create(OsmMapPtr& map1, OsmMapPtr& map2, const QString& output)
{
  // TODO: implement progress

  // don't want to include review relations
  std::shared_ptr<TagKeyCriterion> elementCriterion(
    new TagKeyCriterion(MetadataTags::HootReviewNeeds()));
  RemoveElementsVisitor removeElementsVisitor;
  removeElementsVisitor.setRecursive(false);
  removeElementsVisitor.addCriterion(elementCriterion);
  map1->visitRw(removeElementsVisitor);
  map2->visitRw(removeElementsVisitor);

  // Truncate tags over 255 characters to push into OSM API.
  ApiTagTruncateVisitor truncateTags;
  map1->visitRw(truncateTags);
  map2->visitRw(truncateTags);

  //sortedElements1 is the former state of the data
  ElementInputStreamPtr sortedElements1;
  //sortedElements2 is the newer state of the data
  ElementInputStreamPtr sortedElements2;

  // no need to implement application of ops for this logic path

  if (map2)
  {
    sortedElements1 = _sortElementsInMemory(map1);
    sortedElements2 = _sortElementsInMemory(map2);
  }
  else
  {
    sortedElements1 = _getEmptyInputStream();
    sortedElements2 = _sortElementsInMemory(map1);
  }

  // write out the changeset file
  assert(sortedElements1.get() && sortedElements2.get());;
  _streamChangesetOutput(sortedElements1, sortedElements2, output);
}

void ChangesetCreator::_parseBuffer()
{
  // TODO: get rid of changeset buffer
  LOG_DEBUG("Parsing changeset buffer...");

  const double changesetBuffer = ConfigOptions().getChangesetBuffer();
  LOG_VARD(changesetBuffer);
  if (changesetBuffer > 0.0)
  {
    //allow for calculating the changeset with a slightly different AOI than the default specified
    //bounding box

    QString bboxStr;
    QString convertBoundsParamName;
    //only one of these three should be specified (why?)
    if (!ConfigOptions().getConvertBoundingBox().isEmpty())
    {
      bboxStr = ConfigOptions().getConvertBoundingBox();
      convertBoundsParamName = ConfigOptions::getConvertBoundingBoxKey();
    }
    else if (!ConfigOptions().getConvertBoundingBoxHootApiDatabase().isEmpty())
    {
      bboxStr = ConfigOptions().getConvertBoundingBoxHootApiDatabase();
      convertBoundsParamName = ConfigOptions::getConvertBoundingBoxHootApiDatabaseKey();
    }
    else if (!ConfigOptions().getConvertBoundingBoxOsmApiDatabase().isEmpty())
    {
      bboxStr = ConfigOptions().getConvertBoundingBoxOsmApiDatabase();
      convertBoundsParamName = ConfigOptions::getConvertBoundingBoxOsmApiDatabaseKey();
    }
    else
    {
      throw IllegalArgumentException(
        QString("A changeset buffer was specified but no convert bounding box was specified ") +
        QString("with the convert.bounding.box configuration option."));
    }
    geos::geom::Envelope convertBounds = GeometryUtils::envelopeFromConfigString(bboxStr);
    // shrink the bbox by the specified distance to give us a small buffer for reconnecting split
    // features
    convertBounds.expandBy(-1 * changesetBuffer, -1 * changesetBuffer);
    conf().set(convertBoundsParamName, GeometryUtils::envelopeToConfigString(convertBounds));
  }
}

bool ChangesetCreator::_isSupportedOutputFormat(const QString& format) const
{
  return format.endsWith(".osc") || format.endsWith(".osc.sql");
}

bool ChangesetCreator::_inputIsSorted(const QString& input) const
{
  // ops could change the ordering
  if (ConfigOptions().getConvertOps().size() > 0)
  {
    return false;
  }

  //Streaming db inputs actually do not come back sorted, despite the order by id clause
  //in the query (see ApiDb::selectElements).  Otherwise, we'd skip sorting them too.

  //pbf sets a sort flag
  if (OsmPbfReader().isSupported(input) && OsmPbfReader().isSorted(input))
  {
    return true;
  }
  return false;
}

bool ChangesetCreator::_inputIsStreamable(const QString& input) const
{
  LOG_VARD(OsmMapReaderFactory::hasElementInputStream(input));
  LOG_VARD(ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()));
  LOG_VARD(ConfigOptions().getElementSorterElementBufferSize());
  return
    // The input format itself must be streamable (partially read).
    OsmMapReaderFactory::hasElementInputStream(input) &&
    // All ops must be streamable, otherwise we'll load both inputs into memory.
    ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()) &&
    // If no sort buffer size is set, we sort in-memory. If we're already loading the data
    // into memory for sorting, might as well force it into memory for the initial read as well.
    ConfigOptions().getElementSorterElementBufferSize() != -1;
}

void ChangesetCreator::_handleUnstreamableConvertOpsInMemory(const QString& input1,
                                                             const QString& input2,
                                                             OsmMapPtr& map1, OsmMapPtr& map2,
                                                             Progress progress)
{
  LOG_DEBUG("Handling unstreamable convert ops in memory...");

  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input ...");
  OsmMapPtr fullMap(new OsmMap());
  if (!_singleInput)
  {
    // We must preserve the original element IDs while loading in order for changeset derivation
    // to work.

    // Load the first map. If we have a bounded query, let's check for the crop related option
    // overrides.
    if (ConfigUtils::boundsOptionEnabled())
    {
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
        ConfigOptions().getChangesetReferenceKeepEntireFeaturesCrossingBounds());
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
        ConfigOptions().getChangesetReferenceKeepOnlyFeaturesInsideBounds());
    }
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown1);
    OsmMapWriterFactory::writeDebugMap(fullMap, "after-initial-read-unstreamable-ref-map");

    // append the second map onto the first one

    OsmMapPtr tmpMap(new OsmMap());
    // same as above but for the secondary features
    if (ConfigUtils::boundsOptionEnabled())
    {
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
        ConfigOptions().getChangesetSecondaryKeepEntireFeaturesCrossingBounds());
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
        ConfigOptions().getChangesetSecondaryKeepOnlyFeaturesInsideBounds());
    }
    IoUtils::loadMap(tmpMap, input2, true, Status::Unknown2);
    OsmMapWriterFactory::writeDebugMap(tmpMap, "after-initial-read-unstreamable-sec-map");

    // appending maps requires they have node hashes
    CalculateHashVisitor2 hashVis;
    fullMap->visitRw(hashVis);
    LOG_VARD(fullMap->getElementCount());
    tmpMap->visitRw(hashVis);
    LOG_VARD(tmpMap->getElementCount());

    try
    {
      fullMap->append(tmpMap);
    }
    catch (const HootException& e)
    {
      // If there were any element IDs in common between the two input files, we'll get this error.
      // In that case we must fail.
      if (e.getWhat().contains("already contains"))
      {
        throw HootException(
          QString("It is not possible to run a non-streamable map operation ") +
          QString("(OsmMapOperation) on two data sources with overlapping element IDs: ") +
          e.what());
      }
      throw e;
    }
  }
  else
  {
    // Just load the first map, but as unknown2 to end up with a changeset made up of just this
    // input.
    if (ConfigUtils::boundsOptionEnabled())
    {
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
        ConfigOptions().getChangesetReferenceKeepEntireFeaturesCrossingBounds());
      conf().set(
        ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
        ConfigOptions().getChangesetReferenceKeepOnlyFeaturesInsideBounds());
    }
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown2);
  }
  LOG_VARD(fullMap->getElementCount());
  OsmMapWriterFactory::writeDebugMap(fullMap, "after-initial-read-unstreamable-full-map");
  _currentTaskNum++;

  // Apply our convert ops to the entire map. If any of these are map consumers (OsmMapOperation)
  // then they some will exhibit undefined behavior if you try to exec them on the inputs
  // separately.
  LOG_DEBUG("Applying convert ops...");
  NamedOp convertOps(ConfigOptions().getConvertOps());
  convertOps.setProgress(
    Progress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0 / (float)_numTotalTasks));
  convertOps.apply(fullMap);
  // get back into wgs84 in case some op changed the proj
  MapProjector::projectToWgs84(fullMap);
  _currentTaskNum++;

  // We need the two inputs separated for changeset derivation, so split them back out by status.
  LOG_DEBUG("Separating maps by status...");
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Separating out input maps...");
  RemoveUnknown1Visitor remove1Vis;
  RemoveUnknown2Visitor remove2Vis;
  map1.reset(new OsmMap(fullMap));
  if (!_singleInput)
  {
    map1->visitRw(remove2Vis);

    map2.reset(new OsmMap(fullMap));
    map2->visitRw(remove1Vis);
  }
  else
  {
    map1->visitRw(remove1Vis);
  }
  LOG_VARD(map1->getElementCount());
  OsmMapWriterFactory::writeDebugMap(map1, "unstreamable-separated-map-1");
  LOG_VARD(map2->getElementCount());
  OsmMapWriterFactory::writeDebugMap(map2, "unstreamable-separated-map-2");
  _currentTaskNum++;
}

void ChangesetCreator::_handleStreamableConvertOpsInMemory(const QString& input1,
                                                           const QString& input2, OsmMapPtr& map1,
                                                           OsmMapPtr& map2, Progress progress)
{
  LOG_DEBUG("Handling streamable convert ops in memory...");

  // Preserving source IDs is important here.

  // There's no need to check for the crop related config opts here, as we do in
  // _handleUnstreamableConvertOpsInMemory, as a bounded query will always prevent us from
  // streaming.

  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input ...");
  if (!_singleInput)
  {
    // Load each input into a separate map. There's no need to combine them, since we have all
    // streamable convert ops (avoids the extra cost of splitting them back apart).
    IoUtils::loadMap(map1, input1, true, Status::Unknown1);
    IoUtils::loadMap(map2, input2, true, Status::Unknown2);
  }
  else
  {
    // Just load the first map, but as unknown2 to end up with a changeset made up of just this
    // input.
    IoUtils::loadMap(map1, input1, true, Status::Unknown2);
  }
  OsmMapWriterFactory::writeDebugMap(map1, "after-initial-read-streamable-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, "after-initial-read-streamable-map-2");
  _currentTaskNum++;

  // Apply our convert ops to each map separately.
  LOG_DEBUG("Applying convert ops...");
  NamedOp convertOps(ConfigOptions().getConvertOps());
  convertOps.setProgress(
    Progress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0 / (float)_numTotalTasks));
  convertOps.apply(map1);
  MapProjector::projectToWgs84(map1);
  if (!_singleInput)
  {
    convertOps.apply(map2);
    MapProjector::projectToWgs84(map2);
  }
  _currentTaskNum++;
}

void ChangesetCreator::_readInputsFully(const QString& input1, const QString& input2,
                                        OsmMapPtr& map1, OsmMapPtr& map2, Progress progress)
{  
  LOG_VARD(ConfigOptions().getConvertOps().size());
  if (ConfigOptions().getConvertOps().size() > 0)
  {
    if (!ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()))
    {
      /*
       * If any op in the convert ops is a map consumer, then it must go through this logic, which
       * requires combining both map inputs into one. If there are any overlapping element IDs
       * between the two datasets, an error will occur.
       */
      _handleUnstreamableConvertOpsInMemory(input1, input2, map1, map2, progress);
    }
    else
    {
      /*
       * If none of the ops are map consumers, we can avoid having to load both inputs into the same
       * map, which gets around the ID overlap problem.
       */
      _handleStreamableConvertOpsInMemory(input1, input2, map1, map2, progress);
    }
  }
  else
  {
    LOG_DEBUG("Processing inputs without convert ops...");

    // We didn't have any convert ops, so just load everything up.
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input...");
    // Preserving source IDs is important here.
    if (!_singleInput)
    {
      // Load each input into a separate map; see related comments in
      // _handleUnstreamableConvertOpsInMemory
      // TODO: can this separate bounds handling be removed after the addition of
      // ChangesetReplacementCreator?
      if (ConfigUtils::boundsOptionEnabled())
      {
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
          ConfigOptions().getChangesetReferenceKeepEntireFeaturesCrossingBounds());
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
          ConfigOptions().getChangesetReferenceKeepOnlyFeaturesInsideBounds());
      }
      LOG_VARD(ConfigOptions().getConvertBoundingBoxKeepEntireFeaturesCrossingBounds());
      LOG_VARD(ConfigOptions().getConvertBoundingBoxKeepOnlyFeaturesInsideBounds());
      IoUtils::loadMap(map1, input1, true, Status::Unknown1);
      // TODO: hack
      SuperfluousNodeRemover::removeNodes(map1);
      if (ConfigUtils::boundsOptionEnabled())
      {
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
          ConfigOptions().getChangesetSecondaryKeepEntireFeaturesCrossingBounds());
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
          ConfigOptions().getChangesetSecondaryKeepOnlyFeaturesInsideBounds());
      }
      LOG_VARD(ConfigOptions().getConvertBoundingBoxKeepEntireFeaturesCrossingBounds());
      LOG_VARD(ConfigOptions().getConvertBoundingBoxKeepOnlyFeaturesInsideBounds());
      IoUtils::loadMap(map2, input2, true, Status::Unknown2);
      // TODO: hack
      SuperfluousNodeRemover::removeNodes(map2);
    }
    else
    {
      // Just load the first map, but as unknown2 to end up with a changeset made up of just this
      // input.
      if (ConfigUtils::boundsOptionEnabled())
      {
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepEntireFeaturesCrossingBoundsKey(),
          ConfigOptions().getChangesetReferenceKeepEntireFeaturesCrossingBounds());
        conf().set(
          ConfigOptions::getConvertBoundingBoxKeepOnlyFeaturesInsideBoundsKey(),
          ConfigOptions().getChangesetReferenceKeepOnlyFeaturesInsideBounds());
      }
      IoUtils::loadMap(map1, input1, true, Status::Unknown2);
    }
    OsmMapWriterFactory::writeDebugMap(map1, "after-initial-read-no-ops-map-1");
    OsmMapWriterFactory::writeDebugMap(map2, "after-initial-read-no-ops-map-2");
    _currentTaskNum++;
  }

  // We don't want to include review relations.
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Removing review relations...");
  std::shared_ptr<TagKeyCriterion> elementCriterion(
    new TagKeyCriterion(MetadataTags::HootReviewNeeds()));
  RemoveElementsVisitor removeElementsVisitor;
  removeElementsVisitor.setRecursive(false);
  removeElementsVisitor.addCriterion(elementCriterion);
  map1->visitRw(removeElementsVisitor);
  if (!_singleInput)
  {
    map2->visitRw(removeElementsVisitor);
  }
  OsmMapWriterFactory::writeDebugMap(map1, "after-remove-reviews-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, "after-remove-reviews-map-2");
  _currentTaskNum++;

  // Truncate tags over 255 characters to push into OSM API.
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Preparing tags for changeset...");
  ApiTagTruncateVisitor truncateTags;
  map1->visitRw(truncateTags);
  if (!_singleInput)
  {
    map2->visitRw(truncateTags);
  }
  OsmMapWriterFactory::writeDebugMap(map1, "after-truncate-tags-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, "after-truncate-tags-map-2");
  _currentTaskNum++;

  // Node comparisons require hashes be present on the elements.
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Adding element hashes...");
  CalculateHashVisitor2 hashVis;
  map1->visitRw(hashVis);
  if (!_singleInput)
  {
    map2->visitRw(hashVis);
  }
  OsmMapWriterFactory::writeDebugMap(map1, "after-adding-hashes-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, "after-adding-hashes-map-2");
  _currentTaskNum++;

  // TODO: can this be removed after the addition of ChangesetReplacementCreator?
  if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())
  {
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
      "Adding changeset ref delete exclude tags...");

    // The strictness of the bounds check is governed by a config option. Generally, would use
    // not strict for linear features and strict for point or poly features.
    conf().set("in.bounds.criterion.bounds", ConfigOptions().getConvertBoundingBox());
    std::shared_ptr<InBoundsCriterion> boundsCrit(new InBoundsCriterion());
    boundsCrit->setOsmMap(map1.get());
    std::shared_ptr<NotCriterion> notInBoundsCrit(new NotCriterion(boundsCrit));
    std::shared_ptr<ChainCriterion> elementCrit(
      new ChainCriterion(std::shared_ptr<WayCriterion>(new WayCriterion()), notInBoundsCrit));

    RecursiveSetTagValueOp tagSetter(elementCrit, MetadataTags::HootChangeExcludeDelete(), "yes");
    tagSetter.apply(map1);

    OsmMapWriterFactory::writeDebugMap(map1, "after-adding-ref-delete-exclude-tags-map-1");
    _currentTaskNum++;
  }
}

ElementInputStreamPtr ChangesetCreator::_getExternallySortedElements(const QString& input,
                                                                     Progress progress)
{
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
    "Sorting input elements ..." + input.right(25) + "...");

  ElementInputStreamPtr sortedElements;

  //Some in these datasets may have status=3 if you're loading conflated data, so use
  //reader.use.file.status and reader.keep.status.tag if you want to retain that value.

  // Only sort if input isn't already sorted.
  if (!_inputIsSorted(input))
  {
    sortedElements = _sortElementsExternally(input);
  }
  else
  {
    // If it was sorted, just get a stream with the ops we need to be applied inline and don't do
    // any sorting.
    sortedElements = _getFilteredInputStream(input);
  }
  _currentTaskNum++;

  return sortedElements;
}

ElementInputStreamPtr ChangesetCreator::_getEmptyInputStream()
{
  // a no-op here since InMemoryElementSorter taking in an empty map will just return an empty
  // element stream
  return InMemoryElementSorterPtr(new InMemoryElementSorter(OsmMapPtr(new OsmMap())));
}

ElementInputStreamPtr ChangesetCreator::_getFilteredInputStream(const QString& input)
{
  LOG_DEBUG("Retrieving filtered input stream for: " << input.right(25) << "...");

  QList<ElementVisitorPtr> visitors;
  // We don't want to include review relations.
  std::shared_ptr<ElementCriterion> elementCriterion(
    new NotCriterion(
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootReviewNeeds()))));
  // Node comparisons require hashes be present on the elements.
  visitors.append(std::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));
  // Tags need to be truncated if they are over 255 characters.
  visitors.append(std::shared_ptr<ApiTagTruncateVisitor>(new ApiTagTruncateVisitor()));
  // TODO: don't know any way we can support this yet
  //if (!ConfigOptions().getChangesetAllowDeletingReferenceFeaturesOutsideBounds())

  // open a stream to the input data
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  ElementInputStreamPtr inputStream = std::dynamic_pointer_cast<ElementInputStream>(reader);
  ElementInputStreamPtr filteredInputStream(
    new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));

  // Add convert ops supporting streaming into the pipeline, if there are any. TODO: Any
  // OsmMapOperations in the bunch need to operate on the entire map made up of both inputs to
  // work correctly.
  return
    ElementStreamer::getFilteredInputStream(filteredInputStream, ConfigOptions().getConvertOps());
}

ElementInputStreamPtr ChangesetCreator::_sortElementsInMemory(OsmMapPtr map)
{
  return InMemoryElementSorterPtr(new InMemoryElementSorter(map));
}

ElementInputStreamPtr ChangesetCreator::_sortElementsExternally(const QString& input)
{
  std::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());
  sorted->sort(_getFilteredInputStream(input));
  return sorted;
}

void ChangesetCreator::_streamChangesetOutput(ElementInputStreamPtr input1,
                                              ElementInputStreamPtr input2, const QString& output)
{
  LOG_INFO("Streaming changeset output to " << output.right(25) << "...")

  QString stats;
  LOG_VARD(output);

  // Could this eventually be cleaned up to use OsmChangeWriterFactory and the OsmChange interface
  // instead?
  ChangesetDeriverPtr changesetDeriver(new ChangesetDeriver(input1, input2));
  if (output.endsWith(".osc"))
  {
    OsmXmlChangesetFileWriter writer;
    writer.write(output, changesetDeriver);
    stats = writer.getStatsTable();
  }
  else if (output.endsWith(".osc.sql"))
  {
    assert(!_osmApiDbUrl.isEmpty());
    OsmApiDbSqlChangesetFileWriter(QUrl(_osmApiDbUrl)).write(output, changesetDeriver);
  }

  LOG_VARD(changesetDeriver->getNumCreateChanges());
  LOG_VARD(changesetDeriver->getNumModifyChanges());
  LOG_VARD(changesetDeriver->getNumDeleteChanges());
  LOG_VARD(changesetDeriver->getNumFromElementsParsed());
  LOG_VARD(changesetDeriver->getNumToElementsParsed());
  if (changesetDeriver->getNumChanges() == 0)
  {
    LOG_WARN("No changes written to changeset.");
  }

  // close the output stream
  std::shared_ptr<PartialOsmMapReader> partialReader1 =
    std::dynamic_pointer_cast<PartialOsmMapReader>(input1);
  if (partialReader1)
  {
    partialReader1->finalizePartial();
  }
  input1->close();
  std::shared_ptr<PartialOsmMapReader> partialReader2 =
    std::dynamic_pointer_cast<PartialOsmMapReader>(input2);
  if (partialReader2)
  {
    partialReader2->finalizePartial();
  }
  input2->close();

  if (_printStats)
  {
    LOG_INFO("Changeset Stats:\n" << stats);
  }
}

}
