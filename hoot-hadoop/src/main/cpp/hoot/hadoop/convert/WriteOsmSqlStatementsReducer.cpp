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
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/DbUtils.h>

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
using namespace pp;

// Qt
#include <QMap>
#include <QStringBuilder>

#include <stdio.h>
#include <stdlib.h>

#include <postgresql/libpq-fe.h>

using namespace std;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WriteOsmSqlStatementsReducer)

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer() :
_tableHeader(""),
_sqlStatements(""),
_sqlStatementBufferSize(0),
_retainSqlFile(false)
{
  _context = NULL;
}

WriteOsmSqlStatementsReducer::~WriteOsmSqlStatementsReducer()
{
}

void WriteOsmSqlStatementsReducer::_flush()
{
  assert(_sqlStatementBufferSize > 0);
  assert(!_sqlStatements.isEmpty());

  LOG_VART(_tableHeader);

  _sqlStatements += "\\.\n";

  if (_retainSqlFile)
  {
    LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to disk...");
    _context->emit(_tableHeader.toStdString(), _sqlStatements.toStdString());
  }

  if (!_dbConnStr.isEmpty())
  {
    LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to database...");

    const char* conninfo = ApiDb::getPqString(_dbConnStr).toLatin1().data();
    PGconn* conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK)
    {
      throw HootException("Connection not open.");
    }

    PGresult* res;
    const char* header = _tableHeader.toLatin1().data();
    LOG_VART(header);
    res = PQexec(conn, header);
    const char* statements = _sqlStatements.toLatin1().data();
    LOG_VART(statements);
    if (PQresultStatus(res) != PGRES_COPY_IN)
    {
      throw HootException("copy in not ok");
    }
    else
    {
      if (PQputCopyData(conn, statements, strlen(statements)) == 1)
      {
        if (PQputCopyEnd(conn, NULL) == 1)
        {
          int flushStatus = 1;
          int numTries = 0;  //safety feature
          while (flushStatus == 1 && numTries < 10)
          {
            flushStatus = PQflush(conn);
            numTries++;
          }

          res = PQgetResult(conn);
          if (PQresultStatus(res) == PGRES_COMMAND_OK)
          {
            LOG_INFO("done");
          }
          else
          {
            throw HootException(QString::fromAscii(PQerrorMessage(conn)));
          }
        }
        else
        {
          throw HootException(QString::fromAscii(PQerrorMessage(conn)));
        }
      }
      else
      {
        throw HootException(QString::fromAscii(PQerrorMessage(conn)));
      }
    }

    PQclear(res);
    PQfinish(conn);
  }

  _sqlStatements = "";
  _sqlStatementBufferSize = 0;
}

void WriteOsmSqlStatementsReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_context == NULL)
  {
    _context = &context;
  }

  shared_ptr<pp::Configuration> config(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  //LOG_VARD(config->getInt("mapred.reduce.tasks"));
  const bool localJobTracker = config->get("mapred.job.tracker") == "local";
  const long writeBufferSize = config->getLong("writeBufferSize");
  if (config->hasKey("dbConnUrl"))
  {
    _dbConnStr = QString::fromStdString(config->get("dbConnUrl"));
  }
  //LOG_VART(_execSqlWithMapreduce);
  LOG_VART(_dbConnStr);
  _retainSqlFile = config->get("retainSqlFile") == "1" ? true : false;
  LOG_VART(_retainSqlFile);
  _sqlStatementBufferSize = 0;
  _sqlStatements = "";

  //I wanted to track the counts with counters instead, but pipes won't let you retrieve counter
  //values, so doing it this way.
  //TODO: this causes scability issues due to the map storage
  QMap<QString, long> elementCounts;
  elementCounts["nodes"] = 0;
  elementCounts["ways"] = 0;
  elementCounts["relations"] = 0;

  _tableHeader = QString::fromStdString(context.getInputKey());
  while (context.nextValue())
  {
    const QString value = QString::fromStdString(context.getInputValue());

    if (_tableHeader.contains("current_nodes"))
    {
      elementCounts["nodes"] = elementCounts["nodes"] + 1;
    }
    else if (_tableHeader.contains("current_ways"))
    {
      elementCounts["ways"] = elementCounts["ways"] + 1;
    }
    else if (_tableHeader.contains("current_relations"))
    {
      elementCounts["relations"] = elementCounts["relations"] + 1;
    }

    _sqlStatements = _sqlStatements % value;
    _sqlStatementBufferSize++;

    if (!localJobTracker && !value.trimmed().isEmpty() && value.trimmed() != "\\.")
    {
      //I'm getting a different number here than what comes from the mapper, so need to figure
      //out what's up with that.
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
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
  //same type will be sent to each reducer...still remains to be tested on a real cluster, though.

  QString sequenceUpdateStatement;
  if (elementCounts["nodes"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_nodes_id_seq', " +
      QString::number(elementCounts["nodes"]) + ");";
    _context->emit("/* nodes */\n", sequenceUpdateStatement.toStdString());
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
  if (elementCounts["ways"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_ways_id_seq', " +
      QString::number(elementCounts["ways"]) + ");";
    _context->emit("/* ways */\n", sequenceUpdateStatement.toStdString());
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
  if (elementCounts["relations"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_relations_id_seq', " +
      QString::number(elementCounts["relations"]) + ");";
    _context->emit("/* relations */\n", sequenceUpdateStatement.toStdString());
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL statements"), 1);
    }
  }
}

}
