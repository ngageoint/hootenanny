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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "ChangesetCreator.h"

// Hoot
#include <hoot/core/algorithms/changeset/ChangesetCleaner.h>
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/elements/ExternalMergeElementSorter.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>
#include <hoot/core/io/OsmChangesetFileWriterFactory.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/ops/OpExecutor.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>

namespace hoot
{

const QString ChangesetCreator::JOB_SOURCE = "Derive Changeset";

ChangesetCreator::ChangesetCreator(const bool printDetailedStats, const QString& statsOutputFile,
                                   const QString osmApiDbUrl)
  : _osmApiDbUrl(osmApiDbUrl),
    _numTotalTasks(0),
    _currentTaskNum(0),
    _includeReviews(false),
    _printDetailedStats(printDetailedStats),
    _statsOutputFile(statsOutputFile),
    _singleInput(false),
    _clean(false),
    _numCreateChanges(0),
    _numModifyChanges(0),
    _numDeleteChanges(0)
{
  if (printDetailedStats)
  {
    QFile statsFile(statsOutputFile);
    if (statsFile.exists() && !statsFile.remove())
    {
      LOG_ERROR("Unable to remove changeset statistics file: " << statsOutputFile);
    }
  }
}

void ChangesetCreator::create(const QString& output, const QString& input1, const QString& input2)
{
  if (!_isSupportedOutputFormat(output))
    throw HootException("Unsupported changeset output format: " + output);
  else if (output.endsWith(".osc.sql") && _osmApiDbUrl.isEmpty())
    throw IllegalArgumentException("Output to SQL changeset requires an OSM API database URL be specified.");
  else if (!output.endsWith(".osc.sql") && !_osmApiDbUrl.isEmpty())
  {
    LOG_WARN(
      "Ignoring OSM API database URL: " << _osmApiDbUrl << " for non-SQL changeset output...");
  }

  // write the output dir now so we don't get a nasty surprise at the end of a long job that it
  // can't be written
  IoUtils::writeOutputDir(output);

  _singleInput = input2.trimmed().isEmpty();
  LOG_VARD(_singleInput);
  // both inputs must support streaming to use streaming I/O
  const bool useStreamingIo =
    // TODO: may be able to move this check to IoUtils::areValidStreamingOps
    !ConfigUtils::boundsOptionEnabled() &&
    _inputIsStreamable(input1) &&
    (_singleInput || _inputIsStreamable(input2));
  LOG_VARD(useStreamingIo);

  // The number of steps here must be updated as you add/remove job steps in the logic.
  _numTotalTasks = 2;
  // for non-streamable convert ops and other inline ops that occur when not streaming
  if (!useStreamingIo)
  {
    // For non-streaming I/O we can divide each data conversion task into a separate step, hence
    // the larger number of steps. With streaming I/O that isn't possible since all the data
    // conversion operations are executed inline at the same time the data is read in.
    _numTotalTasks += 3;
    if (!ConfigOptions().getConvertOps().empty())
    {
      // Convert ops get a single task, which OpExecutor will break down into sub-tasks during
      // progress reporting.
      _numTotalTasks++;
      // Have the extra work of combining and separating data inputs when any of the convert
      // ops aren't streamable.
      if (!IoUtils::areValidStreamingOps(ConfigOptions().getConvertOps()))
        _numTotalTasks++;
    }
    if (!_includeReviews)
      _numTotalTasks--;
  }

  _currentTaskNum = 1;
  Progress progress(ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running);
  const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
  QString msg = "Deriving output changeset: ..." + FileUtils::toLogFormat(output, maxFilePrintLength) +
                " from inputs: ..." + FileUtils::toLogFormat(input1, maxFilePrintLength) + " and ..." +
                FileUtils::toLogFormat(input2, maxFilePrintLength);
  if (ConfigUtils::boundsOptionEnabled())
    msg += " over bounds: ..." + ConfigUtils::getBoundsString().right(maxFilePrintLength);

  msg += "...";
  progress.set(0.0, msg);

  //sortedElements1 is the former state of the data
  ElementInputStreamPtr sortedElements1;
  //sortedElements2 is the newer state of the data
  ElementInputStreamPtr sortedElements2;

  // TODO: We could use VersionUtils::checkVersionLessThanOneCountAndLogWarning() somewhere in here
  // like we do with ChangesetDeriveReplacementCommand and ConflateCmd.

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
    OsmMapPtr map1 = std::make_shared<OsmMap>();
    OsmMapPtr map2 = std::make_shared<OsmMap>();
    _readInputsFully(input1, input2, map1, map2, progress);
    _map1List.append(map1);
    _map2List.append(map2);

    // TODO: There need to be checks here to only sort if the input isn't already sorted like
    // there are for the external sorting (e.g. pre-sorted PBF file).

    progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Sorting input elements...");
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

  progress.set(1.0, Progress::JobState::Successful,
               "Changeset written to: ..." + FileUtils::toLogFormat(output, maxFilePrintLength));
}

void ChangesetCreator::create(const OsmMapPtr& map1, const OsmMapPtr& map2, const QString& output)
{
  QList<OsmMapPtr> map1Inputs;
  map1Inputs.append(map1);
  QList<OsmMapPtr> map2Inputs;
  map2Inputs.append(map2);
  create(map1Inputs, map2Inputs, output);
}

void ChangesetCreator::create(const QList<OsmMapPtr>& map1Inputs, const QList<OsmMapPtr>& map2Inputs,
                              const QString& output)
{
  if (map1Inputs.size() != map2Inputs.size())
    throw IllegalArgumentException("Changeset input data inputs are not the same size.");

  QList<ElementInputStreamPtr> sortedInputs1;
  QList<ElementInputStreamPtr> sortedInputs2;
  for (int i = 0; i < map1Inputs.size(); i++)
  {
    OsmMapPtr map1 = map1Inputs.at(i);
    // If map2 is empty, we'll end up deleting features from map1 in the resultant changeset.
    OsmMapPtr map2 = map2Inputs.at(i);
    LOG_DEBUG("Creating changeset from inputs: " << FileUtils::toLogFormat(map1->getName()) << " of size: " <<
              map1->size() << " and " << FileUtils::toLogFormat(map2->getName()) << " of size: " <<
              map2->size() << " to output: " << FileUtils::toLogFormat(output, 25) << "...");
    OsmMapWriterFactory::writeDebugMap(map1, className(), "map1-before-changeset-derivation-" + map1->getName());
    OsmMapWriterFactory::writeDebugMap(map2, className(), "map2-before-changeset-derivation-" + map2->getName());

    if (!_includeReviews)
    {
      std::shared_ptr<TagKeyCriterion> elementCriterion =
        std::make_shared<TagKeyCriterion>(MetadataTags::HootReviewNeeds());
      RemoveElementsVisitor removeElementsVisitor;
      removeElementsVisitor.setRecursive(false);
      removeElementsVisitor.addCriterion(elementCriterion);
      map1->visitRw(removeElementsVisitor);
      map2->visitRw(removeElementsVisitor);
    }

    // Truncate tags over max tag length characters to push into OSM API.
    ApiTagTruncateVisitor truncateTags;
    map1->visitRw(truncateTags);
    map2->visitRw(truncateTags);

    LOG_VART(MapProjector::toWkt(map1->getProjection()));
    LOG_VART(MapProjector::toWkt(map2->getProjection()));

    if (ConfigUtils::boundsOptionEnabled())
    {
      _map1List.append(map1);
      _map2List.append(map2);
    }

    // no need to implement application of ops for this logic path

    // sortedInputs1 is the former state of the data
    // An empty former map, means the changeset will be made up of entirely statements based on
    // map2.
    if (map1->isEmpty())
      sortedInputs1.append(_getEmptyInputStream());
    else
      sortedInputs1.append(_sortElementsInMemory(map1));
    // sortedInputs2 is the newer state of the data
    sortedInputs2.append(_sortElementsInMemory(map2));
  }

  // write out the changeset file
  _streamChangesetOutput(sortedInputs1, sortedInputs2, output);
}

bool ChangesetCreator::_isSupportedOutputFormat(const QString& format) const
{
  return format.endsWith(".osc") || format.endsWith(".osc.sql");
}

bool ChangesetCreator::_inputIsSorted(const QString& input) const
{
  // ops could change the ordering
  if (!ConfigOptions().getConvertOps().empty())
    return false;

  // Streaming db inputs actually do not come back sorted, despite the order by id clause
  // in the query (see ApiDb::selectElements). Otherwise, we'd skip sorting them too.

  // pbf sets a sort flag
  if (OsmPbfReader().isSupported(input) && OsmPbfReader().isSorted(input))
    return true;

  return false;
}

bool ChangesetCreator::_inputIsStreamable(const QString& input) const
{
  LOG_VARD(IoUtils::isStreamableInput(input));
  LOG_VARD(IoUtils::areValidStreamingOps(ConfigOptions().getConvertOps()));
  LOG_VARD(ConfigOptions().getElementSorterElementBufferSize());
  return
    // The input format itself must be streamable (partially read).
    IoUtils::isStreamableInput(input) &&
    // All ops must be streamable, otherwise we'll load both inputs into memory.
    IoUtils::areValidStreamingOps(ConfigOptions().getConvertOps()) &&
    // If no sort buffer size is set, we sort in-memory. If we're already loading the data
    // into memory for sorting, might as well force it into memory for the initial read as well.
    ConfigOptions().getElementSorterElementBufferSize() != -1;
}

void ChangesetCreator::_handleUnstreamableConvertOpsInMemory(const QString& input1,
                                                             const QString& input2,
                                                             OsmMapPtr& map1,
                                                             OsmMapPtr& map2,
                                                             Progress progress)
{
  LOG_DEBUG("Handling unstreamable convert ops in memory...");

  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input ...");
  OsmMapPtr fullMap = std::make_shared<OsmMap>();
  if (!_singleInput)
  {
    // We must preserve the original element IDs while loading in order for changeset derivation
    // to work.

    // Load the first map. If we have a bounded query, let's check for the crop related option
    // overrides.
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown1);
    OsmMapWriterFactory::writeDebugMap(fullMap, className(), "after-initial-read-unstreamable-ref-map");

    // append the second map onto the first one
    OsmMapPtr tmpMap = std::make_shared<OsmMap>();
    IoUtils::loadMap(tmpMap, input2, true, Status::Unknown2);
    OsmMapWriterFactory::writeDebugMap(tmpMap, className(), "after-initial-read-unstreamable-sec-map");

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
      //  rethrow the original exception
      throw;
    }
  }
  else
  {
    // Just load the first map, but as unknown2 to end up with a changeset made up of just this
    // input.
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown2);
  }

  LOG_VARD(fullMap->getElementCount());
  OsmMapWriterFactory::writeDebugMap(fullMap, className(), "after-initial-read-unstreamable-full-map");
  _currentTaskNum++;

  // Apply our convert ops to the entire map. If any of these are map consumers (OsmMapOperation)
  // then they some will exhibit undefined behavior if you try to exec them on the inputs
  // separately.
  LOG_DEBUG("Applying convert ops...");
  OpExecutor convertOps(ConfigOptions().getConvertOps());
  convertOps.setProgress(
    Progress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0f / (float)_numTotalTasks));
  convertOps.apply(fullMap);
  // get back into wgs84 in case some op changed the proj
  MapProjector::projectToWgs84(fullMap);
  _currentTaskNum++;

  // We need the two inputs separated for changeset derivation, so split them back out by status.
  LOG_DEBUG("Separating maps by status...");
  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Separating out input maps...");
  RemoveUnknown1Visitor remove1Vis;
  RemoveUnknown2Visitor remove2Vis;
  map1 = std::make_shared<OsmMap>(fullMap);
  if (!_singleInput)
  {
    map1->visitRw(remove2Vis);

    map2 = std::make_shared<OsmMap>(fullMap);
    map2->visitRw(remove1Vis);
  }
  else
    map1->visitRw(remove1Vis);

  LOG_VARD(map1->getElementCount());
  OsmMapWriterFactory::writeDebugMap(map1, className(), "unstreamable-separated-map-1");
  LOG_VARD(map2->getElementCount());
  OsmMapWriterFactory::writeDebugMap(map2, className(), "unstreamable-separated-map-2");
  _currentTaskNum++;
}

void ChangesetCreator::_handleStreamableConvertOpsInMemory(const QString& input1,
                                                           const QString& input2,
                                                           OsmMapPtr& map1,
                                                           OsmMapPtr& map2,
                                                           Progress progress)
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
  OsmMapWriterFactory::writeDebugMap(map1, className(), "after-initial-read-streamable-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, className(), "after-initial-read-streamable-map-2");
  _currentTaskNum++;

  // Apply our convert ops to each map separately.
  LOG_DEBUG("Applying convert ops...");
  OpExecutor convertOps(ConfigOptions().getConvertOps());
  convertOps.setProgress(
    Progress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0f / (float)_numTotalTasks));
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
  if (!ConfigOptions().getConvertOps().empty())
  {
    if (!IoUtils::areValidStreamingOps(ConfigOptions().getConvertOps()))
    {
      /*
       * If any op in the convert ops is a map consumer, then it must go through this logic, which
       * requires combining both map inputs into one. If there are any overlapping element IDs
       * between the two datasets, an error will occur.
       *
       * It is possible there could be situations where we may be ok with applying a non-streamable
       * op separately to each map, in which case we overlapping element IDs wouldn't be an issue.
       * If that situation arises, we will need to refactor here.
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
      IoUtils::loadMap(map1, input1, true, Status::Unknown1);
      IoUtils::loadMap(map2, input2, true, Status::Unknown2);
    }
    else
    {
      // Just load the first map, but as unknown2 to end up with a changeset made up of just this
      // input.
      IoUtils::loadMap(map1, input1, true, Status::Unknown2);
    }
    OsmMapWriterFactory::writeDebugMap(map1, className(), "after-initial-read-no-ops-map-1");
    OsmMapWriterFactory::writeDebugMap(map2, className(), "after-initial-read-no-ops-map-2");
    _currentTaskNum++;
  }

  if (!_includeReviews)
  {
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Removing review relations...");
    std::shared_ptr<TagKeyCriterion> elementCriterion =
      std::make_shared<TagKeyCriterion>(MetadataTags::HootReviewNeeds());
    RemoveElementsVisitor removeElementsVisitor;
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(elementCriterion);
    map1->visitRw(removeElementsVisitor);
    if (!_singleInput)
      map2->visitRw(removeElementsVisitor);

    OsmMapWriterFactory::writeDebugMap(map1, className(), "after-remove-reviews-map-1");
    OsmMapWriterFactory::writeDebugMap(map2, className(), "after-remove-reviews-map-2");
    _currentTaskNum++;
  }

  // Truncate tags over max tag length characters to push into OSM API.
  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Preparing tags for changeset...");
  ApiTagTruncateVisitor truncateTags;
  map1->visitRw(truncateTags);
  if (!_singleInput)
    map2->visitRw(truncateTags);

  OsmMapWriterFactory::writeDebugMap(map1, className(), "after-truncate-tags-map-1");
  OsmMapWriterFactory::writeDebugMap(map2, className(), "after-truncate-tags-map-2");
  _currentTaskNum++;
}

ElementInputStreamPtr ChangesetCreator::_getExternallySortedElements(const QString& input,
                                                                     Progress progress)
{
  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks,
               "Sorting input elements ..." + FileUtils::toLogFormat(input, 25) + "...");

  ElementInputStreamPtr sortedElements;

  // Some in these datasets may have status=3 if you're loading conflated data, so use
  // reader.use.file.status and reader.keep.status.tag if you want to retain that value.

  // Only sort if input isn't already sorted.
  if (!_inputIsSorted(input))
    sortedElements = _sortElementsExternally(input);
  else
    sortedElements = _getFilteredInputStream(input);

  _currentTaskNum++;

  return sortedElements;
}

ElementInputStreamPtr ChangesetCreator::_getEmptyInputStream() const
{
  // a no-op here since InMemoryElementSorter taking in an empty map will just return an empty
  // element stream
  return std::make_shared<InMemoryElementSorter>(std::make_shared<OsmMap>());
}

ElementInputStreamPtr ChangesetCreator::_getFilteredInputStream(const QString& input) const
{
  LOG_DEBUG("Retrieving filtered input stream for: " << FileUtils::toLogFormat(input, 25) << "...");

  QList<ElementVisitorPtr> visitors;
  std::shared_ptr<ElementCriterion> elementCriterion;
  if (!_includeReviews)
  {
    elementCriterion =
      std::make_shared<NotCriterion>(
        std::make_shared<TagKeyCriterion>(MetadataTags::HootReviewNeeds()));
  }
  // Tags need to be truncated if they are over max tag length characters.
  visitors.append(std::make_shared<ApiTagTruncateVisitor>());

  // open a stream to the input data
  std::shared_ptr<PartialOsmMapReader> reader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(input));
  reader->setUseDataSourceIds(true);
  reader->open(input);
  ElementInputStreamPtr inputStream = std::dynamic_pointer_cast<ElementInputStream>(reader);
  ElementInputStreamPtr filteredInputStream;
  if (elementCriterion)
    filteredInputStream = std::make_shared<ElementCriterionVisitorInputStream>(inputStream, elementCriterion, visitors);
  else
    filteredInputStream = std::make_shared<ElementVisitorInputStream>(inputStream, visitors.at(0));

  // Add convert ops supporting streaming into the pipeline, if there are any.
  // TODO: Any OsmMapOperations in the bunch need to operate on the entire map made up of both
  // inputs to work correctly.
  return IoUtils::getFilteredInputStream(filteredInputStream, ConfigOptions().getConvertOps());
}

ElementInputStreamPtr ChangesetCreator::_sortElementsInMemory(OsmMapPtr map) const
{
  return std::make_shared<InMemoryElementSorter>(map);
}

ElementInputStreamPtr ChangesetCreator::_sortElementsExternally(const QString& input) const
{
  std::shared_ptr<ExternalMergeElementSorter> sorted = std::make_shared<ExternalMergeElementSorter>();
  sorted->sort(_getFilteredInputStream(input));
  return sorted;
}

void ChangesetCreator::_streamChangesetOutput(const QList<ElementInputStreamPtr>& inputs1,
                                              const QList<ElementInputStreamPtr>& inputs2,
                                              const QString& output)
{
  LOG_VARD(inputs1.size());
  LOG_VARD(inputs2.size());
  if (inputs1.size() != inputs2.size())
  {
    throw IllegalArgumentException(
      "Changeset input data inputs are not the same size for streaming to output.");
  }

  LOG_INFO("Streaming changeset output to ..." << FileUtils::toLogFormat(output, 25) << "...");

  QString detailedStats;
  _numCreateChanges = 0;
  _numModifyChanges = 0;
  _numDeleteChanges = 0;

  QList<ChangesetProviderPtr> changesetProviders;
  for (int i = 0; i < inputs1.size(); i++)
    changesetProviders.append(std::make_shared<ChangesetDeriver>(inputs1.at(i), inputs2.at(i)));

  LOG_VARD(changesetProviders.size());
  assert(inputs1.size() == changesetProviders.size());

  if (_clean)
  {
    // This was added primarily for use by ChangesetReplacementCreator implementations. Eventually,
    // it may be able to be removed if its changeset writing bugs can be fixed.
    std::shared_ptr<ChangesetCleaner> cleaner = std::make_shared<ChangesetCleaner>(changesetProviders);
    changesetProviders.clear();
    changesetProviders.append(cleaner);
  }
  LOG_VARD(changesetProviders.size());

  std::shared_ptr<OsmChangesetFileWriter> writer =
    OsmChangesetFileWriterFactory::getInstance().createWriter(output, _osmApiDbUrl);
  // Changeset writing honors the bounds config opt, if specified. To do bounds checking a map is
  // needed, so pass in maps to the changeset writer.
  if (ConfigUtils::boundsOptionEnabled())
  {
    LOG_VARD(_map1List.size());
    LOG_VARD(_map2List.size());
    writer->setMap1List(_map1List);
    writer->setMap2List(_map2List);
  }
  writer->write(output, changesetProviders);
  if (_printDetailedStats)
  {
    if (output.endsWith(".osc")) // detailed stats currently only implemented for xml output
    {
      // Get the stats output format from the file extension, or if no extension is there assume a
      // text table output to the display.
      ChangesetStatsFormat statsFormat;
      if (!_statsOutputFile.isEmpty())
      {
        QFileInfo statsFileInfo(_statsOutputFile);
        statsFormat.setFormat(ChangesetStatsFormat::fromString(statsFileInfo.completeSuffix()));
      }
      else
        statsFormat.setFormat(ChangesetStatsFormat::TextFormat);

      detailedStats = writer->getStatsTable(statsFormat);
    }
    else
    {
      LOG_INFO(
        "Skipping changeset statistics generation, as not implemented for output: " << output);
    }
  }

  LOG_DEBUG("Collecting changeset stats...");
  for (int i = 0; i < changesetProviders.size(); i++)
  {
    ChangesetProviderPtr changesetProvider = changesetProviders.at(i);
    LOG_DEBUG("Derived changeset: " << i + 1 << " of " << changesetProviders.size() << ": ");

    _numCreateChanges += changesetProvider->getNumCreateChanges();
    _numModifyChanges += changesetProvider->getNumModifyChanges();
    _numDeleteChanges += changesetProvider->getNumDeleteChanges();

    LOG_VART(changesetProvider->getNumFromElementsParsed());
    LOG_VART(changesetProvider->getNumToElementsParsed());
    if (changesetProvider->getNumChanges() == 0)
    {
      LOG_TRACE("No changes written to changeset.");
    }
    else
    {
      LOG_VART(changesetProvider->getNumCreateChanges());
      LOG_VART(changesetProvider->getNumModifyChanges());
      LOG_VART(changesetProvider->getNumDeleteChanges());
    }
  }

  LOG_DEBUG("Closing changeset streams...");
  for (int i = 0; i < inputs1.size(); i++)
  {
    // close the output streams
    ElementInputStreamPtr input1 = inputs1.at(i);
    std::shared_ptr<PartialOsmMapReader> partialReader1 =
      std::dynamic_pointer_cast<PartialOsmMapReader>(input1);
    if (partialReader1)
      partialReader1->finalizePartial();
    input1->close();
    ElementInputStreamPtr input2 = inputs2.at(i);
    std::shared_ptr<PartialOsmMapReader> partialReader2 =
      std::dynamic_pointer_cast<PartialOsmMapReader>(input2);
    if (partialReader2)
      partialReader2->finalizePartial();
    input2->close();
  }

  LOG_VART(_printDetailedStats);
  if (_printDetailedStats && !detailedStats.isEmpty())
  {
    if (_statsOutputFile.isEmpty())
    {
      LOG_STATUS("Changeset Stats:\n" << detailedStats);
    }
    else
    {
      LOG_DEBUG("Writing changeset stats to: ..." << FileUtils::toLogFormat(_statsOutputFile, 25) << "...");
      FileUtils::writeFully(_statsOutputFile, detailedStats);
    }
  }
  else
  {
    LOG_DEBUG("Total create changes: " << StringUtils::formatLargeNumber(_numCreateChanges));
    LOG_DEBUG("Total modify changes: " << StringUtils::formatLargeNumber(_numModifyChanges));
    LOG_DEBUG("Total delete changes: " << StringUtils::formatLargeNumber(_numDeleteChanges));
  }
}

void ChangesetCreator::_streamChangesetOutput(
  ElementInputStreamPtr input1, ElementInputStreamPtr input2, const QString& output)
{
  QList<ElementInputStreamPtr> input1List;
  input1List.append(input1);
  QList<ElementInputStreamPtr> input2List;
  input2List.append(input2);
  _streamChangesetOutput(input1List, input2List, output);
}

}
