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

#include <utility>
#include <boost/shared_ptr.hpp>

#include <QtCore/QString>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>

#include <hoot/core/util/Settings.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/LogLog4Cxx.h>
#include <hoot/core/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, PostgresqlDumpfileWriter)

PostgresqlDumpfileWriter::PostgresqlDumpfileWriter():

  _outputSections(),
  _sectionNames(_createSectionNameList()),
  _outputFilename(),
  _writeStats()
{
  setConfiguration(conf());

  boost::shared_ptr<QTemporaryFile> nullPtr;

  /*
  for ( std::list<QString>::const_iterator it = _sectionNames.begin();
        it != _sectionNames.end(); ++it )
  {
    QTemporaryFile tempFile;
    //_outputSections.insert( std::make_pair(*it, std::make_pair(tempFile, &tempFile)) );
  }
  */

  _writeStats.nodesWritten = 0;
  _writeStats.nodeTagsWritten = 0;
  _writeStats.waysWritten = 0;
  _writeStats.wayNodesWritten = 0;
  _writeStats.wayTagsWritten = 0;
  _writeStats.relationsWritten = 0;
  _writeStats.relationMembersWritten = 0;
  _writeStats.relationTagsWritten = 0;
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
}

void PostgresqlDumpfileWriter::finalizePartial()
{
  LOG_INFO( QString("Finalize called, time to create ") + _outputFilename);
}

void PostgresqlDumpfileWriter::writePartial(const ConstNodePtr& n)
{
  if ( _writeStats.nodesWritten == 0 )
  {
    _createNodeTables();
  }
  _writeStats.nodesWritten++;
}

void PostgresqlDumpfileWriter::writePartial(const ConstWayPtr& w)
{
  _writeStats.waysWritten++;
}

void PostgresqlDumpfileWriter::writePartial(const ConstRelationPtr& r)
{
  _writeStats.relationsWritten++;
}

void PostgresqlDumpfileWriter::setConfiguration(const hoot::Settings &conf)
{
  ;
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
  boost::shared_ptr<QTemporaryFile> tempfile;
  QString tableName;


  tableName = "current_nodes";
  tempfile = boost::shared_ptr<QTemporaryFile>( new QTemporaryFile() );

  _outputSections[tableName] =
      std::pair< boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> >(
        tempfile, boost::shared_ptr<QTextStream>(new QTextStream(tempfile.get())) );

  *(_outputSections[tableName].second) <<
    "COPY current_nodes (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version) FROM stdin;\n";


  tableName = "current_node_tags";
  tempfile = boost::shared_ptr<QTemporaryFile>( new QTemporaryFile() );

  _outputSections[tableName] =
      std::pair< boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> >(
        tempfile, boost::shared_ptr<QTextStream>(new QTextStream(tempfile.get())) );

  *(_outputSections[tableName].second) << "COPY current_node_tags (node_id, k, v) FROM stdin;\n";


  tableName = "nodes";
  tempfile = boost::shared_ptr<QTemporaryFile>( new QTemporaryFile() );

  _outputSections[tableName] =
      std::pair< boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> >(
        tempfile, boost::shared_ptr<QTextStream>(new QTextStream(tempfile.get())) );

  *(_outputSections[tableName].second) <<
    "COPY nodes (node_id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, redaction_id) FROM stdin;\n";


  tableName = "node_tags";
  tempfile = boost::shared_ptr<QTemporaryFile>( new QTemporaryFile() );

  _outputSections[tableName] =
      std::pair< boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> >(
        tempfile, boost::shared_ptr<QTextStream>(new QTextStream(tempfile.get())) );

  *(_outputSections[tableName].second) << "COPY node_tags (node_id, version, k, v) FROM stdin;\n";


  /*
  _sectionTempFiles["current_node_tags"] = boost::shared_ptr<QTemporaryFile>(new QTemporaryFile());
  _sectionTempFiles["nodes"] = boost::shared_ptr<QTemporaryFile>(new QTemporaryFile());
  _sectionTempFiles["node_tags"] = boost::shared_ptr<QTemporaryFile>(new QTemporaryFile());
  */

}

}
