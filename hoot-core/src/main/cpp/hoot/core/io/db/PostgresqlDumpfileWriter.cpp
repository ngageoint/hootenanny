/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PostgresqlDumpfileWriter.h"

#include <cstdlib>  // for std::system
#include <cstdio>   // for std::remove
#include <math.h>   // for ::round (cmath header is C++11 only)
#include <utility>  // For std::pair
#include <vector>

#include <boost/shared_ptr.hpp>

#include <QtCore/QString>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <hoot/core/util/Settings.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, PostgresqlDumpfileWriter)

PostgresqlDumpfileWriter::PostgresqlDumpfileWriter():

  _outputSections(),
  _sectionNames(_createSectionNameList()),
  _outputFilename(),
  _writeStats(),
  _configData(),
  _idMappings(),
  _changesetData(),
  _unresolvedRefs(),
  _dataWritten(false)
{
  setConfiguration(conf());

  _changesetData.changesetId = 1;
  _changesetData.changesInChangeset = 0;
  _zeroWriteStats();
}

PostgresqlDumpfileWriter::~PostgresqlDumpfileWriter()
{
  close();
}

bool PostgresqlDumpfileWriter::isSupported(QString url)
{
  return ( url.endsWith(".sql") );
}

void PostgresqlDumpfileWriter::open(QString url)
{
  if ( isSupported(url) == false )
  {
    throw HootException( QString("Could not open URL ") + url);
  }

  _outputFilename.setFileName(url);
  //LOG_INFO( QString("Output filename set to ") + _outputFilename);

  _zeroWriteStats();

  _changesetData.changesetId  = _configData.startingChangesetId;
  _changesetData.changesInChangeset = 0;

  _idMappings.nextNodeId      = _configData.startingNodeId;
  _idMappings.nodeIdMap.reset();

  _idMappings.nextWayId       = _configData.startingWayId;
  _idMappings.wayIdMap.reset();

  _idMappings.nextRelationId  = _configData.startingRelationId;
  _idMappings.relationIdMap.reset();

  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  _dataWritten = false;
}

void PostgresqlDumpfileWriter::close()
{
  // Not writing any new data, can drop ID mappings
  _idMappings.nodeIdMap.reset();
  _idMappings.wayIdMap.reset();
  _idMappings.relationIdMap.reset();
  _unresolvedRefs.unresolvedWaynodeRefs.reset();
  _unresolvedRefs.unresolvedRelationRefs.reset();

  finalizePartial();

  if ( (_writeStats.nodesWritten > 0) || (_writeStats.waysWritten > 0) || (_writeStats.relationsWritten > 0) )
  {
    LOG_DEBUG("Write stats:");
    LOG_DEBUG("\t              Nodes written: " + QString::number(_writeStats.nodesWritten) );
    LOG_DEBUG("\t               Ways written: " + QString::number(_writeStats.waysWritten) );
    LOG_DEBUG("\t          Relations written: " + QString::number(_writeStats.relationsWritten) );
    LOG_DEBUG("\t   Relation members written:" + QString::number(_writeStats.relationMembersWritten));
    LOG_DEBUG("\tUnresolved relation members:" + QString::number(_writeStats.relationMembersWritten));
  }

  _zeroWriteStats();
  _outputSections.clear();
  _sectionNames.erase(_sectionNames.begin(), _sectionNames.end());
  _changesetData.changesetId  = _configData.startingChangesetId;
  _changesetData.changesInChangeset = 0;
}

void PostgresqlDumpfileWriter::finalizePartial()
{
  if ( (_writeStats.nodesWritten == 0) && (_writeStats.waysWritten == 0) &&
       (_writeStats.relationsWritten == 0) )
  {
    return;
  }

  if ( _dataWritten == true )
  {
    return;
  }

  LOG_DEBUG( QString("Finalize called, time to create ") + _outputFilename.fileName());

  // Remove file if it used to be there;
  if (_outputFilename.exists())
  {
    _outputFilename.remove();
  }

  // Output updates for sequences to ensure database sanity
  _writeSequenceUpdates();

  // Create our user data if the email value is set
  if ( _configData.addUserEmail.isEmpty() == false )
  {
    _createTable( "users", "COPY users (email, id, pass_crypt, creation_time) FROM stdin;\n");

    _outputSections["users"] <<
      QString("%1\t%2\t\tNOW()\n").arg(
        _configData.addUserEmail,
        QString::number(_configData.addUserId) );
  }

  // Do we have an unfinished changeset that needs flushing?
  if ( _changesetData.changesInChangeset >  0 )
  {
    LOG_DEBUG("Flushed changeset to disk");
    _writeChangesetToTable();
  }

  _outputFilename.open(QIODevice::Append);
  QTextStream outStream(&_outputFilename);

  for ( std::list<QString>::const_iterator it = _sectionNames.begin();
        it != _sectionNames.end(); ++it )
  {
   if ( _outputSections.find(*it) == _outputSections.end() )
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    // Write close marker for table
    if ( (*it != "byte_order_mark") && (*it != "sequence_updates") )
    {
      _outputSections[*it] << QString("\\.\n\n\n");
    }

    // Append contents of file to output file
    outStream << _outputSections[*it].join("");

    LOG_DEBUG( "Wrote contents of section " + *it );
  }
  _outputFilename.close();

  _dataWritten = true;
}

void PostgresqlDumpfileWriter::writePartial(const ConstNodePtr& n)
{
  Tags t = n->getTags();
  if (n->getCircularError() >= 0.0)
  {
    t["error:circular"] = QString::number(n->getCircularError());
  }

  if ( _writeStats.nodesWritten == 0 )
  {
    _createNodeTables();
    _idMappings.nodeIdMap = boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> >(
          new Tgs::BigMap<ElementIdDatatype, ElementIdDatatype>());
  }

  ElementIdDatatype nodeDbId;

  // Do we already know about this node?
  if ( _idMappings.nodeIdMap->contains(n->getId()) == true )
  {
    throw hoot::NotImplementedException("Writer class does not support update operations");
  }

  // Have to establish new mapping
  nodeDbId = _establishNewIdMapping(n->getElementId());

  _writeNodeToTables(n, nodeDbId);

  _writeTagsToTables(t, nodeDbId,
    "current_node_tags", "%1\t%2\t%3\n",
    "node_tags", "%1\t1\t%2\t%3\n");

  _writeStats.nodesWritten++;
  _incrementChangesInChangeset();

  _checkUnresolvedReferences( n, nodeDbId );
}

void PostgresqlDumpfileWriter::writePartial(const ConstWayPtr& w)
{
  Tags t = w->getTags();
  if (w->getCircularError() >= 0.0)
  {
    t["error:circular"] = QString::number(w->getCircularError());
  }

  if ( _writeStats.waysWritten == 0 )
  {
    _createWayTables();

    _idMappings.wayIdMap = boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> >(
          new Tgs::BigMap<ElementIdDatatype, ElementIdDatatype>());
  }

  ElementIdDatatype wayDbId;

  // Do we already know about this way?
  if ( _idMappings.wayIdMap->contains(w->getId()) == true )
  {
    throw hoot::NotImplementedException("Writer class does not support update operations");
  }

  // Have to establish new mapping
  wayDbId = _establishNewIdMapping(w->getElementId());

  _writeWayToTables( wayDbId );

  _writeWaynodesToTables( _idMappings.wayIdMap->at( w->getId() ), w->getNodeIds() );

  _writeTagsToTables(t, wayDbId,
    "current_way_tags", "%1\t%2\t%3\n",
    "way_tags", "%1\t1\t%2\t%3\n");

  _writeStats.waysWritten++;
  _incrementChangesInChangeset();

  _checkUnresolvedReferences( w, wayDbId );
}

void PostgresqlDumpfileWriter::writePartial(const ConstRelationPtr& r)
{
  Tags t = r->getTags();
  if (r->getCircularError() >= 0.0)
  {
    t["error:circular"] = QString::number(r->getCircularError());
  }

  if ( _writeStats.relationsWritten == 0 )
  {
    _createRelationTables();

    _idMappings.relationIdMap = boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> >(
          new Tgs::BigMap<ElementIdDatatype, ElementIdDatatype>());
  }

  ElementIdDatatype relationDbId;

  // Do we already know about this node?
  if ( _idMappings.relationIdMap->contains(r->getId()) == true )
  {
    throw hoot::NotImplementedException("Writer class does not support update operations");
  }

  // Have to establish new mapping
  relationDbId = _establishNewIdMapping(r->getElementId());

  _writeRelationToTables( relationDbId );

  _writeRelationMembersToTables( r );

  _writeTagsToTables(t, relationDbId,
    "current_relation_tags", "%1\t%2\t%3\n",
    "relation_tags", "%1\t1\t%2\t%3\n");

  _writeStats.relationsWritten++;
  _incrementChangesInChangeset();

  _checkUnresolvedReferences( r, relationDbId );
}

void PostgresqlDumpfileWriter::setConfiguration(const hoot::Settings &conf)
{
  const ConfigOptions confOptions(conf);

  _configData.addUserEmail        = confOptions.getPostgresqlDumpfileWriterUserEmail();
  _configData.addUserId           = confOptions.getPostgresqlDumpfileWriterUserId();
  _configData.startingChangesetId = confOptions.getPostgresqlDumpfileWriterStartIdChangeset();
  _configData.startingNodeId      = confOptions.getPostgresqlDumpfileWriterStartIdNode();
  _configData.startingWayId       = confOptions.getPostgresqlDumpfileWriterStartIdWay();
  _configData.startingRelationId  = confOptions.getPostgresqlDumpfileWriterStartIdRelation();
  _configData.changesetUserId     = confOptions.getPostgresqlDumpfileWriterChangesetUserId();

  LOG_DEBUG("Starting node ID: " << QString::number(_configData.startingNodeId));

  // TODO: needs to be a config value
  _configData.maxMapElements      = 1000000000;
}

std::list<QString> PostgresqlDumpfileWriter::_createSectionNameList()
{
  std::list<QString> sections;

  sections.push_back(QString("byte_order_mark"));
  sections.push_back(QString("sequence_updates"));
  sections.push_back(QString("users"));
  sections.push_back(QString("changesets"));
  sections.push_back(QString("current_nodes"));
  sections.push_back(QString("current_node_tags"));
  sections.push_back(QString("nodes"));
  sections.push_back(QString("node_tags"));
  sections.push_back(QString("current_ways"));
  sections.push_back(QString("current_way_nodes"));
  sections.push_back(QString("current_way_tags"));
  sections.push_back(QString("ways"));
  sections.push_back(QString("way_nodes"));
  sections.push_back(QString("way_tags"));
  sections.push_back(QString("current_relations"));
  sections.push_back(QString("current_relation_members"));
  sections.push_back(QString("current_relation_tags"));
  sections.push_back(QString("relations"));
  sections.push_back(QString("relation_members"));
  sections.push_back(QString("relation_tags"));

  return sections;
}

void PostgresqlDumpfileWriter::_createNodeTables()
{
  _createTable( "current_nodes",
                "COPY current_nodes (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version) FROM stdin;\n" );
  _createTable( "current_node_tags",
                "COPY current_node_tags (node_id, k, v) FROM stdin;\n" );

  _createTable( "nodes",
                "COPY nodes (node_id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, redaction_id) FROM stdin;\n" );
  _createTable( "node_tags",
                "COPY node_tags (node_id, version, k, v) FROM stdin;\n" );
}

void PostgresqlDumpfileWriter::_zeroWriteStats()
{
  _writeStats.nodesWritten = 0;
  _writeStats.nodeTagsWritten = 0;
  _writeStats.waysWritten = 0;
  _writeStats.wayNodesWritten = 0;
  _writeStats.wayTagsWritten = 0;
  _writeStats.relationsWritten = 0;
  _writeStats.relationMembersWritten = 0;
  _writeStats.relationMembersUnresolved = 0;
  _writeStats.relationTagsWritten = 0;
}

PostgresqlDumpfileWriter::ElementIdDatatype PostgresqlDumpfileWriter::_establishNewIdMapping(
    const ElementId& sourceId)
{
  ElementIdDatatype dbIdentifier;

  switch( sourceId.getType().getEnum() )
  {
  case ElementType::Node:
    dbIdentifier = _idMappings.nextNodeId;
    _idMappings.nodeIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.nextNodeId++;
    break;

  case ElementType::Way:
    dbIdentifier = _idMappings.nextWayId;
    _idMappings.wayIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.nextWayId++;
    break;
  case ElementType::Relation:
    dbIdentifier = _idMappings.nextRelationId;
    _idMappings.relationIdMap->insert(sourceId.getId(), dbIdentifier);
    _idMappings.nextRelationId++;
    break;
  default:
    throw NotImplementedException("Unsupported element type");
    break;
  }

  return dbIdentifier;
}

unsigned int PostgresqlDumpfileWriter::_tileForPoint(const double lat, const double lon) const
{
  const int lonInt = round((lon + 180.0) * 65535.0 / 360.0);
  const int latInt = round((lat + 90.0) * 65535.0 / 180.0);

  unsigned int tile = 0;

  for (int i = 15; i >= 0; i--)
  {
    tile = (tile << 1) | ((lonInt >> i) & 1);
    tile = (tile << 1) | ((latInt >> i) & 1);
  }

  return tile;
}

unsigned int PostgresqlDumpfileWriter::_convertDegreesToNanodegrees(const double degrees) const
{
  return ( round(degrees * 10000000.0) );
}

void PostgresqlDumpfileWriter::_writeNodeToTables(
  const ConstNodePtr& node,
  const ElementIdDatatype nodeDbId)
{
  //LOG_DEBUG("Writing node with ID: " << nodeDbId);

  const double nodeY = node->getY();
  const double nodeX = node->getX();
  const int nodeYNanodegrees = _convertDegreesToNanodegrees(nodeY);
  const int nodeXNanodegrees = _convertDegreesToNanodegrees(nodeX);
  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  const QString tileNumberString(QString::number(_tileForPoint(nodeY, nodeX)));

  if ( (nodeYNanodegrees < -900000000) || (nodeYNanodegrees > 900000000) )
  {
    throw HootException(QString("Invalid latitude conversion, Y = %1 to %2").arg(
      QString::number(nodeY), QString::number(nodeYNanodegrees)));
  }
  if ( (nodeXNanodegrees < -1800000000) || (nodeXNanodegrees > 1800000000) )
  {
    throw HootException(QString("Invalid longitude conversion, X = %1 to %2").arg(
      QString::number(nodeX), QString::number(nodeXNanodegrees)));
  }

  QString outputLine = QString("%1\t%2\t%3\t%4\tt\t%5\t%6\t1\n").arg(
    QString::number(nodeDbId),
    QString::number(nodeYNanodegrees),
    QString::number(nodeXNanodegrees),
    QString::number(changesetId),
    datestring,
    tileNumberString );

  _outputSections["current_nodes"] << outputLine;

  outputLine = QString("%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n").arg(
    QString::number(nodeDbId),
    QString::number(nodeYNanodegrees),
    QString::number(nodeXNanodegrees),
    QString::number(changesetId),
    datestring,
    tileNumberString );

  _outputSections["nodes"] << outputLine;
}

void PostgresqlDumpfileWriter::_writeTagsToTables(
  const Tags& tags,
  const ElementIdDatatype nodeDbId,
  const QString& currentTableName,
  const QString& currentTableFormatString,
  const QString& historicalTableName,
  const QString& historicalTableFormatString )
{
  const QString nodeDbIdString( QString::number(nodeDbId) );

  QStringList currentTable = _outputSections[currentTableName];
  QStringList historicalTable = _outputSections[historicalTableName];
  for ( Tags::const_iterator it = tags.begin(); it != tags.end(); ++it )
  {
    const QString key = _escapeCopyToData( it.key() );
    //LOG_VARD(key);
    const QString value = _escapeCopyToData( it.value() );
    //LOG_VARD(value);

    currentTable << currentTableFormatString.arg(nodeDbIdString, key, value );
    historicalTable << historicalTableFormatString.arg(nodeDbIdString, key, value );
  }
  _outputSections[currentTableName] = currentTable;
  _outputSections[historicalTableName] = historicalTable;
}

void PostgresqlDumpfileWriter::_createWayTables()
{
  _createTable( "current_ways",       "COPY current_ways (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n" );
  _createTable( "current_way_tags",   "COPY current_way_tags (way_id, k, v) FROM stdin;\n" );
  _createTable( "current_way_nodes",  "COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;\n" );

  _createTable( "ways",               "COPY ways (way_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n" );
  _createTable( "way_tags",           "COPY way_tags (way_id, version, k, v) FROM stdin;\n" );
  _createTable( "way_nodes",          "COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;\n" );
}

void PostgresqlDumpfileWriter::_writeWayToTables(const ElementIdDatatype wayDbId )
{
  //LOG_DEBUG("Writing way with ID: " << wayDbId);

  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString outputLine = QString("%1\t%2\t%3\tt\t1\n")
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);

  _outputSections["current_ways"] << outputLine;

  outputLine = QString("%1\t%2\t%3\t1\tt\t\\N\n")
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);

  _outputSections["ways"] << outputLine;
}

void PostgresqlDumpfileWriter::_writeWaynodesToTables( const ElementIdDatatype dbWayId,
  const std::vector<long>& waynodeIds )
{
  unsigned int nodeIndex = 1;

  QStringList currentWayNodesStream  = _outputSections["current_way_nodes"];
  QStringList wayNodesStream         = _outputSections["way_nodes"];
  const QString currentWaynodesFormat("%1\t%2\t%3\n");
  const QString waynodesFormat("%1\t%2\t1\t%3\n");
  const QString dbWayIdString( QString::number(dbWayId));

  for ( std::vector<long>::const_iterator it = waynodeIds.begin();
      it != waynodeIds.end(); ++it )
  {
    if ( _idMappings.nodeIdMap->contains(*it) == true )
    {
      const QString dbNodeIdString = QString::number( _idMappings.nodeIdMap->at(*it) );
      const QString nodeIndexString( QString::number(nodeIndex) );
      currentWayNodesStream << currentWaynodesFormat.arg(dbWayIdString, dbNodeIdString, nodeIndexString);
      wayNodesStream << waynodesFormat.arg(dbWayIdString, dbNodeIdString, nodeIndexString);
    }
    else
    {
      LOG_WARN( QString("Way %1 has reference to unknown node ID %2").arg(dbWayId, *it) );
      throw NotImplementedException("Unresolved waynodes are not supported");
    }

    ++nodeIndex;
  }
  _outputSections["current_way_nodes"] = currentWayNodesStream;
  _outputSections["way_nodes"] = wayNodesStream;
}

void PostgresqlDumpfileWriter::_createRelationTables()
{
  _createTable( "current_relations",        "COPY current_relations (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n" );
  _createTable( "current_relation_tags",    "COPY current_relation_tags (relation_id, k, v) FROM stdin;\n" );
  _createTable( "current_relation_members", "COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n" );

  _createTable( "relations",                "COPY relations (relation_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n" );
  _createTable( "relation_tags",            "COPY relation_tags (relation_id, version, k, v) FROM stdin;\n" );
  _createTable( "relation_members",         "COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;\n" );
}

void PostgresqlDumpfileWriter::_writeRelationToTables(const ElementIdDatatype relationDbId )
{
  //LOG_DEBUG("Writing relation with ID: " << relationDbId);

  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString outputLine = QString("%1\t%2\t%3\tt\t1\n")
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);

  _outputSections["current_relations"] << outputLine;

  outputLine = QString("%1\t%2\t%3\t1\tt\t\\N\n")
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);

  _outputSections["relations"] << outputLine;
}

void PostgresqlDumpfileWriter::_writeRelationMembersToTables( const ConstRelationPtr& relation )
{
  unsigned int memberSequenceIndex = 1;
  const ElementIdDatatype relationId = relation->getId();
  const ElementIdDatatype dbRelationId = _idMappings.relationIdMap->at(relationId);
  const std::vector<RelationData::Entry> relationMembers = relation->getMembers();
  boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> > knownElementMap;

  for ( vector<RelationData::Entry>::const_iterator it = relationMembers.begin();
      it != relationMembers.end(); ++it )
  {
    const ElementId memberElementId = it->getElementId();

    switch ( memberElementId.getType().getEnum() )
    {
    case ElementType::Node:
      knownElementMap = _idMappings.nodeIdMap;
      break;
    case ElementType::Way:
      knownElementMap = _idMappings.wayIdMap;
      break;
    case ElementType::Relation:
      knownElementMap = _idMappings.relationIdMap;
      break;
    default:
      throw HootException("Unsupported element member type");
      break;
    }

    if ( (knownElementMap != boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> >())
          && (knownElementMap->contains(memberElementId.getId()) == true) )
    {
      _writeRelationMember(dbRelationId, *it, knownElementMap->at(memberElementId.getId()), memberSequenceIndex);
    }
    else
    {
      if ( _unresolvedRefs.unresolvedRelationRefs ==
          boost::shared_ptr< std::map<ElementId, _UnresolvedRelationReference > >() )
      {
        _unresolvedRefs.unresolvedRelationRefs =
          boost::shared_ptr< std::map<ElementId, _UnresolvedRelationReference > >( new
          std::map<ElementId, _UnresolvedRelationReference>() );
      }

      const _UnresolvedRelationReference relationRef = { relationId, dbRelationId, *it, memberSequenceIndex };

      _unresolvedRefs.unresolvedRelationRefs->insert(std::pair<ElementId,
        _UnresolvedRelationReference>(memberElementId, relationRef) );
    }

    ++memberSequenceIndex;
  }
}

void PostgresqlDumpfileWriter::_writeRelationMember( const ElementIdDatatype sourceRelationDbId,
  const RelationData::Entry& memberEntry, const ElementIdDatatype memberDbId, const unsigned int memberSequenceIndex )
{
  QString memberType;
  const ElementId memberElementId = memberEntry.getElementId();

  switch ( memberElementId.getType().getEnum() )
  {
  case ElementType::Node:
    memberType = "Node";
    break;
  case ElementType::Way:
    memberType = "Way";
    break;
  case ElementType::Relation:
    memberType = "Relation";
    break;
  default:
    throw HootException("Unsupported element member type");
    break;
  }

  const QString dbRelationIdString( QString::number(sourceRelationDbId));
  const QString memberRefIdString( QString::number(memberDbId) );
  const QString memberSequenceString( QString::number(memberSequenceIndex) );
  const QString memberRole = _escapeCopyToData( memberEntry.getRole() );
  QStringList currentRelationMembersStream  = _outputSections["current_relation_members"];
  QStringList relationMembersStream         = _outputSections["relation_members"];
  const QString currentRelationMemberFormat("%1\t%2\t%3\t%4\t%5\n");
  const QString relationMembersFormat("%1\t%2\t%3\t%4\t1\t%5\n");

  currentRelationMembersStream << currentRelationMemberFormat.arg(
    dbRelationIdString, memberType, memberRefIdString, memberRole, memberSequenceString);
  relationMembersStream        << relationMembersFormat.arg(
    dbRelationIdString, memberType, memberRefIdString, memberRole, memberSequenceString);

  _outputSections["current_relation_members"] = currentRelationMembersStream;
  _outputSections["relation_members"] = relationMembersStream;

  _writeStats.relationMembersWritten++;
}

void PostgresqlDumpfileWriter::_createTable(const QString &tableName, const QString &tableHeader)
{
  _outputSections[tableName.toUtf8()] << tableHeader.toUtf8();
}

void PostgresqlDumpfileWriter::_incrementChangesInChangeset()
{
  _changesetData.changesInChangeset++;
  if ( _changesetData.changesInChangeset == ConfigOptions().getChangesetMaxSize() )
  {
    //LOG_DEBUG("Changeset " + QString::number(_changesetData.changesetId) + " hit max edits" );
    _writeChangesetToTable();
    _changesetData.changesetId++;
    _changesetData.changesInChangeset = 0;
  }
}

void PostgresqlDumpfileWriter::_checkUnresolvedReferences(const ConstElementPtr& element,
  const ElementIdDatatype elementDbId )
{
  // Regardless of type, may be referenced in relation
  if ( _unresolvedRefs.unresolvedRelationRefs !=
      boost::shared_ptr< std::map<ElementId, _UnresolvedRelationReference > >() )
  {
    std::map<ElementId, _UnresolvedRelationReference >::iterator relationRef =
      _unresolvedRefs.unresolvedRelationRefs->find( element->getElementId() );

    if ( relationRef != _unresolvedRefs.unresolvedRelationRefs->end() )
    {
      LOG_DEBUG("Found unresolved relation member ref!");
      LOG_DEBUG( QString( "Relation ID ") + QString::number(relationRef->second.sourceRelationId) +
        QString(" (DB ID=") + QString::number(relationRef->second.sourceDbRelationId) +
        QString(") has ref to ") + relationRef->second.relationMemberData.toString() );

      _writeRelationMember(relationRef->second.sourceDbRelationId, relationRef->second.relationMemberData,
        elementDbId, relationRef->second.relationMemberSequenceId );

      // Remove entry from unresolved list
      _unresolvedRefs.unresolvedRelationRefs->erase(relationRef);
    }
  }

  // If newly-written element is a node, check noderefs as well
  if ( element->getElementType().getEnum() == ElementType::Node )
  {
    if ( (_unresolvedRefs.unresolvedWaynodeRefs !=
        boost::shared_ptr<
        Tgs::BigMap<ElementIdDatatype, std::vector<
        std::pair<ElementIdDatatype, unsigned long> > > >()) &&
        ( _unresolvedRefs.unresolvedWaynodeRefs->contains(element->getId()) == true) )
    {
      LOG_DEBUG("Found unresolved waynode ref!");
      throw NotImplementedException("Need to insert waynode ref that is now resolved");
    }
  }
}

QString PostgresqlDumpfileWriter::_escapeCopyToData(const QString& stringToOutput) const
{
  QString escapedString(stringToOutput);

  // Escape any special characters as required by
  //    http://www.postgresql.org/docs/9.2/static/sql-copy.html
  escapedString.replace(QChar(92), QString("\\\\"));  // Escape single backslashes first
  escapedString.replace(QChar(8), QString("\\b"));
  escapedString.replace(QChar(9), QString("\\t"));
  escapedString.replace(QChar(10), QString("\\n"));
  escapedString.replace(QChar(11), QString("\\v"));
  escapedString.replace(QChar(12), QString("\\f"));
  escapedString.replace(QChar(13), QString("\\r"));

  return escapedString;
}

void PostgresqlDumpfileWriter::_writeChangesetToTable()
{
  if ( _changesetData.changesetId == _configData.startingChangesetId )
  {
    _createTable( "changesets", "COPY changesets (id, user_id, created_at, closed_at, num_changes) FROM stdin;\n" );
  }

  QStringList changesetsStream  = _outputSections["changesets"];
  const QString datestring = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  const QString changesetFormat("%1\t%2\t%3\t%4\t%5\n");

  changesetsStream << changesetFormat.arg(
    QString::number(_changesetData.changesetId),
    QString::number(_configData.changesetUserId),
    datestring,
    datestring,
    QString::number(_changesetData.changesInChangeset) );
  _outputSections["changesets"] = changesetsStream;
}

void PostgresqlDumpfileWriter::_writeSequenceUpdates()
{
  _createTable( "sequence_updates", "" );

  QStringList sequenceUpdatesStream  = _outputSections["sequence_updates"];
  const QString sequenceUpdateFormat("SELECT pg_catalog.setval('%1', %2);\n");

  // Users
  if ( _configData.addUserEmail.isEmpty() == false )
  {
    sequenceUpdatesStream << sequenceUpdateFormat.arg("users_id_seq",
      QString::number(_configData.addUserId + 1) );
  }

  // Changesets
  sequenceUpdatesStream << sequenceUpdateFormat.arg("changesets_id_seq",
    QString::number(_changesetData.changesetId + 1) );

  // Nodes
  sequenceUpdatesStream << sequenceUpdateFormat.arg("current_nodes_id_seq",
    QString::number(_idMappings.nextNodeId) );

  // Ways
  sequenceUpdatesStream << sequenceUpdateFormat.arg("current_ways_id_seq",
    QString::number(_idMappings.nextWayId) );

  // Relations
  sequenceUpdatesStream << sequenceUpdateFormat.arg("current_relations_id_seq",
    QString::number(_idMappings.nextRelationId) ) << "\n\n";

  _outputSections["sequence_updates"] = sequenceUpdatesStream;
}

}
