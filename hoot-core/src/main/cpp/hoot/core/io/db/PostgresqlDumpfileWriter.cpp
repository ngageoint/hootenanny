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

#include <boost/shared_ptr.hpp>

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

  _sectionTempFiles(),
  _sectionNames(_createSectionNameList()),
  _outputFilename(),
  _writeStats()
{
  setConfiguration(conf());

  boost::shared_ptr<QTemporaryFile> nullPtr;

  for ( std::list<QString>::const_iterator it = _sectionNames.begin();
        it != _sectionNames.end(); ++it )
  {
    _sectionTempFiles.insert(std::pair<QString, boost::shared_ptr<QTemporaryFile> >(*it, nullPtr));
  }

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
}

void PostgresqlDumpfileWriter::finalizePartial()
{
  LOG_INFO( QString("Finalize called, time to create ") + _outputFilename);
}

void PostgresqlDumpfileWriter::writePartial(const boost::shared_ptr<const hoot::Node>& n)
{
  ;
}

void PostgresqlDumpfileWriter::writePartial(const boost::shared_ptr<const hoot::Way>& w)
{
  ;
}

void PostgresqlDumpfileWriter::writePartial(const boost::shared_ptr<const hoot::Relation>& r)
{
  ;
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

}
