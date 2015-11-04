#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
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

}

void OsmChangeWriterSql::_updateExistingElement(const ConstElementPtr updatedElement)
{
  //_modify(updatedElement);



  switch ( updatedElement->getElementType().getEnum())
  {
  case ElementType::Node:
  {
    _modify(dynamic_pointer_cast<const Node>(updatedElement));
    break;
  }
  case ElementType::Way:
    //_modify(dynamic_pointer_cast<const ConstWayPtr>(updatedElement));
    throw NotImplementedException("Updating way not implemented");
    break;
  case ElementType::Relation:
    throw NotImplementedException("Updating relation not implemented");
    break;
  case ElementType::Unknown:
  default:
    throw HootException("Unknown element type");
    break;
  }
}

void OsmChangeWriterSql::_deleteExistingElement(const ConstElementPtr removedElement)
{
  throw NotImplementedException("Deleting existing element not supported");
}

void OsmChangeWriterSql::_modify(const ConstNodePtr node)
{
  ;
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

}
