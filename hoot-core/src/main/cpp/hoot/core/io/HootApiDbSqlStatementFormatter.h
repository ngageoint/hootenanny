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
 * @copyright Copyright (C) 2017, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef HOOTAPIDBSQLSTATEMENTFORMATTER_H
#define HOOTAPIDBSQLSTATEMENTFORMATTER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/io/ApiDbSqlStatementFormatter.h>
#include <hoot/core/io/HootApiDb.h>

namespace hoot
{

//These match up exclusively with the hoot api database and shouldn't be changed:
static const QString HOOTAPIDB_CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\t%10\n";
static const QString HOOTAPIDB_CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t%5\t%6\t%7\t\\N\n";
static const QString HOOTAPIDB_CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t%4\t\\N\n";
static const QString HOOTAPIDB_CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString HOOTAPIDB_CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t%4\t\\N\n";
static const QString HOOTAPIDB_CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\n";

/**
 * Converts OSM elements and their children into executable Postgres SQL COPY statements against an
 * Hootenanny API database
 */
class HootApiDbSqlStatementFormatter : public ApiDbSqlStatementFormatter
{

public:

  HootApiDbSqlStatementFormatter(const QString& delimiter, const long mapId);
  ~HootApiDbSqlStatementFormatter() override = default;

  QString nodeToSqlString(const ConstNodePtr& node, const long nodeId, const long changesetId,
                          const long version, const bool validate = false);
  QString wayToSqlString(const long wayId, const long changesetId,
                         const Tags& tags, const long version);
  QString wayNodeToSqlString(const long wayId, const long wayNodeId,
                             const unsigned int wayNodeIndex);
  QString relationToSqlString(const long relationId, const long changesetId,
                              const Tags& tags,
                              const long version);
  QString relationMemberToSqlString(const long relationId, const long memberId,
                                    const RelationData::Entry& member,
                                    const unsigned int memberSequenceIndex,
                                    const unsigned long version);

  inline static QString getNodeSqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (id, latitude, longitude, changeset_id, visible, \"timestamp\", tile, version, tags) FROM stdin;\n")
            .arg(HootApiDb::getCurrentNodesTableName(mapId));
  }

  inline static QString getWaySqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (id, changeset_id, \"timestamp\", visible, version, tags) FROM stdin;\n")
            .arg(HootApiDb::getCurrentWaysTableName(mapId));
  }

  inline static QString getWayNodeSqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (way_id, node_id, sequence_id) FROM stdin;\n")
            .arg(HootApiDb::getCurrentWayNodesTableName(mapId));
  }

  inline static QString getRelationSqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (id, changeset_id, \"timestamp\", visible, version, tags) FROM stdin;\n")
            .arg(HootApiDb::getCurrentRelationsTableName(mapId));
  }

  inline static QString getRelationMemberSqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (relation_id, member_type, member_id, member_role, sequence_id) FROM stdin;\n")
        .arg(HootApiDb::getCurrentRelationMembersTableName(mapId));
  }

  inline static QString getChangesetSqlHeaderString(const long mapId)
  {
    return QString("COPY %1 (id, user_id, created_at, min_lat, max_lat, min_lon, max_lon, closed_at, num_changes, tags) FROM stdin;\n")
            .arg(HootApiDb::getChangesetsTableName(mapId));
  }

 inline static QString getElementSqlHeaderStrings(const ElementType& elementType, const long mapId)
  {
    switch (elementType.getEnum())
    {
    case ElementType::Node:
      return getNodeSqlHeaderString(mapId);
    case ElementType::Way:
      return getWaySqlHeaderString(mapId);
    case ElementType::Relation:
      return getRelationSqlHeaderString(mapId);
    default:
      throw HootException("Unsupported element member type.");
    }
    return "";
  }

protected:

  void _initOutputFormatStrings(const QString& delimiter) override;

private:

  int _precision;
  long _mapId;

  QString _toTagsString(const Tags& tags) const;
};

}

#endif // HOOTAPIDBSQLSTATEMENTFORMATTER_H
