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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryIngester.h"

// hoot
#include <hoot/core/algorithms/changeset/ChangesetDeriver.h>
#include <hoot/core/io/OsmMapReaderFactory.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/rnd/io/SparkChangesetWriter.h>
#include <hoot/core/io/HootApiDbReader.h>
#include <hoot/core/io/ElementCriterionVisitorInputStream.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/visitors/SchemaTranslationVisitor.h>
#include <hoot/rnd/visitors/MultiaryPoiHashVisitor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/rnd/io/MultiaryIngestChangesetReader.h>
#include <hoot/rnd/io/OsmFileSorter.h>
#include <hoot/core/io/OgrReader.h>

// tgs
#include <tgs/System/Time.h>

// Qt
#include <QFileInfo>
#include <QDir>

namespace hoot
{

MultiaryIngester::MultiaryIngester() :
_logUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval())
{
  //in order for the sorting to work, all original POI ids must be retained...no new ones
  //assigned; we're assuming no duplicate ids in the input
  conf().set(ConfigOptions::getHootapiDbWriterRemapIdsKey(), false);
  //for the changeset derivation to work, all input IDs must not be modified as they are read
  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);

  //for debugging only
  //conf().set(ConfigOptions::getMaxElementsPerPartialMapKey(), 1000);
}

void MultiaryIngester::_doInputErrorChecking(const QString& newInput,
                                             const QString& translationScript,
                                             const QString& referenceOutput,
                                             const QString& changesetOutput)
{
  if (!OsmMapReaderFactory::hasElementInputStream(newInput))
  {
    throw IllegalArgumentException(
      QString("Multiary ingest only supports streamable input formats.") +
      QString("See the Supported Data Formats section in README.md for more detail.") +
      QString("Specified input: ") + newInput);
  }

  if (!HootApiDbReader().isSupported(referenceOutput))
  {
    throw IllegalArgumentException(
      "Multiary ingest only supports a Hootenanny API database data source (" +
      MetadataTags::HootApiDbScheme() + "://) as the reference output.  Specified reference layer: " +
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

  if (translationScript.trimmed().isEmpty())
  {
    throw IllegalArgumentException("A translation script must be defined.");
  }
}

void MultiaryIngester::ingest(const QString& newInput, const QString& translationScript,
                              const QString& referenceOutput, const QString& changesetOutput)
{
  LOG_INFO("Ingesting Multiary data from " << newInput << "...");

  //do some input error checking before kicking off a potentially lengthy sort process
  _doInputErrorChecking(newInput, translationScript, referenceOutput, changesetOutput);

  //script for translating input to OSM
  conf().set(ConfigOptions::getSchemaTranslationScriptKey(), translationScript);

  const QStringList dbUrlParts = referenceOutput.split("/");
  const QString mapName = dbUrlParts[dbUrlParts.size() - 1];
  HootApiDb referenceDb;
  referenceDb.open(referenceOutput);

  if (!referenceDb.mapExists(mapName))
  {
    LOG_INFO("The reference output dataset does not exist.");
    LOG_INFO("Writing the input data directly to the reference layer.");
    LOG_INFO("Generating a changeset file consisting entirely of create statements.");

    //If there's no existing reference data, then there's no point in deriving a changeset diff
    //or sorting the data by ID.  So in that case, write all of the input data directly to the ref
    //layer and generate a Spark changeset consisting entirely of create statements.
    _writeNewReferenceData(_getFilteredNewInputStream(newInput), referenceOutput, changesetOutput);
  }
  else
  {
    LOG_INFO("The reference output dataset exists.");
    LOG_INFO("Deriving the changes between the input and reference data.");
    LOG_INFO("Writing the changes to the reference layer and a changeset file.");

    //assuming no duplicate map names here
    referenceDb.setMapId(referenceDb.getMapIdByName(mapName));

    //decided to always sort the input
    _sortInputFile(newInput);
    QString sortedNewInput = _sortTempFile->fileName();

    //create the changes and write them to the ref db layer and also to a changeset file for
    //external use by Spark
    _writeChangesToReferenceLayer(
      _deriveAndWriteChangesToChangeset(
        _getFilteredNewInputStream(sortedNewInput), referenceOutput, changesetOutput)->fileName(),
      referenceOutput);
  }
}

void MultiaryIngester::_sortInputFile(const QString& input)
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
          ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + sortTempFileBaseName + "." +
          newInputFileInfo.completeSuffix()));
  }
  else
  {
    //OGR formats have to be converted to PBF before sorting
    _sortTempFile.reset(
      new QTemporaryFile(
        ConfigOptions().getApidbBulkInserterTempFileDir() + "/" + sortTempFileBaseName +
        ".osm.pbf"));
  }
  //for debugging only
  //sortTempFile->setAutoRemove(false);
  if (!_sortTempFile->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortTempFile->fileName() + ".");
  }

  OsmFileSorter::sort(input, _sortTempFile->fileName());

  LOG_INFO(input << " sorted by POI ID to output: " << _sortTempFile->fileName() << ".");
  LOG_INFO("Time elapsed: " << StringUtils::millisecondsToDhms(_timer.elapsed()));
}

std::shared_ptr<ElementInputStream> MultiaryIngester::_getFilteredNewInputStream(
  const QString& sortedNewInput)
{
  std::shared_ptr<PartialOsmMapReader> newInputReader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(sortedNewInput));
  newInputReader->setUseDataSourceIds(true);
  newInputReader->open(sortedNewInput);
  std::shared_ptr<ElementInputStream> inputStream =
    std::dynamic_pointer_cast<ElementInputStream>(newInputReader);

  //filter data down to POIs only, translate each element, and assign it a unique hash id
  std::shared_ptr<PoiCriterion> elementCriterion(new PoiCriterion());
  QList<ElementVisitorPtr> visitors;

  std::shared_ptr<SchemaTranslationVisitor> translationVisitor(new SchemaTranslationVisitor());
  // I think we always want to be going to OSM here unless otherwise specified (or maybe
  // regardless if its specified), but that should be verified.
  QString translationDirection =
    conf().getString(ConfigOptions::getSchemaTranslationDirectionKey());
  if (translationDirection.trimmed().isEmpty())
  {
    translationDirection = "toosm";
  }
  LOG_VARD(translationDirection);
  translationVisitor->setTranslationDirection(translationDirection);
  // always set the direction before setting the script
  translationVisitor->setTranslationScript(
    conf().getString(ConfigOptions::getSchemaTranslationScriptKey()));

  visitors.append(translationVisitor);
  std::shared_ptr<MultiaryPoiHashVisitor> hashVis(new MultiaryPoiHashVisitor());
  hashVis->setIncludeCircularError(true);
  visitors.append(hashVis);
  std::shared_ptr<ElementInputStream> filteredNewInputStream(
    new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));
  return filteredNewInputStream;
}

void MultiaryIngester::_writeNewReferenceData(const std::shared_ptr<ElementInputStream>& filteredNewInputStream,
                                              const QString& referenceOutput,
                                              const QString& changesetOutput)
{
  _timer.restart();
  LOG_INFO("Writing POIs to reference layer: " << referenceOutput << "...");

  //cast to this so we can get the total/skipped features count
  std::shared_ptr<ElementCriterionVisitorInputStream> critInputStrm =
    std::dynamic_pointer_cast<ElementCriterionVisitorInputStream>(filteredNewInputStream);

  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);
  //We're able to use the faster bulk copy inserter here, since this is a brand new dataset.  In
  //general for inserts, there is as much as a 70% performance increase when using the bulk copy
  //inserter vs the regular inserter.  However, for this workflow we're seeing a ~15% write
  //performance improvement.  This seems to be b/c the reads are much slower than the writing, due
  //to the POI filtering and translation.  So, we could maybe get better performance by splitting
  //the reading and writing into separate threads if we really wanted to (?).
  conf().set(ConfigOptions::getHootapiDbWriterCopyBulkInsertKey(), true);
  std::shared_ptr<PartialOsmMapWriter> referenceWriter =
    std::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::createWriter(referenceOutput));
  referenceWriter->open(referenceOutput);

  std::shared_ptr<OsmChangeWriter> changesetFileWriter =
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
          "POIs found: " << StringUtils::formatLargeNumber(changesParsed) <<
          " Non-POIs skipped: " << StringUtils::formatLargeNumber(featuresSkipped));
      }
    }
  }

  LOG_DEBUG("Flushing data...");
  referenceWriter->finalizePartial();
  changesetFileWriter->close();

  LOG_INFO("POIs written to reference layer: " << StringUtils::formatLargeNumber(changesParsed));
  LOG_INFO("Non-POIs skipped: " << StringUtils::formatLargeNumber(featuresSkipped));
  LOG_INFO("Changes written to changeset file: " << StringUtils::formatLargeNumber(changesParsed));
  LOG_INFO("Time elapsed: " << StringUtils::millisecondsToDhms(_timer.elapsed()));
}

std::shared_ptr<QTemporaryFile> MultiaryIngester::_deriveAndWriteChangesToChangeset(
    const std::shared_ptr<ElementInputStream>& filteredNewInputStream,
    const QString& referenceInput, const QString& changesetOutput)
{
  //The changeset file changes and reference layer POI updates are written in two separate steps.
  //If we tried to write the POI changes to the reference layer as we streamed in the POIs
  //from the ref layer to the changeset deriver, we'd have a moving target as far as number of
  //elements is concerned, and the partial map reader wouldn't behave correctly.

  _timer.restart();
  LOG_INFO("Deriving and writing changes to changeset file: " << changesetOutput << "...");

  //cast to this so we can get the total/skipped features count
  std::shared_ptr<ElementCriterionVisitorInputStream> critInputStrm =
    std::dynamic_pointer_cast<ElementCriterionVisitorInputStream>(filteredNewInputStream);

  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  std::shared_ptr<PartialOsmMapReader> referenceReader =
    std::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::createReader(referenceInput));
  referenceReader->open(referenceInput);
  LOG_DEBUG("Opened reference reader.");

  ChangesetDeriver changesetDeriver(
    std::dynamic_pointer_cast<ElementInputStream>(referenceReader), critInputStrm);
  LOG_DEBUG("Initialized changeset deriver.");

  //this spark changeset writer will write the element payload as json for external spark use
  std::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput, "json");
  changesetFileWriter->open(changesetOutput);
  LOG_DEBUG("Opened change file writer.");

  //This is an unfortunate extra expense, but due to not being able to correctly read back out
  //the element payload json for writing the change to the database in the next step, write out
  //a second changeset with the payload in xml; this spark changeset writer will write the
  //element payload as xml for db writing
  std::shared_ptr<QTemporaryFile> tmpChangeset(
    new QTemporaryFile(
      ConfigOptions().getApidbBulkInserterTempFileDir() +
      "/multiary-ingest-changeset-temp-XXXXXX.mic"));
  //for debugging only
  //tmpChangeset->setAutoRemove(false);
  if (!tmpChangeset->open())
  {
    throw HootException("Unable to open changeset temp file: " + tmpChangeset->fileName() + ".");
  }
  LOG_VART(tmpChangeset->fileName());
  std::shared_ptr<OsmChangeWriter> changesetTempFileWriter =
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
        //not completely sure why these progress logs don't overwrite each other like the others
        //do...
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
  LOG_INFO("Time elapsed: " << StringUtils::millisecondsToDhms(_timer.elapsed()));

  return tmpChangeset;
}

void MultiaryIngester::_writeChangesToReferenceLayer(const QString& changesetOutput,
                                                     const QString& referenceOutput)
{
  _timer.restart();
  LOG_INFO("Writing changes to reference layer: " << referenceOutput << "...");

  //would cast straight to OsmChangeWriter here, but haven't figured out a way around
  //the fact that you can't use the factory macro twice on the same class.  Since HootApiDbWriter
  //already has the macro for OsmMapWriter, it can't be added for OsmChangeWriter as well.
  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), false);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), false);
  //The bulk copy inserter won't work here, b/c we're writing more than just inserts, and even if we
  //executed the inserts separately with the bulk inserter, we would still want the modifies/deletes
  //to run within the same transaction.  Having all of them in the same transaction isn't possible
  //here unless we use the HootApiDbWriter.
  conf().set(ConfigOptions::getHootapiDbWriterCopyBulkInsertKey(), false);
  std::shared_ptr<PartialOsmMapWriter> referenceWriter =
    std::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::createWriter(referenceOutput));
  referenceWriter->initializePartial();
  std::shared_ptr<OsmChangeWriter> referenceChangeWriter =
    std::dynamic_pointer_cast<OsmChangeWriter>(referenceWriter);
  referenceChangeWriter->open(referenceOutput);
  LOG_DEBUG("Opened change layer writer.");

  //this spark changeset reader will read in the element payload as xml (see comment in
  //_deriveAndWriteChangesToChangeset)
  // add a ChangesetProviderFactory or an OsmChangeReaderFactory to get rid of this
  //SparkChangesetReader dependency?
  MultiaryIngestChangesetReader changesetFileReader;
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
        "Wrote " << StringUtils::formatLargeNumber(changesWritten) << " changes to ref layer.");
    }
  }

  LOG_INFO("Flushing data...");
  referenceWriter->finalizePartial();
  referenceChangeWriter->close();
  changesetFileReader.close();

  LOG_INFO(
    StringUtils::formatLargeNumber(changesWritten) <<
    " changes written to reference layer: " << referenceOutput << ".");
  LOG_INFO("Time elapsed: " << StringUtils::millisecondsToDhms(_timer.elapsed()));
}

}
