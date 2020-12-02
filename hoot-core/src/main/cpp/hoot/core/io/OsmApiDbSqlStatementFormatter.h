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
 * @copyright Copyright (C) 2016, 2017, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBSQLSTATEMENTFORMATTER_H
#define OSMAPIDBSQLSTATEMENTFORMATTER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/io/ApiDbSqlStatementFormatter.h>

// Qt
#include <QMap>
#include <QStringList>

namespace hoot
{

//These match up exclusively with the v0.6 OSM API database and shouldn't be changed:

static const QString OSMAPIDB_CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\n";
static const QString OSMAPIDB_CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t%5\t%6\t%7\n";
static const QString OSMAPIDB_HISTORICAL_NODES_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\tt\t%5\t%6\t%7\t\\N\n";
static const QString OSMAPIDB_CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t%4\n";
static const QString OSMAPIDB_HISTORICAL_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t\\N\n";
static const QString OSMAPIDB_CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString OSMAPIDB_HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\n";
static const QString OSMAPIDB_CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t%4\n";
static const QString OSMAPIDB_HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t\\N\n";
static const QString OSMAPIDB_CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\n";
static const QString OSMAPIDB_HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t%5\t%6\n";
static const QString OSMAPIDB_CURRENT_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString OSMAPIDB_HISTORICAL_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\n";
//for whatever strange reason, the historical node tags table column order in the API datbase
//is different than the other historical tags tables; this makes a difference when using the
//offline loader, since it is sensitive to ordering
static const QString OSMAPIDB_HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\n";

/**
 * Converts OSM elements and their children into executable Postgres SQL COPY statements against an
 * OSM API database
 */
class OsmApiDbSqlStatementFormatter : public ApiDbSqlStatementFormatter
{

public:

  OsmApiDbSqlStatementFormatter(const QString& delimiter);

  QStringList nodeToSqlStrings(const ConstNodePtr& node, const long nodeId, const long changesetId,
                               const bool validate = false);
  QStringList wayToSqlStrings(const long wayId, const long changesetId, const long version);
  QStringList wayNodeToSqlStrings(const long wayId, const long wayNodeId,
                                  const unsigned int wayNodeIndex, const long version);
  QStringList relationToSqlStrings(const long relationId, const long changesetId, const long version);
  QStringList relationMemberToSqlStrings(const long relationId, const long memberId,
                                         const RelationData::Entry& member,
                                         const unsigned int memberSequenceIndex, const unsigned int version);
  QStringList tagToSqlStrings(const long elementId, const ElementType& elementType,
                              const QString& tagKey, const QString& tagValue, const long version);
  QString changesetToSqlString(const long changesetId, const long changesetUserId,
                               const long numChangesInChangeset,
                               const geos::geom::Envelope& changesetBounds);
  QStringList elementToSqlStrings(const ConstElementPtr& element, const long elementId,
                                  const long changesetId);

  static QString escapeCopyToData(const QString& stringToOutput);

  inline unsigned int _convertDegreesToNanodegrees(const double degrees)
  {
    return round(degrees * ApiDb::COORDINATE_SCALE);
  }

  inline static QStringList getNodeSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentNodesTableName() +
                   " (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version) " +
                   "FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getNodesTableName() +
                   " (node_id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, " +
                   "redaction_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getWaySqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentWaysTableName() +
                   " (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getWaysTableName() +
                   " (way_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getWayNodeSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentWayNodesTableName() +
                   " (way_id, node_id, sequence_id) FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getWayNodesTableName() +
                   " (way_id, node_id, version, sequence_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getRelationSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentRelationsTableName() +
                   " (id, changeset_id, \"timestamp\", visible, version) FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getRelationsTableName() +
                   " (relation_id, changeset_id, \"timestamp\", version, visible, redaction_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getRelationMemberSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentRelationMembersTableName() +
                   " (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getRelationMembersTableName() +
                   " (relation_id, member_type, member_id, member_role, version, sequence_id) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getNodeTagSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentNodeTagsTableName() + " (node_id, k, v) FROM stdin;\n");
    //yes, this one is different than the others...see explanation in header file
    sqlStrs.append("COPY " + ApiDb::getNodeTagsTableName() + " (node_id, version, k, v) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getWayTagSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append("COPY " + ApiDb::getCurrentWayTagsTableName() + " (way_id, k, v) FROM stdin;\n");
    sqlStrs.append("COPY " + ApiDb::getWayTagsTableName() + " (way_id, k, v, version) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QStringList getRelationTagSqlHeaderStrings()
  {
    QStringList sqlStrs;
    sqlStrs.append(
      "COPY " + ApiDb::getCurrentRelationTagsTableName() + " (relation_id, k, v) FROM stdin;\n");
    sqlStrs.append(
      "COPY " + ApiDb::getRelationTagsTableName() + " (relation_id, k, v, version) FROM stdin;\n");
    return sqlStrs;
  }

  inline static QString getChangesetSqlHeaderString()
  {
    return "COPY " + ApiDb::getChangesetsTableName() +
            " (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes) " +
            "FROM stdin;\n";
  }

 inline static QStringList getElementSqlHeaderStrings(const ElementType& elementType)
  {
    switch (elementType.getEnum())
    {
      case ElementType::Node:
        return getNodeSqlHeaderStrings();

      case ElementType::Way:
        return getWaySqlHeaderStrings();

      case ElementType::Relation:
        return getRelationSqlHeaderStrings();

      default:
        throw HootException("Unsupported element member type.");
    }
    return QStringList();
  }

protected:

  void _initOutputFormatStrings(const QString& delimiter) override;
};

}

#endif // OSMAPIDBSQLSTATEMENTFORMATTER_H
