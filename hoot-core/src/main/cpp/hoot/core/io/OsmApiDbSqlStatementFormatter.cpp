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
#include <QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

OsmApiDbSqlStatementFormatter::OsmApiDbSqlStatementFormatter(const QString delimiter)
{
  _initOutputFormatStrings(delimiter);
  //let's just do this once at the beginning, since it could be expensive
  _dateString = QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz");
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
                                                            const long changesetId,
                                                            const bool validate)
{
  QStringList sqlStrs;

  const QString nodeIdStr = QString::number(nodeId);
  //TODO: should be able to use OsmApiDb::toOsmApiDbCoord here instead
  QString nodeYNanodegreesStr;
  QString nodeXNanodegreesStr;
  if (validate)
  {
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

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentNodesTableName()].arg(
      nodeIdStr,
      nodeYNanodegreesStr,
      nodeXNanodegreesStr,
      changesetIdStr,
      _dateString,
      tileNumberString));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getNodesTableName()].arg(
      nodeIdStr,
      nodeYNanodegreesStr,
      nodeXNanodegreesStr,
      changesetIdStr,
      _dateString,
      tileNumberString));

  return sqlStrs;
}

QStringList OsmApiDbSqlStatementFormatter::wayToSqlStrings(const long wayId, const long changesetId)
{
  QStringList sqlStrs;

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getWaysTableName()]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString));

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

  sqlStrs.append(
    _outputFormatStrings[ApiDb::getCurrentRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString));
  sqlStrs.append(
    _outputFormatStrings[ApiDb::getRelationsTableName()]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString));

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
  //pre-allocating the string memory here reduces memory fragmentation significantly when parsing
  //larger datasets due to the varying string sizes
  QString key;
  key.reserve(10);
  key.append(_escapeCopyToData(tagKey));
  LOG_VART(key);
  QString value;
  value.reserve(50);
  value.append(_escapeCopyToData(tagValue));
  LOG_VART(value);

  //all three of them are the same for current
  QString currentSql;
  currentSql.reserve(75);
  currentSql.append(
    _outputFormatStrings[ApiDb::getCurrentNodeTagsTableName()]
      .arg(elementIdStr, key, value));
  sqlStrs.append(currentSql);
  //all three of them are not the same for historical
  QString historicalFormatString = _outputFormatStrings[ApiDb::getWayTagsTableName()];
  if (elementType == ElementType::Node)
  {
    //see explanation for this silliness in the header file
    historicalFormatString = _outputFormatStrings[ApiDb::getNodeTagsTableName()];
  }
  QString historicalSql;
  historicalSql.reserve(75);
  historicalSql.append(historicalFormatString.arg(elementIdStr, key, value));
  sqlStrs.append(historicalSql);

  return sqlStrs;
}

QString OsmApiDbSqlStatementFormatter::changesetToSqlString(const long changesetId,
                                                            const long changesetUserId,
                                                            const long numChangesInChangeset,
                                                            const Envelope& changesetBounds)
{
  return
    _outputFormatStrings[ApiDb::getChangesetsTableName()]
      .arg(
        QString::number(changesetId),
        QString::number(changesetUserId),
        _dateString,
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMinY())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMaxY())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMinX())),
        QString::number((qlonglong)OsmApiDb::toOsmApiDbCoord(changesetBounds.getMaxX())),
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
