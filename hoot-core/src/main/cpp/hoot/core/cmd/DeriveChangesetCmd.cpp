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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/elements/InMemoryElementSorter.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/elements/ExternalMergeElementSorter.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/util/Progress.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/visitors/RemoveUnknownVisitor.h>
#include <hoot/core/visitors/FilteredVisitor.h>
#include <hoot/core/visitors/ElementCountVisitor.h>
#include <hoot/core/criterion/StatusCriterion.h>

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>

using namespace std;

namespace hoot
{

const QString JOB_SOURCE = "Derive Changeset";

/**
 * Derives a set of changes given two map inputs
 *
 * Streaming I/O and external element are available to this command.  However, the in-memory input
 * reading/sorting has been left in place to support faster I/O in the situation where large inputs
 * are being dealt with and large amounts of memory are available for reading/sorting or when
 * conversion operations are passed in which require reading an entire map into memory. Access to
 * the the sorter implementation is controlled by the configuration option,
 * element.sorter.element.buffer.size where a size = -1 results in the in-memory implementation
 * being used and a positive size results in the external sorter being used.
 *
 * If convert operations are passed in and any are not streamable, then in memory sorting is forced
 * to occur.
 */
class DeriveChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveChangesetCmd"; }

  DeriveChangesetCmd() :
  _osmApiDbUrl(""),
  _numTotalTasks(0),
  _currentTaskNum(0),
  _printStats(false),
  _useStreamingIo(false),
  _singleInput(false)
  {
  }

  virtual QString getName() const { return "changeset-derive"; }

  virtual QString getDescription() const { return "Creates an OSM changeset"; }

  virtual int runSimple(QStringList args)
  {
    if (args.contains("--stats"))
    {
      _printStats = true;
      args.removeAll("--stats");
    }

    if (args.size() < 3 || args.size() > 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three or four parameters.").arg(getName()));
    }

    const QString input1 = args[0];
    const QString input2 = args[1];
    const QString output = args[2];

    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(output);

    if (!_isSupportedOutputFormat(output))
    {
      throw HootException("Unsupported changeset output format: " + output);
    }
    else if (output.endsWith(".osc.sql"))
    {
      if (args.size() != 4)
      {
        cout << getHelp() << endl << endl;
        throw HootException(
          QString("%1 with SQL output takes four parameters.").arg(getName()));
      }
      _osmApiDbUrl = args[3];
    }
    LOG_VARD(_osmApiDbUrl);

    _singleInput = input2.trimmed().isEmpty();
    LOG_VARD(_singleInput);
    _useStreamingIo = _inputIsStreamable(input1) && (_singleInput || _inputIsStreamable(input2));
    LOG_VARD(_useStreamingIo);

    const QString jobSource = "Derive Changeset";
    // The number of steps here must be updated as you add/remove job steps in the logic.
    _numTotalTasks = 3;
    // for non-streamable convert ops and other inline ops that occur when not streaming
    if (!_useStreamingIo)
    {
      _numTotalTasks += 6;
      if (ConfigOptions().getConvertOps().size() > 0)
      {
        _numTotalTasks++;
      }
    }

    _currentTaskNum = 1;
    Progress progress(ConfigOptions().getJobId(), jobSource, Progress::JobState::Running);
    const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();

    progress.set(
      0.0,
      "Deriving output changeset: ..." + output.right(maxFilePrintLength) + " from inputs: ..." +
      input1.right(maxFilePrintLength) + " and ..." + input2.right(maxFilePrintLength) + "...");

    _parseBuffer();

    //progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Sorting features...");
    ElementInputStreamPtr sortedElements1;
    ElementInputStreamPtr sortedElements2;

    if (!_useStreamingIo)
    {
      // In the case that not all input formats or convert ops are streamable or the
      // user chose to force in memory streaming by not specifying a sort buffer size, let's
      // use memory bound sorting.

      //_getElementsSortedInMemory(input1, input2, sortedElements1, sortedElements2, progress);

      // Only sort if input isn't already sorted.
      bool input1Sorted = _inputIsSorted(input1);
      LOG_VARD(input1Sorted);
      bool input2Sorted = false;
      if (!_singleInput)
      {
        input2Sorted = _inputIsSorted(input2);
        LOG_VARD(input2Sorted);
      }

      OsmMapPtr map1(new OsmMap());
      OsmMapPtr map2(new OsmMap());
      _readInputsFully2(input1, input2, map1, map2, progress);

      LOG_VARD(map1->getElementCount());
      int map1Unknown1Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown1Count);
      int map1Unknown2Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown2Count);

      LOG_VARD(map2->getElementCount());
      int map2Unknown1Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map2);
      LOG_VARD(map2Unknown1Count);
      int map2Unknown2Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map2);
      LOG_VARD(map2Unknown2Count);

      if (!_singleInput)
      {
        // TODO: fix
        //if (!input1Sorted)
        //{
          sortedElements1 = _sortElementsInMemory(map1, progress);
        //}
        _currentTaskNum++;
        //if (!input2Sorted)
        //{
          assert(map2.get());
          sortedElements2 = _sortElementsInMemory(map2, progress);
        //}
        _currentTaskNum++;
      }
      else
      {
        sortedElements1 = _getEmptyInputStream();
        _currentTaskNum++;
        //if (!input1Sorted)
        //{
          sortedElements2 = _sortElementsInMemory(map1, progress);
        //}
        _currentTaskNum++;
      }
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
        _currentTaskNum++;
        sortedElements2 = _getExternallySortedElements(input1, progress);
      }
    }

    LOG_VARD(sortedElements1.get());
    LOG_VARD(sortedElements2.get());
    assert(sortedElements1.get() && sortedElements2.get());
    progress.set((float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Writing changeset...");
    _streamChangesetOutput(sortedElements1, sortedElements2, output);
    _currentTaskNum++;

    progress.set(
      1.0, Progress::JobState::Successful,
      "Changeset written to: ..." + output.right(maxFilePrintLength));

    return 0;
  }

private:

  QString _osmApiDbUrl;

  int _numTotalTasks;
  int _currentTaskNum;

  bool _printStats;

  bool _useStreamingIo;
  bool _singleInput;

  void _parseBuffer()
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

  bool _isSupportedOutputFormat(const QString& format) const
  {
    return format.endsWith(".osc") || format.endsWith(".osc.sql");
  }

  bool _inputIsSorted(const QString& input) const
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

  bool _inputIsStreamable(const QString& input) const
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

  void _readInputsFully2(const QString& input1, const QString& input2, OsmMapPtr& map1,
                         OsmMapPtr& map2, Progress progress)
  {
    if (ConfigOptions().getConvertOps().size() > 0)
    {
      NamedOp convertOps(ConfigOptions().getConvertOps());
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0 / (float)_numTotalTasks));
      if (!ElementStreamer::areValidStreamingOps(ConfigOptions().getConvertOps()))
      {
        OsmMapPtr fullMap(new OsmMap());
        if (!_singleInput)
        {
          IoUtils::loadMap(fullMap, input1, true, Status::Unknown1);
        }
        else
        {
          IoUtils::loadMap(fullMap, input1, true, Status::Unknown2);
        }
        LOG_VARD(fullMap->getElementCount());
        if (!_singleInput)
        {
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
                QString("It is not possible to run a map operation on two data sources with ") +
                QString("overlapping element IDs: ") + e.what());
            }
            throw e;
          }
          LOG_VARD(fullMap->getElementCount());
        }
        convertOps.apply(fullMap);
        // get back into wgs84 in case some op changed the proj
        MapProjector::projectToWgs84(fullMap);
        LOG_VARD(fullMap->getElementCount());

        RemoveUnknown1Visitor remove1Vis;
        RemoveUnknown2Visitor remove2Vis;
        if (!_singleInput)
        {
          progress.set(
            (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
              "Separating out first input map...");
          map1.reset(new OsmMap(fullMap));
          //map1.reset(new OsmMap());
          //map1->append(fullMap);
          LOG_VARD(map1->getElementCount());
          int map1Unknown1Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map1);
          LOG_VARD(map1Unknown1Count);
          int map1Unknown2Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map1);
          LOG_VARD(map1Unknown2Count);
          map1->visitRw(remove2Vis);
          LOG_VARD(map1->getElementCount());
          LOG_DEBUG(remove2Vis.getCompletedStatusMessage());
          _currentTaskNum++;

          progress.set(
            (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
            "Separating out second input map...");
          map2.reset(new OsmMap(fullMap));
          //map2.reset(new OsmMap());
          //map2->append(fullMap);
          LOG_VARD(map2->getElementCount());
          int map2Unknown1Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map2);
          LOG_VARD(map2Unknown1Count);
          int map2Unknown2Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map2);
          LOG_VARD(map2Unknown2Count);
          map2->visitRw(remove1Vis);
          LOG_DEBUG(remove1Vis.getCompletedStatusMessage());
          LOG_VARD(map2->getElementCount());
        }
        else
        {
          progress.set(
            (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
              "Separating out first input map...");
          map1.reset(new OsmMap(fullMap));
          //map1.reset(new OsmMap());
          //map1->append(fullMap);
          LOG_VARD(map1->getElementCount());
          int map1Unknown1Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map1);
          LOG_VARD(map1Unknown1Count);
          int map1Unknown2Count =
            (int)FilteredVisitor::getStat(
              ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
              ConstElementVisitorPtr(new ElementCountVisitor()), map1);
          LOG_VARD(map1Unknown2Count);
          map1->visitRw(remove1Vis);
          LOG_DEBUG(remove1Vis.getCompletedStatusMessage());
          LOG_VARD(map1->getElementCount());
        }
        _currentTaskNum++;
      }
      else
      {
        progress.set(
          (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
          "Reading entire input map 1 ..." + input1.right(25) + "...");
        if (!_singleInput)
        {
          IoUtils::loadMap(map1, input1, true, Status::Unknown1);
        }
        else
        {
          IoUtils::loadMap(map1, input1, true, Status::Unknown2);
        }
        LOG_VARD(map1->getElementCount());
        _currentTaskNum++;

        if (!_singleInput)
        {
          progress.set(
            (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
                "Reading entire input map 2 ..." + input2.right(25) + "...");
          IoUtils::loadMap(map2, input2, true, Status::Unknown2);
          LOG_VARD(map2->getElementCount());
        }
        _currentTaskNum++;

        convertOps.apply(map1);
        MapProjector::projectToWgs84(map1);
        LOG_VARD(map1->getElementCount());
        if (!_singleInput)
        {
          convertOps.apply(map2);
          MapProjector::projectToWgs84(map2);
          LOG_VARD(map2->getElementCount());
        }
      }
      _currentTaskNum++;
    }
    else
    {
      progress.set(
        (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
        "Reading entire input map 1 ..." + input1.right(25) + "...");
      IoUtils::loadMap(map1, input1, true, Status::Unknown1);
      _currentTaskNum++;

      if (!_singleInput)
      {
        progress.set(
          (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
              "Reading entire input map 2 ..." + input2.right(25) + "...");
        IoUtils::loadMap(map2, input2, true, Status::Unknown2);
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
    LOG_VARD(map1->getElementCount());
    if (!_singleInput)
    {
      map2->visitRw(removeElementsVisitor);
      LOG_VARD(map2->getElementCount());
    }
    _currentTaskNum++;

    //node comparisons require hashes be present on the elements
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Adding element hashes...");
    CalculateHashVisitor2 hashVis;
    map1->visitRw(hashVis);
    LOG_VARD(map1->getElementCount());
    if (!_singleInput)
    {
      map2->visitRw(hashVis);
      LOG_VARD(map2->getElementCount());
    }
    // TODO: fix
    _currentTaskNum += 2;
  }

  void _readInputsFully(const QString& input1, const QString& input2, OsmMapPtr& map1,
                        OsmMapPtr& map2, Progress progress)
  {
    //some in these datasets may have status=3 if you're loading conflated data, so use
    //reader.use.file.status and reader.keep.status.tag if you want to retain that value
    OsmMapPtr fullMap(new OsmMap());

    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
      "Reading entire input map 1 ..." + input1.right(25) + "...");
    IoUtils::loadMap(fullMap, input1, true, Status::Unknown1);
    LOG_VARD(fullMap->getElementCount());
    int fullUnknown1Count =
      (int)FilteredVisitor::getStat(
        ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
        ConstElementVisitorPtr(new ElementCountVisitor()), fullMap);
    LOG_VARD(fullUnknown1Count);
    int fullUnknown2Count =
      (int)FilteredVisitor::getStat(
        ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
        ConstElementVisitorPtr(new ElementCountVisitor()), fullMap);
    LOG_VARD(fullUnknown2Count);
    _currentTaskNum++;

    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
      "Reading entire input map 2 ..." + input2.right(25) + "...");
    IoUtils::loadMap(fullMap, input2, true, Status::Unknown2);
    LOG_VARD(fullMap->getElementCount());
    fullUnknown1Count =
      (int)FilteredVisitor::getStat(
        ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
        ConstElementVisitorPtr(new ElementCountVisitor()), fullMap);
    LOG_VARD(fullUnknown1Count);
    fullUnknown2Count =
      (int)FilteredVisitor::getStat(
        ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
        ConstElementVisitorPtr(new ElementCountVisitor()), fullMap);
    LOG_VARD(fullUnknown2Count);
    _currentTaskNum++;

    // Add convert ops into the pipeline, if there are any. TODO: Any OsmMapOperations in the
    // bunch need to operate on the entire map made up of both inputs to work correctly.
    LOG_VARD(ConfigOptions().getConvertOps().size());
    if (ConfigOptions().getConvertOps().size() > 0)
    {
      NamedOp convertOps(ConfigOptions().getConvertOps());
      convertOps.setProgress(
        Progress(
          ConfigOptions().getJobId(), JOB_SOURCE, Progress::JobState::Running,
          (float)(_currentTaskNum - 1) / (float)_numTotalTasks, 1.0 / (float)_numTotalTasks));
      convertOps.apply(fullMap);
      // get back into wgs84 in case some op changed the proj
      MapProjector::projectToWgs84(fullMap);
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
    fullMap->visitRw(removeElementsVisitor);
    LOG_VARD(fullMap->getElementCount());
    _currentTaskNum++;

    //node comparisons require hashes be present on the elements
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks, "Adding element hashes...");
    CalculateHashVisitor2 hashVis;
    fullMap->visitRw(hashVis);
    _currentTaskNum++;

    RemoveUnknown1Visitor remove1Vis;
    RemoveUnknown2Visitor remove2Vis;
    if (!_singleInput)
    {
      progress.set(
        (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
          "Separating out first input map...");
      map1.reset(new OsmMap(fullMap));
      //map1.reset(new OsmMap());
      //map1->append(fullMap);
      LOG_VARD(map1->getElementCount());
      int map1Unknown1Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown1Count);
      int map1Unknown2Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown2Count);
      map1->visitRw(remove2Vis);
      LOG_VARD(map1->getElementCount());
      LOG_DEBUG(remove2Vis.getCompletedStatusMessage());
      _currentTaskNum++;

      progress.set(
        (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
        "Separating out second input map...");
      map2.reset(new OsmMap(fullMap));
      //map2.reset(new OsmMap());
      //map2->append(fullMap);
      LOG_VARD(map2->getElementCount());
      int map2Unknown1Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map2);
      LOG_VARD(map2Unknown1Count);
      int map2Unknown2Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map2);
      LOG_VARD(map2Unknown2Count);
      map2->visitRw(remove1Vis);
      LOG_DEBUG(remove1Vis.getCompletedStatusMessage());
      LOG_VARD(map2->getElementCount());
    }
    else
    {
      progress.set(
        (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
          "Separating out first input map...");
      map1.reset(new OsmMap(fullMap));
      //map1.reset(new OsmMap());
      //map1->append(fullMap);
      LOG_VARD(map1->getElementCount());
      int map1Unknown1Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown1)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown1Count);
      int map1Unknown2Count =
        (int)FilteredVisitor::getStat(
          ElementCriterionPtr(new StatusCriterion(Status::Unknown2)),
          ConstElementVisitorPtr(new ElementCountVisitor()), map1);
      LOG_VARD(map1Unknown2Count);
      map1->visitRw(remove1Vis);
      LOG_DEBUG(remove1Vis.getCompletedStatusMessage());
      LOG_VARD(map1->getElementCount());
    }
    _currentTaskNum++;
  }

  ElementInputStreamPtr _getExternallySortedElements(const QString& input, Progress progress)
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

  ElementInputStreamPtr _getEmptyInputStream()
  {
    //no-op here since InMemoryElementSorter taking in an empty map will just return an empty
    //element stream
    return InMemoryElementSorterPtr(new InMemoryElementSorter(OsmMapPtr(new OsmMap())));
  }

  ElementInputStreamPtr _getFilteredInputStream(const QString& input)
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

  ElementInputStreamPtr _sortElementsInMemory(OsmMapPtr map, Progress progress)
  {
    progress.set(
      (float)(_currentTaskNum - 1) / (float)_numTotalTasks,
      "Sorting input elements; task #" + QString::number(_currentTaskNum) + "...");
    return InMemoryElementSorterPtr(new InMemoryElementSorter(map));
  }

  ElementInputStreamPtr _sortElementsExternally(const QString& input)
  {
    std::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());
    sorted->sort(_getFilteredInputStream(input));
    return sorted;
  }

  void _streamChangesetOutput(ElementInputStreamPtr input1, ElementInputStreamPtr input2,
                              const QString& output)
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
};

HOOT_FACTORY_REGISTER(Command, DeriveChangesetCmd)

}


