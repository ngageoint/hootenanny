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

#include "OsmApiDbBulkWriterOffline.h"

#include <QDateTime>
#include <QFileInfo>
#include <QStringBuilder>

#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>

#include <tgs/System/SystemInfo.h>

namespace hoot
{

using namespace Tgs;

unsigned int OsmApiDbBulkWriterOffline::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmApiDbBulkWriterOffline)

OsmApiDbBulkWriterOffline::OsmApiDbBulkWriterOffline() :
OsmApiDbBulkWriter()
{
  _outputDelimiter = ',';
}

OsmApiDbBulkWriterOffline::~OsmApiDbBulkWriterOffline()
{
  close();
}

bool OsmApiDbBulkWriterOffline::isSupported(QString urlStr)
{
  LOG_VART(_mode);
  QUrl url(urlStr);
  return _database.isSupported(url) && _mode == "offline";
}

void OsmApiDbBulkWriterOffline::open(QString url)
{
  OsmApiDbBulkWriter::open(url);

  if (!_database.hasExtension("pg_bulkload"))
  {
    throw HootException(
      QString("When running OSM API database writing in 'offline' mode, the database being ") +
      QString("written to must have the pg_bulkload extension installed."));
  }
}

void OsmApiDbBulkWriterOffline::finalizePartial()
{
  //TODO: may be able to reuse the parent logic some here

  //go ahead and clear out some of the data structures we don't need anymore
  _idMappings.nodeIdMap.reset();
  _idMappings.wayIdMap.reset();
  _idMappings.relationIdMap.reset();
  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  if (_writeStats.nodesWritten == 0)
  {
    LOG_DEBUG("No data was written.");
    return;
  }

  LOG_INFO("Input records parsed stats:");
  _logStats();

  // Do we have an unfinished changeset that needs flushing?
  if (_changesetData.changesInChangeset > 0)
  {
    _writeChangesetToStream();
  }
  //If there was only one changeset written total, this won't have yet been incremented, so do it
  //now.
  if (_changesetData.changesetsWritten == 0)
  {
    _changesetData.changesetsWritten++;
  }

  //retain the csv output files if that option was selected
  if (!_outputFileCopyLocation.isEmpty())
  {
    _retainOutputFiles();
  }

  if (_executeSql)
  {
    _writeDataToDb();
    LOG_INFO("Final database write stats:");
  }
  else
  {
    LOG_INFO("Skipping writing records to database due to configuration...");
    LOG_INFO("Final CSV file write stats:");
  }
  _logStats();
}

void OsmApiDbBulkWriterOffline::_retainOutputFiles()
{
  QFileInfo baseFileInfo(_outputFileCopyLocation);
  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (_outputSections[*sectionNamesItr].first)
    {
      const QString outputPath =
        baseFileInfo.path() + "/" + baseFileInfo.baseName() + "-" + *sectionNamesItr + "." +
        baseFileInfo.completeSuffix();
      QFileInfo fileToCopyInfo((_outputSections[*sectionNamesItr].first)->fileName());
      LOG_INFO(
        "Copying " << SystemInfo::humanReadable(fileToCopyInfo.size()) << " CSV output file " <<
        "to " << outputPath << "...");
      if (!(_outputSections[*sectionNamesItr].first)->copy(outputPath))
      {
        LOG_WARN("Unable to copy CSV output file to " << outputPath);
      }
      else
      {
        LOG_DEBUG("Copied CSVL file output to " << outputPath);
      }
    }
  }
}

void OsmApiDbBulkWriterOffline::_writeDataToDb()
{
  LOG_INFO(
    "Writing CSV data for " << _formatPotentiallyLargeNumber(_getTotalRecordsWritten()) <<
    " records.  " << _outputSections.size() - 1 << " CSV files will be written to the database...");

  const QString outputLogPath = "/home/vagrant/pg_bulkload/bin/pg_bulkload_osmapidb.log";
  QFile outputLog(outputLogPath);
  if (outputLog.exists())
  {
    outputLog.remove();
  }

  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (*sectionNamesItr != "byte_order_mark" && _outputSections[*sectionNamesItr].first)
    {
      LOG_DEBUG("Closing temp file for " << *sectionNamesItr << "...");
      _outputSections[*sectionNamesItr].second.reset();
      _outputSections[*sectionNamesItr].first->close();

      const QMap<QString, QString> dbUrlParts = ApiDb::getDbUrlParts(_outputUrl);
      //TODO:
      // -this needs to work for hoot user
      // -need to remove vagrant home path
      // -don't hardcode log path
//      QString cmd =
//        "sudo -u postgres /home/vagrant/pg_bulkload/bin/pg_bulkload -d " + dbUrlParts["database"] +
//        " -O " + *sectionNamesItr + " -i " + (_outputSections[*sectionNamesItr]).first->fileName() +
//        " -l " + outputLogPath;
      QString cmd =
        "pg_bulkload -d " + dbUrlParts["database"] + " -h " + dbUrlParts["host"] + " -p " +
        dbUrlParts["port"] + " -U " + dbUrlParts["user"] + " -W " + dbUrlParts["password"] +
        " -O " + *sectionNamesItr + " -i " + (_outputSections[*sectionNamesItr]).first->fileName() +
        " -l " + outputLogPath;
      if (!_disableConstraints)
      {
        cmd += " -o \"CHECK_CONSTRAINTS=YES\"";
      }
      if (!_disableWriteAheadLogging)
      {
        cmd += " -o \"WRITER=BUFFERED\"";
      }
      if (_writeMultiThreaded)
      {
        cmd += " -o \"MULTI_PROCESS=YES\"";
      }
      if (!(Log::getInstance().getLevel() <= Log::Trace))
      {
        cmd += " -o \"VERBOSE=YES\"";
      }
      if (!(Log::getInstance().getLevel() <= Log::Info))
      {
        //TODO: re-enable
        //cmd += " > /dev/null";
      }
      LOG_DEBUG(cmd);

      LOG_INFO("Writing CSV data for " << *sectionNamesItr << "...");
      if (system(cmd.toStdString().c_str()) != 0)
      {
        throw HootException(
          "Failed executing record write for table " + *sectionNamesItr +
          " against the OSM API database: " + _outputUrl);
      }
      LOG_DEBUG("Wrote CSV data for " << *sectionNamesItr << ".");

      LOG_DEBUG("Removing temp file for " << *sectionNamesItr << "...");
      _outputSections[*sectionNamesItr].first->remove();
      _outputSections[*sectionNamesItr].first.reset();
    }
  }

  LOG_DEBUG("Record writing complete.");
}

QString OsmApiDbBulkWriterOffline::_getChangesetsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getChangesetsOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentNodesOutputFormatString() const
{
  //LOG_VARD(QString(OsmApiDbBulkWriter::outputDelimiter));
  //LOG_VARD(QString(_outputDelimiter));
  return
    OsmApiDbBulkWriter::_getCurrentNodesOutputFormatString().replace("\t", _outputDelimiter);
}

//TODO: need a cleaner way to swap this delimiter out

QString OsmApiDbBulkWriterOffline::_getHistoricalNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalNodesOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentWaysOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentWaysOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalWaysOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalWaysOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentWayNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentWayNodesOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalWayNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalWayNodesOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentRelationsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentRelationsOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalRelationsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalRelationsOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentRelationMembersOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentRelationMembersOutputFormatString()
      .replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalRelationMembersOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalRelationMembersOutputFormatString()
      .replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentTagsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentTagsOutputFormatString().replace("\t", _outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalTagsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalTagsOutputFormatString().replace("\t", _outputDelimiter);
}

}
