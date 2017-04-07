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

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
using namespace pp;

// Qt
#include <QMap>
#include <QStringBuilder>

using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WriteOsmSqlStatementsReducer)

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer() :
_tableHeader(""),
_sqlStatements(""),
_sqlStatementBufferSize(0),
_retainSqlFile(false),
_localJobTracker(false)
{
  _context = NULL;
  _pqConn = NULL;
}

WriteOsmSqlStatementsReducer::~WriteOsmSqlStatementsReducer()
{
  if (!_dbConnStr.isEmpty())
  {
    if (_pqConn != NULL)
    {
      PQfinish(_pqConn);
      _pqConn = NULL;
    }
  }
}

void WriteOsmSqlStatementsReducer::_flush()
{
  assert(_sqlStatementBufferSize > 0);
  assert(!_sqlStatements.isEmpty());

  //_sqlStatements += "\\.\n";

  //Even though if our target is a database and we're not actually going to execute a sql file,
  //we'll write the file out here if requested.
  if (_retainSqlFile)
  {
    LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to disk...");
    LOG_VART(_tableHeader);
    const QString fileSqlStatements = _sqlStatements + "\\.\n";
    LOG_VART(fileSqlStatements);
    _context->emit(_tableHeader.toStdString(), /*_sqlStatements*/fileSqlStatements.toStdString());
  }

  if (!_dbConnStr.isEmpty())
  {
    _flushToDb();
  }

  if (!_localJobTracker)
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "tables"), 1);
  }

  _sqlStatements = "";
  _sqlStatementBufferSize = 0;
}

void WriteOsmSqlStatementsReducer::_flushToDb()
{
  LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to database...");

  assert(!_dbConnStr.isEmpty());

  //There's no way to stream a table copy with QSqlQuery, so we're using libpq here instead.

  if (_pqConn == NULL)
  {
    _pqConn = PQconnectdb(ApiDb::getPqString(_dbConnStr)./*toUtf8()*/toLatin1().data());
    //PQsetClientEncoding(_pqConn, "UTF8");
    LOG_TRACE(pg_encoding_to_char(PQclientEncoding(_pqConn)));
    PQsetErrorVerbosity(_pqConn, PQERRORS_VERBOSE);
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
    tableHeaderCopyResult = PQexec(_pqConn, _tableHeader.toUtf8().data());

    //write the data; We have to get rid of the UTF null here, or postgres will choke.
    QByteArray sqlStatementsBytes = _sqlStatements.toUtf8();
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

void WriteOsmSqlStatementsReducer::_updateElementCounts()
{
  if (_tableHeader.contains("current_nodes"))
  {
    _elementCounts["nodes"] = _elementCounts["nodes"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "nodes"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (_tableHeader.contains("current_node_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "node tags"), 1);
  }
  else if (_tableHeader.contains("current_ways"))
  {
    _elementCounts["ways"] = _elementCounts["ways"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "ways"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (_tableHeader.contains("current_way_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "way tags"), 1);
  }
  else if (_tableHeader.contains("current_way_nodes"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "way nodes"), 1);
  }
  else if (_tableHeader.contains("current_relations"))
  {
    _elementCounts["relations"] = _elementCounts["relations"] + 1;
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relations"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
  }
  else if (_tableHeader.contains("current_relation_tags"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relation tags"), 1);
  }
  else if (_tableHeader.contains("current_relation_members"))
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "relation members"), 1);
  }
}

void WriteOsmSqlStatementsReducer::_writeSequenceUpdateStatements()
{
  QString sequenceUpdateStatement;
  if (_elementCounts["nodes"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_nodes_id_seq', " +
      QString::number(_elementCounts["nodes"]) + ");";
    _context->emit("/* nodes */\n", sequenceUpdateStatement.toStdString());
    if (!_localJobTracker)
    {
      _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
  if (_elementCounts["ways"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_ways_id_seq', " +
      QString::number(_elementCounts["ways"]) + ");";
    _context->emit("/* ways */\n", sequenceUpdateStatement.toStdString());
    if (!_localJobTracker)
    {
      _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
  if (_elementCounts["relations"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_relations_id_seq', " +
      QString::number(_elementCounts["relations"]) + ");";
    _context->emit("/* relations */\n", sequenceUpdateStatement.toStdString());
    if (!_localJobTracker)
    {
      _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
}

void WriteOsmSqlStatementsReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_context == NULL)
  {
    _context = &context;
  }

  shared_ptr<Configuration> config(HadoopPipesUtils::toConfiguration(context.getJobConf()));
  //LOG_VARD(config->getInt("mapred.reduce.tasks"));
  _localJobTracker = config->get("mapred.job.tracker") == "local";
  const long writeBufferSize = config->getLong("writeBufferSize");
  if (config->hasKey("dbConnUrl"))
  {
    _dbConnStr = QString::fromStdString(config->get("dbConnUrl"));
  }

  LOG_VART(_dbConnStr);
  _retainSqlFile = config->get("retainSqlFile") == "1" ? true : false;
  LOG_VART(_retainSqlFile);
  _sqlStatementBufferSize = 0;
  _sqlStatements = "";

  //I wanted to track the counts with hadoop counters instead, but pipes won't let you retrieve
  //counter values, so doing it this way.
  _elementCounts["nodes"] = 0;
  _elementCounts["ways"] = 0;
  _elementCounts["relations"] = 0;

  _tableHeader = QString::fromStdString(context.getInputKey());
  while (context.nextValue())
  {
    const QString value = QString::fromStdString(context.getInputValue());

    _updateElementCounts();

    _sqlStatements = _sqlStatements % value;
    _sqlStatementBufferSize++;

    if (!_localJobTracker && !value.trimmed().isEmpty() && value.trimmed() != "\\.")
    {
      //Get a different number here than what comes from the mapper, which must be due to mapper
      //extra mapper jobs which start but get cancelled early.
      _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }

    //this flush can cause the same table to be written to the file twice, each time with a
    //different set of sql records...but that's ok.
    if (_sqlStatementBufferSize >= writeBufferSize && !_sqlStatements.isEmpty())
    {
      _flush();
    }
  }
  if (_sqlStatementBufferSize > 0 && !_sqlStatements.isEmpty())
  {
    _flush();
  }

  //write the sequence id update sql statements - I believe this will work since all keys of the
  //same type will be sent to each reducer
  _writeSequenceUpdateStatements();
}

}
