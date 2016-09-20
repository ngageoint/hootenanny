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
#ifndef POSTGRESQLDUMPFILEWRITER_H
#define POSTGRESQLDUMPFILEWRITER_H

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

#include <string>
#include <map>
#include <list>
#include <vector>
#include <utility>

#include <boost/shared_ptr.hpp>

#include <QString>
#include <QTemporaryFile>
#include <QTextStream>
#include <Q_INT64>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/io/OsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/RelationData.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/io/OsmApiDb.h>

#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

/**
 * OSM element write optimization that writes out a Postgres dump file which can later be applied
 * to Postgres.
 */
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

private:

  std::map<QString,
    std::pair<boost::shared_ptr<QTemporaryFile>, boost::shared_ptr<QTextStream> > > _outputSections;

  std::list<QString> _sectionNames;

  QFile _outputFilename;

  struct _ElementWriteStats
  {
    unsigned long nodesWritten;
    unsigned long nodeTagsWritten;
    unsigned long waysWritten;
    unsigned long wayNodesWritten;
    unsigned long wayTagsWritten;
    unsigned long relationsWritten;
    unsigned long relationMembersWritten;
    unsigned long relationMembersUnresolved;
    unsigned long relationTagsWritten;
  };

  _ElementWriteStats _writeStats;

  // A lot of the Hootenanny code assumes we are on 64-bit platforms and declares ID type as "long."
  // Ensuring this code works on any platform that Qt is supported on
  typedef qint64 ElementIdDatatype;

  struct ConfigData
  {
    QString           addUserEmail;
    qint64            addUserId;
    ElementIdDatatype changesetUserId;
    qint64            startingChangesetId;
    ElementIdDatatype startingNodeId;
    ElementIdDatatype startingWayId;
    ElementIdDatatype startingRelationId;
    unsigned long     maxMapElements;
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
    unsigned int  changesInChangeset;
    Envelope changesetBounds;
  };

  _ChangesetData _changesetData;

  struct _UnresolvedRelationReference
  {
    ElementIdDatatype     sourceRelationId;
    ElementIdDatatype     sourceDbRelationId;
    RelationData::Entry   relationMemberData;
    unsigned int          relationMemberSequenceId;
  };

  struct _UnresolvedReferences
  {
    // Schema: node ID -> vector of entries w/ type: pair(way ID for waynode, 1-based sequence order for waynode)
    boost::shared_ptr< Tgs::BigMap<ElementIdDatatype, std::vector< std::pair<ElementIdDatatype, unsigned long> > > > unresolvedWaynodeRefs;

    boost::shared_ptr< std::map<ElementId, _UnresolvedRelationReference > > unresolvedRelationRefs;
  };

  _UnresolvedReferences _unresolvedRefs;

  virtual void setConfiguration(const hoot::Settings& conf);

  qint64 _getChangesetId() const { return _changesetData.changesetId; }

  std::list<QString> _createSectionNameList();

  bool _dataWritten;

  void _closeSectionTempFilesAndConcat();

  void _createNodeTables();

  void _zeroWriteStats();

  ElementIdDatatype _establishNewIdMapping(const ElementId& sourceId);

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;

  void _writeNodeToTables(const ConstNodePtr& node, const ElementIdDatatype nodeDbId);

  void _writeTagsToTables(
    const Tags& tags,
    const ElementIdDatatype nodeDbId,
    boost::shared_ptr<QTextStream>& currentTable,
    const QString& currentTableFormatString,
    boost::shared_ptr<QTextStream>& historicalTable,
    const QString& historicalTableFormatString );

  void _createWayTables();

  void _writeWayToTables(const ElementIdDatatype wayDbId );

  void _writeWaynodesToTables( const ElementIdDatatype wayId,
    const std::vector<long>& waynodeIds );

  void _createRelationTables();

  void _writeRelationToTables(const ElementIdDatatype relationDbId );

  void _writeRelationMembersToTables( const ConstRelationPtr& relation );

  void _writeRelationMember( const ElementIdDatatype sourceRelation,
    const RelationData::Entry& memberEntry, const ElementIdDatatype memberDbId,
    const unsigned int memberSequenceIndex );

  void _createTable( const QString& tableName, const QString& tableHeader );

  void _createTable( const QString& tableName, const QString& tableHeader, const bool addByteOrderMarker );

  void _incrementChangesInChangeset();

  void _checkUnresolvedReferences( const ConstElementPtr& element,
    const ElementIdDatatype elementDbId );

  QString _escapeCopyToData( const QString& stringToOutput) const;

  void _writeChangesetToTable();

  void _writeSequenceUpdates();

  //this is used for the optional id sequence queries only
  OsmApiDb _db;
};

}

#endif // POSTGRESQLDUMPFILEWRITER_H
