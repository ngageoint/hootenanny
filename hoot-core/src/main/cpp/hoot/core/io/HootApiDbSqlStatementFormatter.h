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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HOOTAPIDBSQLSTATEMENTFORMATTER_H
#define HOOTAPIDBSQLSTATEMENTFORMATTER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/io/HootApiDb.h>

// Qt
#include <QMap>
#include <QStringList>

namespace hoot
{

//These match up exclusively with the hoot api database and shouldn't be changed:

static const QString CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\t%10\n";
static const QString CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n";
static const QString CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\t\\N\n";
static const QString CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\t\\N\n";
static const QString CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\n";

/**
 * Converts OSM elements and their children into executable Postgres SQL COPY statements against an
 * Hootenanny API database
 *
 * TODO: It may be possible to reduce duplicated code by sharing some with
 * OsmApiDbSqlStatementFormatter.
 */
class HootApiDbSqlStatementFormatter
{

public:

  HootApiDbSqlStatementFormatter(const QString delimiter, const long mapId);

  QStringList nodeToSqlStrings(const ConstNodePtr& node, const long nodeId, const long changesetId,
                               const bool validate = false);
  QStringList wayToSqlStrings(const long wayId, const long changesetId, const Tags& tags);
  QStringList wayNodeToSqlStrings(const long wayId, const long wayNodeId,
                                  const unsigned int wayNodeIndex);
  QStringList relationToSqlStrings(const long relationId, const long changesetId, const Tags& tags);
  QStringList relationMemberToSqlStrings(const long relationId, const long memberId,
                                         const RelationData::Entry& member,
                                         const unsigned int memberSequenceIndex);
  QString changesetToSqlString(const long changesetId, const long changesetUserId,
                               const long numChangesInChangeset,
                               const geos::geom::Envelope& changesetBounds);
  QStringList elementToSqlStrings(const ConstElementPtr& element, const long elementId,
                                  const long changesetId);

  inline QString _escapeCopyToData(const QString stringToOutput)
  {
    //TODO: this is likely redundant with other code

    QString escapedString;
    escapedString.reserve(50);
    escapedString.append(stringToOutput);
    // Escape any special characters as required by
    //    http://www.postgresql.org/docs/9.2/static/sql-copy.html
    escapedString.replace(QChar(92), QString("\\\\"));  // Escape single backslashes first
    escapedString.replace(QChar(8), QString("\\b"));
    escapedString.replace(QChar(9), QString("\\t"));
    escapedString.replace(QChar(10), QString("\\n"));
    escapedString.replace(QChar(11), QString("\\v"));
    escapedString.replace(QChar(12), QString("\\f"));
    escapedString.replace(QChar(13), QString("\\r"));
    return escapedString;
  }

  // WITH NULL AS ''
  inline static QStringList getNodeSqlHeaderStrings(const long mapId)
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + HootApiDb::getCurrentNodesTableName(mapId) +
                   " (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, tags) " +
                   "FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getWaySqlHeaderStrings(const long mapId)
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + HootApiDb::getCurrentWaysTableName(mapId) +
                   " (id, changeset_id, \"timestamp\", visible, version, tags) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getWayNodeSqlHeaderStrings(const long mapId)
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + HootApiDb::getCurrentWayNodesTableName(mapId) +
                   " (way_id, node_id, sequence_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getRelationSqlHeaderStrings(const long mapId)
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + HootApiDb::getCurrentRelationsTableName(mapId) +
                   " (id, changeset_id, \"timestamp\", visible, version, tags) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getRelationMemberSqlHeaderStrings(const long mapId)
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + HootApiDb::getCurrentRelationMembersTableName(mapId) +
                   " (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QString getChangesetSqlHeaderString(const long mapId)
  {
    return "COPY " + HootApiDb::getChangesetsTableName(mapId) +
            " (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes, tags) " +
            "FROM stdin;\n";
  }

 inline static QStringList getElementSqlHeaderStrings(const ElementType& elementType, const long mapId)
  {
    switch (elementType.getEnum())
    {
      case ElementType::Node:
        return getNodeSqlHeaderStrings(mapId);

      case ElementType::Way:
        return getWaySqlHeaderStrings(mapId);

      case ElementType::Relation:
        return getRelationSqlHeaderStrings(mapId);

      default:
        throw HootException("Unsupported element member type.");
    }
    return QStringList();
  }

private:

  QMap<QString, QString> _outputFormatStrings;
  QString _dateString;\
  int _precision;
  long _mapId;

  void _initOutputFormatStrings(const QString delimiter);
  QString _toTagsString(const Tags& tags);
};

}

#endif // HOOTAPIDBSQLSTATEMENTFORMATTER_H
