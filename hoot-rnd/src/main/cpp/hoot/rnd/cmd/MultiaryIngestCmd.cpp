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

    const QStringList inputs = args[0].split(";");
    const QStringList outputs = args[1].split(";");
    const QStringList changesetOutputs = args[2].split(";");
    QStringList sorted;
    if (args.size() > 3)
    {
      sorted = args[3].split(";");
    }
    else
    {
      for (int i = 0; i < inputs.size(); i++)
      {
        sorted.append("false");
      }
    }

    LOG_VARD(inputs);
    LOG_VARD(outputs);
    LOG_VARD(changesetOutputs);
    LOG_VARD(sorted);

    if (inputs.size() != outputs.size() || inputs.size() != changesetOutputs.size())
    {
      throw IllegalArgumentException(
        "Mismatching number of inputs and outputs.  Inputs: " + QString::number(inputs.size()) +
        ", Layer Outputs: " + QString::number(outputs.size()) +
        ", Changeset Outputs: " + QString::number(changesetOutputs.size()));
    }

    if (sorted.size() > 0 && (sorted.size() != inputs.size()))
    {
      throw IllegalArgumentException(
        "Mismatching number of sorted descriptors.  Sorted descriptors: " +
        QString::number(sorted.size()) + ", Inputs: " + QString::number(inputs.size()) +
        ", Layer Outputs: " + QString::number(outputs.size()) +
        ", Changeset Outputs: " + QString::number(changesetOutputs.size()));
    }

    HootApiDbReader tmpDbReader;
    HootApiDbWriter tmpDbWriter;
    for (int i = 0; i < outputs.size(); i++)
    {
      const QString dbOutput = outputs[i];
      if (!tmpDbReader.isSupported(dbOutput) || !tmpDbWriter.isSupported(dbOutput))
      {
        throw IllegalArgumentException(
          "Database layer output must be a Hootenanny API database URL: " + dbOutput);
      }
    }

    //TODO: replace with actual changeset writer
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

    QString sortedInput;
    for (int i = 0; i < inputs.size(); i++)
    {
      const QString input = inputs[i];
      const QString output = outputs[i];
      const QString changesetOutput = changesetOutputs[i];

      LOG_INFO("Streaming data ingest from " << sortedInput << " to " << output << "...");

      //only supporting streamable i/o for the time being
      if (!OsmMapReaderFactory::getInstance().hasElementInputStream(input))
      {
        throw IllegalArgumentException(
          "Command does not support non-streamable inputs: " + input);
      }

      if (sorted[i].toLower() == "true")
      {
        sortedInput = input;
      }
      else
      {
        //TODO: check pbf format flag
        //TODO: implement file based element sorter which returns the sorted element file output
        //path
        //sortedInput = ElementSorter.sort(input);
      }

      boost::shared_ptr<PartialOsmMapReader> inputDataReader =
        boost::dynamic_pointer_cast<PartialOsmMapReader>(
          OsmMapReaderFactory::getInstance().createReader(sortedInput));
      inputDataReader->open(sortedInput);

      boost::shared_ptr<PartialOsmMapReader> existingDataDbReader =
        boost::dynamic_pointer_cast<PartialOsmMapReader>(
          OsmMapReaderFactory::getInstance().createReader(output));
      //TODO: the db contents have to get sorted somehow too??
      existingDataDbReader->open(outputs[i]);

      ChangesetDeriverPtr changesetDeriver(
        new ChangesetDeriver(existingDataDbReader, inputDataReader));

      boost::shared_ptr<PartialOsmMapWriter> existingDataDbWriter =
        boost::dynamic_pointer_cast<PartialOsmMapWriter>(
          OsmMapWriterFactory::getInstance().createWriter(output));
      existingDataDbWriter->open(outputs[i]);
      existingDataDbWriter->initializePartial();

      //TODO: write actual spark changeset writer
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

          existingDataDbWriter->writeElement(change.e);
          changesetFileWriter->writeElement(change.e);
        }
        else
        {
          LOG_TRACE("Element did not satisfy filter: " << change.e->getElementId());
        }
      }

      inputDataReader->finalizePartial();
      existingDataDbWriter->finalizePartial();
      changesetFileWriter->finalizePartial();
    }

    return 0;
  }

private:



};

HOOT_FACTORY_REGISTER(Command, MultiaryIngestCmd)

}


