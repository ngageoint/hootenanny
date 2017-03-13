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
    _writeChangesetToTable();
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

  //TODO: finish
  QString cmd = "";
  LOG_DEBUG(cmd);
  if (system(cmd.toStdString().c_str()) != 0)
  {
    throw HootException("Failed executing bulk element SQL write against the OSM API database.");
  }
  LOG_DEBUG("Element SQL execution complete.");
}

}
