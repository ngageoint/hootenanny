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

  const QString key = QString::fromStdString(context.getInputKey());
  QString values = "";
  QMap<QString, long> elementCounts;
  elementCounts["nodes"] = 0;
  elementCounts["ways"] = 0;
  elementCounts["relations"] = 0;
  while (context.nextValue())
  {
    const QString value = QString::fromStdString(context.getInputValue());
    values += value;

    if (key.contains("current_nodes"))
    {
      elementCounts["nodes"] = elementCounts["nodes"] + 1;
    }
    else if (key.contains("current_ways"))
    {
      elementCounts["ways"] = elementCounts["ways"] + 1;
    }
    if (key.contains("current_relations"))
    {
      elementCounts["relations"] = elementCounts["relations"] + 1;
    }
  }
  values += "\\.\n";
  _writer->emit(key.toStdString(), values.toStdString());

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
