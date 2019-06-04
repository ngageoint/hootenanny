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
#include "ChangesetWriter.h"

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

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>

namespace hoot
{

const QString ChangesetWriter::JOB_SOURCE = "Derive Changeset";

ChangesetWriter::ChangesetWriter(const bool printStats, const QString osmApiDbUrl) :
_osmApiDbUrl(osmApiDbUrl),
_numTotalTasks(0),
_currentTaskNum(0),
_printStats(printStats),
_singleInput(false)
{
}

void ChangesetWriter::write(const QString& output, const QString& input1, const QString& input2)
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
  const bool useStreamingIo =
    _inputIsStreamable(input1) && (_singleInput || _inputIsStreamable(input2));
  LOG_VARD(useStreamingIo);

  // The number of steps here must be updated as you add/remove job steps in the logic.
  _numTotalTasks = 2;
  // for non-streamable convert ops and other inline ops that occur when not streaming
  if (!useStreamingIo)
  {
    _numTotalTasks += 4;
    if (ConfigOptions().getConvertOps().size() > 0)
    {
      _numTotalTasks++;
      if (ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()))
      {
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

  _parseBuffer();

  ElementInputStreamPtr sortedElements1;
  ElementInputStreamPtr sortedElements2;

  if (!useStreamingIo)
  {
    // In the case that not all input formats or convert ops are streamable or the
    // user chose to force in memory streaming by not specifying a sort buffer size, let's
    // use memory bound sorting.

    OsmMapPtr map1(new OsmMap());
    OsmMapPtr map2(new OsmMap());
    _readInputsFully(input1, input2, map1, map2, progress);

    if (!_singleInput)
    {
      // TODO: There need to be checks here to only sort if the input isn't already sorted like
      // there are for the external sorting (e.g. pre-sorted PBF file).
      sortedElements1 = _sortElementsInMemory(map1, progress);
      assert(map2.get());
      sortedElements2 = _sortElementsInMemory(map2, progress);
    }
    else
    {
      sortedElements1 = _getEmptyInputStream();
      sortedElements2 = _sortElementsInMemory(map1, progress);
    }
    _currentTaskNum++;
  }
  else
  {
    // If external sorting is enabled and the input and convert ops are streamable, externally
    // sort the elements to avoid potential memory issues.

    if (!_singleInput)
    {
      //sortedElements1 is the former state of the data
      sortedElements1 = _getExternallySortedElements(input1, progress);
      //sortedElements2 is the newer state of the data
      sortedElements2 = _getExternallySortedElements(input2, progress);
    }
    else
    {
      //Here we're passing all the input data through to the output changeset, so put it in the
      //sortedElements2 newer data and leave the first one empty.
      sortedElements1 = _getEmptyInputStream();
      sortedElements2 = _getExternallySortedElements(input1, progress);
    }
    _currentTaskNum++;
  }

  assert(sortedElements1.get() && sortedElements2.get());
  progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Writing changeset...");
  _streamChangesetOutput(sortedElements1, sortedElements2, output);
  _currentTaskNum++;

  progress.set(
    1.0, Progress::JobState::Successful,
    "Changeset written to: ..." + output.right(maxFilePrintLength));
}

void ChangesetWriter::_parseBuffer()
{
  LOG_DEBUG("Parsing changeset buffer...");

  const double changesetBuffer = ConfigOptions().getChangesetBuffer();
  if (changesetBuffer > 0.0)
  {
    //allow for calculating the changeset with a slightly larger AOI than the default specified
    //bounding box

    QString bboxStr;
    QString convertBoundsParamName;
    //only one of these three should be specified
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
      throw HootException(
        "A changeset buffer was specified but no convert bounding box was specified.");
    }
    geos::geom::Envelope convertBounds = GeometryUtils::envelopeFromConfigString(bboxStr);
    convertBounds.expandBy(changesetBuffer, changesetBuffer);
    conf().set(
      convertBoundsParamName,
      GeometryUtils::envelopeToConfigString(convertBounds));
  }
}

bool ChangesetWriter::_isSupportedOutputFormat(const QString& format) const
{
  return format.endsWith(".osc") || format.endsWith(".osc.sql");
}

bool ChangesetWriter::_inputIsSorted(const QString& input) const
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

bool ChangesetWriter::_inputIsStreamable(const QString& input) const
{
  LOG_VARD(OsmMapReaderFactory::hasElementInputStream(input));
  LOG_VARD(ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()));
  LOG_VARD(ConfigOptions().getElementSorterElementBufferSize());
  return
    OsmMapReaderFactory::hasElementInputStream(input) &&
    // All ops must be streamable, otherwise we'll load both inputs into memory.
    ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()) &&
    // If not sort buffer size is set, we sort in-memory. If we're already loading the data
    // into memory for sorting, might as well force it into memory for the initial read as well.
    ConfigOptions().getElementSorterElementBufferSize() != -1;
}

void ChangesetWriter::_handleUnstreamableConvertOpsInMemory(const QString& input1,
                                                            const QString& input2,
                                                            OsmMapPtr& map1, OsmMapPtr& map2,
                                                            Progress progress)
{
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input ...");
  OsmMapPtr fullMap(new OsmMap());
  if (!_singleInput)
  {
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown1);

    OsmMapPtr tmpMap(new OsmMap());
    IoUtils::loadMap(tmpMap, input2, true, Status::Unknown2);
    try
    {
      fullMap->append(tmpMap);
    }
    catch (const HootException& e)
    {
      if (e.getWhat().contains("already contains"))
      {
        throw HootException(
          QString("It is not possible to run a non-streamable map operation ") +
          QString("OsmMapOperation) on two data sources with overlapping element IDs: ") +
          e.what());
      }
      throw e;
    }
  }
  else
  {
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown2);
  }
  _currentTaskNum++;

  NamedOp convertOps(ConfigOptions().getConvertOps());
  convertOps.setProgress(
    Progress(
      ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0 / (float)_numTotalTasks));
  convertOps.apply(fullMap);
  // get back into wgs84 in case some op changed the proj
  MapProjector::projectToWgs84(fullMap);
  _currentTaskNum++;

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
  _currentTaskNum++;
}

void ChangesetWriter::_handleStreamableConvertOpsInMemory(const QString& input1,
                                                          const QString& input2, OsmMapPtr& map1,
                                                          OsmMapPtr& map2, Progress progress)
{
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input ...");
  if (!_singleInput)
  {
    IoUtils::loadMap(map1, input1, true, Status::Unknown1);
    IoUtils::loadMap(map2, input2, true, Status::Unknown2);
  }
  else
  {
    IoUtils::loadMap(map1, input1, true, Status::Unknown2);
  }
  _currentTaskNum++;

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

void ChangesetWriter::_readInputsFully(const QString& input1, const QString& input2,
                                       OsmMapPtr& map1, OsmMapPtr& map2, Progress progress)
{
  if (ConfigOptions().getConvertOps().size() > 0)
  {
    if (!ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()))
    {
      _handleUnstreamableConvertOpsInMemory(input1, input2, map1, map2, progress);
    }
    else
    {
      _handleStreamableConvertOpsInMemory(input1, input2, map1, map2, progress);
    }
  }
  else
  {
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Reading entire input...");
    if (!_singleInput)
    {
      IoUtils::loadMap(map1, input1, true, Status::Unknown1);
      IoUtils::loadMap(map2, input2, true, Status::Unknown2);
    }
    else
    {
      IoUtils::loadMap(map1, input1, true, Status::Unknown2);;
    }
    _currentTaskNum++;
  }

  //we don't want to include review relations
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
  _currentTaskNum++;

  //  Truncate tags over 255 characters to push into OSM API
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Preparing tags for changeset...");
  ApiTagTruncateVisitor truncateTags;
  map1->visitRw(truncateTags);
  if (!_singleInput)
  {
    map2->visitRw(truncateTags);
  }
  _currentTaskNum++;

  //node comparisons require hashes be present on the elements
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Adding element hashes...");
  CalculateHashVisitor2 hashVis;
  map1->visitRw(hashVis);
  if (!_singleInput)
  {
    map2->visitRw(hashVis);
  }
  _currentTaskNum++;
}

ElementInputStreamPtr ChangesetWriter::_getExternallySortedElements(const QString& input,
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
    sortedElements = _getFilteredInputStream(input);
  }
  _currentTaskNum++;

  return sortedElements;
}

ElementInputStreamPtr ChangesetWriter::_getEmptyInputStream()
{
  //no-op here since InMemoryElementSorter taking in an empty map will just return an empty
  //element stream
  return InMemoryElementSorterPtr(new InMemoryElementSorter(OsmMapPtr(new OsmMap())));
}

ElementInputStreamPtr ChangesetWriter::_getFilteredInputStream(const QString& input)
{
  LOG_DEBUG("Retrieving filtered input stream for: " << input.right(25) << "...");

  QList<ElementVisitorPtr> visitors;
  //we don't want to include review relations
  std::shared_ptr<ElementCriterion> elementCriterion(
    new NotCriterion(
      std::shared_ptr<TagKeyCriterion>(
        new TagKeyCriterion(MetadataTags::HootReviewNeeds()))));
  //node comparisons require hashes be present on the elements
  visitors.append(std::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));
  //  Tags need to be truncated if they are over 255 characters
  visitors.append(std::shared_ptr<ApiTagTruncateVisitor>(new ApiTagTruncateVisitor()));

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

ElementInputStreamPtr ChangesetWriter::_sortElementsInMemory(OsmMapPtr map, Progress progress)
{
  progress.set(
    (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
    // TODO: fix
    "Sorting input elements; task #" + QString::number(_currentTaskNum) + "...");
  return InMemoryElementSorterPtr(new InMemoryElementSorter(map));
}

ElementInputStreamPtr ChangesetWriter::_sortElementsExternally(const QString& input)
{
  std::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());
  sorted->sort(_getFilteredInputStream(input));
  return sorted;
}

void ChangesetWriter::_streamChangesetOutput(ElementInputStreamPtr input1,
                                             ElementInputStreamPtr input2, const QString& output)
{
  LOG_INFO("Streaming changeset output to " << output.right(25) << "...")

  QString stats;
  LOG_VARD(output);

  //Could this eventually this could be cleaned up to use OsmChangeWriterFactory and the
  //OsmChange interface instead?
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
