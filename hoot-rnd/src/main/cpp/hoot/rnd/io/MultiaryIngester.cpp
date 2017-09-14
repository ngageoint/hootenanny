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
#include "MultiaryIngester.h"

// hoot
#include <hoot/core/io/ChangesetDeriver.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/io/SparkChangesetWriter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/rnd/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/filters/PoiCriterion.h>
#include <hoot/core/visitors/TranslationVisitor.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/rnd/io/SparkChangesetReader.h>
#include <hoot/core/io/OsmFileSorter.h>
#include <hoot/core/io/OgrReader.h>

// tgs
#include <tgs/System/Time.h>

// Qt
#include <QFileInfo>
#include <QDir>

namespace hoot
{

MultiaryIngester::MultiaryIngester() :
_sortInput(true),
_logUpdateInterval(ConfigOptions().getOsmapidbBulkInserterFileOutputStatusUpdateInterval())
{
  //in order for the sorting to work, all original POI ids must be retained...no new ones
  //assigned; we're assuming no duplicate ids in the input
  conf().set(ConfigOptions::getHootapiDbWriterRemapIdsKey(), false);
  //for the changeset derivation to work, all input IDs must not be modified as they are read
  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  //script for translating input to OSM
  conf().set(ConfigOptions::getTranslationScriptKey(), "translations/OSM_Ingest.js");

  //for debugging only
  //conf().set(ConfigOptions::getMaxElementsPerPartialMapKey(), 1000);
}

void MultiaryIngester::_doInputErrorChecking(const QString newInput, const QString referenceOutput,
                                             const QString changesetOutput)
{
  if (!OsmMapReaderFactory::getInstance().hasElementInputStream(newInput))
  {
    throw IllegalArgumentException(
      QString("Multiary ingest only supports streamable input formats.") +
      QString("See the Supported Data Formats section in README.md for more detail.") +
      QString("Specified input: ") + newInput);
  }

  if (!HootApiDbReader().isSupported(referenceOutput))
  {
    throw IllegalArgumentException(
      QString("Multiary ingest only supports a Hootenanny API database data source ") +
      QString("(hootapidb://) as the reference output.  Specified reference layer: ") +
      referenceOutput);
  }

  if (!SparkChangesetWriter().isSupported(changesetOutput))
  {
    throw IllegalArgumentException(
      QString("Multiary ingest only supports a Spark changeset file (.spark.x) for changeset ") +
      QString("output.  Specified changeset output: ") + changesetOutput);
  }

  if (OgrReader().isSupported(newInput) &&
      conf().get(ConfigOptions::getOgrReaderNodeIdFieldNameKey()).toString().trimmed().isEmpty())
  {
    throw IllegalArgumentException(
      "When ingesting OGR formats the " + ConfigOptions::getOgrReaderNodeIdFieldNameKey() +
      " configuration option must be set in order to identify the ID field.");
  }
}

void MultiaryIngester::ingest(const QString newInput, const QString referenceOutput,
                              const QString changesetOutput, const bool sortInput)
{
  LOG_INFO("Ingesting Multiary data from " << newInput << "...");

  _sortInput = sortInput;

  //do some input error checking before kicking off a potentially lengthy sort process
  _doInputErrorChecking(newInput, referenceOutput, changesetOutput);

  const QStringList dbUrlParts = referenceOutput.split("/");
  const QString mapName = dbUrlParts[dbUrlParts.size() - 1];
  HootApiDb referenceDb;
  referenceDb.open(referenceOutput);

  if (!referenceDb.mapExists(mapName))
  {
    LOG_INFO("The reference output dataset does not exist.");
    LOG_INFO("Skipping POI sorting and changeset derivation.");
    LOG_INFO("Writing the input data directly to the reference layer");
    LOG_INFO("and generating a changeset file consisting entirely of create statements...");

    //If there's no existing reference data, then there's no point in deriving a changeset diff
    //or sorting the data by ID.  So in that case, write all of the input data directly to the ref
    //layer and generate a Spark changeset consisting entirely of create statements.
    _writeNewReferenceData(_getFilteredNewInputStream(newInput), referenceOutput, changesetOutput);
  }
  else
  {
    LOG_INFO("The reference output dataset exists.");
    LOG_INFO("Deriving a changeset between the input and reference data,");
    LOG_INFO("writing the changes to the reference layer,");
    LOG_INFO("and writing the changes to the changeset file...");

    //assuming no duplicate map names here
    referenceDb.setMapId(referenceDb.getMapIdByName(mapName));

    QString sortedNewInput;
    if (!_sortInput)
    {
      LOG_INFO("The input data will not be sorted by POI ID.");
      sortedNewInput = newInput;
    }
    else
    {
      _sortInputFile(newInput);
      sortedNewInput = _sortTempFile->fileName();
    }

    //create the changes and write them to the ref db layer and also to a changeset file for
    //external use by Spark
    _writeChangesToReferenceLayer(
      _deriveAndWriteChangesToChangeset(
        _getFilteredNewInputStream(sortedNewInput), referenceOutput, changesetOutput)->fileName(),
      referenceOutput);
  }
}

void MultiaryIngester::_sortInputFile(const QString input)
{
  _timer.restart();
  LOG_INFO("Sorting " << input << " by POI ID...");

  //sort incoming data by POI id, if necessary, for changeset derivation
  const QString sortTempFileBaseName = "multiary-ingest-sort-temp-XXXXXX";
  if (!OgrReader().isSupported(input))
  {
    QFileInfo newInputFileInfo(input);
      _sortTempFile.reset(
        new QTemporaryFile(
          QDir::tempPath() + "/" + sortTempFileBaseName + "." + newInputFileInfo.completeSuffix()));
  }
  else
  {
    //OGR formats have to be converted to PBF before sorting
    _sortTempFile.reset(
      new QTemporaryFile(QDir::tempPath() + "/" + sortTempFileBaseName + ".osm.pbf"));
  }
  //for debugging only
  //sortTempFile->setAutoRemove(false);
  if (!_sortTempFile->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortTempFile->fileName() + ".");
  }

  OsmFileSorter::sort(input, _sortTempFile->fileName());

  LOG_INFO(input << " sorted by POI ID to output: " << _sortTempFile->fileName() << ".");
  LOG_INFO("Time elapsed: " << StringUtils::secondsToDhms(_timer.elapsed()));
}

boost::shared_ptr<ElementInputStream> MultiaryIngester::_getFilteredNewInputStream(
  const QString sortedNewInput)
{
  boost::shared_ptr<PartialOsmMapReader> newInputReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(sortedNewInput));
  newInputReader->setUseDataSourceIds(true);
  newInputReader->open(sortedNewInput);
  boost::shared_ptr<ElementInputStream> inputStream =
    boost::dynamic_pointer_cast<ElementInputStream>(newInputReader);

  //filter new data down to POIs only and translate each element
  boost::shared_ptr<PoiCriterion> elementCriterion(new PoiCriterion());
  QList<ElementVisitorPtr> visitors;
  visitors.append(boost::shared_ptr<TranslationVisitor>(new TranslationVisitor()));
  visitors.append(boost::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));
  boost::shared_ptr<ElementInputStream> filteredNewInputStream(
    new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));
  return filteredNewInputStream;
}

void MultiaryIngester::_writeNewReferenceData(
  boost::shared_ptr<ElementInputStream> filteredNewInputStream, const QString referenceOutput,
  const QString changesetOutput)
{
  _timer.restart();
  LOG_INFO("Writing POIs to reference layer: " << referenceOutput << "...");

  //cast to this so we can get the total/skipped features count
  boost::shared_ptr<ElementCriterionVisitorInputStream> critInputStrm =
    boost::dynamic_pointer_cast<ElementCriterionVisitorInputStream>(filteredNewInputStream);

  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);
  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->open(referenceOutput);

  boost::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput, "json");
  changesetFileWriter->open(changesetOutput);

  long changesParsed = 0;
  long featuresSkipped = 0;
  while (critInputStrm->hasMoreElements())
  {
    ElementPtr element = critInputStrm->readNextElement();
    //since the element stream is filtered, the stream will always return a null element at the end
    if (element.get())
    {
      referenceWriter->writeElement(element);
      changesetFileWriter->writeChange(Change(Change::Create, element));

      changesParsed++;
      featuresSkipped = critInputStrm->getNumFeaturesTotal() -
        critInputStrm->getNumFeaturesPassingCriterion();

      if (changesParsed % _logUpdateInterval == 0 ||
          critInputStrm->getNumFeaturesTotal() % _logUpdateInterval == 0)
      {
        PROGRESS_INFO(
          "POIs written to ref layer: " << StringUtils::formatLargeNumber(changesParsed) <<
          " Non-POIs skipped: " << StringUtils::formatLargeNumber(featuresSkipped));
      }
    }
  }

  LOG_INFO("Flushing data...");
  referenceWriter->finalizePartial();
  changesetFileWriter->close();

  LOG_INFO("POIs written to reference layer: " << StringUtils::formatLargeNumber(changesParsed));
  LOG_INFO("Non-POIs skipped: " << StringUtils::formatLargeNumber(featuresSkipped));
  LOG_INFO("Changes written to changeset file: " << StringUtils::formatLargeNumber(changesParsed));
  LOG_INFO("Time elapsed: " << StringUtils::secondsToDhms(_timer.elapsed()));
}

boost::shared_ptr<QTemporaryFile> MultiaryIngester::_deriveAndWriteChangesToChangeset(
  boost::shared_ptr<ElementInputStream> filteredNewInputStream, const QString referenceInput,
  const QString changesetOutput)
{
  //The changeset file changes and reference layer POI updates are written in two separate steps.
  //If we tried to write the POI changes to the reference layer as we streamed in the POIs
  //from the ref layer to the changeset deriver, we'd have a moving target as far as number of
  //elements is concerned, and the partial map reader would behave correctly.

  _timer.restart();
  LOG_INFO("Deriving and writing changes to changeset file: " << changesetOutput << "...");

  //cast to this so we can get the total/skipped features count
  boost::shared_ptr<ElementCriterionVisitorInputStream> critInputStrm =
    boost::dynamic_pointer_cast<ElementCriterionVisitorInputStream>(filteredNewInputStream);

  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  boost::shared_ptr<PartialOsmMapReader> referenceReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(referenceInput));
  referenceReader->open(referenceInput);
  LOG_DEBUG("Opened reference reader.");

  ChangesetDeriver changesetDeriver(
    boost::dynamic_pointer_cast<ElementInputStream>(referenceReader), critInputStrm);
  LOG_DEBUG("Initialized changeset deriver.");

  //this spark changeset writer will write the element payload as json for external spark use
  boost::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput, "json");
  changesetFileWriter->open(changesetOutput);
  LOG_DEBUG("Opened change file writer.");

  //This is an unfortunate extra expense, but due to not being able to correctly read back out
  //the element payload json for writing the change to the database in the next step, write out
  //a second changeset with the payload in xml; this spark changeset writer will write the
  //element payload as xml for db writing
  boost::shared_ptr<QTemporaryFile> tmpChangeset(
    new QTemporaryFile(QDir::tempPath() + "/multiary-ingest-changeset-temp-XXXXXX.spark.1"));
  //for debugging only
  //tmpChangeset->setAutoRemove(false);
  if (!tmpChangeset->open())
  {
    throw HootException("Unable to open changeset temp file: " + tmpChangeset->fileName() + ".");
  }
  LOG_VART(tmpChangeset->fileName());
  boost::shared_ptr<OsmChangeWriter> changesetTempFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(tmpChangeset->fileName(), "xml");
  changesetTempFileWriter->open(tmpChangeset->fileName());
  LOG_DEBUG("Opened temp change file writer.");

  long changesParsed = 0;
  long featuresSkipped = 0;
  long referencePoisParsed = 0;
  long newPoisParsed = 0;
  QMap<Change::ChangeType, long> changesByType;
  changesByType[Change::Create] = 0;
  changesByType[Change::Modify] = 0;
  changesByType[Change::Delete] = 0;

  while (changesetDeriver.hasMoreChanges())
  {
    const Change change = changesetDeriver.readNextChange();
    LOG_VART(change.getType());
    if (change.getType() != Change::Unknown)
    {
      //write changeset file with json element payload for external spark use
      changesetFileWriter->writeChange(change);
      //write temp changeset file with xml element payload to avoid reading back in corrupted
      //unicode chars when writing the final changes to the reference db
      changesetTempFileWriter->writeChange(change);

      changesParsed++;
      const long numChanges = changesByType[change.getType()];
      changesByType[change.getType()] = numChanges + 1;
      featuresSkipped =
        critInputStrm->getNumFeaturesTotal() - critInputStrm->getNumFeaturesPassingCriterion();
      referencePoisParsed = changesetDeriver.getNumFromElementsParsed();
      newPoisParsed = changesetDeriver.getNumToElementsParsed();

      if ((changesParsed % _logUpdateInterval == 0) ||
          ((referencePoisParsed + newPoisParsed) % _logUpdateInterval == 0) ||
          ( critInputStrm->getNumFeaturesTotal() % _logUpdateInterval == 0))
      {
        PROGRESS_INFO(
          "Ref: " << StringUtils::formatLargeNumber(referencePoisParsed) <<
          " New: " << StringUtils::formatLargeNumber(newPoisParsed) <<
          " Skip: " << StringUtils::formatLargeNumber(featuresSkipped) <<
          " Chng: " << StringUtils::formatLargeNumber(changesParsed) <<
          " Cr: " << StringUtils::formatLargeNumber(changesByType[Change::Create]) <<
          " Mod: " << StringUtils::formatLargeNumber(changesByType[Change::Modify]) <<
          " Del: " << StringUtils::formatLargeNumber(changesByType[Change::Delete]));
      }
    }
  }

  LOG_INFO("Flushing data...");
  referenceReader->finalizePartial();
  changesetFileWriter->close();
  changesetDeriver.close();
  changesetTempFileWriter->close();

  LOG_INFO("Reference POIs parsed: " << StringUtils::formatLargeNumber(referencePoisParsed));
  LOG_INFO("New POIs parsed: " << StringUtils::formatLargeNumber(newPoisParsed));
  LOG_INFO("Non-POIs skipped: " << StringUtils::formatLargeNumber(featuresSkipped));
  LOG_INFO("Changes written to changeset file: " << StringUtils::formatLargeNumber(changesParsed));
  LOG_INFO("  Create statements: " << StringUtils::formatLargeNumber(changesByType[Change::Create]));
  LOG_INFO("  Modify statements: " << StringUtils::formatLargeNumber(changesByType[Change::Modify]));
  LOG_INFO("  Delete statements: " << StringUtils::formatLargeNumber(changesByType[Change::Delete]));
  LOG_INFO("Time elapsed: " << StringUtils::secondsToDhms(_timer.elapsed()));

  return tmpChangeset;
}

void MultiaryIngester::_writeChangesToReferenceLayer(const QString changesetOutput,
                                                     const QString referenceOutput)
{
  _timer.restart();
  LOG_INFO("Writing changes to reference layer: " << referenceOutput << "...");

  //would cast straight to OsmChangeWriter here, but haven't figured out a way around
  //the fact that you can't use the factory macro twice on the same class.  Since HootApiDbWriter
  //already has the macro for OsmMapWriter, it can't be added for OsmChangeWriter as well.
  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), false);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), false);
  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->initializePartial();
  boost::shared_ptr<OsmChangeWriter> referenceChangeWriter =
    boost::dynamic_pointer_cast<OsmChangeWriter>(referenceWriter);
  referenceChangeWriter->open(referenceOutput);
  LOG_DEBUG("Opened change layer writer.");

  //this spark changeset reader will read in the element payload as xml
  //TODO: add an ChangesetProviderFactory or OsmChangeReaderFactory to get rid of this
  //SparkChangesetReader dependency?
  SparkChangesetReader changesetFileReader;
  changesetFileReader.open(changesetOutput);
  LOG_DEBUG("Opened temp changeset file reader.");

  long changesWritten = 0;
  while (changesetFileReader.hasMoreChanges())
  {
    referenceChangeWriter->writeChange(changesetFileReader.readNextChange());
    changesWritten++;

    if (changesWritten % _logUpdateInterval == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(changesWritten) <<
        " changes to ref layer.");
    }
  }

  LOG_INFO("Flushing data...");
  referenceWriter->finalizePartial();
  referenceChangeWriter->close();
  changesetFileReader.close();

  LOG_INFO(
    StringUtils::formatLargeNumber(changesWritten) <<
    " changes written to reference layer: " << referenceOutput << ".");
  LOG_INFO("Time elapsed: " << StringUtils::secondsToDhms(_timer.elapsed()));
}

}
