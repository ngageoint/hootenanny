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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "HootApiDbSqlStatementFormatter.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/io/OsmApiDbSqlStatementFormatter.h>

// Qt
#include <QDateTime>
#include <QStringBuilder>

namespace hoot
{

HootApiDbSqlStatementFormatter::HootApiDbSqlStatementFormatter(const QString& delimiter,
                                                               const long mapId)
  : _precision(ConfigOptions().getWriterPrecision()),
    _mapId(mapId)
{
  _initOutputFormatStrings(delimiter);
}

void HootApiDbSqlStatementFormatter::_initOutputFormatStrings(const QString& delimiter)
{
  QString formatString = HOOTAPIDB_CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getChangesetsTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = HOOTAPIDB_CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentNodesTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = HOOTAPIDB_CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentWaysTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = HOOTAPIDB_CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentWayNodesTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = HOOTAPIDB_CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentRelationsTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = HOOTAPIDB_CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentRelationMembersTableName(_mapId)] =
    formatString.replace("\t", delimiter);
}

QString HootApiDbSqlStatementFormatter::_toTagsString(const Tags& tags)
{
  QString tagsStr;
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key = it.key().trimmed();
    QString value = it.value().trimmed();
    if (!value.isEmpty())
    {
      key.replace("\\", "/");
      key.replace("'", "\'");
      key.replace("=>", "\\=\\>");
      key.replace("\"", "\\\"");
      tagsStr.append("\"" % key % "\"");

      tagsStr.append("=>");

      value.replace("\\", "/");
      value.replace("'", "\'");
      value.replace("=>", "\\=\\>");
      value.replace("\"", "\\\"");
      tagsStr.append("\"" % value % "\"");

      tagsStr.append(",");
    }
  }
  if (tagsStr.endsWith(","))
  {
    tagsStr.chop(1);
  }
  assert(tagsStr != "''");
  return tagsStr;
}

QString HootApiDbSqlStatementFormatter::nodeToSqlString(const ConstNodePtr& node, const long nodeId,
                                                        const long changesetId, const long version,
                                                        const bool validate)
{
  const QString nodeIdStr = QString::number(nodeId);
  if (validate)
  {
    const double nodeYNanodegrees = node->getY();
    const double nodeXNanodegrees = node->getX();
    if ((nodeYNanodegrees < -90.0) || (nodeYNanodegrees > 90.0))
    {
      throw HootException(
        QString("Invalid latitude conversion, Y = %1 to %2").arg(
          QString::number(node->getY()), QString::number(nodeYNanodegrees)));
    }
    if ((nodeXNanodegrees < -180.0) || (nodeXNanodegrees > 180.0))
    {
      throw HootException(
        QString("Invalid longitude conversion, X = %1 to %2").arg(
          QString::number(node->getX()), QString::number(nodeXNanodegrees)));
    }
  }
  const QString changesetIdStr = QString::number(changesetId);
  const QString tileNumberString(QString::number(ApiDb::tileForPoint(node->getY(), node->getX())));
  long ver = _useSourceVersion ? version : 1;

  QString nodeStr =
    _outputFormatStrings[HootApiDb::getCurrentNodesTableName(_mapId)]
      .arg(nodeIdStr)
      .arg(QString::number(node->getY(), 'g', _precision))
      .arg(QString::number(node->getX(), 'g', _precision))
      .arg(changesetIdStr)
      .arg(_dateString)
      .arg(tileNumberString)
      .arg(ver);
  if (node->getTags().size() > 0)
  {
    nodeStr.replace(
      "\\N", OsmApiDbSqlStatementFormatter::escapeCopyToData(_toTagsString(node->getTags())));
  }

  return nodeStr;
}

QString HootApiDbSqlStatementFormatter::wayToSqlString(const long wayId, const long changesetId,
                                                       const Tags& tags, const long version)
{
  long ver = _useSourceVersion ? version : 1;
  QString wayStr =
    _outputFormatStrings[HootApiDb::getCurrentWaysTableName(_mapId)]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver);
  if (tags.size() > 0)
  {
    wayStr.replace("\\N", OsmApiDbSqlStatementFormatter::escapeCopyToData(_toTagsString(tags)));
  }

  return wayStr;
}

QString HootApiDbSqlStatementFormatter::wayNodeToSqlString(const long wayId,
                                                           const long wayNodeId,
                                                           const unsigned int wayNodeIndex)
{
  const QString wayIdStr(QString::number(wayId));
  const QString wayNodeIdStr(QString::number(wayNodeId));
  const QString wayNodeIndexStr(QString::number(wayNodeIndex));
  return
    _outputFormatStrings[HootApiDb::getCurrentWayNodesTableName(_mapId)]
      .arg(wayIdStr, wayNodeIdStr, wayNodeIndexStr);
}

QString HootApiDbSqlStatementFormatter::relationToSqlString(const long relationId,
                                                            const long changesetId,
                                                            const Tags& tags,
                                                            const long version)
{
  long ver = _useSourceVersion ? version : 1;
  QString relationStr =
    _outputFormatStrings[HootApiDb::getCurrentRelationsTableName(_mapId)]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver);
  if (tags.size() > 0)
  {
    relationStr.replace("\\N", OsmApiDbSqlStatementFormatter::escapeCopyToData(_toTagsString(tags)));
  }

  return relationStr;
}

QString HootApiDbSqlStatementFormatter::relationMemberToSqlString(const long relationId,
                                                                  const long memberId,
                                                                  const RelationData::Entry& member,
                                                                  const unsigned int memberSequenceIndex,
                                                                  const unsigned long /*version*/)
{
  const QString relationIdStr(QString::number(relationId));
  const QString memberIdStr(QString::number(memberId));
  const QString memberType = member.getElementId().getType().toString().toLower();
  const QString memberSequenceIndexStr(QString::number(memberSequenceIndex));
  QString memberRole = OsmApiDbSqlStatementFormatter::escapeCopyToData(member.getRole());
  // handle empty data; This was needed for pg_bulkload, but we haven't integrated with it yet.
//  if (memberRole.trimmed().isEmpty())
//  {
//    memberRole = "<no role>";
//  }
  return
    _outputFormatStrings[HootApiDb::getCurrentRelationMembersTableName(_mapId)]
      .arg(relationIdStr, memberType, memberIdStr, memberRole, memberSequenceIndexStr);
}

QString HootApiDbSqlStatementFormatter::changesetToSqlString(const long changesetId,
                                                             const long changesetUserId,
                                                             const long numChangesInChangeset,
                                                             const geos::geom::Envelope& changesetBounds)
{
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = HOOT_PACKAGE_NAME;
  return
    _outputFormatStrings[HootApiDb::getChangesetsTableName(_mapId)]
      .arg(QString::number(changesetId))
      .arg(QString::number(changesetUserId))
      .arg(_dateString)
      .arg(QString::number(changesetBounds.getMinY(), 'g', _precision))
      .arg(QString::number(changesetBounds.getMaxX(), 'g', _precision))
      .arg(QString::number(changesetBounds.getMinX(), 'g', _precision))
      .arg(QString::number(changesetBounds.getMaxX(), 'g', _precision))
      .arg(_dateString)
      .arg(QString::number(numChangesInChangeset))
      .arg(OsmApiDbSqlStatementFormatter::escapeCopyToData(_toTagsString(tags)));
}

QString HootApiDbSqlStatementFormatter::elementToSqlString(const ConstElementPtr& element,
                                                                const long elementId,
                                                                const long changesetId)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return nodeToSqlString(std::dynamic_pointer_cast<const Node>(element), elementId, changesetId, element->getVersion());

    case ElementType::Way:
      return wayToSqlString(elementId, changesetId, element->getTags(), element->getVersion());

    case ElementType::Relation:
      return relationToSqlString(elementId, changesetId, element->getTags(), element->getVersion());

    default:
      throw HootException("Unsupported element member type.");
  }
  return "";
}

}
