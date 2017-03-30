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
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>
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

WriteOsmSqlStatementsReducer::WriteOsmSqlStatementsReducer() :
_elementCount(0),
_outputDelimiter("\t")
{
  _writer = NULL;
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
  //empty dummy bounds
  _bounds.init();
}

void WriteOsmSqlStatementsReducer::_updateRecordChangesetId(const QString tableName,
                                                            const long changesetId,
                                                            QString& recordLine)
{
  LOG_TRACE("Updating changeset ID for line...");
  LOG_VART(tableName);
  LOG_VART(changesetId);

  QStringList lineParts = recordLine.split(_outputDelimiter);
  LOG_VART(lineParts.size());

  bool lineUpdated = true;
  if (tableName == ApiDb::getCurrentNodesTableName() || tableName == ApiDb::getNodesTableName())
  {
    lineParts[3] = QString::number(changesetId);
  }
  else if (tableName == ApiDb::getCurrentWaysTableName() || tableName == ApiDb::getWaysTableName())
  {
    lineParts[1] = QString::number(changesetId);
  }
  else if (tableName == ApiDb::getCurrentRelationsTableName() ||
           tableName == ApiDb::getRelationsTableName())
  {
    lineParts[1] = QString::number(changesetId);
  }
  else
  {
    lineUpdated = false;
  }

  if (!lineUpdated)
  {
    throw HootException("Bad line passed to record changeset ID updater: " + recordLine.left(100));
  }

  recordLine = lineParts.join(_outputDelimiter);
  LOG_TRACE("Changeset ID updated for line: " << recordLine.left(100));
}

//TODO: buffer this writing

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

  shared_ptr<pp::Configuration> config(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  long changesetMaxSize = config->getLong("changesetMaxSize");
  //TODO: temp
  changesetMaxSize = 5;
  //LOG_VARD(config->getInt("mapred.reduce.tasks"));

  const QString changesetHeaderStr = _sqlFormatter->getChangesetSqlHeaderString();
  const long changesetUserId = ConfigOptions().getChangesetUserId(); //TODO: fix

  QMap<QString, long> elementCounts;
  elementCounts["nodes"] = 0;
  elementCounts["ways"] = 0;
  elementCounts["relations"] = 0;

  const QString key = QString::fromStdString(context.getInputKey());
  QString values = "";
  long currentChangesetId = 1;
  while (context.nextValue())
  {
    QString value = QString::fromStdString(context.getInputValue());

    if (key.contains(ApiDb::getNodesTableName()) || key.contains(ApiDb::getWaysTableName()) ||
        key.contains(ApiDb::getRelationsTableName()))
    {
      //all changeset id's default to 1, so don't start editing until its equal to 2
//      if (currentChangesetId >= 2)
//      {
//        if (key.contains(ApiDb::getCurrentNodesTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getCurrentNodesTableName(), currentChangesetId, value);
//        }
//        else if (key.contains(ApiDb::getCurrentWaysTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getCurrentWaysTableName(), currentChangesetId, value);
//        }
//        else if (key.contains(ApiDb::getCurrentRelationsTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getCurrentRelationsTableName(), currentChangesetId, value);
//        }
//        else if (key.contains(ApiDb::getNodesTableName()) &&
//                 !key.contains(ApiDb::getWayNodesTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getNodesTableName(), currentChangesetId, value);
//        }
//        else if (key.contains(ApiDb::getWaysTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getWaysTableName(), currentChangesetId, value);
//        }
//        else if (key.contains(ApiDb::getRelationsTableName()))
//        {
//          _updateRecordChangesetId(ApiDb::getRelationsTableName(), currentChangesetId, value);
//        }
//      }

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
    }

    values += value;

    _elementCount++;
    if (_elementCount == changesetMaxSize)
    {
      const QString changesetStatement =
        _sqlFormatter->changesetToSqlString(
          currentChangesetId, changesetUserId, _elementCount, _bounds);
      //_writer->emit(changesetHeaderStr.toStdString(), changesetStatement.toStdString());
      currentChangesetId++;
      _elementCount = 0;
    }
  }
  values += "\\.\n";
  //write the record data
  _writer->emit(key.toStdString(), values.toStdString());

  //write the sequence id update sql statements

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
//  sequenceUpdateStatement =
//    "SELECT pg_catalog.setval('changesets_id_seq', " +
//    QString::number(currentChangesetId) + ");";
//  _writer->emit("/* changesets */\n", sequenceUpdateStatement.toStdString());
}

}
