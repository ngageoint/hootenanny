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
#include <pp/conf/Configuration.h>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>
#include <hoot/core/elements/Tags.h>

// Qt
#include <QStringBuilder>

namespace hoot
{

PP_FACTORY_REGISTER(pp::Mapper, WriteOsmSqlStatementsMapper)

WriteOsmSqlStatementsMapper::WriteOsmSqlStatementsMapper() :
_outputDelimiter("\t"),
_localJobTracker(false)
{
  _sqlFormatter.reset(new OsmApiDbSqlStatementFormatter(_outputDelimiter));
  _statementsBuffer.reset(new QVector<QPair<QString, QString> >());
  _context = NULL;
}

void WriteOsmSqlStatementsMapper::_writeElementAndTagsSqlStatements(const ConstElementPtr& element,
                                                                    const unsigned long elementId,
                                                               const QStringList& elementSqlHeaders,
                                                           const QStringList& elementTagSqlHeaders)
{
  const QStringList elementSqlStatements =
    _sqlFormatter->elementToSqlStrings(element, elementId, 1);
  _statementsBuffer->append(QPair<QString, QString>(elementSqlHeaders[0], elementSqlStatements[0]));
  _statementsBuffer->append(QPair<QString, QString>(elementSqlHeaders[1], elementSqlStatements[1]));
  const QString elementTypeStr = element->getElementType().toString().toLower() % "s";
  //the pretty pipes version of the local job runner doesn't support counters
  if (!_localJobTracker)
  {
    _context->incrementCounter(
      _context->getCounter("WriteOsmSqlStatements", elementTypeStr.toStdString()), 1);
    //x2 due to one statement for the current table and one for the historical table
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 2);
  }

  const Tags& tags = element->getTags();
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    const QStringList elementTagSqlStatements =
      _sqlFormatter->tagToSqlStrings(
        elementId, element->getElementId().getType(), it.key(), it.value());
    _statementsBuffer->append(
      QPair<QString, QString>(elementTagSqlHeaders[0], elementTagSqlStatements[0]));
    _statementsBuffer->append(
      QPair<QString, QString>(elementTagSqlHeaders[1], elementTagSqlStatements[1]));
    if (!_localJobTracker)
    {
      QString elementTagsTypeStr = elementTypeStr;
      elementTagsTypeStr.chop(1);
      elementTagsTypeStr = elementTagsTypeStr % " tags";
      _context->incrementCounter(
        _context->getCounter("WriteOsmSqlStatements", elementTagsTypeStr.toStdString()), 1);
      _context->incrementCounter(
        _context->getCounter("WriteOsmSqlStatements", "SQL statements"), 2);
    }
  }

  if (!_localJobTracker)
  {
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "elements"), 1);
    _context->incrementCounter(_context->getCounter("WriteOsmSqlStatements", "SQL statements"), 2);
  }
}

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
  _localJobTracker = config->get("mapred.job.tracker") == "local";
  const long writeBufferSize = config->getLong("writeBufferSize");

  //nodes

  const QStringList nodeSqlHeaders = _sqlFormatter->getNodeSqlHeaderStrings();
  const QStringList nodeTagSqlHeaders = _sqlFormatter->getNodeTagSqlHeaderStrings();
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    ConstNodePtr node = it->second;

    assert(node->getId() != 0);
    const unsigned long elementId = abs(node->getId());

    _writeElementAndTagsSqlStatements(node, elementId, nodeSqlHeaders, nodeTagSqlHeaders);

    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  //ways

  const QStringList waySqlHeaders = _sqlFormatter->getWaySqlHeaderStrings();
  const QStringList wayNodeSqlHeaders = _sqlFormatter->getWayNodeSqlHeaderStrings();
  const QStringList wayTagSqlHeaders = _sqlFormatter->getWayTagSqlHeaderStrings();
  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    ConstWayPtr way = it->second;

    assert(way->getId() != 0);
    const unsigned long elementId = abs(way->getId());

    _writeElementAndTagsSqlStatements(way, elementId, waySqlHeaders, wayTagSqlHeaders);

    const vector<long> wayNodeIds = way->getNodeIds();
    unsigned int wayNodeIndex = 1;
    for (vector<long>::const_iterator it = wayNodeIds.begin(); it != wayNodeIds.end(); ++it)
    {
      assert(*it != 0);
      const long wayNodeId = abs(*it);

      const QStringList wayNodeSqlStatements =
        _sqlFormatter->wayNodeToSqlStrings(elementId, wayNodeId, wayNodeIndex);
      _statementsBuffer->append(
        QPair<QString, QString>(wayNodeSqlHeaders[0], wayNodeSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(wayNodeSqlHeaders[1], wayNodeSqlStatements[1]));
      wayNodeIndex++;
      if (!_localJobTracker)
      {
        _context->incrementCounter(
          _context->getCounter("WriteOsmSqlStatements", "way nodes"), 1);
        _context->incrementCounter(
          _context->getCounter("WriteOsmSqlStatements", "SQL statements"), 2);
      }
    }

    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  //relations

  const QStringList relationSqlHeaders = _sqlFormatter->getRelationSqlHeaderStrings();
  const QStringList relationMemberSqlHeaders = _sqlFormatter->getRelationMemberSqlHeaderStrings();
  const QStringList relationTagSqlHeaders = _sqlFormatter->getRelationTagSqlHeaderStrings();
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr relation = it->second;

    assert(relation->getId() != 0);
    const unsigned long elementId = abs(relation->getId());

    _writeElementAndTagsSqlStatements(
      relation, elementId, relationSqlHeaders, relationTagSqlHeaders);

    unsigned int memberSequenceIndex = 1;
    const vector<RelationData::Entry> relationMembers = relation->getMembers();
    for (vector<RelationData::Entry>::const_iterator it = relationMembers.begin();
         it != relationMembers.end(); ++it)
    {
      const RelationData::Entry member = *it;

      assert(member.getElementId().getId() != 0);
      const long memberId = abs(member.getElementId().getId());

      const QStringList relationMemberSqlStatements =
        _sqlFormatter->relationMemberToSqlStrings(
          elementId, memberId, member, memberSequenceIndex);
      _statementsBuffer->append(
        QPair<QString, QString>(relationMemberSqlHeaders[0], relationMemberSqlStatements[0]));
      _statementsBuffer->append(
        QPair<QString, QString>(relationMemberSqlHeaders[1], relationMemberSqlStatements[1]));
      memberSequenceIndex++;
      if (!_localJobTracker)
      {
        _context->incrementCounter(
          _context->getCounter("WriteOsmSqlStatements", "relation members"), 1);
        _context->incrementCounter(
          _context->getCounter("WriteOsmSqlStatements", "SQL statements"), 2);
      }
    }

    if (_statementsBuffer->size() >= writeBufferSize)
    {
      _flush();
    }
  }

  _flush();
}

}
