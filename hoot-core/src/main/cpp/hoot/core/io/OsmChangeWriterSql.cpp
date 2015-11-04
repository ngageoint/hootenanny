#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QFile>
#include <QSqlError>
#include <QXmlStreamWriter>

namespace hoot
{

OsmChangeWriterSql::OsmChangeWriterSql(QUrl url)
{
  _open(url);
}

long OsmChangeWriterSql::_getNextId(const ElementType type)
{

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

}
