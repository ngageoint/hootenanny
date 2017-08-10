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
#include <hoot/rnd/io/SparkJsonWriter.h>
#include <hoot/core/io/HootApiDbReader.h>

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
    if (args.size() != 3 && args.size() != 4)
    {
      cout << getHelp() << endl << endl;
      throw HootException(QString("%1 takes three or four parameters.").arg(getName()));
    }

    const QStringList newDataInputs = args[0].split(";");
    const QStringList existingDbLayerOutputs = args[1].split(";");
    const QStringList changesetOutputs = args[2].split(";");
    QStringList sorted;
    if (args.size() > 3)
    {
      sorted = args[3].split(";");
    }
    else
    {
      for (int i = 0; i < newDataInputs.size(); i++)
      {
        sorted.append("false");
      }
    }

    LOG_VARD(newDataInputs);
    LOG_VARD(existingDbLayerOutputs);
    LOG_VARD(changesetOutputs);
    LOG_VARD(sorted);

    if (newDataInputs.size() != existingDbLayerOutputs.size() ||
        newDataInputs.size() != changesetOutputs.size() ||
        (sorted.size() > 0 && (sorted.size() != newDataInputs.size())))
    {
      QString errorMsg =
        "Mismatching input parameter list sizes.  Inputs: " + QString::number(newDataInputs.size()) +
        ", Layer Outputs: " + QString::number(existingDbLayerOutputs.size()) +
        ", Changeset Outputs: " + QString::number(changesetOutputs.size());
      if (sorted.size() > 0)
      {
        errorMsg += ", Sorted descriptors: " + QString::number(sorted.size());
      }
      throw IllegalArgumentException(errorMsg);
    }

    for (int i = 0; i < newDataInputs.size(); i++)
    {
      const QString input = newDataInputs[i];
      //only supporting streamable i/o for the time being
      if (!OsmMapReaderFactory::getInstance().hasElementInputStream(input))
      {
        throw IllegalArgumentException(
          "This command does not support non-streamable inputs: " + input);
      }
    }

    HootApiDbReader tmpDbReader;
    HootApiDbWriter tmpDbWriter;
    for (int i = 0; i < existingDbLayerOutputs.size(); i++)
    {
      const QString dbOutput = existingDbLayerOutputs[i];
      if (!tmpDbReader.isSupported(dbOutput) || !tmpDbWriter.isSupported(dbOutput))
      {
        throw IllegalArgumentException(
          "Database layer output must be a Hootenanny API database URL: " + dbOutput);
      }
    }

    SparkJsonWriter tmpChangesetWriter;
    for (int i = 0; i < changesetOutputs.size(); i++)
    {
      const QString changesetOutput = changesetOutputs[i];
      if (!tmpChangesetWriter.isSupported(changesetOutput))
      {
        throw IllegalArgumentException(
          "Changeset output must be a Spark format: " + changesetOutput);
      }
    }

    for (int i = 0; i < newDataInputs.size(); i++)
    {
      const QString newDataInput = newDataInputs[i];
      const QString existingDbLayerOutput = existingDbLayerOutputs[i];
      const QString changesetOutput = changesetOutputs[i];

      LOG_INFO(
        "Streaming data ingest from " << newDataInput << " to database layer: " <<
        existingDbLayerOutput << " and changeset: " << changesetOutput << "...");

      QString sortedNewDataInput;
      if (sorted[i].toLower() == "true")
      {
        sortedNewDataInput = newDataInput;
      }
      else
      {
        //TODO: if pbf, check pbf format flag
        //TODO: implement file based element sorter which returns the sorted element file output
        //path
        //sortedNewDataInput = ElementSorter.sort(input);
      }

      boost::shared_ptr<PartialOsmMapReader> newInputDataReader =
        boost::dynamic_pointer_cast<PartialOsmMapReader>(
          OsmMapReaderFactory::getInstance().createReader(sortedNewDataInput));
      newInputDataReader->open(sortedNewDataInput);

      boost::shared_ptr<PartialOsmMapReader> existingDbLayerReader =
        boost::dynamic_pointer_cast<PartialOsmMapReader>(
          OsmMapReaderFactory::getInstance().createReader(existingDbLayerOutput));
      //TODO: the db contents have to get sorted somehow too??
      existingDbLayerReader->open(existingDbLayerOutput);

      ChangesetDeriverPtr changesetDeriver(
        new ChangesetDeriver(existingDbLayerReader, newInputDataReader));

      boost::shared_ptr<PartialOsmMapWriter> existingDbLayerWriter =
        boost::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::getInstance().createWriter(existingDbLayerOutput));
      existingDbLayerWriter->open(existingDbLayerOutput);
      existingDbLayerWriter->initializePartial();

      //TODO: update spark changeset writer
      boost::shared_ptr<PartialOsmMapWriter> changesetFileWriter =
        boost::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::getInstance().createWriter(changesetOutput));
      changesetFileWriter->open(changesetOutput);
      changesetFileWriter->initializePartial();

      boost::shared_ptr<ElementCriterion> criterion = IoUtils::getStreamingCriterion();

      while (changesetDeriver->hasMoreChanges())
      {
        const Change change = changesetDeriver->readNextChange();
        if (!criterion.get() || criterion->isSatisfied(change.e))
        {
          // TODO: add support for deletes
          existingDbLayerWriter->writeElement(change.e);
          changesetFileWriter->writeElement(change.e);
        }
        else
        {
          LOG_TRACE("Element did not satisfy filter: " << change.e->getElementId());
        }
      }

      newInputDataReader->finalizePartial();
      existingDbLayerReader->finalizePartial();
      existingDbLayerWriter->finalizePartial();
      changesetFileWriter->finalizePartial();
    }

    return 0;
  }

private:



};

HOOT_FACTORY_REGISTER(Command, MultiaryIngestCmd)

}


