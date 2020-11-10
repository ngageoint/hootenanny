/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "WriteOsmSqlStatementsReducer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/DbUtils.h>
#include <hoot/core/util/UuidHelper.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
using namespace pp;

// Qt
#include <QMap>
#include <QStringBuilder>

using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WriteOsmSqlStatementsReducer)

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer() :
_sqlStatementBufferSize(0),
_retainSqlFile(false),
_localJobTracker(false)
{
  _context = NULL;
  _pqConn = NULL;

  //We need the element counts so that the driver can write the setval statements after the job is
  //finished.  I wanted to track the element counts with hadoop counters, but pipes won't
  //let you retrieve counter values, so doing it this way instead.
  _elementCounts["nodes"] = 0;
  _elementCounts["ways"] = 0;
  _elementCounts["relations"] = 0;
}

WriteOsmSqlStatementsReducer::~WriteOsmSqlStatementsReducer()
{
  if (!_dbConnStr.isEmpty() && _pqConn != NULL)
  {
    PQfinish(_pqConn);
    _pqConn = NULL;
  }
}

void WriteOsmSqlStatementsReducer::close()
{
  if (_sqlStatementBufferSize > 0)
  {
    _flush();
  }
}

void WriteOsmSqlStatementsReducer::_flush()
{
  assert(_sqlStatementBufferSize > 0);
  //max number of possible header keys = number of db table types
  assert(_sqlStatementBuffer.size() <= 17);
  LOG_VART(_sqlStatementBufferSize);
  LOG_VART(_sqlStatementBuffer.size());
  LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records...");

  for (QMap<QString, QString>::const_iterator it = _sqlStatementBuffer.constBegin();
       it != _sqlStatementBuffer.constEnd(); ++it)
  {
    const QString tableHeader = it.key();
    LOG_VART(tableHeader);
    const QString data = it.value();
    LOG_VART(data);

    //Even though if our target is a database and we're not actually going to execute a sql file,
    //we'll write the file out here if requested.
    if (_retainSqlFile)
    {
      LOG_TRACE("Flushing records to file for table " << tableHeader << "...");
      LOG_VART(tableHeader);
      const QString fileSqlStatements = data + "\\.\n";
      LOG_VART(fileSqlStatements);
      _context->emit(tableHeader.toStdString(), fileSqlStatements.toStdString());
    }

    if (!_dbConnStr.isEmpty())
    {
      _flushToDb(tableHeader, data);
    }

    if (!_localJobTracker)
    {
      _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "tables"), 1);
    }
  }

  //write the element counts to a separate auxiliary file
  _writeElementCounts();

  _sqlStatementBuffer.clear();
  _sqlStatementBufferSize = 0;
}

QString WriteOsmSqlStatementsReducer::_tableHeaderToTableName(const QString tableHeader) const
{
  return tableHeader.split(" ")[1];
}

void WriteOsmSqlStatementsReducer::_flushToDb(const QString tableHeader, const QString tableData)
{
  LOG_TRACE(
    "Flushing records to database for table: " << _tableHeaderToTableName(tableHeader) << "...");

  assert(!_dbConnStr.isEmpty());

  //There's no way to stream a table copy with QSqlQuery, so we're using libpq here instead.

  if (_pqConn == NULL)
  {
    _pqConn = PQconnectdb(ApiDb::getPqString(_dbConnStr).toLatin1().data());
    //PQsetClientEncoding(_pqConn, "UTF8");
    LOG_TRACE(pg_encoding_to_char(PQclientEncoding(_pqConn)));
    PQsetErrorVerbosity(_pqConn, PQERRORS_VERBOSE);

    LOG_DEBUG("Postgres database version: " << PQserverVersion(_pqConn));
  }
  else if (PQstatus(_pqConn) != CONNECTION_OK)
  {
    PQreset(_pqConn);
    LOG_TRACE("Reset pq connection.");
  }
  if (PQstatus(_pqConn) != CONNECTION_OK)
  {
    throw HootException("Unable to open pq database connection.");
  }
  if (PQisnonblocking(_pqConn))
  {
    throw HootException("Non-blocking connection.");
  }

  PGresult* tableHeaderCopyResult = NULL;
  PGresult* statementsCopyResult = NULL;
  try
  {
    //write the header
    tableHeaderCopyResult = PQexec(_pqConn, tableHeader.toUtf8().data());

    //write the data; We have to get rid of the UTF null here, or postgres will choke on it.
    QByteArray sqlStatementsBytes = tableData.toUtf8();
    sqlStatementsBytes.replace("\\x00", " ");
    const int size = sqlStatementsBytes.size();

    if (PQresultStatus(tableHeaderCopyResult) != PGRES_COPY_IN)
    {
      throw HootException("Error writing copy header to database.");
    }
    else
    {
      if (PQputCopyData(_pqConn, sqlStatementsBytes.data(), size) != 1 ||
          PQputCopyEnd(_pqConn, NULL) != 1)
      {
        throw HootException(QString::fromAscii(PQerrorMessage(_pqConn)));
      }

      statementsCopyResult = PQgetResult(_pqConn);
      if (PQresultStatus(statementsCopyResult) != PGRES_COMMAND_OK)
      {
        throw HootException(
          QString("Error writing data: ") + QString::fromAscii(PQerrorMessage(_pqConn)));
      }
      else
      {
        _context->incrementCounter(
          _context->getCounter("WriteOsmSqlStatements", "table copies executed"), 1);
      }
    }
  }
  catch (const std::exception& e)
  {
    LOG_ERROR(e.what());
    if (tableHeaderCopyResult != NULL)
    {
      PQclear(tableHeaderCopyResult);
    }
    if (statementsCopyResult != NULL)
    {
      PQclear(statementsCopyResult);
    }
    throw e;
  }
}

void WriteOsmSqlStatementsReducer::_updateElementCounts(const QString tableHeader)
{
  if (tableHeader.contains("current_nodes"))
  {
    _elementCounts["nodes"] = _elementCounts["nodes"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "nodes"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (tableHeader.contains("current_node_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "node tags"), 1);
  }
  else if (tableHeader.contains("current_ways"))
  {
    _elementCounts["ways"] = _elementCounts["ways"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "ways"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (tableHeader.contains("current_way_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "way tags"), 1);
  }
  else if (tableHeader.contains("current_way_nodes"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "way nodes"), 1);
  }
  else if (tableHeader.contains("current_relations"))
  {
    _elementCounts["relations"] = _elementCounts["relations"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relations"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (tableHeader.contains("current_relation_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relation tags"), 1);
  }
  else if (tableHeader.contains("current_relation_members"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relation members"), 1);
  }
}

void WriteOsmSqlStatementsReducer::_writeElementCounts()
{
  boost::shared_ptr<Configuration> config(HadoopPipesUtils::toConfiguration(_context->getJobConf()));
  //if it doesn't already exist, add a dir under our main output dir to store the aux element count
  //files in
  Hdfs fs;
  const string mainOutputDir = config->get("mapred.output.dir");
  const string auxDir = mainOutputDir + "/elementCounts";
  if (!fs.exists(auxDir))
  {
    fs.mkdirs(auxDir);
  }
  const string outputFile =
    auxDir + "/elementCount-" + UuidHelper::createUuid().toString().toStdString();
  boost::shared_ptr<ostream> out(fs.create(outputFile));
  if (!out->good())
  {
    throw Exception("Output stream is not good.");
  }

  //write out the element counts for this reduce task to be manually summed up by the driver
  LOG_VART(_elementCounts["nodes"]);
  const QString nodeCntStr = "nodes;" + QString::number(_elementCounts["nodes"]) + "\n";
  out->write(nodeCntStr.toLatin1().data(), nodeCntStr.toLatin1().size());
  LOG_VART(_elementCounts["ways"]);
  const QString wayCntStr = "ways;" + QString::number(_elementCounts["ways"]) + "\n";
  out->write(wayCntStr.toLatin1().data(), wayCntStr.toLatin1().size());
  LOG_VART(_elementCounts["relations"]);
  const QString relationCntStr = "relations;" + QString::number(_elementCounts["relations"]) + "\n";
  out->write(relationCntStr.toLatin1().data(), relationCntStr.toLatin1().size());

  _elementCounts["nodes"] = 0;
  _elementCounts["ways"] = 0;
  _elementCounts["relations"] = 0;
}

void WriteOsmSqlStatementsReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_context == NULL)
  {
    _context = &context;
  }

  boost::shared_ptr<Configuration> config(HadoopPipesUtils::toConfiguration(_context->getJobConf()));
  //LOG_VARD(config->getInt("mapred.reduce.tasks"));
  _localJobTracker = config->get("mapred.job.tracker") == "local";
  const long writeBufferSize = config->getLong("writeBufferSize");
  LOG_VART(writeBufferSize);
  if (config->hasKey("dbConnUrl"))
  {
    _dbConnStr = QString::fromStdString(config->get("dbConnUrl"));
    LOG_VART(_dbConnStr);
  }
  _retainSqlFile = config->get("retainSqlFile") == "1" ? true : false;
  LOG_VART(_retainSqlFile);

  //key syntax: <element id>;<table header> OR <element id>;<member id>;<table header>
  const QStringList keyParts = QString::fromStdString(context.getInputKey()).split(";");
  const QString tableHeader = keyParts[keyParts.size() - 2] + ";\n";
  LOG_VART(tableHeader);
  while (context.nextValue())
  {
    _updateElementCounts(tableHeader);

    const QString newValue = QString::fromStdString(context.getInputValue());
    LOG_VART(newValue);
    _sqlStatementBuffer[tableHeader] = _sqlStatementBuffer[tableHeader] % newValue;
    LOG_VART(_sqlStatementBuffer[tableHeader]);
    _sqlStatementBufferSize++;

    if (!_localJobTracker)
    {
      _context->incrementCounter(
        _context->getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }

    //this flush will cause the same table to be written to the file twice, each time with a
    //different set of sql records...but that's ok...SQL is still valid
    if (_sqlStatementBufferSize >= writeBufferSize)
    {
      _flush();
    }
  }
}

}
