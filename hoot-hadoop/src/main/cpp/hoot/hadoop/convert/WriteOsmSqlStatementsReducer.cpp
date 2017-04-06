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

#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WriteOsmSqlStatementsReducer)

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer() :
_tableHeader(""),
_sqlStatements(""),
_sqlStatementBufferSize(0),
//_execSqlWithMapreduce(false),
_retainSqlFile(false)
{
  _context = NULL;
}

WriteOsmSqlStatementsReducer::~WriteOsmSqlStatementsReducer()
{
//  if (_execSqlWithMapreduce)
//  {
//    _database.close();
//  }
}

void WriteOsmSqlStatementsReducer::_flush()
{
  assert(_sqlStatementBufferSize > 0);
  assert(!_sqlStatements.isEmpty());

  LOG_VART(_tableHeader);

  if (_retainSqlFile)
  {
    LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to disk...");
    const QString fileSqlStatements = _sqlStatements + "\\.\n";
    _context->emit(_tableHeader.toStdString(), fileSqlStatements.toStdString());
  }

  //if (_execSqlWithMapreduce)
  if (!_dbConnStr.isEmpty())
  {
    LOG_TRACE("Flushing " << _sqlStatementBufferSize << " records to database...");
    //_tableHeader = _tableHeader.replace("FROM stdin;", "");
    //const QString query = "BEGIN TRANSACTION;\n" + _tableHeader + _sqlStatements + "COMMIT;";
    //LOG_VARD(query);

//    DbUtils::execNoPrepare(
//      _database.getDB(),
//      //QString("BEGIN TRANSACTION;\n") +
//      QString(_tableHeader + _sqlStatements /*+ "\n"*/)// +
//      /*QString("COMMIT;")*/);

    //ApiDb::execSqlCommand(_dbConnStr, query);

    //TODO: move the db connection to a mem var and buffer this write
    connection conn(ApiDb::getPqxxString(_dbConnStr).toStdString());
    if (!conn.is_open())
    {
      throw HootException("Connection not open.");
    }
    work txn(conn);
    const QString tableName = _tableHeader.split(" ")[1];
    const QStringList columnNamesTemp = _tableHeader.split("(")[1].split(")")[0].split(",");
    vector<string> columnNames;
    for (int i = 0; i < columnNamesTemp.size(); i++)
    {
      columnNames.push_back(columnNamesTemp[i].toStdString());
    }
    tablewriter tw(txn, tableName.toStdString(), columnNames.begin(), columnNames.end(), "\\N");
    const QStringList sqlStatementsTemp = _sqlStatements.split("\n");
    for (int i = 0; i < sqlStatementsTemp.size(); i++)
    {
      const QString sqlStatementTemp = sqlStatementsTemp[i];
      if (!sqlStatementTemp.contains("\\.") && !sqlStatementTemp.trimmed().isEmpty())
      {
        QStringList valuesTemp = sqlStatementTemp.split("\t");
        //value.replace("\n", "");
        vector<string> values;
        for (int j = 0; j < valuesTemp.size(); j++)
        {
          QString valueTemp = valuesTemp[j];
          //if (valueTemp.contains("\N"))
          //{
            //valueTemp = "";
          //}
          LOG_VART(valueTemp);
          values.push_back(valueTemp.toStdString());
        }
        tw.insert(values);
      }
    }
    tw.complete();
    try
    {
      txn.commit();
    }
    catch (const std::exception& e)
    {
      txn.abort();
      conn.disconnect();
      throw HootException(e.what());
    }
    conn.disconnect();
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
    //const QString dbConnStr = QString::fromStdString(config->get("dbConnUrl"));
    _dbConnStr = QString::fromStdString(config->get("dbConnUrl"));
//    LOG_VART(dbConnStr);
//    if (!dbConnStr.isEmpty())
//    {
//      QUrl url(dbConnStr);
//      _database.open(url);
//      _execSqlWithMapreduce = true;
//    }
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
