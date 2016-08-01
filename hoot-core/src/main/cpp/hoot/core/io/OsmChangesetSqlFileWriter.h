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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
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
#include <QSet>

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
  ~OsmChangesetSqlFileWriter();

  /**
   * Write a SQL changeset to the specified output path
   *
   * @param path SQL file output path
   * @param changesetProvider changeset data
   */
  void write(const QString path, ChangeSetProviderPtr changesetProvider);

private:

  void _createChangeSet();
  void _updateChangeset(const int numChanges);

  long _getNextId(const ElementType type);

  //clones the input so local element version tracking can be done
  ElementPtr _getChangeElement(ConstElementPtr element);

  void _createNewElement(ConstElementPtr newElement);
  QString _getInsertValuesStr(ConstElementPtr element) const;
  QString _getInsertValuesNodeStr(ConstNodePtr node) const;
  QString _getInsertValuesWayOrRelationStr(ConstElementPtr element) const;

  void _createTags(ConstElementPtr element);
  QStringList _tagTableNamesForElement(const ElementId& eid) const;
  void _deleteCurrentTags(const ElementId& eid);
  void _createWayNodes(ConstWayPtr way);
  void _createRelationMembers(ConstRelationPtr relation);

  void _updateExistingElement(ConstElementPtr updatedElement);
  QString _getUpdateValuesStr(ConstElementPtr element) const;
  QString _getUpdateValuesNodeStr(ConstNodePtr node) const;
  QString _getUpdateValuesWayOrRelationStr(ConstElementPtr element) const;

  void _deleteExistingElement(ConstElementPtr removedElement);
  void _deleteAll(const QString tableName, const QString idFieldName, const long id);

  QString _getVisibleStr(const bool visible) const { return visible ? "true" : "false"; }

  OsmApiDb _db;
  QFile _outputSql;

  long _changesetId;
  Envelope _changesetBounds;

  friend class ServiceOsmApiDbChangesetSqlFileWriterTest;

};

}

#endif // OSMCHANGESETSQLFILEWRITER_H