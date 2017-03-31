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

#include "WriteOsmSqlStatementsMapper.h"

// Pretty Pipes
#include <pp/Factory.h>
#include <pp/HadoopPipesUtils.h>
#include <pp/Hdfs.h>
#include <pp/conf/Configuration.h>

// Hoot
#include <hoot/hadoop/Debug.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>
#include <hoot/core/elements/Tags.h>

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, WriteOsmSqlStatementsMapper)

WriteOsmSqlStatementsMapper::WriteOsmSqlStatementsMapper() :
_outputDelimiter("\t")
{
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
  _statementsBuffer.reset(new QVector<QPair<QString, QString> >());
  _context = NULL;
}

//TODO: consolidate duplicated code here
//void WriteElementSqlStatementsMapper::_writeElementSqlStatements(const ConstElementPtr& element,
//                                                             HadoopPipes::MapContext& context)
//{

//}

void WriteOsmSqlStatementsMapper::_flush()
{
  LOG_TRACE("Flushing " << _statementsBuffer->size() << " records to disk...");
  for (QVector<QPair<QString, QString> >::const_iterator it = _statementsBuffer->begin();
       it != _statementsBuffer->end(); ++it)
  {
    QPair<QString, QString> statementPair = *it;
    LOG_TRACE(statementPair.first << " " << statementPair.second);
    _context->emit(statementPair.first.toStdString(), statementPair.second.toStdString());
  }
  _statementsBuffer->clear();
}

void WriteOsmSqlStatementsMapper::_map(shared_ptr<OsmMap>& map, HadoopPipes::MapContext& context)
{
  if (_context == NULL)
  {
    _context = &context;
  }

  shared_ptr<pp::Configuration> config(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  //LOG_VARD(config->getInt("mapred.map.tasks"));
  const bool localJobTracker = config->get("mapred.job.tracker") == "local";
  const long writeBufferSize = config->getLong("writeBufferSize");

  const QStringList nodeSqlHeaders = _sqlFormatter->getNodeSqlHeaderStrings();
  const QStringList nodeTagSqlHeaders = _sqlFormatter->getNodeTagSqlHeaderStrings();
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    shared_ptr<const Node> node = it->second;

    long nodeId = node->getId();
    if (nodeId < 1)
    {
      assert(nodeId != 0);
      nodeId = nodeId * -1;
    }

    const QStringList nodeSqlStatements = _sqlFormatter->nodeToSqlStrings(node, nodeId, 1);
    _statementsBuffer->append(QPair<QString, QString>(nodeSqlHeaders[0], nodeSqlStatements[0]));
    _statementsBuffer->append(QPair<QString, QString>(nodeSqlHeaders[1], nodeSqlStatements[1]));
    //the pretty pipes version of the local job runner doesn't support counters
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "nodes"), 1);
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
    }

    const Tags& tags = node->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList nodeTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          nodeId, node->getElementId().getType(), it.key(), it.value());
      _statementsBuffer->append(
        QPair<QString, QString>(nodeTagSqlHeaders[0], nodeTagSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(nodeTagSqlHeaders[1], nodeTagSqlStatements[1]));
      if (!localJobTracker)
      {
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "node tags"), 1);
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
      }
    }

    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "elements"), 1);
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
    }
    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  const QStringList waySqlHeaders = _sqlFormatter->getWaySqlHeaderStrings();
  const QStringList wayNodeSqlHeaders = _sqlFormatter->getWayNodeSqlHeaderStrings();
  const QStringList wayTagSqlHeaders = _sqlFormatter->getWayTagSqlHeaderStrings();
  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    const shared_ptr<Way>& way = it->second;

    long wayId = way->getId();
    if (wayId < 1)
    {
      assert(wayId != 0);
      wayId = wayId * -1;
    }

    const QStringList waySqlStatements = _sqlFormatter->wayToSqlStrings(wayId, 1);
    _statementsBuffer->append(QPair<QString, QString>(waySqlHeaders[0], waySqlStatements[0]));
    _statementsBuffer->append(QPair<QString, QString>(waySqlHeaders[1], waySqlStatements[1]));
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "ways"), 1);
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
    }

    const vector<long> wayNodeIds = way->getNodeIds();
    unsigned int wayNodeIndex = 1;
    for (vector<long>::const_iterator it = wayNodeIds.begin(); it != wayNodeIds.end(); ++it)
    {
      long wayNodeId = *it;
      if (wayNodeId < 1)
      {
        assert(wayNodeId != 0);
        wayNodeId = wayNodeId * -1;
      }

      const QStringList wayNodeSqlStatements =
        _sqlFormatter->wayNodeToSqlStrings(wayId, wayNodeId, wayNodeIndex);
      _statementsBuffer->append(
        QPair<QString, QString>(wayNodeSqlHeaders[0], wayNodeSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(wayNodeSqlHeaders[1], wayNodeSqlStatements[1]));
      wayNodeIndex++;
      if (!localJobTracker)
      {
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "way nodes"), 1);
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
      }
    }

    const Tags& tags = way->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList wayTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          wayId, way->getElementId().getType(), it.key(), it.value());
      _statementsBuffer->append(
        QPair<QString, QString>(wayTagSqlHeaders[0], wayTagSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(wayTagSqlHeaders[1], wayTagSqlStatements[1]));
      if (!localJobTracker)
      {
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "way tags"), 1);
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
      }
    }

    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "elements"), 1);
    }
    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  const QStringList relationSqlHeaders = _sqlFormatter->getRelationSqlHeaderStrings();
  const QStringList relationMemberSqlHeaders = _sqlFormatter->getRelationMemberSqlHeaderStrings();
  const QStringList relationTagSqlHeaders = _sqlFormatter->getRelationTagSqlHeaderStrings();
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;

    long relationId = relation->getId();
    if (relationId < 1)
    {
      assert(relationId != 0);
      relationId = relationId * -1;
    }

    const QStringList relationSqlStatements = _sqlFormatter->relationToSqlStrings(relationId, 1);
    _statementsBuffer->append(
      QPair<QString, QString>(relationSqlHeaders[0], relationSqlStatements[0]));
    _statementsBuffer->append(
      QPair<QString, QString>(relationSqlHeaders[1], relationSqlStatements[1]));
    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "relations"), 1);
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
    }

    unsigned int memberSequenceIndex = 1;
    const vector<RelationData::Entry> relationMembers = relation->getMembers();
    for (vector<RelationData::Entry>::const_iterator it = relationMembers.begin();
         it != relationMembers.end(); ++it)
    {
      const RelationData::Entry member = *it;

      long memberId = member.getElementId().getId();
      if (memberId < 1)
      {
        assert(memberId != 0);
        memberId = memberId * -1;
      }

      const QStringList relationMemberSqlStatements =
        _sqlFormatter->relationMemberToSqlStrings(
          relationId, memberId, member, memberSequenceIndex);
      _statementsBuffer->append(
        QPair<QString, QString>(relationMemberSqlHeaders[0], relationMemberSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(relationMemberSqlHeaders[1], relationMemberSqlStatements[1]));
      memberSequenceIndex++;
      if (!localJobTracker)
      {
        context.incrementCounter(
          context.getCounter("WriteOsmSqlStatements", "relation members"), 1);
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
      }
    }

    const Tags& tags = relation->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList relationTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          relationId, relation->getElementId().getType(), it.key(), it.value());
      _statementsBuffer->append(
        QPair<QString, QString>(relationTagSqlHeaders[0], relationTagSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(relationTagSqlHeaders[1], relationTagSqlStatements[1]));
      if (!localJobTracker)
      {
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "relation tags"), 1);
        context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "SQL records"), 2);
      }
    }

    if (!localJobTracker)
    {
      context.incrementCounter(context.getCounter("WriteOsmSqlStatements", "elements"), 1);
    }
    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  _flush();
}

}
