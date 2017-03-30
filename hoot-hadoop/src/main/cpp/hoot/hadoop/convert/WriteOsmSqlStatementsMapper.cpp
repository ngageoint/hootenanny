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
}

//TODO: consolidate duplicated code here
//void WriteElementSqlStatementsMapper::_writeElementSqlStatements(const ConstElementPtr& element,
//                                                             HadoopPipes::MapContext& context)
//{

//}

//TODO: buffer this writing

void WriteOsmSqlStatementsMapper::_map(shared_ptr<OsmMap>& map, HadoopPipes::MapContext& context)
{
//  if (_context == NULL)
//  {
//    _context = &context;
//  }

  shared_ptr<pp::Configuration> config(pp::HadoopPipesUtils::toConfiguration(context.getJobConf()));
  long changesetMaxSize = config->getLong("changesetMaxSize");
  //TODO: temp
  changesetMaxSize = 5;
  //LOG_VARD(config->getInt("mapred.map.tasks"));

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
    context.emit(nodeSqlHeaders[0].toStdString(), nodeSqlStatements[0].toStdString());
    context.emit(nodeSqlHeaders[1].toStdString(), nodeSqlStatements[1].toStdString());

    const Tags& tags = node->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList nodeTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          nodeId, node->getElementId().getType(), it.key(), it.value());
      context.emit(nodeTagSqlHeaders[0].toStdString(), nodeTagSqlStatements[0].toStdString());
      context.emit(nodeTagSqlHeaders[1].toStdString(), nodeTagSqlStatements[1].toStdString());
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
    context.emit(waySqlHeaders[0].toStdString(), waySqlStatements[0].toStdString());
    context.emit(waySqlHeaders[1].toStdString(), waySqlStatements[1].toStdString());

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
      context.emit(wayNodeSqlHeaders[0].toStdString(), wayNodeSqlStatements[0].toStdString());
      context.emit(wayNodeSqlHeaders[1].toStdString(), wayNodeSqlStatements[1].toStdString());
      wayNodeIndex++;
    }

    const Tags& tags = way->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList wayTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          wayId, way->getElementId().getType(), it.key(), it.value());
      context.emit(wayTagSqlHeaders[0].toStdString(), wayTagSqlStatements[0].toStdString());
      context.emit(wayTagSqlHeaders[1].toStdString(), wayTagSqlStatements[1].toStdString());
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
    context.emit(relationSqlHeaders[0].toStdString(), relationSqlStatements[0].toStdString());
    context.emit(relationSqlHeaders[1].toStdString(), relationSqlStatements[1].toStdString());

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
      context.emit(
        relationMemberSqlHeaders[0].toStdString(), relationMemberSqlStatements[0].toStdString());
      context.emit(
        relationMemberSqlHeaders[1].toStdString(), relationMemberSqlStatements[1].toStdString());
      memberSequenceIndex++;
    }

    const Tags& tags = relation->getTags();
    for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
    {
      const QStringList relationTagSqlStatements =
        _sqlFormatter->tagToSqlStrings(
          relationId, relation->getElementId().getType(), it.key(), it.value());
      context.emit(
        relationTagSqlHeaders[0].toStdString(), relationTagSqlStatements[0].toStdString());
      context.emit(
        relationTagSqlHeaders[1].toStdString(), relationTagSqlStatements[1].toStdString());
    }
  }
}

}
