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

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>

using namespace std;

namespace hoot
{

/**
 * Derives a set of changes given two map inputs
 *
 * Streaming I/O and external element are available to this command.  However, the in memory input
 * reading/sorting has been left in place to support faster I/O in the situation where large inputs
 * are being dealt with and large amounts of memory are available for reading/sorting.  Access to
 * the in memory implementation is controlled by the configuration option,
 * element.sorter.element.buffer.size (size = -1 results in the in memory implementation being
 * used).
 */
class DeriveChangesetCmd : public BaseCommand
{
public:

  static string className() { return "hoot::DeriveChangesetCmd"; }

  DeriveChangesetCmd() :
  _osmApiDbUrl("")
  {
  }

  virtual QString getName() const { return "changeset-derive"; }

  virtual QString getDescription() const { return "Creates an OSM changeset"; }

  bool _printStats = false;

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

    const int maxFilePrintLength = ConfigOptions().getProgressVarPrintLengthMax();
    LOG_STATUS(
      "Deriving output changeset: " << output.right(maxFilePrintLength) << " from inputs: " <<
      input1.right(maxFilePrintLength) << " and " << input2.right(maxFilePrintLength) << "...");

    const QString jobSource = "Derive Changeset";
    // The number of steps here must be updated as you add/remove job steps (don't count
    // tasks where you pass in the progress).
    const int numTotalTasks = 2;
    int currentTaskNum = 1;
    Progress progress(jobSource);

    _parseBuffer();

    const bool singleInput = input2.trimmed().isEmpty();

    progress.set(
      (float)(currentTaskNum - 1) / (float)numTotalTasks, "Running", false, "Sorting features...");
    ElementInputStreamPtr sortedElements1;
    ElementInputStreamPtr sortedElements2;
    if (!singleInput)
    {
      //sortedElements1 is the former state of the data
      sortedElements1 = _getSortedElements(input1, Status::Unknown1);
      //sortedElements2 is the newer state of the data
      sortedElements2 = _getSortedElements(input2, Status::Unknown2);
    }
    else
    {
      //Here we're passing all the input data through to the output changeset, so put it in the
      //sortedElements2 newer data and leave the first one empty.
      sortedElements1 = _getEmptyInputStream();
      sortedElements2 = _getSortedElements(input1, Status::Unknown2);
    }
    currentTaskNum++;

    // We could make this progress reporting more granular, but for in-memory changesets only.
    progress.set(
      (float)(currentTaskNum - 1) / (float)numTotalTasks, "Running", false, "Writing changeset...");
    _streamChangesetOutput(sortedElements1, sortedElements2, output);
    currentTaskNum++;

    progress.set(
      1.0, "Successful", false, "Changeset written to: " + output.right(maxFilePrintLength));

    return 0;
  }

private:

  QString _osmApiDbUrl;

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

  bool _isSupportedOutputFormat(const QString format) const
  {
    return format.endsWith(".osc") || format.endsWith(".osc.sql");
  }

  bool _inputIsSorted(const QString input) const
  {
    //Streaming db inputs actually do not come back sorted, despite the order by id clause
    //in the query (see ApiDb::selectElements).  Otherwise, we'd skip sorting them too.

    //pbf sets a sort flag
    if (OsmPbfReader().isSupported(input) && OsmPbfReader().isSorted(input))
    {
      return true;
    }
    return false;
  }

  /*
   * Reads entire input into memory
   */
  OsmMapPtr _readInputFully(const QString input, const Status& elementStatus)
  {
    LOG_INFO("Reading entire input into memory for " << input.right(25) << "...");

    //some in these datasets may have status=3 if you're loading conflated data, so use
    //reader.use.file.status and reader.keep.status.tag if you want to retain that value
    OsmMapPtr map(new OsmMap());
    IoUtils::loadMap(map, input, true, elementStatus);

    //we don't want to include review relations
    boost::shared_ptr<TagKeyCriterion> elementCriterion(
      new TagKeyCriterion(MetadataTags::HootReviewNeeds()));
    RemoveElementsVisitor removeElementsVisitor;
    removeElementsVisitor.setRecursive(false);
    removeElementsVisitor.addCriterion(elementCriterion);
    map->visitRw(removeElementsVisitor);

    //node comparisons require hashes be present on the elements
    CalculateHashVisitor2 hashVis;
    map->visitRw(hashVis);

    return map;
  }

  ElementInputStreamPtr _getSortedElements(const QString input, const Status& status)
  {
    ElementInputStreamPtr sortedElements;

    //Some in these datasets may have status=3 if you're loading conflated data, so use
    //reader.use.file.status and reader.keep.status.tag if you want to retain that value.

    const bool inputSorted = _inputIsSorted(input);
    LOG_VARD(inputSorted);
    //Only sort if input isn't already sorted.
    if (!inputSorted)
    {
      //If external sorting is enabled and both inputs are streamable, externally sort the elements
      //to avoid potential memory issues.
      LOG_VARD(ConfigOptions().getElementSorterElementBufferSize());
      LOG_VARD(OsmMapReaderFactory::hasElementInputStream(input));
      if (OsmMapReaderFactory::hasElementInputStream(input) &&
          ConfigOptions().getElementSorterElementBufferSize() != -1)
      {
        sortedElements = _sortElementsExternally(input);
      }
      else
      {
        //Otherwise, since currently not all input formats are supported as streamable, switch over
        //to memory bound sorting.
        sortedElements = _sortElementsInMemory(_readInputFully(input, status));
      }
    }
    else
    {
      sortedElements = _getFilteredInputStream(input);
    }
    return sortedElements;
  }

  ElementInputStreamPtr _getEmptyInputStream()
  {
    //no-op here since InMemoryElementSorter taking in an empty map will just return an empty
    //element stream
    return InMemoryElementSorterPtr(new InMemoryElementSorter(OsmMapPtr(new OsmMap())));
  }

  ElementInputStreamPtr _getFilteredInputStream(const QString input)
  {
    LOG_DEBUG("Retrieving filtered input stream for: " << input.right(25) << "...");

    QList<ElementVisitorPtr> visitors;
    //we don't want to include review relations
    boost::shared_ptr<ElementCriterion> elementCriterion(
      new NotCriterion(
        boost::shared_ptr<TagKeyCriterion>(
          new TagKeyCriterion(MetadataTags::HootReviewNeeds()))));
    //node comparisons require hashes be present on the elements
    visitors.append(boost::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));

    boost::shared_ptr<PartialOsmMapReader> reader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    ElementInputStreamPtr inputStream = boost::dynamic_pointer_cast<ElementInputStream>(reader);
    ElementInputStreamPtr filteredInputStream(
      new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));

    return filteredInputStream;
  }

  ElementInputStreamPtr _sortElementsInMemory(OsmMapPtr map)
  {
    return InMemoryElementSorterPtr(new InMemoryElementSorter(map));
  }

  ElementInputStreamPtr _sortElementsExternally(const QString input)
  {
    boost::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());
    sorted->sort(_getFilteredInputStream(input));
    return sorted;
  }

  void _streamChangesetOutput(ElementInputStreamPtr input1, ElementInputStreamPtr input2,
                              const QString output)
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

    boost::shared_ptr<PartialOsmMapReader> partialReader1 =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(input1);
    if (partialReader1)
    {
      partialReader1->finalizePartial();
    }
    input1->close();
    boost::shared_ptr<PartialOsmMapReader> partialReader2 =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(input2);
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


