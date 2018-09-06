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
#include <hoot/core/algorithms/changeset/ElementSorter.h>
#include <hoot/core/io/OsmXmlChangesetFileWriter.h>
#include <hoot/core/io/OsmApiDbSqlChangesetFileWriter.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/criterion/TagKeyCriterion.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/util/IoUtils.h>
#include <hoot/core/algorithms/changeset/ElementExternalMergeSorter.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
#include <hoot/core/io/OsmChangeWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/visitors/AddAttributesVisitor.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/io/PartialOsmMapReader.h>

//GEOS
#include <geos/geom/Envelope.h>

// Qt
#include <QUrl>
#include <QDateTime>
#include <QFileInfo>

using namespace std;

namespace hoot
{

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
    const QStringList outputs = args[2].split(";");;

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

    //try to stream the changeset to avoid memory issues
    //This element sorter implementation check is here temporarily during initial testing so that
    //ElementExternalMergeSorter can be manually turned off until any issues are worked through.
    if (ConfigOptions().getChangesetElementSorter() == "hoot::ElementExternalMergeSorter" &&
        _inputFormatsStreamable(input1, input2))
    {
      _streamChangesets(input1, input2, outputs);
    }
    //Currently, not all formats are supported as streamable when sorting element IDs, so switch
    //over to memory bound I/O.
    else
    {
      _writeOutputs(_readInputs(input1, input2), outputs);
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
    LOG_VARD(ElementExternalMergeSorter::isSupportedInputFormat(input1));
    bool streamable =
      OsmMapReaderFactory::getInstance().hasElementInputStream(input1) &&
      ElementExternalMergeSorter::isSupportedInputFormat(input1);
    if (!singleInput)
    {
      LOG_VARD(OsmMapReaderFactory::getInstance().hasElementInputStream(input2));
      LOG_VARD(ElementExternalMergeSorter::isSupportedInputFormat(input2));
      streamable =
        streamable && OsmMapReaderFactory::getInstance().hasElementInputStream(input2) &&
        ElementExternalMergeSorter::isSupportedInputFormat(input2);
    }
    return streamable;
  }

  /*
   * Streams changesets on input and output; the data must have timestamps
   */
  void _streamChangesets(const QString input1, const QString input2, const QStringList outputs)
  {
    const bool singleInput = input2.trimmed().isEmpty();

    QString msg = "Streaming changeset output from " + input1.right(25);
    if (!singleInput)
    {
      msg += " and " + input2.right(25);
    }
    msg += " to " + outputs.join(",").right(50);
    LOG_INFO(msg);

    QList<ElementVisitorPtr> visitors;
    //we don't want to include review relations
    boost::shared_ptr<ElementCriterion> elementCriterion(
      new NotCriterion(
        boost::shared_ptr<TagKeyCriterion>(
          new TagKeyCriterion(MetadataTags::HootReviewNeeds()))));
    //node comparisons require hashes be present on the elements
    visitors.append(boost::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));

    boost::shared_ptr<PartialOsmMapReader> reader1 =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(input1));
    reader1->setUseDataSourceIds(true);
    reader1->open(input1);
    ElementInputStreamPtr inputStream1 = boost::dynamic_pointer_cast<ElementInputStream>(reader1);
    ElementInputStreamPtr filteredSortedInputStream1(
      new ElementCriterionVisitorInputStream(inputStream1, elementCriterion, visitors));

    ElementSorterPtr sorted1;
    ElementSorterPtr sorted2;
    QFileInfo inputFileInfo1(input1);
    sorted1.reset(
      new ElementExternalMergeSorter(filteredSortedInputStream1, inputFileInfo1.completeSuffix()));
    if (!singleInput)
    {
      boost::shared_ptr<PartialOsmMapReader> reader2 =
        boost::dynamic_pointer_cast<PartialOsmMapReader>(
          OsmMapReaderFactory::getInstance().createReader(input2));
      reader2->setUseDataSourceIds(true);
      reader2->open(input2);
      ElementInputStreamPtr inputStream2 = boost::dynamic_pointer_cast<ElementInputStream>(reader2);
      ElementInputStreamPtr filteredSortedInputStream2(
        new ElementCriterionVisitorInputStream(inputStream2, elementCriterion, visitors));

      QFileInfo inputFileInfo2(input2);
      sorted2.reset(
        new ElementExternalMergeSorter(
          filteredSortedInputStream2, inputFileInfo2.completeSuffix()));
    }

    //Eventually this could be cleaned up to use OsmChangeWriterFactory and the OsmChange interface
    //instead.
    _writeOutputs(
      outputs,
      boost::shared_ptr<ChangesetDeriver>(new ChangesetDeriver(sorted1, sorted2)));
  }

  /*
   * Reads entire inputs into memory
   */
  QList<OsmMapPtr> _readInputs(const QString input1, const QString input2)
  {
    const bool singleInput = input2.trimmed().isEmpty();

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
   * memory bound sorting
   */
  ChangesetDeriverPtr _sortInputs(QList<OsmMapPtr> inputMaps)
  {
    ElementSorterPtr sorted1(new ElementSorter(inputMaps[0]));
    ElementSorterPtr sorted2(new ElementSorter(inputMaps[1]));
    ChangesetDeriverPtr delta(new ChangesetDeriver(sorted1, sorted2));
    return delta;
  }

  /*
   * Writes output for the given changeset provider
   */
  void _writeOutputs(const QStringList outputs, ChangesetDeriverPtr changesetDeriver)
  {
    LOG_INFO("Writing changeset output to " << outputs.join(",").right(50) << "...")

    LOG_VARD(outputs.size());
    QString stats;
    for (int i = 0; i < outputs.size(); i++)
    {
      const QString output = outputs[i];
      LOG_VARD(output);

      if (output.endsWith(".osc"))
      {
        OsmXmlChangesetFileWriter writer;
        writer.write(output, changesetDeriver);
        stats = writer.getStatsTable();
      }
      else if (output.endsWith(".osc.sql"))
      {
        assert(!_osmApiDbUrl.isEmpty());;
        OsmApiDbSqlChangesetFileWriter(QUrl(_osmApiDbUrl)).write(output, changesetDeriver);
      }
    }

    if (_printStats)
    {
      LOG_INFO("Changeset Stats:\n" << stats);
    }
  }

  /*
   * Writes output after first creating a changeset deriver with sorted elements
   */
  void _writeOutputs(const QList<OsmMapPtr>& inputMaps, const QStringList outputs)
  {
    _writeOutputs(outputs, _sortInputs(inputMaps));
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


