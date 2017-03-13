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
  _offline = true;
  outputDelimiter = ',';
}

OsmApiDbBulkWriterOffline::~OsmApiDbBulkWriterOffline()
{
  close();
}

bool OsmApiDbBulkWriterOffline::isSupported(QString urlStr)
{
  return OsmApiDbBulkWriter::isSupported(urlStr) &&
         (_disableWriteAheadLogging || _writeMultiThreaded);
}

void OsmApiDbBulkWriterOffline::finalizePartial()
{
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

  //retain the sql output file if that option was selected
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
    LOG_INFO("Skipping SQL execution against database due to configuration...");
    LOG_INFO("Final SQL file write stats:");
  }
  _logStats();
}

void OsmApiDbBulkWriterOffline::_retainOutputFiles()
{
  QFileInfo baseFileInfo(_outputFileCopyLocation);
  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (_outputSections[*sectionNamesItr].first.get())
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
    " records.  " << _outputSections.size() - 1 << " CSV files will be written...");

  const QString outputLogPath = "/home/vagrant/pg_bulkload/bin/pg_bulkload_osmapidb.log";
  QFile outputLog(outputLogPath);
  if (outputLog.exists())
  {
    outputLog.remove();
  }

  for (QStringList::const_iterator sectionNamesItr = _sectionNames.begin();
       sectionNamesItr != _sectionNames.end(); sectionNamesItr++)
  {
    if (*sectionNamesItr != "byte_order_mark" && _outputSections[*sectionNamesItr].first.get())
    {
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
      //TODO: option for check constraints?
      if (!_disableWriteAheadLogging)
      {
        cmd += " -o \"WRITER=BUFFERED\"";
      }
      if (!_writeMultiThreaded)
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
    }
  }

  LOG_DEBUG("Record writing complete.");
}

QString OsmApiDbBulkWriterOffline::_getChangesetsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getChangesetsOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentNodesOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalNodesOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentWaysOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentWaysOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalWaysOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalWaysOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentWayNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentWayNodesOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalWayNodesOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalWayNodesOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentRelationsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentRelationsOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalRelationsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalRelationsOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentRelationMembersOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentRelationMembersOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalRelationMembersOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalRelationMembersOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getCurrentTagsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getCurrentTagsOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

QString OsmApiDbBulkWriterOffline::_getHistoricalTagsOutputFormatString() const
{
  return
    OsmApiDbBulkWriter::_getHistoricalTagsOutputFormatString()
      .replace(OsmApiDbBulkWriterOffline::outputDelimiter, outputDelimiter);
}

}
