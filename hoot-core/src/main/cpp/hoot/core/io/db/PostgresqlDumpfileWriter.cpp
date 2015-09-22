/**
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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "PostgresqlDumpfileWriter.h"

#include <cstdlib>  // for std::system
#include <cstdio>   // for std::remove
#include <math.h>   // for ::round (cmath header is C++11 only)

#include <utility>
#include <boost/shared_ptr.hpp>

#include <QtCore/QString>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>

#include <hoot/core/util/Settings.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/LogLog4Cxx.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
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
  _changesetData()
{
  setConfiguration(conf());

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
  // Make sure we're not already open and the URL is valid
  if ( _outputFilename.length() > 0)
  {
    throw HootException( QString("Tried to open writer when already open") );
  }
  else if ( isSupported(url) == false )
  {
    throw HootException( QString("Could not open URL ") + url);
  }

  _outputFilename = url;
  LOG_INFO( QString("Output filename set to ") + _outputFilename);

  _zeroWriteStats();

  _changesetData.changesetId  = _configData.startingChangesetId;

  _idMappings.nextNodeId      = _configData.startingNodeId;
  _idMappings.nodeIdMap.reset();

  _idMappings.nextWayId       = _configData.startingWayId;
  _idMappings.wayIdMap.reset();

  _idMappings.nextRelationId  = _configData.startingRelationId;
  _idMappings.relationIdMap.reset();
}

void PostgresqlDumpfileWriter::close()
{
  finalizePartial();

  if ( (_writeStats.nodesWritten > 0) || (_writeStats.waysWritten > 0) || (_writeStats.relationsWritten > 0) )
  {
    LOG_DEBUG("Write stats:");
    LOG_DEBUG("\t    Nodes written: " + QString::number(_writeStats.nodesWritten) );
    LOG_DEBUG("\t     Ways written: " + QString::number(_writeStats.waysWritten) );
    LOG_DEBUG("\tRelations written: " + QString::number(_writeStats.relationsWritten) );
  }

  _zeroWriteStats();
  _outputFilename = "";
  _outputSections.clear();
  _sectionNames.erase(_sectionNames.begin(), _sectionNames.end());
  _idMappings.nodeIdMap.reset();
  _idMappings.wayIdMap.reset();
  _idMappings.relationIdMap.reset();
}

void PostgresqlDumpfileWriter::finalizePartial()
{
  LOG_INFO( QString("Finalize called, time to create ") + _outputFilename);

  // Remove file if it used to be there;
  std::remove(_outputFilename.toStdString().c_str());

  for ( std::list<QString>::const_iterator it = _sectionNames.begin();
        it != _sectionNames.end(); ++it )
  {
    if ( _outputSections.find(*it) == _outputSections.end() )
    {
      LOG_DEBUG("No data for table " + *it);
      continue;
    }

    // Write close marker for table
    *(_outputSections[*it].second) << QString("\\.\n\n\n");

    // Flush any residual content from text stream/file
    (_outputSections[*it].second)->flush();
    if ( (_outputSections[*it].first)->flush() == false )
    {
      throw HootException("Could not flush tempfile for table " + *it);
    }

    // Append contents of file to output file
    QString cmdToExec(
      QString("/bin/cat ") + (_outputSections[*it].first)->fileName() +
      QString(" >> ") + _outputFilename );

    const int systemRetval = std::system( cmdToExec.toStdString().c_str() );
    LOG_DEBUG("Command: \"" + cmdToExec + "\", retval: " +
              QString::number(systemRetval) );
  }

}

void PostgresqlDumpfileWriter::writePartial(const ConstNodePtr& n)
{
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

  _writeTagsToTables( n->getTags(), nodeDbId,
    _outputSections["current_node_tags"].second, "%1\t%2\t%3\n",
    _outputSections["node_tags"].second, "%1\t1\t%2\t%3\n");

  _writeStats.nodesWritten++;
}

void PostgresqlDumpfileWriter::writePartial(const ConstWayPtr& w)
{
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

  _writeTagsToTables( w->getTags(), wayDbId,
    _outputSections["current_way_tags"].second, "%1\t%2\t%3\n",
    _outputSections["way_tags"].second, "%1\t1\t%2\t%3\n");

  _writeStats.waysWritten++;
}

void PostgresqlDumpfileWriter::writePartial(const ConstRelationPtr& r)
{
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

  _writeTagsToTables( r->getTags(), relationDbId,
    _outputSections["current_relation_tags"].second, "%1\t%2\t%3\n",
    _outputSections["relation_tags"].second, "%1\t1\t%2\t%3\n");


  _writeStats.relationsWritten++;
}

void PostgresqlDumpfileWriter::setConfiguration(const hoot::Settings &conf)
{
  // TODO: pull these from config values
  _configData.startingChangesetId = 1;
  _configData.startingNodeId      = 1;
  _configData.startingWayId       = 1;
  _configData.startingRelationId  = 1;
}

std::list<QString> PostgresqlDumpfileWriter::_createSectionNameList()
{
  std::list<QString> sections;

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
  const ElementIdDatatype nodeDbId )
{
  const double nodeY = node->getY();
  const double nodeX = node->getX();
  const unsigned int nodeYNanodegrees = _convertDegreesToNanodegrees(nodeY);
  const unsigned int nodeXNanodegrees = _convertDegreesToNanodegrees(nodeX);
  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz");
  const QString tileNumberString(QString::number(_tileForPoint(nodeY, nodeX)));

  QString outputLine = QString("%1\t%2\t%3\t%4\tt\t%5\t%6\t1\n")
      .arg(nodeDbId)
      .arg(nodeYNanodegrees)
      .arg(nodeXNanodegrees)
      .arg(changesetId)
      .arg(datestring)
      .arg(tileNumberString);

  *(_outputSections["current_nodes"].second) << outputLine;

  outputLine = QString("%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n")
      .arg(nodeDbId)
      .arg(nodeYNanodegrees)
      .arg(nodeXNanodegrees)
      .arg(changesetId)
      .arg(datestring)
      .arg(tileNumberString);

  *(_outputSections["nodes"].second) << outputLine;
}

void PostgresqlDumpfileWriter::_writeTagsToTables(
  const Tags& tags,
  const ElementIdDatatype nodeDbId,
  boost::shared_ptr<QTextStream>& currentTable,
  const QString& currentTableFormatString,
  boost::shared_ptr<QTextStream>& historicalTable,
  const QString& historicalTableFormatString )
{
  const QString nodeDbIdString( QString::number(nodeDbId) );

  for ( Tags::const_iterator it = tags.begin(); it != tags.end(); ++it )
  {
    const QString key = it.key();
    const QString value = it.value();

    *currentTable << currentTableFormatString.arg(nodeDbIdString, key, value );
    *historicalTable << historicalTableFormatString.arg(nodeDbIdString, key, value );
  }
}

void PostgresqlDumpfileWriter::_createWayTables()
{
  _createTable( "current_ways",       "COPY current_ways (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version) FROM stdin;\n" );
  _createTable( "current_way_tags",   "COPY current_way_tags (way_id, k, v) FROM stdin;\n" );
  _createTable( "current_way_nodes",  "COPY current_way_nodes (way_id, node_id, sequence_id) FROM stdin;\n" );

  _createTable( "ways",               "COPY ways (way_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n" );
  _createTable( "way_tags",           "COPY way_tags (way_id, k, v, version) FROM stdin;\n" );
  _createTable( "way_nodes",          "COPY way_nodes (way_id, node_id, version, sequence_id) FROM stdin;\n" );
}

void PostgresqlDumpfileWriter::_writeWayToTables(const ElementIdDatatype wayDbId )
{
  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString outputLine = QString("%1\t%2\t%3\tt\t1\n")
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);

  *(_outputSections["current_ways"].second) << outputLine;

  outputLine = QString("%1\t%2\t%3\tt\t1\t\\N\n")
      .arg(wayDbId)
      .arg(changesetId)
      .arg(datestring);

  *(_outputSections["ways"].second) << outputLine;
}

void PostgresqlDumpfileWriter::_createRelationTables()
{
  _createTable( "current_relations",        "COPY current_relations (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n" );
  _createTable( "current_relation_tags",    "COPY current_relation_tags (relation_id, k, v) FROM stdin;\n" );
  _createTable( "current_relation_members", "COPY current_relation_members (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n" );

  _createTable( "relations",                "COPY relations (relation_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n" );
  _createTable( "relation_tags",            "COPY relation_tags (relation_id, k, v, version) FROM stdin;\n" );
  _createTable( "relation_members",         "COPY relation_members (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;\n" );
}

void PostgresqlDumpfileWriter::_writeRelationToTables(const ElementIdDatatype relationDbId )
{
  const int changesetId = _getChangesetId();
  const QString datestring = QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QString outputLine = QString("%1\t%2\t%3\tt\t1\n")
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);

  *(_outputSections["current_relations"].second) << outputLine;

  outputLine = QString("%1\t%2\t%3\tt\t1\t\\N\n")
      .arg(relationDbId)
      .arg(changesetId)
      .arg(datestring);

  *(_outputSections["relations"].second) << outputLine;
}


void PostgresqlDumpfileWriter::_createTable( const QString& tableName, const QString& tableHeader )
{
  boost::shared_ptr<QTemporaryFile> tempfile( new QTemporaryFile() );
  if ( tempfile->open() == false )
  {
    throw HootException("Could not open temp file for contents of table " + tableName);
  }

  _outputSections[tableName] =
      std::pair< boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> >(
        tempfile, boost::shared_ptr<QTextStream>(new QTextStream(tempfile.get())) );

  *(_outputSections[tableName].second) << tableHeader;
}

}
