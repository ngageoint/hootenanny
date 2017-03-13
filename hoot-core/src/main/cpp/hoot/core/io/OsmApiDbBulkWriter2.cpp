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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiDbBulkWriter2.h"

#include <QDateTime>
#include <QFileInfo>
#include <QStringBuilder>

#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/OsmApiDbCsvTableFileSetWriter.h>

#include <tgs/System/SystemInfo.h>

namespace hoot
{

using namespace Tgs;

unsigned int OsmApiDbBulkWriter2::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbBulkWriter2)

OsmApiDbBulkWriter2::OsmApiDbBulkWriter2()
{
  setConfiguration(conf());
  _csvWriter.reset(new OsmApiDbCsvTableFileSetWriter());
}

OsmApiDbBulkWriter2::~OsmApiDbBulkWriter2()
{
  close();
}

bool OsmApiDbBulkWriter2::isSupported(QString urlStr)
{
  QUrl url(urlStr);
  return _database.isSupported(url) && (_disableWriteAheadLogging || _writeMultiThreaded);
}

void OsmApiDbBulkWriter2::open(QString url)
{
  if (_database.getDB().isOpen())
  {
    throw HootException(
      "Database already open.  Close the existing database connection before opening a new one.");
  }

  // Make sure we're not already open and the URL is valid
  if (!isSupported(url))
  {
    throw HootException(QString("Could not open URL ") + url);
  }

  _outputUrl = url;
  _database.open(_outputUrl);
  shared_ptr<QTemporaryFile> csvOutputBase(new QTemporaryFile());
  _csvWriter->open(csvOutputBase->fileName());

  //would rather put this message in the write method, but if both reader and writer implement the
  //streaming interface and neither implement the partial interface you'll never see this message,
  //due to how ElementOutputStream::writeAllElements works
  LOG_INFO("Streaming elements from input to temporary CSV file outputs.  Data pass #1 of 2...");
}

void OsmApiDbBulkWriter2::close()
{
  _csvWriter->close();
  _database.close();
}

//TODO: move this and the one in OsmApiDbBulkWriter to a StringUtils class?
QString OsmApiDbBulkWriter2::_formatPotentiallyLargeNumber(const long number)
{
  //I want to see comma separators...probably a better way to handle this...will go with this for
  //now.
  const QLocale& cLocale = QLocale::c();
  QString ss = cLocale.toString((qulonglong)number);
  ss.replace(cLocale.groupSeparator(), ',');
  return ss;
}

void OsmApiDbBulkWriter2::finalizePartial()
{
  _csvWriter->finalizePartial();

  //TODO: generate pgbulk ctl file

  //retain the csv output files if that option was selected
  if (!_outputFilesCopyBaseLocation.isEmpty())
  {
    //TODO:
    //_retainOutputFiles();
  }

  if (_executeSql)
  {
    _writeToDb();
    LOG_INFO("Final database write stats:");
  }
  else
  {
    LOG_INFO("Skipping SQL execution against database due to configuration...");
    LOG_INFO("Final CSV file write stats:");
  }

  _csvWriter->logStats(false);
}

void OsmApiDbBulkWriter2::_writeToDb()
{
  //I believe a COPY header is created whether there are any records to copy for the table or not,
  //which is why the number of copy statements to be executed is hardcoded here.  Might be cleaner
  //to not write the header if there are no records to copy for the table...s
  LOG_INFO(
    "Writing " << _formatPotentiallyLargeNumber(_csvWriter->getTotalRecordsWritten()) <<
    " records.  " << _csvWriter->getOutputFileNames().size() << " separate CSV files will " <<
    "be written to the database...");

  //write the records to the db using pg_bulkload
//  QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(_outputUrl);
//  QString cmd = "export PGPASSWORD=" + dbUrlParts["password"] + "; psql";
//  if (!(Log::getInstance().getLevel() <= Log::Info))
//  {
//    cmd += " --quiet";
//  }
//  cmd += " " + ApiDb::getPsqlString(_outputUrl) + " -f " + _sqlOutputMasterFile->fileName();
//  if (!(Log::getInstance().getLevel() <= Log::Info))
//  {
//    cmd += " > /dev/null";
//  }
//  LOG_TRACE(cmd);
//  if (system(cmd.toStdString().c_str()) != 0)
//  {
//    throw HootException("Failed executing bulk element SQL write against the OSM API database.");
//  }
//  LOG_DEBUG("Element SQL execution complete.");
}

void OsmApiDbBulkWriter2::writePartial(const ConstNodePtr& node)
{
  _csvWriter->writePartial(node);
}

void OsmApiDbBulkWriter2::writePartial(const ConstWayPtr& way)
{
  _csvWriter->writePartial(way);
}

void OsmApiDbBulkWriter2::writePartial(const ConstRelationPtr& relation)
{
  _csvWriter->writePartial(relation);
}

void OsmApiDbBulkWriter2::setConfiguration(const Settings& conf)
{
  const ConfigOptions confOptions(conf);
  //_changesetData.changesetUserId = confOptions.getChangesetUserId();
  setFileOutputLineBufferSize(confOptions.getOsmapidbBulkWriterFileOutputBufferMaxLineSize());
  setStatusUpdateInterval(confOptions.getOsmapidbBulkWriterFileOutputStatusUpdateInterval());
  setExecuteSql(confOptions.getOsmapidbBulkWriterExecuteSql());
  setMaxChangesetSize(confOptions.getChangesetMaxSize());
  setDisableWriteAheadLogging(confOptions.getOsmapidbBulkWriterBypassWriteAheadLogging());
  setWriteMultithreaded(confOptions.getOsmapidbBulkWriterMultithreaded());
  setOutputFilesCopyBaseLocation(
    confOptions.getOsmapidbBulkWriterOutputFileCopyLocation().trimmed());
}

}
