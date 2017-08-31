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
#include <hoot/core/io/OsmPbfReader.h>
#include <hoot/core/io/OsmXmlReader.h>
#include <hoot/core/io/OgrReader.h>
#include <hoot/core/io/ElementStreamer.h>
#include <hoot/core/io/OsmChangeWriterFactory.h>
#include <hoot/core/io/PartialOsmMapWriter.h>

// Qt
#include <QFileInfo>

namespace hoot
{

MultiaryIngester::MultiaryIngester() :
_sortInput(true),
_changesParsed(0),
_logUpdateInterval(ConfigOptions().getOsmapidbBulkInserterFileOutputStatusUpdateInterval())
{
  //inputs must be sorted by element id for changeset derivation to work
  conf().set(ConfigOptions::getApiDbReaderSortByIdKey(), true);
  //in order for the sorting to work, all original element ids must be retained...no new ones
  //assigned; we're assuming no duplicate ids in the input
  conf().set(ConfigOptions::getHootapiDbWriterRemapIdsKey(), false);
  //translate inputs to OSM
  conf().set(ConfigOptions::getTranslationScriptKey(), "translations/OSM_Ingest.js");
  //for the changeset derivation to work, all input IDs must not be modified
  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
}

void MultiaryIngester::ingest(const QString newInput, const QString referenceOutput,
                              const QString changesetOutput, const bool sortInput)
{
  LOG_VARD(newInput);
  LOG_VARD(referenceOutput);
  LOG_VARD(changesetOutput);
  LOG_VARD(sortInput);

  _changesParsed = 0;
  _sortInput = sortInput;

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
      QString("Multiary ingest only supports a hootapidb:// data source as the reference ") +
      QString("output.  Specified reference layer: ") + referenceOutput);
  }

  if (!SparkChangesetWriter().isSupported(changesetOutput))
  {
    throw IllegalArgumentException(
      QString("Multiary ingest only supports a .spark.x file for changeset output.  Specified ") +
      QString("changeset output: ") + changesetOutput);
  }

  LOG_INFO(
    "Ingesting Multiary data from " << newInput << " into reference output layer: " <<
    referenceOutput << " and writing changes to changeset file: " << changesetOutput << "...");

  HootApiDb referenceDb;
  referenceDb.open(referenceOutput);
  const QStringList dbUrlParts = referenceOutput.split("/");
  if (!referenceDb.mapExists(dbUrlParts[dbUrlParts.size() - 1]))
  {
    LOG_INFO(
      "The reference output dataset does not exist.  Skipping node sorting and changeset " <<
      "derivation.  Writing the input data directly to the reference layer and generating " <<
      "a changeset file consisting entirely of create statements...");

    //If there's no existing reference data, then there's no point in deriving a changeset diff
    //or sorting the data by ID.  So in that case, write all of the input data directly to the ref
    //layer and generate a Spark changeset consisting entirely of create statements.
    _writeChanges(_getFilteredNewInputStream(newInput), referenceOutput, changesetOutput);
  }
  else
  {
    LOG_INFO(
      "The reference output dataset exists.  Deriving a changeset between the input and " <<
      "reference data, writing the changes to the reference layer, and also writing " <<
      "the changes to the changeset file...");
    if (!_sortInput)
    {
      LOG_INFO("The input data will not be sorted by node ID.");
    }

    //sort incoming data by element id, if necessary, for changeset derivation (only passing nodes
    //through, so don't need to also sort by element type)
    _sortedNewInput = _getSortedNewInput(newInput);

    //create the changes and write them to the ref db layer and also to a changeset file for
    //external use by Spark
    _deriveAndWriteChanges(
      _getFilteredNewInputStream(_sortedNewInput), referenceOutput, changesetOutput);
  }

  LOG_INFO(
    "Multiary data ingested from " << newInput << " into reference layer: " <<
    referenceOutput << " and changes written to changeset file: " << changesetOutput <<
    ".  Time: " << FileUtils::secondsToDhms(_timer.elapsed()));
  LOG_INFO("Total Changes Parsed: " << FileUtils::formatPotentiallyLargeNumber(_changesParsed));
}

void MultiaryIngester::_checkForOsmosis() const
{
  if (std::system(QString("osmosis -q > /dev/null").toStdString().c_str()) != 0)
  {
    throw HootException(
      QString("Unable to access the Osmosis application.  Osmosis is required to") +
      QString("sort OSM PBF and OSM XML files.  Is Osmosis installed?"));
  }
}

void MultiaryIngester::_sortPbf(const QString input, const QString output)
{
  _checkForOsmosis();

  const QString cmd =
    "osmosis -q --read-pbf file=\"" + input + "\" --sort --write-pbf " +
    "omitmetadata=true file=\"" + output + "\" > /dev/null";
  if (std::system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Unable to sort OSM PBF file.");
  }
}

boost::shared_ptr<QTemporaryFile> MultiaryIngester::_ogrToPbfTemp(const QString input)
{
  boost::shared_ptr<QTemporaryFile> pbfTemp(
    new QTemporaryFile("multiary-ingest-sort-temp-XXXXXX.osm.pbf"));
  if (!pbfTemp->open())
  {
    throw HootException("Unable to open sort temp file: " + pbfTemp->fileName() + ".");
  }
  LOG_INFO(
    "Converting OGR input: " << input << " to sortable PBF output: " << pbfTemp->fileName() <<
    "...");

  ElementStreamer::stream(input, pbfTemp->fileName());

  return pbfTemp;
}

QString MultiaryIngester::_getSortedNewInput(const QString newInput)
{
  if (!_sortInput)
  {
    return newInput;
  }

  LOG_INFO("Sorting " << newInput << " by node ID...");
  _timer.restart();

  const QString sortTempFileBaseName = "multiary-ingest-sort-temp-XXXXXX";
  if (!OgrReader().isSupported(newInput))
  {
    QFileInfo newInputFileInfo(newInput);
    _sortTempFile.reset(
      new QTemporaryFile(sortTempFileBaseName + "." + newInputFileInfo.completeSuffix()));
  }
  else
  {
    //OGR formats have to be converted to PBF for sorting (see below)
    _sortTempFile.reset(new QTemporaryFile(sortTempFileBaseName + ".osm.pbf"));
  }
  if (!_sortTempFile->open())
  {
    throw HootException("Unable to open sort temp file: " + _sortTempFile->fileName() + ".");
  }

  if (GeoNamesReader().isSupported(newInput))
  {
    //sort the input by node id (first field) using the unix sort command
    if (std::system(
         QString("sort " + newInput + " --output=" +
           _sortTempFile->fileName()).toStdString().c_str()) != 0)
    {
      throw HootException("Unable to sort input file.");
    }
  }
  //use osmosis to sort the OSM files by node id
  else if (OsmPbfReader().isSupported(newInput))
  {
    //check for sorted flag
    if (OsmPbfReader().isSorted(newInput))
    {
      LOG_WARN(
        "OSM PBF input file: " << newInput << " is marked as sorted by node ID, as " <<
        "indicated by its header, yet Hootenanny was instructed to sort the file.");
    }
    _sortPbf(newInput, _sortTempFile->fileName());
  }
  else if (OsmXmlReader().isSupported(newInput))
  {
    _checkForOsmosis();

    const QString cmd =
      "osmosis -q --read-xml file=\"" + newInput + "\" --sort --write-xml file=\"" +
      _sortTempFile->fileName() + "\"  > /dev/null";
    if (std::system(cmd.toStdString().c_str()) != 0)
    {
      throw HootException("Unable to sort OSM XML file.");
    }
  }
  else if (OgrReader().isSupported(newInput))
  {
    //Unfortunately for now, sorting an OGR input is going to require an extra pass over the data
    //to first write it to a sortable format.
    LOG_WARN(
      "OGR inputs are not currently sortable by node ID.  Must convert input to OSM format, " <<
      "which will increase the processing time.");
    _sortPbf(_ogrToPbfTemp(newInput)->fileName(), _sortTempFile->fileName());
  }
  else
  {
    throw HootException("Unsupported input format for node sorting.");
  }

  LOG_INFO(newInput << " sorted by node ID.  Time: " << FileUtils::secondsToDhms(_timer.elapsed()));

  return _sortTempFile->fileName();
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

  //as the incoming data is read, filter it down to POIs only and translate each element;

  boost::shared_ptr<PoiCriterion> elementCriterion;
  //all geonames are pois, so skip the element filtering expense for that format
  if (!GeoNamesReader().isSupported(sortedNewInput))
  {
    elementCriterion.reset(new PoiCriterion());
  }
  else
  {
    LOG_INFO("Skipping POI filtering for: " << sortedNewInput << ".");
  }
  QList<ElementVisitorPtr> visitors;
  visitors.append(boost::shared_ptr<TranslationVisitor>(new TranslationVisitor()));
  visitors.append(boost::shared_ptr<CalculateHashVisitor2>(new CalculateHashVisitor2()));
  boost::shared_ptr<ElementInputStream> filteredNewInputStream(
    new ElementCriterionVisitorInputStream(inputStream, elementCriterion, visitors));
  return filteredNewInputStream;
}

void MultiaryIngester::_writeChanges(boost::shared_ptr<ElementInputStream> filteredNewInputStream,
                                     const QString referenceOutput, const QString changesetOutput)
{
  _timer.restart();
  LOG_DEBUG("Writing changes...");

  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), true);

  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->open(referenceOutput);

  boost::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput);
  changesetFileWriter->open(changesetOutput);

  while (filteredNewInputStream->hasMoreElements())
  {
    ElementPtr element = filteredNewInputStream->readNextElement();
    if (element.get())
    {
      referenceWriter->writeElement(element);
      changesetFileWriter->writeChange(Change(Change::Create, element));
      _changesParsed++;

      if (_changesParsed % _logUpdateInterval == 0)
      {
        PROGRESS_INFO(
          "Parsed " << FileUtils::formatPotentiallyLargeNumber(_changesParsed) << " changes.");
      }
    }
  }

  referenceWriter->finalizePartial();
  changesetFileWriter->close();
}

void MultiaryIngester::_deriveAndWriteChanges(
  boost::shared_ptr<ElementInputStream> filteredNewInputStream, const QString referenceOutput,
  const QString changesetOutput)
{
  _timer.restart();
  LOG_DEBUG("Deriving and writing changes...");

  conf().set(ConfigOptions::getReaderUseDataSourceIdsKey(), true);
  conf().set(ConfigOptions::getHootapiDbWriterCreateUserKey(), false);
  conf().set(ConfigOptions::getHootapiDbWriterOverwriteMapKey(), false);

  boost::shared_ptr<PartialOsmMapReader> referenceReader =
    boost::dynamic_pointer_cast<PartialOsmMapReader>(
      OsmMapReaderFactory::getInstance().createReader(referenceOutput));
  referenceReader->open(referenceOutput);
  LOG_DEBUG("Opened reader.");

  ChangesetDeriverPtr changesetDeriver(
    new ChangesetDeriver(
      boost::dynamic_pointer_cast<ElementInputStream>(referenceReader), filteredNewInputStream));
  LOG_DEBUG("Initialized changeset deriver.");

  //would cast straight to OsmChangeWriterFactory here, but haven't figured out a way around
  //the fact that you can't use the factory macro twice on the same class.  Since HootApiDbWriter
  //already has the macro for OsmMapWriter, it can't be added for OsmChangeWriter.
  boost::shared_ptr<PartialOsmMapWriter> referenceWriter =
    boost::dynamic_pointer_cast<PartialOsmMapWriter>(
      OsmMapWriterFactory::getInstance().createWriter(referenceOutput));
  referenceWriter->initializePartial();
  boost::shared_ptr<OsmChangeWriter> referenceChangeWriter =
    boost::dynamic_pointer_cast<OsmChangeWriter>(referenceWriter);
  referenceChangeWriter->open(referenceOutput);
  LOG_DEBUG("Opened change layer writer.");

  boost::shared_ptr<OsmChangeWriter> changesetFileWriter =
    OsmChangeWriterFactory::getInstance().createWriter(changesetOutput);
  changesetFileWriter->open(changesetOutput);
  LOG_DEBUG("Opened change file writer.");

  while (changesetDeriver->hasMoreChanges())
  {
    const Change change = changesetDeriver->readNextChange();
    if (change.getType() != Change::Unknown)
    {
      changesetFileWriter->writeChange(change);
      referenceChangeWriter->writeChange(change);
      _changesParsed++;

      if (_changesParsed % _logUpdateInterval == 0)
      {
        PROGRESS_INFO(
          "Parsed " << FileUtils::formatPotentiallyLargeNumber(_changesParsed) << " changes.");
      }
    }
  }

  referenceReader->finalizePartial();
  referenceWriter->finalizePartial();
  changesetFileWriter->close();
}

}
