#ifndef OSMCHANGESETSQLFILEWRITER_H
#define OSMCHANGESETSQLFILEWRITER_H

// Hoot
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/ElementType.h>

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
  void write(const QString path, ChangeSetProviderPtr changesetProvider);

private:

  void _createChangeSet();

  long _getNextId(const ElementType type);

  void _createNewElement(ElementPtr newElement);
  void _updateExistingElement(ElementPtr updatedElement);
  void _deleteExistingElement(ElementPtr removedElement);

  QString _getInsertValuesStr(const ConstElementPtr element) const;
  void _create(NodePtr node);
  QString _getInsertValuesStr(const ConstNodePtr node) const;
  void _create(WayPtr way);
  void _create(RelationPtr relation);
  QString _getInsertValuesWayOrRelationStr(const ConstElementPtr element) const;

  void _createTags(const ConstElementPtr element);
  QStringList _tagTableNamesForElement(const ElementId& eid) const;
  void _deleteCurrentTags(const ElementId& eid);

  void _createWayNodes(const ConstWayPtr way);

  void _createRelationMembers(const RelationPtr relation);

  void _modify(NodePtr node);
  void _modify(WayPtr way);
  void _modify(RelationPtr relation);

  void _deleteAll(const QString tableName, const QString idFieldName, const long id);

  QString _getVisibleStr(const bool visible) const { return visible ? "true" : "false"; }

  long _changesetId;
  long _nodeId;
  long _wayId;
  long _relationId;

  OsmApiDb _db;
  QFile _outputSql;

  //having to keep track of element versions with this may go away with some changeset
  //derivation scrutiny (e.g. changeset with an element modify immediately followed by the same
  //element being deleted)
  QMap<ElementType::Type, QMap<long, long> > _changeElementIdsToVersionsByElementType;

  friend class OsmChangeWriterSqlTest;

};

}

#endif // OSMCHANGESETSQLFILEWRITER_H