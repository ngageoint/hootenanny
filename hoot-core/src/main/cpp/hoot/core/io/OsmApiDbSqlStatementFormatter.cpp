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
#include "OsmApiDbSqlStatementFormatter.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/io/OsmApiDb.h>

// Qt
#include <QDateTime>

using namespace geos::geom;
using namespace std;

namespace hoot
{

OsmApiDbSqlStatementFormatter::OsmApiDbSqlStatementFormatter(const QString delimiter)
{
  _initOutputFormatStrings(delimiter);
}

unsigned int OsmApiDbSqlStatementFormatter::_convertDegreesToNanodegrees(const double degrees)
{
  return round(degrees * ApiDb::COORDINATE_SCALE);
}

void OsmApiDbSqlStatementFormatter::_initOutputFormatStrings(const QString delimiter)
{
  QString formatString = CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getChangesetsTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentNodesTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getNodesTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentWaysTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWaysTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentWayNodesTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWayNodesTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentRelationsTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getRelationsTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentRelationMembersTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getRelationMembersTableName()] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getCurrentNodeTagsTableName()] =
    formatString.replace("\t", delimiter);

  _outputFormatStrings[ApiDb::getCurrentWayTagsTableName()] =
    formatString.replace("\t", delimiter);
  _outputFormatStrings[ApiDb::getCurrentRelationTagsTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getWayTagsTableName()] =
    formatString.replace("\t", delimiter);
  _outputFormatStrings[ApiDb::getRelationTagsTableName()] =
    formatString.replace("\t", delimiter);

  formatString = HISTORICAL_NODE_TAGS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[ApiDb::getNodeTagsTableName()] =
    formatString.replace("\t", delimiter);
}

QStringList OsmApiDbSqlStatementFormatter::nodeToSqlStrings(const ConstNodePtr& node,
                                                            const long nodeId,
                                                            const long changesetId)
{
  QStringList sqlStrs;

  //TODO: should be able to use OsmApiDb::toOsmApiDbCoord here instead
  //const long nodeYNanodegrees = OsmApiDb::toOsmApiDbCoord(node->getY());
  //const long nodeXNanodegrees = OsmApiDb::toOsmApiDbCoord(node->getX());
  const int nodeYNanodegrees = _convertDegreesToNanodegrees(node->getY());
  const int nodeXNanodegrees = _convertDegreesToNanodegrees(node->getX());
  if ((nodeYNanodegrees < -900000000) || (nodeYNanodegrees > 900000000))
  {
    throw HootException(
      QString("Invalid latitude conversion, Y = %1 to %2").arg(
        QString::number(node->getY()), QString::number(nodeYNanodegrees)));
  }
  if ((nodeXNanodegrees < -1800000000) || (nodeXNanodegrees > 1800000000))
  {
    throw HootException(
      QString("Invalid longitude conversion, X = %1 to %2").arg(
        QString::number(node->getX()), QString::number(nodeXNanodegrees)));
  }
  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  const QString tileNumberString(QString::number(ApiDb::tileForPoint(node->getY(), node->getX())));

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentNodesTableName()].arg(
      QString::number(nodeId),
      QString::number(nodeYNanodegrees),
      QString::number(nodeXNanodegrees),
      QString::number(changesetId),
      datestring,
      tileNumberString));

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getNodesTableName()].arg(
      QString::number(nodeId),
      QString::number(nodeYNanodegrees),
      QString::number(nodeXNanodegrees),
      QString::number(changesetId),
      datestring,
      tileNumberString));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::wayToSqlStrings(const long wayId, const long changesetId)
{
  QStringList sqlStrs;

  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(datestring));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(datestring));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::wayNodeToSqlStrings(const long wayId,
                                                               const long wayNodeId,
                                                               const unsigned int wayNodeIndex)
{
  QStringList sqlStrs;

  const QString wayIdStr(QString::number(wayId));
  const QString wayNodeIdStr(QString::number(wayNodeId));
  const QString wayNodeIndexStr(QString::number(wayNodeIndex));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentWayNodesTableName()]
      .arg(wayIdStr, wayNodeIdStr, wayNodeIndexStr));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getWayNodesTableName()]
      .arg(wayIdStr, wayNodeIdStr, wayNodeIndexStr));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::relationToSqlStrings(const long relationId,
                                                                const long changesetId)
{
  QStringList sqlStrs;

  const QString datestring =
    QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(datestring));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(datestring));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::relationMemberToSqlStrings(const long relationId,
                                                                      const long memberId,
                                                                      const RelationData::Entry& member,
                                                                      const unsigned int memberSequenceIndex)
{
  QStringList sqlStrs;

  const QString relationIdStr(QString::number(relationId));
  const QString memberIdStr(QString::number(memberId));
  const QString memberType = member.getElementId().getType().toString();
  const QString memberSequenceIndexStr(QString::number(memberSequenceIndex));
  QString memberRole = _escapeCopyToData(member.getRole());
  //handle empty data
  if (memberRole.trimmed().isEmpty())
  {
    memberRole = "<no role>";
  }

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentRelationMembersTableName()]
      .arg(relationIdStr, memberType, memberIdStr, memberRole, memberSequenceIndexStr));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getRelationMembersTableName()]
      .arg(relationIdStr, memberType, memberIdStr, memberRole, memberSequenceIndexStr));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::tagToSqlStrings(const long elementId,
                                                           const ElementType& elementType,
                                                           const QString tagKey,
                                                           const QString tagValue)
{
  QStringList sqlStrs;

  const QString elementIdStr = QString::number(elementId);
  QString key;
  key.reserve(10);
  key = _escapeCopyToData(tagKey);
  //pg_bulkload doesn't seem to be tolerating the empty data
  if (key.trimmed().isEmpty())
  {
    key = "<empty>";
  }
  LOG_VART(key);
  QString value;
  value.reserve(50);
  value = _escapeCopyToData(tagValue);
  LOG_VART(value);
  if (value.trimmed().isEmpty())
  {
    value = "<empty>";
  }

  //all three of them are the same for current
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentNodeTagsTableName()]
      .arg(elementIdStr, key, value));
  //all three of them are not the same for historical
  QString historicalFormatString = _outputFormatStrings[ApiDb::getWayTagsTableName()];
  if (elementType == ElementType::Node)
  {
    //see explanation for this silliness in the header file
    historicalFormatString = _outputFormatStrings[ApiDb::getNodeTagsTableName()];
  }
  sqlStrs.append(historicalFormatString.arg(elementIdStr, key, value));

  return sqlStrs;
}

QString OsmApiDbSqlStatementFormatter::changesetToSqlString(const long changesetId,
                                                            const long changesetUserId,
                                                            const long numChangesInChangeset,
                                                            const Envelope& changesetBounds)
{
  const QString dateStr = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
  return
    _outputFormatStrings[ApiDb::getChangesetsTableName()]
      .arg(
        QString::number(changesetId),
        QString::number(changesetUserId),
        dateStr,
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMinY())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMaxY())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMinX())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMaxX())),
        dateStr,
        QString::number(numChangesInChangeset));
}

QString OsmApiDbSqlStatementFormatter::_escapeCopyToData(const QString stringToOutput)
{
  //TODO: this is likely redundant with other code

  QString escapedString(stringToOutput);
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

QStringList OsmApiDbSqlStatementFormatter::elementToSqlStrings(const ConstElementPtr& element,
                                                               const long elementId,
                                                               const long changesetId)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return nodeToSqlStrings(boost::dynamic_pointer_cast<const Node>(element), elementId, changesetId);

    case ElementType::Way:
      return wayToSqlStrings(elementId, changesetId);

    case ElementType::Relation:
      return relationToSqlStrings(elementId, changesetId);

    default:
      throw HootException("Unsupported element member type.");
  }
  return QStringList();
}

}
