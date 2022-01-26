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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef OSMAPIDBSQLCHANGESETFILEWRITER_H
#define OSMAPIDBSQLCHANGESETFILEWRITER_H

// Hoot
#include <hoot/core/io/OsmApiDb.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/io/OsmChangesetFileWriter.h>

// Qt
#include <QUrl>
#include <QtSql/QSqlDatabase>

namespace hoot
{

/**
 * Writes out a set of SQL commands, that when executed, will update the contents of
 * an OSM API database with an OSM changeset.
 *
 * This is useful when you want to work with changesets and  don't want a dependency on the OSM API
 * app. Changesets also apply more quickly due to the lack of HTTP overhead.
 */
class OsmApiDbSqlChangesetFileWriter : public OsmChangesetFileWriter
{

public:

  static QString className() { return "OsmApiDbSqlChangesetFileWriter"; }

  OsmApiDbSqlChangesetFileWriter();
  OsmApiDbSqlChangesetFileWriter(const QUrl& url);
  ~OsmApiDbSqlChangesetFileWriter() override;

  /**
   * @see ChangesetFileWriter
   */
  void write(const QString& path, const ChangesetProviderPtr& changesetProvider) override;

  /**
   * @see ChangesetFileWriter
   */
  void write(const QString& path, const QList<ChangesetProviderPtr>& changesetProviders) override;

  /**
   * @see ChangesetFileWriter
   */
  QString getStatsTable(const ChangesetStatsFormat& /*format = StatisticsFormat::Text*/) const override
  { throw NotImplementedException("Changeset statistics not implemented for SQL changesets."); }

  void setMap1List(const QList<ConstOsmMapPtr>& mapList) override { _map1List = mapList; }
  void setMap2List(const QList<ConstOsmMapPtr>& mapList) override { _map2List = mapList; }

  /**
   * @see ChangesetFileWriter
   */
  bool isSupported(const QString& output) const override { return output.endsWith(".osc.sql"); }

  /**
   * @see Configurable
   */
  void setConfiguration(const Settings &conf) override;

  void setOsmApiDbUrl(const QString& url) { _db.open(url); }

private:

  OsmApiDb _db;
  QFile _outputSql;

  long _changesetId;
  geos::geom::Envelope _changesetBounds;

  /** Settings from the config file */
  double _changesetUserId;

  // id mappings for created elements
  QMap<ElementId, ElementId> _remappedIds;

  // element IDs associated with a changes encountered
  QList<ElementId> _parsedChangeIds;

  friend class ServiceOsmApiDbSqlChangesetFileWriterTest;

  QString _getVisibleStr(const bool visible) const { return visible ? "true" : "false"; }

  void _setChangesetUserId(long id) { _changesetUserId = id; }

  void _createChangeSet();
  void _updateChangeset(const int numChanges);

  long _getNextId(const ElementType type);

  //clones the input so local element version tracking can be done
  ElementPtr _getChangeElement(ConstElementPtr element) const;

  // All of the create methods here assume you've already set the ID correctly in terms of
  // preventing conflicts with the OSM API target db for the element to be created.  The one
  // exception is for new elements with negative ids.

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
  void _deleteAll(const QString& tableName, const QString& idFieldName, const long id);
};

}

#endif // OSMAPIDBSQLCHANGESETFILEWRITER_H
