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
//#include <hoot/core/io/OsmPbfReader.h>

using namespace std;

namespace hoot
{

/**
 * This command takes an input and database and changeset outputs.  The input is translated to OSM
 * and filtered to POIs only, sorted by element type then ID if necessary, then compared to the
 * database output layer in order to derive a changeset.  The changeset changes are written both
 * to the database output layer as features and the changeset output as change statements.
 */
class MultiaryIngestCmd : public BaseCommand
{
public:

  static string className() { return "hoot::MultiaryIngestCmd"; }

  MultiaryIngestCmd() { }

  virtual QString getName() const { return "multiary-ingest"; }

  virtual int runSimple(QStringList args)
  {
    if (args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes four parameters.").arg(getName()));
    }

    const QString newDataInput = args[0];   //this must be streamable
    const QString dbLayerOutput = args[1];  //this must be hootapidb://
    const QString changesetOutput = args[2];    //this must be .spark.x
    bool sort = false;
    if (args[3].toLower() == "true")
    {
      sort = true;
    }

    LOG_VARD(newDataInput);
    LOG_VARD(dbLayerOutput);
    LOG_VARD(changesetOutput);
    LOG_VARD(sort);

    //reads the new input data
    boost::shared_ptr<PartialOsmMapReader> newInputReader;
    //reads out the existing data
    boost::shared_ptr<HootApiDbReader> dbLayerReader(new HootApiDbReader());
    conf().set(ConfigOptions::getApiDbReaderSortByIdKey(), true);
    //derives a changeset between the new and existing data
    ChangesetDeriverPtr changesetDeriver;
    conf().set(ConfigOptions::getTranslationScriptKey(), "OSM_Ingest.js");
    //writes the result of the changeset back to the existing layer
    HootApiDbWriter dbLayerChangeWriter;
    //writes the changeset statements for external use
    SparkChangesetWriter changesetFileWriter;

    //do some input error checking before kicking off a potentially long input sort

    //only supporting streamable i/o for the time being
    if (!OsmMapReaderFactory::getInstance().hasElementInputStream(newDataInput))
    {
      throw IllegalArgumentException(
        "This command does not support non-streamable inputs: " + newDataInput);
    }

    if (!dbLayerReader->isSupported(dbLayerOutput))
    {
      throw HootException(
        getName() + " only supports a hootapidb:// data source as the target changeset layer.  " +
        "Specified target: " + dbLayerOutput);
    }

    if (!changesetFileWriter.isSupported(changesetOutput))
    {
      throw HootException(
        getName() + " only supports a .spark.x file for changeset output.  Specified output: " +
        changesetOutput);
    }

    LOG_INFO(
      "Streaming multiary data ingest from input: " << newDataInput <<
      " to output database layer: " << dbLayerOutput << " and output changeset: " <<
      changesetOutput << "...");

    //sort data by ID, if necessary (only passing nodes through)
    QString sortedNewDataInput = newDataInput;
    //OsmPbfReader tmpPbfReader; //getSortedTypeThenId
    if (sort) //TODO: if pbf, check pbf format flag
    {
      sortedNewDataInput = ElementSorter::sortInput(QUrl(newDataInput));
    }
    newInputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(sortedNewDataInput));

    newInputReader->open(sortedNewDataInput);
    dbLayerReader->open(dbLayerOutput);
    //no need to check this input format type, since we did it for the reader above
    dbLayerChangeWriter.open(dbLayerOutput);
    changesetFileWriter.open(changesetOutput);

    //filter down to POIs only and translate each element as the changeset is generated
    boost::shared_ptr<ElementInputStream> newInputStreamReader =
      boost::dynamic_pointer_cast<ElementInputStream>(newInputReader);
    boost::shared_ptr<PoiCriterion> poiFilter(new PoiCriterion());
    boost::shared_ptr<TranslationVisitor> translator(new TranslationVisitor());
    newInputStreamReader.reset(
      new ElementCriterionVisitorInputStream(newInputStreamReader, poiFilter, translator));

    //stream changeset changes to db and changeset file outputs
    changesetDeriver.reset(
      new ChangesetDeriver(
        boost::dynamic_pointer_cast<ElementInputStream>(dbLayerReader), newInputStreamReader));
    while (changesetDeriver->hasMoreChanges())
    {
      const Change change = changesetDeriver->readNextChange();
      if (change.type != Change::Unknown) //TODO: should this check be in the while loop?
      {
        dbLayerChangeWriter.writeChange(change);
        changesetFileWriter.writeChange(change);
      }
    }

    //are all of these needed?
    newInputReader->finalizePartial();
    dbLayerReader->finalizePartial();
    dbLayerChangeWriter.finalizePartial();

    LOG_INFO(
      "Multiary data ingest complete for input: " << newDataInput <<
      " to output database layer: " << dbLayerOutput << " and output changeset: " <<
      changesetOutput << "...");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryIngestCmd)

}
