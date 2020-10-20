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

// Qt
#include <QCryptographicHash>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, ElementHashVisitor)

ElementHashVisitor::ElementHashVisitor() :
_coordinateComparisonSensitivity(ConfigOptions().getNodeComparisonCoordinateSensitivity()),
_includeCe(false),
_nonMetadataIgnoreKeys(ConfigOptions().getElementHashVisitorNonMetadataIgnoreKeys()),
_useNodeTags(true),
_writeHashes(true),
_collectHashes(false)
{
  if (!_writeHashes && !_collectHashes)
  {
    throw IllegalArgumentException("ElementHashVisitor must either write or collect hashes.");
  }
  LOG_VART(_coordinateComparisonSensitivity);
}

void ElementHashVisitor::clearHashes()
{
  _hashesToElementIds.clear();
  _elementIdsToHashes.clear();
}

void ElementHashVisitor::visit(const ElementPtr& e)
{
  if (!e)
  {
    return;
  }

  // don't calculate hashes on review relations
  if (ReviewMarker::isReview(e) == false)
  {
    LOG_VART(e->getElementId());
    //LOG_VART(e);

    const QString hash = toHashString(e);
    LOG_VART(hash);

    insertHash(e, hash);
  }
}

void ElementHashVisitor::insertHash(const ElementPtr& e, const QString& hash)
{
  if (_writeHashes)
  {
    LOG_TRACE("Writing hash: " << hash << " to " << e->getElementId() << "...");
    e->getTags()[MetadataTags::HootHash()] = hash;
  }
  if (_collectHashes)
  {
    if (_hashesToElementIds.contains(hash))
    {
      LOG_TRACE(
        "Marking duplicate hash: " << hash << " for " << e->getElementId() <<
        "; hash already used by " << _hashesToElementIds[hash] << "...");
      _duplicates.insert(
        std::pair<ElementId, ElementId>(_hashesToElementIds[hash], e->getElementId()));
    }
    else
    {
      LOG_TRACE("Collecting hash: " << hash << " for " << e->getElementId() << "...");
      _hashesToElementIds[hash] = e->getElementId();
    }
    _elementIdsToHashes[e->getElementId()] = hash;
  }
}

QString ElementHashVisitor::toJson(const ConstElementPtr& e) const
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
  LOG_TRACE("json for " << e->getElementId() << ":\n" << result);
  return result;
}

QString ElementHashVisitor::_toJson(const ConstNodePtr& node) const
{
  QString result = "{\"type\":\"node\",\"tags\":{";

  Tags tags;
  if (_useNodeTags)
  {
    tags = node->getTags();
  }
  else
  {
    tags = Tags();
  }
  result += toJson(tags, node->getCircularError());

  result += "},\"x\":";
  result += QString::number(node->getX(), 'f', _coordinateComparisonSensitivity);
  result += ",\"y\":";
  result += QString::number(node->getY(), 'f', _coordinateComparisonSensitivity);
  result += "}";

  return result;
}

QString ElementHashVisitor::_toJson(const ConstWayPtr& way) const
{
  if (_map == 0)
  {
    throw IllegalArgumentException("A map must be set when calculating a way hash.");
  }

  QString result = "{\"type\":\"way\",\"tags\":{";

  result += toJson(way->getTags(), way->getRawCircularError());

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

QString ElementHashVisitor::_toJson(const ConstRelationPtr& relation) const
{
  if (_map == 0)
  {
    throw IllegalArgumentException("A map must be set when calculating a relation hash.");
  }

  QString result = "{\"type\":\"relation\",\"tags\":{";

  result += toJson(relation->getTags(), relation->getRawCircularError());

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

QString ElementHashVisitor::toJson(const Tags& tags, const double ce) const
{
  QString result;

  // Put the tags into an ordered map that only contains the non-metadata (info) tags. As
  // implemented, this is likely quite slow.
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

  if (_includeCe && ce != -1.0)
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

  return result;
}

QByteArray ElementHashVisitor::toHash(const ConstElementPtr& e) const
{
  QCryptographicHash hash(QCryptographicHash::Sha1);
  hash.addData(toJson(e).toLatin1().constData());
  return hash.result();
}

QByteArray ElementHashVisitor::toHash(const Tags& tags, const double ce) const
{
  QCryptographicHash hash(QCryptographicHash::Sha1);
  hash.addData(toJson(tags, ce).toLatin1().constData());
  return hash.result();
}

QString ElementHashVisitor::toHashString(const ConstElementPtr& e) const
{
  return "sha1sum:" + QString::fromUtf8(toHash(e).toHex());
}

QString ElementHashVisitor::toHashString(const Tags& tags, const double ce) const
{
  return "sha1sum:" + QString::fromUtf8(toHash(tags, ce).toHex());
}

}
