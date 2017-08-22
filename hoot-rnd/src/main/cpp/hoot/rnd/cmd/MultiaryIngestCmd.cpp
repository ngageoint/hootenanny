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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/cmd/BaseCommand.h>
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/ElementSorter.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/io/HootApiDbWriter.h>
#include <hoot/rnd/io/SparkChangesetWriter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
#include <hoot/rnd/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/visitors/TranslationVisitor.h>
#include <hoot/core/io/ElementOutputStream.h>
#include <hoot/core/io/GeoNamesReader.h>
//#include <hoot/core/io/OsmPbfReader.h>

// Qt
#include <QUuid>

using namespace std;

namespace hoot
{

/**
 * This command takes a supported data input along with database and changeset output locations.
 * The input is filtered down to POIs only and translated to OSM, then sorted by element ID if
 * necessary, and finally compared to the database output layer in order to derive the difference
 * between the two in the form of a changeset.  The changeset changes are written both to the
 * database output layer as features and to the changeset output file as change statements.
 *
 * The command requires that the input be a streamable format, the output layer be a Hootenanny
 * API database layer, and the changeset output format be a Spark changeset.
 */
class MultiaryIngestCmd : public BaseCommand
{
public:

  static string className() { return "hoot::MultiaryIngestCmd"; }

  MultiaryIngestCmd() :
  _sortInput(false)
  {
  }

  virtual ~MultiaryIngestCmd()
  {
    //delete the temporary db layer used for sorting
    if (_sortInput)
    {
      LOG_DEBUG("Deleting temporary map: " << _sortedNewInput << "...");
      HootApiDbWriter().deleteMap(_sortedNewInput);
    }
  }

  virtual QString getName() const { return "multiary-ingest"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes four parameters.").arg(getName()));
    }

    const QString newInput = args[0];
    const QString referenceOutput = args[1];
    const QString changesetOutput = args[2];
    _sortInput = false;
    if (args[3].toLower() == "true")
    {
      _sortInput = true;
    }

    LOG_VARD(newInput);
    LOG_VARD(referenceOutput);
    LOG_VARD(changesetOutput);
    LOG_VARD(_sortInput);

    if (!OsmMapReaderFactory::getInstance().hasElementInputStream(newInput))
    {
      throw IllegalArgumentException(
        "This command does not support non-streamable inputs: " + newInput);
    }

    if (!HootApiDbReader().isSupported(referenceOutput))
    {
      throw HootException(
        getName() + " only supports a hootapidb:// data source as the reference output.  " +
        "Specified target: " + referenceOutput);
    }

    if (!SparkChangesetWriter().isSupported(changesetOutput))
    {
      throw HootException(
        getName() + " only supports a .spark.x file for changeset output.  Specified output: " +
        changesetOutput);
    }

    LOG_INFO(
      "Streaming multiary data ingest from input: " << newInput << " to reference output: " <<
      referenceOutput << " and changeset: " << changesetOutput << "...");

    //inputs must be sorted by id for changeset derivation to work
    conf().set(ConfigOptions::getApiDbReaderSortByIdKey(), true);
    //in order for the sorting to work, all original element ids must be retained; we're assuming
    //no duplicate ids in the input
    conf().set(ConfigOptions::getHootapiDbWriterRemapIdsKey(), false);
    //translating inputs to OSM
    conf().set(ConfigOptions::getTranslationScriptKey(), "translations/OSM_Ingest.js");

    //sort incoming data by ID, if necessary, for changeset derivation (only passing nodes
    //through, so don't need to also sort by element type)
    _sortedNewInput = _getSortedInput(newInput);

    //create changes and write them to the existing db layer and also to a changeset file for
    //external use in spark
    _writeChangesetData(_getNewInputStream(_sortedNewInput), referenceOutput, changesetOutput);

    LOG_INFO(
      "Multiary data ingest complete for input: " << newInput <<
      " to reference output: " << referenceOutput << " and changeset: " <<
      changesetOutput << "...");

    return 0;
  }

private:

  bool _sortInput;
  QString _sortedNewInput;

  QString _getSortedInput(const QString newInput)
  {
    //OsmPbfReader tmpPbfReader; //getSortedTypeThenId
    if (!_sortInput) //TODO: if pbf, check pbf format flag
    {
      return newInput;
    }

    //write the unsorted input to temp db layer; later it will be queried back out sorted by id

    //TODO: if performance for this ends up being a bottleneck, will later implement something here
    //that performs faster - either a file based merge sort, or using the db like this but with sql
    //copy statements instead

    boost::shared_ptr<PartialOsmMapReader> unsortedNewInputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(newInput));
    unsortedNewInputReader->setUseDataSourceIds(true);
    unsortedNewInputReader->open(newInput);
    boost::shared_ptr<ElementInputStream> unsortedNewInputStream =
      boost::dynamic_pointer_cast<ElementInputStream>(unsortedNewInputReader);

    boost::shared_ptr<PoiCriterion> elementCriterion;
    //as the incoming data is read, filter it down to POIs only and translate each element
    //all geonames data are pois by definition, so skip the element criterion filtering expense
    if (!GeoNamesReader().isSupported(newInput))
    {
      elementCriterion.reset(new PoiCriterion());
    }
    unsortedNewInputStream.reset(
      new ElementCriterionVisitorInputStream(
        unsortedNewInputStream,
        elementCriterion,
        boost::shared_ptr<TranslationVisitor>(new TranslationVisitor())));

    const QString sortedNewInput =
      HootApiDb::getBaseUrl().toString() + "/MultiaryIngest-tempNewInput-" +
      QUuid::createUuid().toString();
    boost::shared_ptr<HootApiDbWriter> unsortedNewInputWriter(new HootApiDbWriter());
    unsortedNewInputWriter->setCreateUser(true);
    unsortedNewInputWriter->setOverwriteMap(true);
    unsortedNewInputWriter->open(sortedNewInput);
    boost::shared_ptr<ElementOutputStream> unsortedNewOutputStream =
      boost::dynamic_pointer_cast<ElementOutputStream>(unsortedNewInputWriter);

    LOG_DEBUG("Writing multiary input to temp location: " << sortedNewInput << "...");
    ElementOutputStream::writeAllElements(*unsortedNewInputStream, *unsortedNewOutputStream);
    LOG_DEBUG("Multiary input written to temp location: " << sortedNewInput);

    return sortedNewInput;
  }

  boost::shared_ptr<ElementInputStream> _getNewInputStream(const QString sortedNewInput)
  {
    boost::shared_ptr<PartialOsmMapReader> newInputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(sortedNewInput));
    newInputReader->setUseDataSourceIds(true);
    newInputReader->open(sortedNewInput);
    return boost::dynamic_pointer_cast<ElementInputStream>(newInputReader);
  }

  void _writeChangesetData(boost::shared_ptr<ElementInputStream> newInputStream,
                           const QString referenceOutput, const QString changesetOutput)
  {
    LOG_DEBUG(
      "Writing multiary change data to existing database layer: " << referenceOutput <<
      " and changeset file: " << changesetOutput << "...")

    boost::shared_ptr<HootApiDbReader> referenceReader(new HootApiDbReader());
    referenceReader->setUseDataSourceIds(true);
    referenceReader->open(referenceOutput);

    ChangesetDeriverPtr changesetDeriver(
      new ChangesetDeriver(
        boost::dynamic_pointer_cast<ElementInputStream>(referenceReader), newInputStream));

    HootApiDbWriter referenceChangeWriter;
    referenceChangeWriter.setCreateUser(false);
    referenceChangeWriter.setOverwriteMap(false);
    referenceChangeWriter.open(referenceOutput);

    SparkChangesetWriter changesetFileWriter;
    changesetFileWriter.open(changesetOutput);

    while (changesetDeriver->hasMoreChanges())
    {
      const Change change = changesetDeriver->readNextChange();
      if (change.type != Change::Unknown)
      {
        changesetFileWriter.writeChange(change);
        referenceChangeWriter.writeChange(change);
      }
    }

    //all readers/writers involved are cleaning up by themselves in their destructors
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryIngestCmd)

}
