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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementHashVisitor.h"

// hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/WayUtils.h>

// Qt
#include <QCryptographicHash>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ElementHashVisitor)

ElementHashVisitor::ElementHashVisitor() :
_includeCe(false),
_writeHashes(true),
_collectHashes(false)
{
  if (!_writeHashes && !_collectHashes)
  {
    throw IllegalArgumentException("ElementHashVisitor must either write or collect hashes.");
  }
  _nonMetadataIgnoreKeys = ConfigOptions().getElementHashVisitorNonMetadataIgnoreKeys();
}

void ElementHashVisitor::visit(const ElementPtr& e)
{
  // don't calculate hashes on review relations
  if (ReviewMarker::isReview(e) == false)
  {
    LOG_VART(e->getElementId());

    const QString hash = toHashString(e);
    LOG_VART(hash);

    if (_writeHashes)
    {
      e->getTags()[MetadataTags::HootHash()] = hash;
    }
    if (_collectHashes)
    {
      if (_hashesToElementIds.contains(hash))
      {
        _duplicates.insert(
          std::pair<ElementId, ElementId>(_hashesToElementIds[hash], e->getElementId()));
      }
      else
      {
        _hashesToElementIds[hash] = e->getElementId();
      }
    }
  }
}

QString ElementHashVisitor::toJson(const ConstElementPtr& e)
{
  QString result;
  if (e->getElementType() == ElementType::Node)
  {
    result = _toJson(std::dynamic_pointer_cast<const Node>(e));
  }
  else if (e->getElementType() == ElementType::Way)
  {
    result = _toJson(std::dynamic_pointer_cast<const Way>(e));
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    result = _toJson(std::dynamic_pointer_cast<const Relation>(e));
  }
  //LOG_VART(result);
  return result;
}

QString ElementHashVisitor::_toJson(const ConstNodePtr& node)
{
  QString result = "{\"type\":\"node\",\"tags\":{";

  result += _toJson(node->getTags(), node->getRawCircularError());

  const int coordinateComparisonSensitivity =
    ConfigOptions().getNodeComparisonCoordinateSensitivity();
  result += "},\"x\":";
  result += QString::number(node->getX(), 'f', coordinateComparisonSensitivity);
  result += ",\"y\":";
  result += QString::number(node->getY(), 'f', coordinateComparisonSensitivity);
  result += "}";

  return result;
}

QString ElementHashVisitor::_toJson(const ConstWayPtr& way)
{
  if (_map == 0)
  {
    throw IllegalArgumentException("A map must be set when calculating a way hash.");
  }

  QString result = "{\"type\":\"way\",\"tags\":{";

  result += _toJson(way->getTags(), way->getRawCircularError());

  result += "},\"nodes\":[";
  const std::vector<long>& nodeIds = way->getNodeIds();
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    ConstNodePtr node = _map->getNode(nodeIds[i]);
    if (node)
    {
      result += _toJson(node);
      if (i != (nodeIds.size() - 1))
      {
        result += ",";
      }
    }
  }
  result += "]}";

  return result;
}

QString ElementHashVisitor::_toJson(const ConstRelationPtr& relation)
{
  if (_map == 0)
  {
    throw IllegalArgumentException("A map must be set when calculating a relation hash.");
  }

  QString result = "{\"type\":\"relation\",\"tags\":{";

  result += _toJson(relation->getTags(), relation->getRawCircularError());

  result += "},\"members\":[";
  const std::vector<RelationData::Entry>& relationMembers = relation->getMembers();
  for (size_t i = 0; i < relationMembers.size(); i++)
  {
    const RelationData::Entry member = relationMembers[i];
    ConstElementPtr memberElement = _map->getElement(member.getElementId());
    if (memberElement)
    {
      QString memberJson = toJson(memberElement);
      const QString typeStr = memberElement->getElementType().toString().toLower();
      memberJson =
        memberJson.replace(
          "\"type\":\"" + typeStr + "\",",
          "\"type\":\"" + typeStr + "\",\"role\":\"" + member.getRole() + "\",");
      result += memberJson;
      if (i != (relationMembers.size() - 1))
      {
        result += ",";
      }
    }
  }
  result += "]}";

  return result;
}

QString ElementHashVisitor::_toJson(const Tags& tags, const double ce)
{
  QString result;

  // Put the tags into an ordered map that only contains the non-metadata (info) tags. As
  // implemented this is likely quite slow.
  QMap<QString, QString> infoTags;
  foreach (QString key, tags.keys())
  {
    QString v = tags[key];
    if (!_nonMetadataIgnoreKeys.contains(key) &&
        OsmSchema::getInstance().isMetaData(key, v) == false)
    {
      infoTags[key] = v;
    }
  }
  //LOG_VART(infoTags.keys());

  if (_includeCe)
  {
    const int circularErrorComparisonSensitivity =
      ConfigOptions().getNodeComparisonCircularErrorSensitivity();
    if (ce >= 0)
    {
      infoTags[MetadataTags::ErrorCircular()] =
        QString::number(ce, 'g', circularErrorComparisonSensitivity);
    }
  }


  bool first = true;
  foreach (QString key, infoTags.keys())
  {
    if (!first)
    {
      result += ",";
    }
    result += QString("\"%1\":\"%2\"").arg(key, infoTags[key]);
    first = false;
  }

  //LOG_VART(result);
  return result;
}

QByteArray ElementHashVisitor::toHash(const ConstElementPtr& e)
{
  QCryptographicHash hash(QCryptographicHash::Sha1);
  hash.addData(toJson(e).toLatin1().constData());
  return hash.result();
}

QString ElementHashVisitor::toHashString(const ConstElementPtr& e)
{
  return "sha1sum:" + QString::fromUtf8(toHash(e).toHex());
}

}
