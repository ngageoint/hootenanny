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

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>

// Qt
#include <QMap>

namespace hoot
{

PP_FACTORY_REGISTER(pp::Reducer, WriteOsmSqlStatementsReducer)

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer()
{
  _writer = NULL;
}

void WriteOsmSqlStatementsReducer::reduce(HadoopPipes::ReduceContext& context)
{
  if (_writer == NULL)
  {
    HadoopPipes::RecordWriter* writer = pp::HadoopPipesUtils::getRecordWriter(&context);
    _writer = dynamic_cast<pp::RecordWriter*>(writer);
    if (_writer == NULL)
    {
      throw HootException("Error getting RecordWriter.");
    }
  }

  //shared_ptr<pp::Configuration> config(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  //LOG_VARD(config->getInt("mapred.reduce.tasks"));

  //I wanted to track the counts with counters instead, but pipes won't let you retrieve counter
  //values, so doing it this way...not sure yet if this is good parallel logic, though.
  QMap<QString, long> elementCounts;
  elementCounts["nodes"] = 0;
  elementCounts["ways"] = 0;
  elementCounts["relations"] = 0;

  const QString key = QString::fromStdString(context.getInputKey());
  QString values = "";
  while (context.nextValue())
  {
    const QString value = QString::fromStdString(context.getInputValue());

    if (key.contains("current_nodes"))
    {
      elementCounts["nodes"] = elementCounts["nodes"] + 1;
    }
    else if (key.contains("current_ways"))
    {
      elementCounts["ways"] = elementCounts["ways"] + 1;
    }
    else if (key.contains("current_relations"))
    {
      elementCounts["relations"] = elementCounts["relations"] + 1;
    }

    values += value;
  }
  values += "\\.\n";
  //write the record data
  _writer->emit(key.toStdString(), values.toStdString());

  //write the sequence id update sql statements
  //TODO: this code is suspect to me; its worked so far with multiple reducers in what I believe
  //to be pseudo-distributed mode.  if all keys of one type are going to each reducer, then I guess
  //this will work...although it does seem like some of the reducers are going to get bogged down

  QString sequenceUpdateStatement;
  if (elementCounts["nodes"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_nodes_id_seq', " +
      QString::number(elementCounts["nodes"]) + ");";
    _writer->emit("/* nodes */\n", sequenceUpdateStatement.toStdString());
  }
  if (elementCounts["ways"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_ways_id_seq', " +
      QString::number(elementCounts["ways"]) + ");";
    _writer->emit("/* ways */\n", sequenceUpdateStatement.toStdString());
  }
  if (elementCounts["relations"] > 0)
  {
    sequenceUpdateStatement =
      "SELECT pg_catalog.setval('current_relations_id_seq', " +
      QString::number(elementCounts["relations"]) + ");";
    _writer->emit("/* relations */\n", sequenceUpdateStatement.toStdString());
  }
}

}
