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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/algorithms/changeset/InMemoryElementSorter.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/algorithms/changeset/ExternalMergeElementSorter.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/io/PartialOsmMapReader.h>
#include <hoot/core/io/OsmXmlReader.h>

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>

using namespace std;

namespace hoot
{

/**
 * Derives a set of changes given two map inputs; can write to multiple outputs
 *
 * As part of #2596, streaming reads and external element sorting were added.  However, the
 * in memory input reading/sorting has been left in place to support faster I/O in the situation
 * where large inputs are being delat with and large amounts of memory are available for
 * reading/sorting.  Access to the in memory implementation is controlled by the configuration
 * option, element.sorter.element.buffer.size (size = -1 results in the in memory implementation
 * being used).
 *
 * Note that cloning the results of external sorting can currently only be used when the underlying
 * source is OSM XML.  Therefore, using other types of data sources will always result an
 * additional external sorting operation for each changeset output.  #2609 would eliminate that
 * limitation.
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

  virtual QString getDescription() const
  { return "Creates an OSM changeset containing the difference between two OSM datasets"; }

  bool _printStats = false;

  virtual int runSimple(QStringList args)
  {
    //  Check if the --stats option is present
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
    const QStringList outputs = args[2].split(";");

    LOG_VARD(input1);
    LOG_VARD(input2);
    LOG_VARD(outputs);

    for (int i = 0; i < outputs.length(); i++)
    {
      const QString format = outputs[i];
      if (!_isSupportedOutputFormat(format))
      {
        throw HootException("Unsupported changeset output format: " + format);
      }
      else if (format.endsWith(".osc.sql"))
      {
        if (args.size() != 4)
        {
          cout << getHelp() << endl << endl;
          throw HootException(
            QString("%1 with SQL output takes four parameters.").arg(getName()));
        }
        _osmApiDbUrl = args[3];
      }
    }
    LOG_VARD(_osmApiDbUrl);

    _parseBuffer();

    //If streaming is enabled, try to stream the changeset to avoid memory issues.
    if (ConfigOptions().getElementSorterElementBufferSize() != -1 &&
        _inputFormatsStreamable(input1, input2))
    {
      if (outputs.size() == 1 ||
          //See comments in class description as to why this check exists.
          (outputs.size() > 1 &&
            (!OsmXmlReader().isSupported(input1) || !OsmXmlReader().isSupported(input2))))
      {
        _sortExternallyAndStreamChangesetOutput(input1, input2, outputs.at(0));
      }
      else
      {
        _sortExternallyAndStreamChangesetOutputs(input1, input2, outputs);
      }
    }
    //Currently, not all input formats are supported as streamable, so switch over to memory bound
    //I/O if necessary.
    else
    {
      _sortInMemoryAndStreamChangesetOutput(_readInputsFully(input1, input2), outputs);
    }

    return 0;
  }

private:

  QString _osmApiDbUrl;

  bool _inputFormatsStreamable(const QString input1, const QString input2) const
  {
    const bool singleInput = input2.trimmed().isEmpty();
    LOG_VART(singleInput);
    LOG_VARD(OsmMapReaderFactory::getInstance().hasElementInputStream(input1));
    bool streamable = OsmMapReaderFactory::getInstance().hasElementInputStream(input1);
    if (!singleInput)
    {
      LOG_VARD(OsmMapReaderFactory::getInstance().hasElementInputStream(input2));
      streamable =
        streamable && OsmMapReaderFactory::getInstance().hasElementInputStream(input2);
    }
    return streamable;
  }

  boost::shared_ptr<ExternalMergeElementSorter> _getSortedElements(const QString input)
  {
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
        OsmMapReaderFactory::getInstance().createReader(input));
    reader->setUseDataSourceIds(true);
    reader->open(input);
    ElementInputStreamPtr inputStream = boost::dynamic_pointer_cast<ElementInputStream>(reader);
    ElementInputStreamPtr filteredSortedInputStream(
      new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));

    boost::shared_ptr<ExternalMergeElementSorter> sorted(new ExternalMergeElementSorter());

    sorted->sort(filteredSortedInputStream);
    reader->finalizePartial();

    return sorted;
  }

  /*
   * Streams inputs, sorts elements externally, and streams changeset output(s).  The sorting will
   * only be done once per output here, since the sort results get cloned after sorting.
   *
   * Currently, this will only work when the underlying stream is an OsmXmlReader.
   */
  void _sortExternallyAndStreamChangesetOutputs(const QString input1, const QString input2,
                                                const QStringList outputs)
  {
    const bool singleInput = input2.trimmed().isEmpty();

    QString msg = "Streaming changeset input from " + input1.right(25);
    if (!singleInput)
    {
      msg += " and " + input2.right(25);
    }
    msg += " sorting, and writing output(s) to " + outputs.join(",").right(50);
    LOG_INFO(msg);

    boost::shared_ptr<ExternalMergeElementSorter> sortedElements1 = _getSortedElements(input1);
    boost::shared_ptr<ExternalMergeElementSorter> sortedElements2;
    if (!singleInput)
    {
      sortedElements2 = _getSortedElements(input2);
    }

    //Not worrying about the unnecessary clone here for a single output, since we're just cloning
    //a stream here and not an entire map, like in _sortInMemoryAndStreamChangesetOutput.
    for (int i = 0; i < outputs.size(); i++)
    {
      _streamChangesetOutput(
        boost::shared_ptr<ChangesetDeriver>(
          new ChangesetDeriver(
            //clone the sorted element streams here, so we don't have to resort the inputs for every
            //single output we want to write
            ElementInputStreamPtr(sortedElements1->clone()),
            ElementInputStreamPtr(sortedElements2->clone()))),
        outputs.at(i));
    }
  }

  /*
   * Streams inputs, sorts elements externally, and streams changeset to a single output.  Each
   * call to this method will result in a sorting operation.
   *
   * This will work with readers other than OsmXmlReader; can be eliminated after #2609.
   */
  void _sortExternallyAndStreamChangesetOutput(const QString input1, const QString input2,
                                               const QString output)
  {
    const bool singleInput = input2.trimmed().isEmpty();

    QString msg = "Streaming changeset input from " + input1.right(25);
    if (!singleInput)
    {
      msg += " and " + input2.right(25);
    }
    msg += " sorting, and writing output to " + output;
    LOG_INFO(msg);

    boost::shared_ptr<ExternalMergeElementSorter> sortedElements1 = _getSortedElements(input1);
    boost::shared_ptr<ExternalMergeElementSorter> sortedElements2;
    if (!singleInput)
    {
      sortedElements2 = _getSortedElements(input2);
    }

    _streamChangesetOutput(
      boost::shared_ptr<ChangesetDeriver>(new ChangesetDeriver(sortedElements1, sortedElements2)),
      output);
  }

  /*
   * Reads entire inputs into memory
   */
  QList<OsmMapPtr> _readInputsFully(const QString input1, const QString input2)
  {
    const bool singleInput = input2.trimmed().isEmpty();

    QString msg = "Reading entire inputs into memory for " + input1;
    if (!singleInput)
    {
      msg += " and " + input2;
    }
    msg += "...";
    LOG_INFO(msg);

    //some in these datasets may have status=3 if you're loading conflated data, so use
    //reader.use.file.status and reader.keep.status.tag if you want to retain that value
    OsmMapPtr map1(new OsmMap());
    OsmMapPtr map2(new OsmMap());
    if (!singleInput)
    {
      //map1 is the former state of the data
      IoUtils::loadMap(map1, input1, true, Status::Unknown1);
      //map2 is the newer state of the data
      IoUtils::loadMap(map2, input2, true, Status::Unknown2);
    }
    else
    {
      //here we're passing all the input data through to the output changeset, so put it in the
      //map2 newer data
      IoUtils::loadMap(map2, input1, true, Status::Unknown2);
    }

    //we don't want to include review relations
    boost::shared_ptr<TagKeyCriterion> elementCriterion(
      new TagKeyCriterion(MetadataTags::HootReviewNeeds()));
    RemoveElementsVisitor removeElementsVisitor(elementCriterion);
    removeElementsVisitor.setRecursive(false);
    map1->visitRw(removeElementsVisitor);
    map2->visitRw(removeElementsVisitor);

    //node comparisons require hashes be present on the elements
    CalculateHashVisitor2 hashVis;
    map1->visitRw(hashVis);
    map2->visitRw(hashVis);

    QList<OsmMapPtr> inputMaps;
    inputMaps.append(map1);
    inputMaps.append(map2);
    return inputMaps;
  }

  /*
   * Writes changeset output for the given changeset provider
   */
  void _streamChangesetOutput(ChangesetDeriverPtr changesetDeriver, const QString output)
  {
    LOG_INFO("Streaming changeset output to " << output.right(25) << "...")

    QString stats;
    LOG_VARD(output);

    //Could this eventually this could be cleaned up to use OsmChangeWriterFactory and the
    //OsmChange interface instead?
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

    if (_printStats)
    {
      LOG_INFO("Changeset Stats:\n" << stats);
    }
  }

  /*
   * Writes output after first sorting elements in memory
   */
  void _sortInMemoryAndStreamChangesetOutput(const QList<OsmMapPtr>& inputMaps,
                                             const QStringList outputs)
  {
    LOG_INFO(
      "Sorting changeset inputs in memory and streaming to " << outputs.join(",").right(50) <<
      "...");

    InMemoryElementSorterPtr sorted1(new InMemoryElementSorter(inputMaps[0]));
    InMemoryElementSorterPtr sorted2(new InMemoryElementSorter(inputMaps[1]));
    if (outputs.size() == 1)
    {
      //we can save a map copy here if there's only one output
      _streamChangesetOutput(
        boost::shared_ptr<ChangesetDeriver>(new ChangesetDeriver(sorted1, sorted2)),
        outputs.at(0));
    }
    else
    {
      for (int i = 0; i < outputs.size(); i++)
      {
        _streamChangesetOutput(
          boost::shared_ptr<ChangesetDeriver>(
            new ChangesetDeriver(
              //Clone the sorted element streams here, so we don't have to re-sort the inputs for
              //every single output we want to write.
              ElementInputStreamPtr(sorted1->clone()), ElementInputStreamPtr(sorted2->clone()))),
          outputs.at(i));
      }
    }
  }

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
};

HOOT_FACTORY_REGISTER(Command, DeriveChangesetCmd)

}


