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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmApiDbSqlStatementFormatter.h"

// hoot
#include <hoot/core/io/OsmApiDb.h>

// Qt
#include <QDateTime>
#include <QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

OsmApiDbSqlStatementFormatter::OsmApiDbSqlStatementFormatter(const QString& delimiter)
{
  _initOutputFormatStrings(delimiter);
}

void OsmApiDbSqlStatementFormatter::_initOutputFormatStrings(const QString& delimiter)
{
  QString formatString = OSMAPIDB_CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getChangesetsTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentNodesTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getNodesTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentWaysTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWaysTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentWayNodesTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWayNodesTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentRelationsTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getRelationsTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentRelationMembersTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getRelationMembersTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_CURRENT_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentNodeTagsTableName()] = formatString.replace("\t", delimiter);

  _outputFormatStrings[ApiDb::getCurrentWayTagsTableName()] = formatString.replace("\t", delimiter);
  _outputFormatStrings[ApiDb::getCurrentRelationTagsTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWayTagsTableName()] = formatString.replace("\t", delimiter);
  _outputFormatStrings[ApiDb::getRelationTagsTableName()] = formatString.replace("\t", delimiter);

  formatString = OSMAPIDB_HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getNodeTagsTableName()] = formatString.replace("\t", delimiter);
}

QString OsmApiDbSqlStatementFormatter::escapeCopyToData(const QString& stringToOutput)
{
  QString escapedString = stringToOutput;
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

QStringList OsmApiDbSqlStatementFormatter::nodeToSqlStrings(const ConstNodePtr& node,
                                                            const long nodeId,
                                                            const long changesetId,
                                                            const bool validate)
{
  QStringList sqlStrs;

  const QString nodeIdStr = QString::number(nodeId);
  // TODO: should be able to use OsmApiDb::toOsmApiDbCoord here instead
  QString nodeYNanodegreesStr;
  QString nodeXNanodegreesStr;
  if (validate)
  {
    const int nodeYNanodegrees = _convertDegreesToNanodegrees(node->getY());
    const int nodeXNanodegrees = _convertDegreesToNanodegrees(node->getX());
    if ((nodeYNanodegrees < -900000000) || (nodeYNanodegrees > 900000000))
    {
      throw HootException(
        QString("Invalid latitude conversion, Y = %1 to %2").arg(QString::number(node->getY()), QString::number(nodeYNanodegrees)));
    }
    if ((nodeXNanodegrees < -1800000000) || (nodeXNanodegrees > 1800000000))
    {
      throw HootException(
        QString("Invalid longitude conversion, X = %1 to %2").arg(QString::number(node->getX()), QString::number(nodeXNanodegrees)));
    }
    nodeYNanodegreesStr = QString::number(nodeYNanodegrees);
    nodeXNanodegreesStr = QString::number(nodeXNanodegrees);
  }
  else
  {
    nodeYNanodegreesStr = QString::number((int)_convertDegreesToNanodegrees(node->getY()));
    nodeXNanodegreesStr = QString::number((int)_convertDegreesToNanodegrees(node->getX()));
  }
  const QString changesetIdStr = QString::number(changesetId);
  const QString tileNumberString(QString::number(ApiDb::tileForPoint(node->getY(), node->getX())));
  const QString versionString = _useSourceVersion ? QString::number(node->getVersion()) : "1";

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentNodesTableName()].arg(
      nodeIdStr,
      nodeYNanodegreesStr,
      nodeXNanodegreesStr,
      changesetIdStr,
      _dateString,
      tileNumberString,
      versionString));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getNodesTableName()].arg(
      nodeIdStr,
      nodeYNanodegreesStr,
      nodeXNanodegreesStr,
      changesetIdStr,
      _dateString,
      tileNumberString,
      versionString));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::wayToSqlStrings(const long wayId, const long changesetId, const long version)
{
  QStringList sqlStrs;
  long ver = _useSourceVersion ? version : 1;

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::wayNodeToSqlStrings(const long wayId,
                                                               const long wayNodeId,
                                                               const unsigned int wayNodeIndex,
                                                               const long version)
{
  QStringList sqlStrs;
  long ver = _useSourceVersion ? version : 1;

  const QString wayIdStr(QString::number(wayId));
  const QString wayNodeIdStr(QString::number(wayNodeId));
  const QString wayNodeIndexStr(QString::number(wayNodeIndex));
  const QString wayVersion(QString::number(ver));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentWayNodesTableName()]
      .arg(wayIdStr, wayNodeIdStr, wayNodeIndexStr));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getWayNodesTableName()]
      .arg(wayIdStr, wayNodeIdStr, wayVersion, wayNodeIndexStr));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::relationToSqlStrings(const long relationId,
                                                                const long changesetId,
                                                                const long version)
{
  QStringList sqlStrs;
  long ver = _useSourceVersion ? version : 1;

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString)
      .arg(ver));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::relationMemberToSqlStrings(const long relationId,
                                                                      const long memberId,
                                                                      const RelationData::Entry& member,
                                                                      const unsigned int memberSequenceIndex,
                                                                      const unsigned int version)
{
  QStringList sqlStrs;
  long ver = _useSourceVersion ? version : 1;

  const QString relationIdStr(QString::number(relationId));
  const QString memberIdStr(QString::number(memberId));
  const QString memberType = member.getElementId().getType().toString();
  const QString memberSequenceIndexStr(QString::number(memberSequenceIndex));
  const QString versionStr(QString::number(ver));
  QString memberRole = escapeCopyToData(member.getRole());
  // handle empty data; This was needed for pg_bulkload, but we haven't integrated with it yet.
//  if (memberRole.trimmed().isEmpty())
//  {
//    memberRole = "<no role>";
//  }

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentRelationMembersTableName()].arg(relationIdStr, memberType, memberIdStr, memberRole, memberSequenceIndexStr));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getRelationMembersTableName()].arg(relationIdStr, memberType, memberIdStr, memberRole, versionStr, memberSequenceIndexStr));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::tagToSqlStrings(const long elementId,
                                                           const ElementType& elementType,
                                                           const QString& tagKey,
                                                           const QString& tagValue,
                                                           const long version)
{
  QStringList sqlStrs;
  long ver = _useSourceVersion ? version : 1;

  const QString elementIdStr = QString::number(elementId);
  QString key = escapeCopyToData(tagKey);
  LOG_VART(key);
  QString value = escapeCopyToData(tagValue);
  LOG_VART(value);
  QString versionStr = QString::number(ver);

  //all three of them are the same for current
  QString currentSql =
    _outputFormatStrings[ApiDb::getCurrentNodeTagsTableName()]
      .arg(elementIdStr, key, value);
  sqlStrs.append(currentSql);
  //all three of them are not the same for historical
  QString historicalSql;
  if (elementType == ElementType::Node) // see explanation for this silliness in the header file
    historicalSql = _outputFormatStrings[ApiDb::getNodeTagsTableName()].arg(elementIdStr, versionStr, key, value);
  else  //  way and relation strings are the same
    historicalSql = _outputFormatStrings[ApiDb::getWayTagsTableName()].arg(elementIdStr, key, value, versionStr);

  sqlStrs.append(historicalSql);

  return sqlStrs;
}

QString OsmApiDbSqlStatementFormatter::changesetToSqlString(const long changesetId,
                                                            const long changesetUserId,
                                                            const long numChangesInChangeset,
                                                            const Envelope& changesetBounds)
{
  double min_x = changesetBounds.isNull() ?  0.0 : changesetBounds.getMinX();
  double min_y = changesetBounds.isNull() ?  0.0 : changesetBounds.getMinY();
  double max_x = changesetBounds.isNull() ? -1.0 : changesetBounds.getMaxX();
  double max_y = changesetBounds.isNull() ? -1.0 : changesetBounds.getMaxY();
  return
    _outputFormatStrings[ApiDb::getChangesetsTableName()]
      .arg(
        QString::number(changesetId),
        QString::number(changesetUserId),
        _dateString,
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(min_y)),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(max_y)),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(min_x)),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(max_x)),
        _dateString,
        QString::number(numChangesInChangeset));
}

QStringList OsmApiDbSqlStatementFormatter::elementToSqlStrings(const ConstElementPtr& element,
                                                               const long elementId,
                                                               const long changesetId)
{
  switch (element->getElementType().getEnum())
  {
  case ElementType::Node:
    return nodeToSqlStrings(std::dynamic_pointer_cast<const Node>(element), elementId, changesetId);
  case ElementType::Way:
    return wayToSqlStrings(elementId, changesetId, element->getVersion());
  case ElementType::Relation:
    return relationToSqlStrings(elementId, changesetId, element->getVersion());
  default:
    throw HootException("Unsupported element member type.");
  }
  return QStringList();
}

}
