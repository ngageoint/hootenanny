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
#include "OsmChangeWriterSql.h"

// hoot
#include <hoot/core/io/OsmWriter.h>
#include <hoot/core/io/HootApiDb.h>
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

  QString values = QString("latitude, longitude, changeset_id, visible, \"timestamp\", "
      "tile,  version) VALUES (%1, %2, %3, %4, true, now(), %5, 1);\n").
      arg(id).
      arg((qlonglong)HootApiDb::round(node->getY() * HootApiDb::COORDINATE_SCALE, 7)).
      arg((qlonglong)HootApiDb::round(node->getX() * HootApiDb::COORDINATE_SCALE, 7)).
      arg(_changesetId).
      arg(HootApiDb::tileForPoint(node->getY(), node->getX()));

  _outputSql.write(("INSERT INTO nodes (node_id, " + values).toUtf8());
  _outputSql.write(("INSERT INTO current_nodes (id, " + values).toUtf8());

  _createTags(node->getTags(), ElementId::node(id));
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
  QString tableName;

  QString tn1 = "current_" + eid.getType().toString().toLower() + "_tags";
  QString tn2 = eid.getType().toString().toLower() + "_tags";

  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString k = it.key();
    QString v = it.value();

    QString values1 = QString("(%1_id, k, v) VALUES (%2, '%3', '%4');\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    QString values2 = QString("(%1_id, k, v, version) VALUES (%2, '%3', '%4', 1);\n").
        arg(eid.getType().toString().toLower()).
        arg(eid.getId()).arg(k.replace('\'', "''")).
        arg(v.replace('\'', "''"));

    _outputSql.write((QString("INSERT INTO %1 ").arg(tn1) + values1).toUtf8());
    _outputSql.write((QString("INSERT INTO %1 ").arg(tn2) + values2).toUtf8());
  }
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

long OsmChangeWriterSql::_getNextId(const ElementType type)
{
  return _getNextId("current_" + type.toString().toLower() + "s");
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

void OsmChangeWriterSql::write(const QString path, const ChangeSetProviderPtr cs)
{
  _outputSql.setFileName(path);
  if (_outputSql.open(QIODevice::WriteOnly | QIODevice::Text) == false)
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  _outputSql.write("BEGIN TRANSACTION;\n");

  int changes = 0;
  _createChangeSet();
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
    case Change::Unknown:

      break;
    }
    changes++;

    if (changes > 40000)
    {
      _createChangeSet();
      changes = 0;
    }
  }

  _outputSql.write("COMMIT;\n");
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

void OsmChangeWriterSql::_deleteExistingElement(const ConstElementPtr /*removedElement*/)
{
  throw NotImplementedException("Deleting existing element not supported");
}

void OsmChangeWriterSql::_modify(const ConstNodePtr /*node*/)
{

}

void OsmChangeWriterSql::_modify(const ConstWayPtr /*way*/)
{
  // Modify contents of way
}

void OsmChangeWriterSql::_modify(const ConstRelationPtr /*relation*/)
{
  throw NotImplementedException("Modifying relations not yet supported");
}

long OsmChangeWriterSql::_getLatestVersion(const ConstElementPtr /*element*/)
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
