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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
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
#include <QFile>
#include <QtSql/QSqlDatabase>
#include <QString>
#include <QSet>

namespace hoot
{

/**
 * Writes out a set of SQL commands, that when executed, will update the contents of
 * an OSM API database with an OSM changeset.
 */
class OsmApiDbSqlChangesetFileWriter : public OsmChangesetFileWriter
{

public:

  static std::string className() { return "hoot::OsmApiDbSqlChangesetFileWriter"; }

  OsmApiDbSqlChangesetFileWriter();
  OsmApiDbSqlChangesetFileWriter(const QUrl& url);
  virtual ~OsmApiDbSqlChangesetFileWriter();

  /**
   * @see ChangesetFileWriter
   */
  virtual void write(const QString& path, const ChangesetProviderPtr& changesetProvider);

  /**
   * @see ChangesetFileWriter
   */
  virtual void write(const QString& path, const QList<ChangesetProviderPtr>& changesetProviders);

  /**
   * @see ChangesetFileWriter
   */
  virtual QString getStatsTable(
    const ChangesetStatsFormat& /*format = StatisticsFormat::Text*/) const
  { throw NotImplementedException("Changeset statistics not implemented for SQL changesets."); }

  /**
   * @see ChangesetFileWriter
   */
  virtual bool isSupported(const QString& output) const { return output.endsWith(".osc.sql"); }

  /**
   * @see Configurable
   */
  virtual void setConfiguration(const Settings &conf);

  void setOsmApiDbUrl(const QString& url) { _db.open(url); }

private:

  void _createChangeSet();
  void _updateChangeset(const int numChanges);

  long _getNextId(const ElementType type);

  //clones the input so local element version tracking can be done
  ElementPtr _getChangeElement(ConstElementPtr element);

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

  QString _getVisibleStr(const bool visible) const { return visible ? "true" : "false"; }

  void setChangesetUserId(long id) { _changesetUserId = id; }

  OsmApiDb _db;
  QFile _outputSql;

  long _changesetId;
  geos::geom::Envelope _changesetBounds;

  /** Settings from the config file */
  double _changesetUserId;

  bool _includeDebugTags;
  bool _includeCircularErrorTags;

  // id mappings for created elements
  QMap<ElementId, ElementId> _remappedIds;

  QList<Change> _parsedChanges;

  friend class ServiceOsmApiDbSqlChangesetFileWriterTest;
};

}

#endif // OSMAPIDBSQLCHANGESETFILEWRITER_H
