#ifndef OSMCHANGEWRITERSQL_H
#define OSMCHANGEWRITERSQL_H

#include <hoot/core/io/ChangesetProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <QString>
#include <QFile>
#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Element.h>

namespace hoot
{

class OsmChangeWriterSql
{
public:
  OsmChangeWriterSql(url);

  // Jason
  void write(const QString& path, const ChangeSetProviderPtr cs);


private:
  QFile _outputSql;

  Tgs::BigMap<long, long> _idMappingsNode;
  Tgs::BigMap<long, long> _idMappingsWay;
  Tgs::BigMap<long, long> _idMappingsRelation;

  // jason
  void _writeNewElement(const ConstElementPtr newElement);
  // Terry
  void _updateExistingElement(const ConstElementPtr updatedElement);
  // terry
  void _deleteExistingElement(const ConstElementPtr removedElement);
  // jason
  long _getNextId(const ElementType type);

  // jason
  void _create(const ConstNodePtr node);
  void _create(const ConstWayPtr node);
  void _create(const ConstRelationPtr node);

  // jason
  void _modify(const ConstNodePtr node);
  void _modify(const ConstWayPtr node);
  void _modify(const ConstRelationPtr node);

  // jason
  void _delete(const ConstNodePtr node);
  void _delete(const ConstWayPtr node);
  void _delete(const ConstRelationPtr node);

  void _open(QString url);

};

}

#endif // OSMCHANGEWRITERSQL_H
