#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/io/ServicesDb.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>
#include <QXmlStreamWriter>

namespace hoot
{

OsmChangeWriterSql::OsmChangeWriterSql(QUrl url)
{
  _open(url);
}

void OsmChangeWriterSql::_create(const ConstNodePtr node)
{
  long id = _getNextId(ElementType::Node);

  QString values = QString("(node_id, latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile,  version) VALUES (%1, %2, %3, %4, true, '%5', %6, 1);\n").
      arg(id).
      arg((qlonglong)ServicesDb::round(node->getY() * ServicesDb::COORDINATE_SCALE, 7)).
      arg((qlonglong)ServicesDb::round(node->getX() * ServicesDb::COORDINATE_SCALE, 7)).
      arg(_changesetId).arg(OsmUtils::toTimeString(node->getTimestamp())).
      arg(ServicesDb::tileForPoint(node->getY(), node->getX()));

  _outputSql.write(("INSERT INTO nodes " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes " + values).toUtf8());
}

long OsmChangeWriterSql::_getNextId(const ElementType type)
{
  long result;
  if (_seqQueries[type.getEnum()].get() == 0)
  {
    _seqQueries[type.getEnum()].reset(new QSqlQuery(_db));
    _seqQueries[type.getEnum()]->setForwardOnly(true);
    _seqQueries[type.getEnum()]->prepare(
      QString("SELECT NEXTVAL('current_%1s_id_seq')").arg(type.toString().toLower()));
  }

  shared_ptr<QSqlQuery> query = _seqQueries[type.getEnum()];
  if (query->exec() == false)
  {
    throw HootException("Error reserving IDs. type: " +
      type.toString() + " Error: " + query->lastError().text());
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
      type.toString() + " Error: " + query->lastError().text());
  }

  query->finish();

  return result;

}

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

void OsmChangeWriterSql::write(const QString& path, const ChangeSetProviderPtr cs)
{
  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  while (cs->hasMoreChanges())
  {
    Change c = cs->readNextChange();
    switch (c.type)
    {
    case Change::Create:
      _writeNewElement(c.e);
      break;
    case Change::Delete:
      //_deleteExistingElement(c.e);
      break;
    case Change::Modify:
      //_updateExistingElement(c.e);
      break;
    }
  }
}

void OsmChangeWriterSql::_updateExistingElement(const ConstElementPtr updatedElement)
{
  //_modify(updatedElement);
}

void OsmChangeWriterSql::_deleteExistingElement(const ConstElementPtr removedElement)
{
  throw NotImplementedException("Deleting existing element not supported");
}

void OsmChangeWriterSql::_modify(const ConstWayPtr way)
{
  // Modify contents of way
}

void OsmChangeWriterSql::_modify(const ConstRelationPtr relation)
{
  throw NotImplementedException("Modifying relations not yet supported");
}

long OsmChangeWriterSql::_getLatestVersion(const ConstElementPtr element)
{
  throw NotImplementedException("Getting latest version not implemented");
  return -1;
}

void OsmChangeWriterSql::_writeNewElement(const ConstElementPtr newElement)
{
  switch (newElement->getElementType().getEnum())
  {
  case ElementType::Node:
    _create(dynamic_pointer_cast<const Node>(newElement));
    break;
  default:
    LOG_WARN("Not implemented");
  }
}

}
