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

#include "OsmCsvWriter.h"

#include <QDateTime>
#include <QFileInfo>
#include <QStringBuilder>

#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/io/OsmApiDb.h>

#include <tgs/System/SystemInfo.h>

namespace hoot
{

using namespace Tgs;

unsigned int OsmCsvWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmCsvWriter)

OsmCsvWriter::OsmCsvWriter() :
_unflushedRecords(0)
{
  setConfiguration(conf());
}

OsmCsvWriter::~OsmCsvWriter()
{
  close();
}

bool OsmCsvWriter::isSupported(QString urlStr)
{
  return urlStr.toLower().endsWith(".csv");
}

void OsmCsvWriter::open(QString url)
{
  _init(url);
}

void OsmCsvWriter::_init(const QString outputUrl)
{
  QStringList outputFileTypes;
  //TODO: deal with this
  //outputFileTypes.push_back(ApiDb::getChangesetsTableName());
  outputFileTypes.push_back(ApiDb::getCurrentNodesTableName());
  outputFileTypes.push_back(ApiDb::getCurrentNodeTagsTableName());
  outputFileTypes.push_back(ApiDb::getNodesTableName());
  outputFileTypes.push_back(ApiDb::getNodeTagsTableName());
  outputFileTypes.push_back(ApiDb::getCurrentWaysTableName());
  outputFileTypes.push_back(ApiDb::getCurrentWayNodesTableName());
  outputFileTypes.push_back(ApiDb::getCurrentWayTagsTableName());
  outputFileTypes.push_back(ApiDb::getWaysTableName());
  outputFileTypes.push_back(ApiDb::getWayNodesTableName());
  outputFileTypes.push_back(ApiDb::getWayTagsTableName());
  outputFileTypes.push_back(ApiDb::getCurrentRelationsTableName());
  outputFileTypes.push_back(ApiDb::getCurrentRelationMembersTableName());
  outputFileTypes.push_back(ApiDb::getCurrentRelationTagsTableName());
  outputFileTypes.push_back(ApiDb::getRelationsTableName());
  outputFileTypes.push_back(ApiDb::getRelationMembersTableName());
  outputFileTypes.push_back(ApiDb::getRelationTagsTableName());

  for (int i = 0 ; i < outputFileTypes; i++)
  {
    QFileInfo baseFileInfo(outputUrl);
    shared_ptr<QFile> outputFile(
      new QFile(baseFileInfo.path() + "/" + baseFileInfo.baseName() + "-" +
                outputFileTypes.at(i) + "." + baseFileInfo.completeSuffix()));
    LOG_VARD(outputFile->fileName());
    if (outputFile->exists())
    {
      outputFile->remove();
    }
    outputFile->open(QIODevice::Append);
    _outputFiles.insert(outputFileTypes.at(i), outputFile);

    shared_ptr<QTextStream> outStream(outputFile.get());
    _outputStreams.insert(outputFileTypes.at(i), outStream);

    _numRecordsWritten[outputFileTypes.at(i)] = 0;
  }
}

void OsmCsvWriter::close()
{
  for (QMap<QString, shared_ptr<QFile> >::iterator outputFileItr = _outputFiles.begin();
       outputFileItr != _outputFiles.end(); outputFileItr++)
  {
    bool closed = false;
    if (*itr.get())
    {
      if (*itr->close())
      {
        closed = true;
        QFileInfo info(*_outputFiles[*itr.get());
        LOG_VAR(info.size());
      }
    }
    if (!closed)
    {
      LOG_WARN("Unable to close file for table: " << itr.key());
    }
  }
}

void OsmCsvWriter::finalizePartial()
{
  for (QMap<QString, shared_ptr<QTextStream> >::iterator streamItr = _outputStreams.begin();
       streamItr != _outputStreams.end(); streamItr++)
  {
    if (*itr.get())
    {
      *itr->flush();
    }
    else
    {
      throw HootException("Unable to flush output stream for table: " << itr.key());
    }
  }

  for (QMap<QString, long>::const_iterator countItr = _numRecordsWritten.begin();
       countItr != _numRecordsWritten.end(); countItr++)
  {
    LOG_INFO(*itr << " total records written for " << itr.key());
  }
}

void OsmCsvWriter::writePartial(const ConstNodePtr& node)
{
  shared_ptr<QTextStream> currentNodesOutStream =
    _outputStreams[ApiDb::getCurrentNodesTableName()];
  *currentNodesOutStream << QString::number(node->getId()) << ",";
  *currentNodesOutStream << QString::number(OsmApiDb::toOsmApiDbCoord(node->getY())) << ",";
  *currentNodesOutStream << QString::number(OsmApiDb::toOsmApiDbCoord(node->getX())) << ",";
  *currentNodesOutStream << QString::number(1) << ",";
  *currentNodesOutStream << "TRUE" << ",";
  *currentNodesOutStream <<
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz") << ",";
  *currentNodesOutStream << QString::number(ApiDb::tileForPoint(node->getY(), node->getX())) <<
    ",";
  *currentNodesOutStream << QString::number(1) << "\n";
  _unflushedRecords++;
  _numRecordsWritten[ApiDb::getCurrentNodesTableName()]++;

  shared_ptr<QTextStream> currentTagsOutStream =
    _outputStreams[ApiDb::getCurrentNodeTagsTableName()];

  _unflushedRecords++;
  _numRecordsWritten[ApiDb::getCurrentNodesTableName()]++;

  shared_ptr<QTextStream> historicalNodesOutStream = _outputStreams[ApiDb::getNodesTableName()];

  _unflushedRecords++;
  _numRecordsWritten[ApiDb::getNodesTableName()]++;

  shared_ptr<QTextStream> historicalTagsOutStream =
    _outputStreams[ApiDb::getNodeTagsTableName()];

  _unflushedRecords++;
  _numRecordsWritten[ApiDb::getNodeTagsTableName()]++;

  if (_unflushedRecords % _fileOutputLineBufferSize == 0)
  {
    currentNodesOutStream->flush();
    currentTagsOutStream->flush();
    historicalNodesOutStream->flush();
    historicalTagsOutStream->flush();
    _unflushedRecords = 0;
  }

  if (_numRecordsWritten[ApiDb::getCurrentNodesTableName()] % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO(_numElementsWritten[ElementType::Node] << " nodes written.");
  }
}

void OsmCsvWriter::writePartial(const ConstWayPtr& /*way*/)
{
  //throw NotImplementedException();
  return;
}

void OsmCsvWriter::writePartial(const ConstRelationPtr& /*relation*/)
{
  //throw NotImplementedException();
  return;
}

void OsmCsvWriter::setConfiguration(const hoot::Settings& conf)
{
  const ConfigOptions confOptions(conf);
  setFileOutputLineBufferSize(confOptions.getOsmapidbBulkWriterFileOutputBufferMaxLineSize());
  setStatusUpdateInterval(confOptions.getOsmapidbBulkWriterFileOutputStatusUpdateInterval());
}

}
