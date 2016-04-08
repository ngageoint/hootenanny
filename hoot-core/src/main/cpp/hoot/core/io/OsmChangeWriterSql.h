#ifndef OSMCHANGEWRITERSQL_H
#define OSMCHANGEWRITERSQL_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
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
class OsmChangeWriterSql
{

public:

  OsmChangeWriterSql(QUrl url);

  /**
   * Write a SQL changeset to the specified output path
   *
   * @param path SQL file output path
   * @param changesetProvider changeset data
   */
  void write(const QString path, const ChangeSetProviderPtr changesetProvider);

private:

  void _open(QUrl url);

  long _createChangeSet();

  long _getNextId(const ElementType type);
  long _getNextId(QString type);

  void _writeNewElement(const ConstElementPtr newElement);
  void _updateExistingElement(const ConstElementPtr updatedElement);
  void _deleteExistingElement(const ConstElementPtr removedElement);

  void _create(const ConstNodePtr node);
  void _create(const ConstWayPtr way);
  void _create(const ConstRelationPtr relation);

  void _modify(const ConstNodePtr node);
  void _modify(const ConstWayPtr way);
  void _modify(const ConstRelationPtr relation);

  void _deleteAll(const QString tableName, const QString idFieldName, const long id);

  void _createTags(const Tags& tags, ElementId eid);
  QStringList _tagTableNamesForElement(ElementId eid) const;
  void _deleteAllTags(ElementId eid);

  void _createWayNodes(const long wayId, const std::vector<long>& nodeIds);

  void _createRelationMembers(const long relationId, const QString type,
                              const vector<RelationData::Entry>& members);

  long _changesetId;
  QSqlDatabase _db;
  QFile _outputSql;
  QHash<QString, shared_ptr<QSqlQuery> > _seqQueries;

  Tgs::BigMap<long, long> _idMappingsNode;
  Tgs::BigMap<long, long> _idMappingsWay;
  Tgs::BigMap<long, long> _idMappingsRelation;

};

}

#endif // OSMCHANGEWRITERSQL_H