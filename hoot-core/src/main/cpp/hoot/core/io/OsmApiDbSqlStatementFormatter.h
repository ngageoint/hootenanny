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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef OSMAPIDBSQLSTATEMENTFORMATTER_H
#define OSMAPIDBSQLSTATEMENTFORMATTER_H

// Hoot
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>

// Qt
#include <QMap>

namespace hoot
{

//These match up exclusively with the v0.6 OSM API database and shouldn't be changed:

static const QString CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8\t%9\n";
static const QString CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\n";
static const QString HISTORICAL_NODES_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\tt\t%5\t%6\t1\t\\N\n";
static const QString CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_WAYS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t1\t%3\n";
static const QString CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\tt\t1\n";
static const QString HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\tt\t\\N\n";
static const QString CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t%4\t%5\n";
static const QString HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT =
  "%1\t%2\t%3\t%4\t1\t%5\n";
static const QString CURRENT_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\n";
static const QString HISTORICAL_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t%2\t%3\t1\n";
//for whatever strange reason, the historical node tags table column order in the API datbase
//is different than the other historical tags tables; this makes a difference when using the
//offline loader, since it is sensitive to ordering
static const QString HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING_DEFAULT = "%1\t1\t%2\t%3\n";

class OsmApiDbSqlStatementFormatter
{

public:

  OsmApiDbSqlStatementFormatter(const QString delimiter);

  QStringList nodeToSqlStrings(const ConstNodePtr& node, const long nodeId, const long changesetId);
  QStringList wayToSqlStrings(const long wayId, const long changesetId);
  QStringList wayNodeToSqlStrings(const long wayId, const long wayNodeId,
                                  const unsigned int wayNodeIndex);
  QStringList relationToSqlStrings(const long relationId, const long changesetId);
  QStringList relationMemberToSqlStrings(const long relationId, const long memberId,
                                         const RelationData::Entry& member,
                                         const unsigned int memberSequenceIndex);
  QStringList tagToSqlStrings(const ElementId& elementId, const QString tagKey,
                              const QString tagValue);
  QString changesetToSqlString(const long changesetId, const long changesetUserId,
                               const long numChangesInChangeset, const Envelope& changesetBounds);

  QStringList getNodeSqlHeaderStrings() const;
  QStringList getWaySqlHeaderStrings() const;
  QStringList getWayNodeSqlHeaderStrings() const;
  QStringList getRelationSqlHeaderStrings() const;
  QStringList getRelationMemberSqlHeaderStrings() const;
  QStringList getNodeTagSqlHeaderStrings() const;
  QStringList getWayTagSqlHeaderStrings() const;
  QStringList getRelationTagSqlHeaderStrings() const;
  QString getChangesetSqlHeaderString() const;

private:

  QMap<QString, QString> _outputFormatStrings;

  void _initOutputFormatStrings(const QString delimiter);
  static unsigned int _convertDegreesToNanodegrees(const double degrees);
  static QString _escapeCopyToData(const QString stringToOutput);
};

}

#endif // OSMAPIDBSQLSTATEMENTFORMATTER_H
