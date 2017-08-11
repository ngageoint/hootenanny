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
#include <hoot/core/io/IoUtils.h>
#include <hoot/rnd/io/SparkChangesetWriter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
//#include <hoot/core/io/OsmPbfReader.h>

using namespace std;

namespace hoot
{

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

    //do some input error checking before kicking a potentially long input sort

    //only supporting streamable i/o for the time being
    if (!OsmMapReaderFactory::getInstance().hasElementInputStream(newDataInput))
    {
      throw IllegalArgumentException(
        "This command does not support non-streamable inputs: " + newDataInput);
    }

    boost::shared_ptr<HootApiDbReader> dbLayerReader(new HootApiDbReader());
    if (!dbLayerReader->isSupported(dbLayerOutput))
    {
      throw HootException(
        getName() + " only supports a hootapidb:// data source as the target changeset layer.  " +
        "Specified target: " + dbLayerOutput);
    }

    SparkChangesetWriter changesetFileWriter;
    if (!changesetFileWriter.isSupported(changesetOutput))
    {
      throw HootException(
        getName() + "only supports a .spark.x file for changeset output.  Specified output: " +
        changesetOutput);
    }

    LOG_INFO(
      "Streaming multiary data ingest from input: " << newDataInput <<
      " to output database layer: " << dbLayerOutput << " and output changeset: " <<
      changesetOutput << "...");
    //TODO: also need to translate to OSM
    QStringList convertOps = conf().get(ConfigOptions::getConvertOpsKey()).toStringList();
    convertOps.append("hoot::PoiCriterion");
    conf().set(ConfigOptions::getConvertOpsKey(), convertOps);
    LOG_INFO("Applying filters: " << convertOps);
    conf().set(ConfigOptions::getApiDbReaderSortByIdKey(), true);

    QString sortedNewDataInput = newDataInput;
    //OsmPbfReader tmpPbfReader; //getSortedTypeThenId
    if (sort) //TODO: if pbf, check pbf format flag
    {
      sortedNewDataInput = ElementSorter::sortInput(QUrl(newDataInput));
    }

    boost::shared_ptr<PartialOsmMapReader> newInputReader =
      boost::dynamic_pointer_cast<PartialOsmMapReader>(
        OsmMapReaderFactory::getInstance().createReader(sortedNewDataInput));
    newInputReader->open(sortedNewDataInput);

    dbLayerReader->open(dbLayerOutput);

    ChangesetDeriverPtr changesetDeriver(
      new ChangesetDeriver(
        boost::dynamic_pointer_cast<ElementInputStream>(dbLayerReader),
        boost::dynamic_pointer_cast<ElementInputStream>(newInputReader)));

    HootApiDbWriter dbLayerChangeWriter;
    //no need to check this input format type, since we did it for the reader above
    dbLayerChangeWriter.open(dbLayerOutput);

    changesetFileWriter.open(changesetOutput);

    boost::shared_ptr<ElementCriterion> criterion = IoUtils::getStreamingCriterion();

    while (changesetDeriver->hasMoreChanges())
    {
      const Change change = changesetDeriver->readNextChange();
      if (change.type != Change::Unknown)
      {
        if (!criterion.get() || criterion->isSatisfied(change.e))
        {
          dbLayerChangeWriter.writeChange(change);
          changesetFileWriter.writeChange(change);
        }
        else
        {
          LOG_TRACE("Element did not satisfy filter: " << change.e->getElementId());
        }
      }
    }

    newInputReader->finalizePartial();

    LOG_INFO(
      "Multiary data ingest complete for input: " << newDataInput <<
      " to output database layer: " << dbLayerOutput << " and output changeset: " <<
      changesetOutput << "...");

    return 0;
  }
};

HOOT_FACTORY_REGISTER(Command, MultiaryIngestCmd)

}
