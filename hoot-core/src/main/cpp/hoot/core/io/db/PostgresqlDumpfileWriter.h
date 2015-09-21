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
#include <map>
#include <list>
#include <utility>

#include <boost/shared_ptr.hpp>

#include <QtCore/QString>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/Q_INT64>


#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <tgs/BigContainers/BigMap.h>

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

#include <tgs/BigContainers/BigMap.h>

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

  virtual void writePartial(const ConstNodePtr& n);

  virtual void writePartial(const ConstWayPtr& w);

  virtual void writePartial(const ConstRelationPtr& r);

protected:

  virtual void setConfiguration(const hoot::Settings& conf);

  std::map<QString,
    std::pair<boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> > > _outputSections;

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

  // A lot of the Hootenanny code assumes we are on 64-bit platforms and declares ID type as "long."
  // Ensuring this code works on any platform that Qt is supported on
  typedef qint64 ElementIdDatatype;

  struct ConfigData
  {
    qint64            startingChangesetId;
    ElementIdDatatype startingNodeId;
    ElementIdDatatype startingWayId;
    ElementIdDatatype startingRelationId;
  };

  ConfigData _configData;

  struct _IdMappings
  {
    ElementIdDatatype nextNodeId;
    boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> > nodeIdMap;

    ElementIdDatatype nextWayId;
    boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> > wayIdMap;

    ElementIdDatatype nextRelationId;
    boost::shared_ptr<Tgs::BigMap<ElementIdDatatype, ElementIdDatatype> > relationIdMap;
  };

  _IdMappings _idMappings;

  struct _ChangesetData
  {
    qint64        changesetId;
    unsigned int  changesInChangeset();
  };

  _ChangesetData _changesetData;

  qint64 _getChangesetId() const { return _changesetData.changesetId; }

  std::list<QString> _createSectionNameList();

  void _closeSectionTempFilesAndConcat();

  void _createNodeTables();

  void _zeroWriteStats();

  ElementIdDatatype _establishNewIdMapping(const ElementId& sourceId);
};

}

#endif // POSTGRESQLDUMPFILEWRITER_H
