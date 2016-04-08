#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/HootApiDb.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QSqlError>
#include <QSqlQuery>

namespace hoot
{

OsmChangeWriterSql::OsmChangeWriterSql(QUrl url)
{
  _open(url);
}

void OsmChangeWriterSql::write(const QString path, const ChangeSetProviderPtr changesetProvider)
{
  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  _outputSql.write("BEGIN TRANSACTION;\n");

  int changes = 0;
  _createChangeSet();
  while (changesetProvider->hasMoreChanges())
  {
    Change change = changesetProvider->readNextChange();
    switch (change.type)
    {
      case Change::Create:
        _writeNewElement(change.e);
        break;
      case Change::Modify:
        _updateExistingElement(change.e);
        break;
      case Change::Delete:
        _deleteExistingElement(change.e);
        break;
    }
    changes++;

    if (changes > 40000) //TODO: move to config
    {
      _createChangeSet();
      changes = 0;
    }
  }

  _outputSql.write("COMMIT;\n");
}

//long OsmChangeWriterSql::_getLatestVersion(const ConstElementPtr /*element*/)
//{
  ////TODO: ??
  //throw NotImplementedException("Getting latest version not implemented");
  //return -1;
//}

void OsmChangeWriterSql::_open(QUrl url)
{
  QStringList pList = url.path().split("/");
  QString db = pList[1];

  QString connectionName = url.toString() + " 0x" + QString::number((qulonglong)this, 16);
  if (QSqlDatabase::contains(connectionName) == false)
  {
    _db = QSqlDatabase::addDatabase("QPSQL", connectionName);
  }
  else
  {
    _db = QSqlDatabase::database(connectionName);
  }

  if (_db.isOpen() == false)
  {
    _db.setDatabaseName(db);
    if (url.host() == "local")
    {
      _db.setHostName("/var/run/postgresql");
    }
    else
    {
      _db.setHostName(url.host());
    }
    _db.setPort(url.port());
    _db.setUserName(url.userName());
    _db.setPassword(url.password());

    if (_db.open() == false)
    {
      throw HootException("Error opening database: " + _db.lastError().text());
    }
  }

  if (_db.tables().size() == 0)
  {
    throw HootException("Attempting to open database " + url.toString() +
                        " but found zero tables. Does the DB exist? Has it been populated?");
  }
}

long OsmChangeWriterSql::_getNextId(const ElementType type)
{
  return _getNextId("current_" + type.toString().toLower() + "s");
}

long OsmChangeWriterSql::_getNextId(QString type)
{
  long result;
  if (_seqQueries[type].get() == 0)
  {
    _seqQueries[type].reset(new QSqlQuery(_db));
    _seqQueries[type]->setForwardOnly(true);
    _seqQueries[type]->prepare(
      QString("SELECT NEXTVAL('%1_id_seq')").arg(type.toLower()));
  }

  shared_ptr<QSqlQuery> query = _seqQueries[type];
  if (query->exec() == false)
  {
    throw HootException("Error reserving IDs. type: " +
      type + " Error: " + query->lastError().text());
  }

  if (query->next())
  {
    bool ok;
    result = query->value(0).toLongLong(&ok);
    if (!ok)
    {
      throw HootException("Did not retrieve starting reserved ID.");
    }
  }
  else
  {
    throw HootException("Error retrieving sequence value. type: " +
      type + " Error: " + query->lastError().text());
  }

  query->finish();

  return result;
}

long OsmChangeWriterSql::_createChangeSet()
{
  long id = _getNextId("changesets");

  _changesetId = id;

  _outputSql.write(QString("INSERT INTO changesets (id, user_id, created_at, closed_at) VALUES "
                   "(%1, %2, now(), now());\n").arg(id).
                   arg(ConfigOptions().getChangesetUserId()).toUtf8());

  return id;
}

void OsmChangeWriterSql::_createTags(const Tags& tags, ElementId eid)
{
  QString tn1 = "current_" + eid.getType().toString().toLower() + "_tags";
  QString tn2 = eid.getType().toString().toLower() + "_tags";

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    QString values1 =
      QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    QString values2 =
      QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', 1);\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    _outputSql.write((QString("INSERT INTO %1 ").arg(tn1) + values1).toUtf8());
    _outputSql.write((QString("INSERT INTO %1 ").arg(tn2) + values2).toUtf8());
  }
}

void OsmChangeWriterSql::_writeNewElement(const ConstElementPtr newElement)
{
  switch (newElement->getElementType().getEnum())
  {
    case ElementType::Node:
      _create(dynamic_pointer_cast<const Node>(newElement));
      break;
    case ElementType::Way:
      _create(dynamic_pointer_cast<const Way>(newElement));
      break;
    case ElementType::Relation:
      _create(dynamic_pointer_cast<const Relation>(newElement));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangeWriterSql::_updateExistingElement(const ConstElementPtr updatedElement)
{
  switch (updatedElement->getElementType().getEnum())
  {
    case ElementType::Node:
      _modify(dynamic_pointer_cast<const Node>(updatedElement));
      break;
    case ElementType::Way:
      _modify(dynamic_pointer_cast<const Way>(updatedElement));
      break;
   case ElementType::Relation:
      _modify(dynamic_pointer_cast<const Relation>(updatedElement));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangeWriterSql::_deleteExistingElement(const ConstElementPtr removedElement)
{
  switch (removedElement->getElementType().getEnum())
  {
    case ElementType::Node:
      _delete(dynamic_pointer_cast<const Node>(removedElement));
      break;
    case ElementType::Way:
      _delete(dynamic_pointer_cast<const Way>(removedElement));
      break;
    case ElementType::Relation:
      _delete(dynamic_pointer_cast<const Relation>(removedElement));
      break;
    default:
      throw HootException("Unknown element type");
  }
}

void OsmChangeWriterSql::_create(const ConstNodePtr node)
{
  const long id = _getNextId(ElementType::Node);

  QString values =
    QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile,  version) VALUES (%1, %2, %3, %4, true, now(), %5, 1);\n")
      .arg(id)
      .arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7))
      .arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7))
      .arg(_changesetId)
      .arg(HootApiDb::tileForPoint(node->getY(), node->getX()));
  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes (id, " + values).toUtf8());

  _createTags(node->getTags(), ElementId::node(id));
}

void OsmChangeWriterSql::_create(const ConstWayPtr way)
{
  const long id = _getNextId(ElementType::Way);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO way (way_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_ways (id, " + values).toUtf8());

  values = "";
  const std::vector<long>& nodeIds = way->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    const long nodeId = nodeIds.at(i);

    values =
      QString("way_id, node_id, version sequence_id) VALUES (%1, %2, 1, %3);\n")
        .arg(id)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO nodes (way_nodes, " + values).toUtf8());

    values =
      QString("way_id, node_id, sequence_id) VALUES (%1, %2, %3);\n")
        .arg(id)
        .arg(nodeId)
        .arg(i + 1);
    _outputSql.write(("INSERT INTO nodes (current_way_nodes, " + values).toUtf8());
  }

  _createTags(way->getTags(), ElementId::way(id));
}

void OsmChangeWriterSql::_create(const ConstRelationPtr relation)
{
  /*const long id = _getNextId(ElementType::Relation);

  QString values =
    QString("changeset_id, visible, \"timestamp\", "
      "version) VALUES (%1, %2, true, now(), 1);\n")
      .arg(id)
      .arg(_changesetId);
  _outputSql.write(("INSERT INTO relations (relation_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_relations (id, " + values).toUtf8());

  values = "";
  const std::vector<long>& nodeIds = relati
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    values +=
      QString("way_id, node_id, sequence_id);\n")
        .arg(id)
        .arg(nodeIds.at(i))
        .arg(i + 1);
    _outputSql.write(("INSERT INTO current_way_nodes (id, " + values).toUtf8());
  }

  _createTags(relation->getTags(), ElementId::relation(id));*/
}

void OsmChangeWriterSql::_modify(const ConstNodePtr /*node*/)
{
  throw NotImplementedException("Updating node not implemented");
}

void OsmChangeWriterSql::_modify(const ConstWayPtr /*way*/)
{
  throw NotImplementedException("Updating way not implemented");
}

void OsmChangeWriterSql::_modify(const ConstRelationPtr /*relation*/)
{
  throw NotImplementedException("Updating relation not implemented");
}

void OsmChangeWriterSql::_delete(const ConstNodePtr /*node*/)
{
  throw NotImplementedException("Deleting node not implemented");
}

void OsmChangeWriterSql::_delete(const ConstWayPtr /*way*/)
{
  throw NotImplementedException("Deleting way not implemented");
}

void OsmChangeWriterSql::_delete(const ConstRelationPtr /*relation*/)
{
  throw NotImplementedException("Deleting relation not implemented");
}

}