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
#include <hoot/core/io/GeoNamesReader.h>
#include <hoot/core/visitors/CalculateHashVisitor2.h>
#include <hoot/core/util/FileUtils.h>
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
_addToExistingRefDb(false),
_changesParsed(0),
_logUpdateInterval(ConfigOptions().getOsmapidbBulkInserterFileOutputStatusUpdateInterval())
{
  //in order for the sorting to work, all original node ids must be retained...no new ones
  //assigned; we're assuming no duplicate ids in the input
  conf().set(ConfigOptions::getHootapiDbWriterRemapIdsKey(), false);
  //for the changeset derivation to work, all input IDs must not be modified as they are read
  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  //script for translating input to OSM
  conf().set(ConfigOptions::getTranslationScriptKey(), "translations/OSM_Ingest.js");
  //for debugging only
  //conf().set(ConfigOptions::getMaxElementsPerPartialMapKey(), 1000);
}

void MultiaryIngester::_clearChangeTypeCounts()
{
  _changesByType[Change::Create] = 0;
  _changesByType[Change::Modify] = 0;
  _changesByType[Change::Delete] = 0;
}

void MultiaryIngester::ingest(const QString newInput, const QString referenceOutput,
                              const QString changesetOutput, const bool sortInput)
{
  _changesParsed = 0;
  _clearChangeTypeCounts();
  _sortInput = sortInput;

  //do some input error checking before kicking off a potentially lengthy sort process

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

  LOG_INFO("Ingesting Multiary data from " << newInput << "...");

  const QStringList dbUrlParts = referenceOutput.split("/");
  const QString mapName = dbUrlParts[dbUrlParts.size() - 1];
  _referenceDb.open(referenceOutput);

  if (!_referenceDb.mapExists(mapName))
  {
    LOG_INFO("The reference output dataset does not exist.");
    LOG_INFO("Skipping node sorting and changeset derivation.");
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

    _addToExistingRefDb = true;
    //assuming no duplicate map names here
    _referenceDb.setMapId(_referenceDb.getMapIdByName(mapName));

    QString sortedNewInput;
    if (!_sortInput)
    {
      LOG_INFO("The input data will not be sorted by node ID.");
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

  LOG_INFO("Multiary data ingested from " << newInput);
  LOG_INFO("into reference layer: " << referenceOutput);
  LOG_INFO("and changeset file: " << changesetOutput << ".");
  _printSummary();
}

void MultiaryIngester::_sortInputFile(const QString input)
{
  _timer.restart();
  LOG_INFO("Sorting " << input << " by node ID...");

  //sort incoming data by node id, if necessary, for changeset derivation
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
    //OGR formats have to be converted to PBF for sorting
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

  LOG_INFO(input << " sorted by node ID to output: " << _sortTempFile->fileName() << ".");
  LOG_INFO("Time elapsed: " << FileUtils::secondsToDhms(_timer.elapsed()));
}

void MultiaryIngester::_printSummary()
{
  if (_addToExistingRefDb)
  {
    LOG_INFO(
      "Changes written to reference layer and changeset file: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesParsed));
    LOG_INFO(
      "  Create statements: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Create]));
    LOG_INFO(
      "  Modify statements: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Modify]));
    LOG_INFO(
      "  Delete statements: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Delete]));
  }
  else
  {
    LOG_INFO(
      "Nodes written to reference layer: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Create]));
    LOG_INFO(
      "Changes written to changeset file: " <<
      FileUtils::formatPotentiallyLargeNumber(_changesParsed));
  }
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

  //as the incoming data is read, filter it down to POIs only and translate each element

  boost::shared_ptr<PoiCriterion> elementCriterion;
  //all geonames are pois, so skip the element filtering expense for that format
  if (!GeoNamesReader().isSupported(sortedNewInput))
  {
    elementCriterion.reset(new PoiCriterion());
  }
  else
  {
    LOG_INFO("Skipping POI filtering for: " << sortedNewInput << ", since data is POI only.");
  }
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
  LOG_INFO("Writing nodes to reference layer: " << referenceOutput << "...");

  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);

  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->open(referenceOutput);

  boost::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput, "json");
  changesetFileWriter->open(changesetOutput);

  while (filteredNewInputStream->hasMoreElements())
  {
    ElementPtr element = filteredNewInputStream->readNextElement();
    if (element.get())
    {
      referenceWriter->writeElement(element);
      changesetFileWriter->writeChange(Change(Change::Create, element));
      _changesParsed++;
      const long numChanges = _changesByType[Change::Create];
      _changesByType[Change::Create] = numChanges + 1;

      if (_changesParsed % _logUpdateInterval == 0)
      {
        PROGRESS_INFO(
          "Wrote " << FileUtils::formatPotentiallyLargeNumber(_changesParsed) <<
          " nodes to the reference layer.");
      }
    }
  }

  referenceWriter->finalizePartial();
  changesetFileWriter->close();

  LOG_INFO("Nodes written to reference layer: " << referenceOutput << ".");
  LOG_INFO("Time elapsed: " << FileUtils::secondsToDhms(_timer.elapsed()));
}

boost::shared_ptr<QTemporaryFile> MultiaryIngester::_deriveAndWriteChangesToChangeset(
  boost::shared_ptr<ElementInputStream> filteredNewInputStream, const QString referenceInput,
  const QString changesetOutput)
{
  //The changeset file changes and reference layer node updates are written in two separate steps.
  //If we tried to write the node changes to the reference layer as we streamed in the nodes
  //from the ref layer to the changeset deriver, we'd have a moving target as far as number of
  //elements is concerned, and the partial map reader would behave correctly.

  _timer.restart();
  LOG_INFO("Deriving and writing changes to changeset file: " << changesetOutput << "...");

  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), false);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), false);

  boost::shared_ptr<PartialOsmMapReader> referenceReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(referenceInput));
  referenceReader->open(referenceInput);
  LOG_DEBUG("Opened reference reader.");

  ChangesetDeriver changesetDeriver(
    boost::dynamic_pointer_cast<ElementInputStream>(referenceReader), filteredNewInputStream);
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
      _changesParsed++;
      const long numChanges = _changesByType[change.getType()];
      _changesByType[change.getType()] = numChanges + 1;

      if (_changesParsed % _logUpdateInterval == 0)
      {
        PROGRESS_INFO(
          "Ref: " <<
          FileUtils::formatPotentiallyLargeNumber(changesetDeriver.getNumFromElementsParsed()) <<
          ", New: " <<
          FileUtils::formatPotentiallyLargeNumber(changesetDeriver.getNumToElementsParsed()) <<
          ", Changes: " << FileUtils::formatPotentiallyLargeNumber(_changesParsed) <<
          " Cr: " << _changesByType[Change::Create] << ", Mod: " <<
          _changesByType[Change::Modify] << ", Del: " << _changesByType[Change::Delete]);
      }
    }
  }

  referenceReader->finalizePartial();
  changesetFileWriter->close();
  changesetDeriver.close();
  changesetTempFileWriter->close();

  LOG_VARD(changesetDeriver.getNumFromElementsParsed());
  LOG_VARD(changesetDeriver.getNumToElementsParsed());

  LOG_INFO(
    FileUtils::formatPotentiallyLargeNumber(_changesParsed) <<
    " changes derived and written to changeset file: " << changesetOutput << ".");
  LOG_INFO(
    "  Create statements: " <<
    FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Create]));
  LOG_INFO(
    "  Modify statements: " <<
    FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Modify]));
  LOG_INFO(
    "  Delete statements: " <<
    FileUtils::formatPotentiallyLargeNumber(_changesByType[Change::Delete]));
  LOG_INFO("Time elapsed: " << FileUtils::secondsToDhms(_timer.elapsed()));

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
  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->initializePartial();
  boost::shared_ptr<OsmChangeWriter> referenceChangeWriter =
    boost::dynamic_pointer_cast<OsmChangeWriter>(referenceWriter);
  referenceChangeWriter->open(referenceOutput);
  LOG_DEBUG("Opened change layer writer.");

  //this spark changeset reader will read in the element payload as xml
  //TODO: add an OsmChangeReaderFactory to get rid of the SparkChangesetReader dependency?
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
        "Wrote " << FileUtils::formatPotentiallyLargeNumber(changesWritten) <<
        " changes to ref layer.");
    }
  }
  assert(changesWritten == _changesParsed);

  referenceWriter->finalizePartial();
  referenceChangeWriter->close();
  changesetFileReader.close();

  LOG_INFO(
    FileUtils::formatPotentiallyLargeNumber(changesWritten) <<
    " changes written to reference layer: " << referenceOutput << ".");
  LOG_INFO("Time elapsed: " << FileUtils::secondsToDhms(_timer.elapsed()));
}

}
