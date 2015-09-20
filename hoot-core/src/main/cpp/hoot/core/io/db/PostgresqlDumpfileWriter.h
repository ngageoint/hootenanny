#ifndef POSTGRESQLDUMPFILEWRITER_H
#define POSTGRESQLDUMPFILEWRITER_H

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
 * @copyright Copyright ...
 */

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <list>

#include <boost/shared_ptr.hpp>

#include <Qt/qstring.h>
#include <Qt/qtemporaryfile.h>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <tgs/BigContainers/BigMap.h>

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

namespace hoot
{

class PostgresqlDumpfileWriter : public hoot::PartialOsmMapWriter, public hoot::Configurable
{
public:
  static std::string className() { return "hoot::PostgresqlDumpfileWriter"; }
  PostgresqlDumpfileWriter();

  ~PostgresqlDumpfileWriter();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  void close();

  virtual void finalizePartial();

  virtual void writePartial(const boost::shared_ptr<const hoot::Node>& n);

  virtual void writePartial(const boost::shared_ptr<const hoot::Way>& w);

  virtual void writePartial(const boost::shared_ptr<const hoot::Relation>& r);

protected:

  virtual void setConfiguration(const hoot::Settings &conf);

  std::map<QString, boost::shared_ptr<QTemporaryFile> > _sectionTempFiles;

  std::list<QString> _sectionNames;

  QString _outputFilename;

  struct _ElementWriteStats
  {
    unsigned long nodesWritten;
    unsigned long nodeTagsWritten;
    unsigned long waysWritten;
    unsigned long wayNodesWritten;
    unsigned long wayTagsWritten;
    unsigned long relationsWritten;
    unsigned long relationMembersWritten;
    unsigned long relationTagsWritten;
  };

  _ElementWriteStats _writeStats;

  std::list<QString> _createSectionNameList();

  void _closeSectionTempFilesAndConcat();





};

}

#endif // POSTGRESQLDUMPFILEWRITER_H
