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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMPOSTGRESQLDUMPFILEWRITER_H
#define OSMPOSTGRESQLDUMPFILEWRITER_H

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

#include <QString>
#include <QTemporaryFile>
#include <QTextStream>
#include <Q_INT64>

#include <hoot/core/io/PartialOsmMapWriter.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>

#include <tgs/BigContainers/BigMap.h>

namespace hoot
{

using namespace boost;
using namespace std;
using namespace Tgs;

/**
 * OSM element writer optimized for bulk element writes.  If you are writing smaller amounts of
 * data, you probably want to create a new writer, as this writer performs additional logic to
 * reserver element IDs as described below.
 *
 * This writer safely be used against a live database.  It first reserves the required element IDs
 * by doing an initial count of the elements and executing the ID reservation SQL, then generates
 * SQL copy statements to a file for the element inserts (dump file), and then finally executes the
 * element SQL statements.
 */
class OsmPostgresqlDumpfileWriter : public PartialOsmMapWriter, public Configurable
{

public:

  static string className() { return "hoot::OsmPostgresqlDumpfileWriter"; }

  OsmPostgresqlDumpfileWriter();

  virtual ~OsmPostgresqlDumpfileWriter();

  virtual bool isSupported(QString url);

  virtual void open(QString url);

  void close();

  virtual void finalizePartial();

  virtual void writePartial(const ConstNodePtr& n);

  virtual void writePartial(const ConstWayPtr& w);

  virtual void writePartial(const ConstRelationPtr& r);

  virtual void setConfiguration(const hoot::Settings& conf);

private:

  map<QString, pair<shared_ptr<QTemporaryFile>, shared_ptr<QTextStream> > > _outputSections;

  list<QString> _sectionNames;

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
    QString addUserEmail;
    qint64 addUserId;
    ElementIdDatatype changesetUserId;
    qint64 startingChangesetId;
    ElementIdDatatype startingNodeId;
    ElementIdDatatype startingWayId;
    ElementIdDatatype startingRelationId;
    unsigned long maxMapElements;
  };
  ConfigData _configData;

  struct _IdMappings
  {
    ElementIdDatatype nextNodeId;
    shared_ptr<BigMap<ElementIdDatatype, ElementIdDatatype> > nodeIdMap;

    ElementIdDatatype nextWayId;
    shared_ptr<BigMap<ElementIdDatatype, ElementIdDatatype> > wayIdMap;

    ElementIdDatatype nextRelationId;
    shared_ptr<BigMap<ElementIdDatatype, ElementIdDatatype> > relationIdMap;
  };
  _IdMappings _idMappings;

  struct _ChangesetData
  {
    qint64 changesetId;
    unsigned int changesInChangeset;
    Envelope changesetBounds;
  };
  _ChangesetData _changesetData;

  struct _UnresolvedRelationReference
  {
    ElementIdDatatype sourceRelationId;
    ElementIdDatatype sourceDbRelationId;
    RelationData::Entry relationMemberData;
    unsigned int relationMemberSequenceId;
  };
  struct _UnresolvedReferences
  {
    // Schema: node ID -> vector of entries w/ type: pair(way ID for waynode, 1-based sequence
    // order for waynode)
    shared_ptr<BigMap<ElementIdDatatype, vector< pair<ElementIdDatatype, unsigned long> > > > unresolvedWaynodeRefs;

    shared_ptr< map<ElementId, _UnresolvedRelationReference > > unresolvedRelationRefs;
  };
  _UnresolvedReferences _unresolvedRefs;

  bool _dataWritten;

  OsmApiDb _database;

  void _zeroWriteStats();
  void _createNodeTables();
  list<QString> _createSectionNameList();
  void _createWayTables();
  void _createRelationTables();
  void _createTable(const QString& tableName, const QString& tableHeader);
  void _createTable(const QString& tableName, const QString& tableHeader,
                    const bool addByteOrderMarker);

  qint64 _getChangesetId() const { return _changesetData.changesetId; }
  void _incrementChangesInChangeset();
  ElementIdDatatype _establishNewIdMapping(const ElementId& sourceId);
  void _checkUnresolvedReferences(const ConstElementPtr& element,
                                  const ElementIdDatatype elementDbId);

  unsigned int _convertDegreesToNanodegrees(const double degrees) const;
  QString _escapeCopyToData(const QString& stringToOutput) const;

  void _writeChangesetToTable();
  void _writeSequenceUpdates();
  void _writeRelationToTables(const ElementIdDatatype relationDbId);
  void _writeRelationMembersToTables(const ConstRelationPtr& relation);
  void _writeRelationMember(const ElementIdDatatype sourceRelation,
    const RelationData::Entry& memberEntry, const ElementIdDatatype memberDbId,
    const unsigned int memberSequenceIndex);
  void _writeWayToTables(const ElementIdDatatype wayDbId);
  void _writeWaynodesToTables(const ElementIdDatatype wayId,
    const vector<long>& waynodeIds);
  void _writeNodeToTables(const ConstNodePtr& node, const ElementIdDatatype nodeDbId);
  void _writeTagsToTables(const Tags& tags, const ElementIdDatatype nodeDbId,
    shared_ptr<QTextStream>& currentTable, const QString& currentTableFormatString,
    shared_ptr<QTextStream>& historicalTable, const QString& historicalTableFormatString);
  void _closeSectionTempFilesAndConcat();


};

}

#endif // OSMPOSTGRESQLDUMPFILEWRITER_H
