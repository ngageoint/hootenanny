#ifndef OSMCHANGEWRITERSQL_H
#define OSMCHANGEWRITERSQL_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementType.h>
#include <tgs/BigContainers/BigMap.h>

// Qt
#include <QUrl>
#include <QFile>
#include <QSqlDatabase>
#include <QString>

namespace hoot
{

class OsmChangeWriterSql
{

public:

  OsmChangeWriterSql(QUrl url);

  void write(const QString path, const ChangeSetProviderPtr cs);

private:

  long _changesetId;
  QSqlDatabase _db;
  QFile _outputSql;
  QHash<QString, shared_ptr<QSqlQuery> > _seqQueries;

  Tgs::BigMap<long, long> _idMappingsNode;
  Tgs::BigMap<long, long> _idMappingsWay;
  Tgs::BigMap<long, long> _idMappingsRelation;

  void _writeNewElement(const ConstElementPtr newElement);
  void _updateExistingElement(const ConstElementPtr updatedElement);
  void _deleteExistingElement(const ConstElementPtr removedElement);
  long _getNextId(const ElementType type);
  long _getNextId(QString type);

  long _getLatestVersion(const ConstElementPtr element);

  void _create(const ConstNodePtr node);
  void _create(const ConstWayPtr way);
  void _create(const ConstRelationPtr relation);
  void _createTags(const Tags& tags, ElementId eid);

  long _createChangeSet();

  void _modify(const ConstNodePtr node);
  void _modify(const ConstWayPtr way);
  void _modify(const ConstRelationPtr relation);

  void _delete(const ConstNodePtr node);

  void _open(QUrl url);

  void _delete(const ConstWayPtr way);
  void _delete(const ConstRelationPtr relation);

};

}

#endif // OSMCHANGEWRITERSQL_H
