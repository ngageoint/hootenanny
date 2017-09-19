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
#include "HootApiDbSqlStatementFormatter.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

// Qt
#include <QDateTime>
#include <QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HootApiDbSqlStatementFormatter::HootApiDbSqlStatementFormatter(const QString delimiter,
                                                               const long mapId) :
//let's just do this once at the beginning, since it could be expensive
_dateString(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-dd hh:mm:ss.zzz")),
_precision(ConfigOptions().getWriterPrecision()),
_mapId(mapId)
{
  _initOutputFormatStrings(delimiter);
}

void HootApiDbSqlStatementFormatter::_initOutputFormatStrings(const QString delimiter)
{
  QString formatString = CHANGESETS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getChangesetsTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentNodesTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_WAYS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentWaysTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_WAY_NODES_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentWayNodesTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_RELATIONS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentRelationsTableName(_mapId)] =
    formatString.replace("\t", delimiter);

  formatString = CURRENT_RELATION_MEMBERS_OUTPUT_FORMAT_STRING_DEFAULT;
  _outputFormatStrings[HootApiDb::getCurrentRelationMembersTableName(_mapId)] =
    formatString.replace("\t", delimiter);
}

QString HootApiDbSqlStatementFormatter::_toTagsString(const Tags& tags)
{
  QString tagsStr;
  tagsStr.reserve(500);
  for (Tags::const_iterator it = tags.begin(); it != tags.end(); ++it)
  {
    QString key;
    key.reserve(10);
    key.append(it.key().trimmed());
    QString value;
    value.reserve(50);
    value.append(it.value().trimmed());
    if (!value.isEmpty())
    {
      key.replace("'", "\'");
      key.replace("=>", "\\=\\>");
      key.replace("\"", "\\\"");
      tagsStr.append("\"" % key % "\"");

      tagsStr.append("=>");

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

QStringList HootApiDbSqlStatementFormatter::nodeToSqlStrings(const ConstNodePtr& node,
                                                            const long nodeId,
                                                            const long changesetId,
                                                            const bool validate)
{
  QStringList sqlStrs;

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

  QString nodeStr;
  nodeStr.reserve(600);
  nodeStr.append(
    _outputFormatStrings[HootApiDb::getCurrentNodesTableName(_mapId)]
      .arg(nodeIdStr)
      .arg(QString::number(node->getY(), 'g', _precision))
      .arg(QString::number(node->getX(), 'g', _precision))
      .arg(changesetIdStr)
      .arg(_dateString)
      .arg(tileNumberString));
  if (node->getTags().size() > 0)
  {
    nodeStr.replace("\\N", _escapeCopyToData(_toTagsString(node->getTags())));
  }
  sqlStrs.append(nodeStr);

  return sqlStrs;
}

QStringList HootApiDbSqlStatementFormatter::wayToSqlStrings(const long wayId, const long changesetId,
                                                            const Tags& tags)
{
  QStringList sqlStrs;

  QString wayStr;
  wayStr.reserve(600);
  wayStr.append(
    _outputFormatStrings[HootApiDb::getCurrentWaysTableName(_mapId)]
      .arg(wayId)
      .arg(changesetId)
      .arg(_dateString));
  if (tags.size() > 0)
  {
    wayStr.replace("\\N", _escapeCopyToData(_toTagsString(tags)));
  }
  sqlStrs.append(wayStr);

  return sqlStrs;
}

QStringList HootApiDbSqlStatementFormatter::wayNodeToSqlStrings(const long wayId,
                                                               const long wayNodeId,
                                                               const unsigned int wayNodeIndex)
{
  QStringList sqlStrs;

  const QString wayIdStr(QString::number(wayId));
  const QString wayNodeIdStr(QString::number(wayNodeId));
  const QString wayNodeIndexStr(QString::number(wayNodeIndex));
  sqlStrs.append(
    _outputFormatStrings[HootApiDb::getCurrentWayNodesTableName(_mapId)]
      .arg(wayIdStr, wayNodeIdStr, wayNodeIndexStr));

  return sqlStrs;
}

QStringList HootApiDbSqlStatementFormatter::relationToSqlStrings(const long relationId,
                                                                 const long changesetId,
                                                                 const Tags& tags)
{
  QStringList sqlStrs;

  QString relationStr;
  relationStr.reserve(600);
  relationStr.append(
    _outputFormatStrings[HootApiDb::getCurrentRelationsTableName(_mapId)]
      .arg(relationId)
      .arg(changesetId)
      .arg(_dateString));
  if (tags.size() > 0)
  {
    relationStr.replace("\\N", _escapeCopyToData(_toTagsString(tags)));
  }
  sqlStrs.append(relationStr);

  return sqlStrs;
}

QStringList HootApiDbSqlStatementFormatter::relationMemberToSqlStrings(const long relationId,
                                                                       const long memberId,
                                                                       const RelationData::Entry& member,
                                                                       const unsigned int memberSequenceIndex)
{
  QStringList sqlStrs;

  const QString relationIdStr(QString::number(relationId));
  const QString memberIdStr(QString::number(memberId));
  const QString memberType = member.getElementId().getType().toString().toLower();
  const QString memberSequenceIndexStr(QString::number(memberSequenceIndex));
  QString memberRole = _escapeCopyToData(member.getRole());
  //handle empty data
  if (memberRole.trimmed().isEmpty())
  {
    memberRole = "<no role>";
  }

  sqlStrs.append(
    _outputFormatStrings[HootApiDb::getCurrentRelationMembersTableName(_mapId)]
      .arg(relationIdStr, memberType, memberIdStr, memberRole, memberSequenceIndexStr));

  return sqlStrs;
}

QString HootApiDbSqlStatementFormatter::changesetToSqlString(const long changesetId,
                                                             const long changesetUserId,
                                                             const long numChangesInChangeset,
                                                             const Envelope& changesetBounds)
{
  Tags tags;
  tags["bot"] = "yes";
  tags["created_by"] = "hootenanny";
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
      .arg(_escapeCopyToData(_toTagsString(tags)));
}

QStringList HootApiDbSqlStatementFormatter::elementToSqlStrings(const ConstElementPtr& element,
                                                                const long elementId,
                                                                const long changesetId)
{
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      return nodeToSqlStrings(boost::dynamic_pointer_cast<const Node>(element), elementId, changesetId);

    case ElementType::Way:
      return wayToSqlStrings(elementId, changesetId, element->getTags());

    case ElementType::Relation:
      return relationToSqlStrings(elementId, changesetId, element->getTags());

    default:
      throw HootException("Unsupported element member type.");
  }
  return QStringList();
}

}
