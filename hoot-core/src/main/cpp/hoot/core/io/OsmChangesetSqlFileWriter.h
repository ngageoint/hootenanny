#ifndef OSMCHANGESETSQLFILEWRITER_H
#define OSMCHANGESETSQLFILEWRITER_H

// Hoot
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/ElementType.h>

// tgs
#include <tgs/BigContainers/BigMap.h>


// Qt
#include <QUrl>
#include <QFile>
#include <QSqlDatabase>
#include <QString>

namespace hoot
{

/**
 * Writes out a set of SQL commands, that when executed, will update the contents of
 * an OSM API database with an OSM changeset.
 */
class OsmChangesetSqlFileWriter
{

public:

  OsmChangesetSqlFileWriter(QUrl url);

  /**
   * Write a SQL changeset to the specified output path
   *
   * @param path SQL file output path
   * @param changesetProvider changeset data
   */
  void write(const QString path, const ChangeSetProviderPtr changesetProvider);

private:

  void _createChangeSet();

  long _getNextId(const ElementType type);

  void _createNewElement(const ConstElementPtr newElement);
  void _updateExistingElement(const ConstElementPtr updatedElement);
  void _deleteExistingElement(const ConstElementPtr removedElement);

  QString _getInsertValuesStr(const long id, const long version, const bool visible,
                              const ConstElementPtr element) const;
  void _create(const ConstNodePtr node);
  QString _getInsertValuesStr(const long id, const long version, const bool visible,
                              const ConstNodePtr node) const;
  void _create(const ConstWayPtr way);
  void _create(const ConstRelationPtr relation);
  QString _getInsertValuesWayOrRelationStr(const long id, const long version,
                                           const bool visible) const;

  void _createTags(const Tags& tags, ElementId eid, const long version);
  QStringList _tagTableNamesForElement(ElementId eid) const;
  void _deleteCurrentTags(ElementId eid);

  void _createWayNodes(const long wayId, const std::vector<long>& nodeIds);

  void _createRelationMembers(const long relationId, const vector<RelationData::Entry>& members);

  void _modify(const ConstNodePtr node);
  void _modify(const ConstWayPtr way);
  void _modify(const ConstRelationPtr relation);

  void _deleteAll(const QString tableName, const QString idFieldName, const long id);

  long _changesetId;
  long _nodeId;
  long _wayId;
  long _relationId;

  OsmApiDb _db;
  QFile _outputSql;

  Tgs::BigMap<long, long> _idMappingsNode;
  Tgs::BigMap<long, long> _idMappingsWay;
  Tgs::BigMap<long, long> _idMappingsRelation;

  friend class OsmChangeWriterSqlTest;

  //QMap<ElementType::Type, QList<long> > _deletedElements;

};

}

#endif // OSMCHANGESETSQLFILEWRITER_H