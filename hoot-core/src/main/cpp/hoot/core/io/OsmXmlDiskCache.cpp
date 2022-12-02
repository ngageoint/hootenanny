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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */

#include "OsmXmlDiskCache.h"

// Hoot includes
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/ConfPath.h>

// QT includes
#include <QFileInfo>


namespace hoot
{

OsmXmlDiskCache::OsmXmlDiskCache()
{
  // Check for temp dir
  // If $HOOT_HOME/tmp doesn't exist, and we can't make it, then use /tmp
  QString tempDir = ConfPath::getHootHome() + "/tmp";
  if (!QDir(tempDir).exists())
    if (!QDir(tempDir).mkpath("."))
      tempDir = "/tmp";

  // Setup our temp file & get guaranteed unique name
  QString fnameTemplate = tempDir + "/disk.cache.osm.XXXXXX";
  _pTempFile = std::make_shared<QTemporaryFile>(fnameTemplate);
  if (!_pTempFile->open())
  {
    LOG_ERROR("Could not open temporary cache file: " << fnameTemplate);
  }
  _tempFileName = _pTempFile->fileName();

  // If we are writing debug maps, don't autoremove the cache
  _pTempFile->setAutoRemove(!ConfigOptions().getDebugMapsWrite());

  // Setup our writer
  _writer.setIncludeIds(true);
  _writer.setIncludeHootInfo(true);
  _writer.setIncludePid(true);
  _writer.setIncludeDebug(true);
  _writer.openunbuff(_tempFileName);

  // Setup our reader
  _reader.setUseDataSourceIds(true);
  _reader.setUseFileStatus(true);
  _reader.setPreserveAllTags(true);
  _reader.open(_tempFileName);

  // Initialize the cache
  _initCache();
}

OsmXmlDiskCache::~OsmXmlDiskCache()
{
  _reader.close();
  _writer.close();
}

void OsmXmlDiskCache::_initCache()
{
  // Make a bogus node for the cache, otherwise our reader gets angry
  uint64_t nodeId(UINT64_MAX);
  double lat(1.123456);
  double lon(1.123456);
  Meters circularError(13.13);
  ConstElementPtr tempNode = std::make_shared<const Node>(Status::Unknown1, nodeId, lon, lat, circularError);
  addElement(tempNode);
  _reader.hasMoreElements();
}

void OsmXmlDiskCache::addElement(ConstElementPtr &newElement)
{
  LOG_TRACE("Adding element: " + newElement->toString() + " to cache...");

  ConstNodePtr newNode;
  ConstWayPtr newWay;
  ConstRelationPtr newRelation;

  switch (newElement->getElementType().getEnum())
  {
  case ElementType::Node:
    newNode = std::dynamic_pointer_cast<const Node>(newElement);
    if (newNode != ConstNodePtr())
    {
      uint64_t id = newNode->getId();
      if (_node2pos.find(id) == _node2pos.end())
      {
        uint64_t pos = _writer.getPos();
        _writer.writePartial(newNode);
        _node2pos.insert(id, pos);
        LOG_TRACE(QString("Cached node id %1 at pos %2").arg(id).arg(pos));
      }
    }
    break;
  case ElementType::Way:
    newWay = std::dynamic_pointer_cast<const Way>(newElement);
    if (newWay != ConstWayPtr())
    {
      uint64_t id = newWay->getId();
      if (_way2pos.find(id) == _way2pos.end())
      {
        uint64_t pos = _writer.getPos();
        _writer.writePartial(newWay);
        _way2pos.insert(id, pos);
        LOG_TRACE(QString("Cached way id %1 at pos %2").arg(id).arg(pos));
      }
    }
    break;
  case ElementType::Relation:
    newRelation = std::dynamic_pointer_cast<const Relation>(newElement);
    if (newRelation != ConstRelationPtr())
    {
      uint64_t id = newRelation->getId();
      if (_relation2pos.find(id) == _relation2pos.end())
      {
        uint64_t pos = _writer.getPos();
        _writer.writePartial(newRelation);
        _relation2pos.insert(id, pos);
        LOG_TRACE(QString("Cached relation id %1 at pos %2").arg(id).arg(pos));
      }
    }
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(newElement->getElementType().toString()));
    break;
  }
}

bool OsmXmlDiskCache::containsElement(const ElementId& eid) const
{
  const ElementType::Type type = eid.getType().getEnum();
  const long id = eid.getId();
  switch (type)
  {
  case ElementType::Node:
    return (_node2pos.find(id) != _node2pos.end());
    break;
  case ElementType::Way:
    return (_way2pos.find(id) != _way2pos.end());
    break;
  case ElementType::Relation:
    return (_relation2pos.find(id) != _relation2pos.end());
    break;
  default:
    break;
  }
  // If we get to this point, it means we couldn't find it
  return false;
}

ElementPtr OsmXmlDiskCache::getElement(const ElementId& eid)
{
  const long id = eid.getId();
  switch (eid.getType().getEnum())
  {
  case ElementType::Node:
    return getNode(id);
    break;
  case ElementType::Way:
    return getWay(id);
    break;
  case ElementType::Relation:
    return getRelation(id);
    break;
  default:
    break;
  }

  // Error!!!
  return ElementPtr();
}

NodePtr OsmXmlDiskCache::getNode(long id)
{
  auto it = _node2pos.find(id);
  if (it != _node2pos.end())
  {
    uint64_t pos = it.value();
    _reader.seekAndReset(pos);
    if (_reader.hasMoreElements())
    {
      ElementPtr elmnt = _reader.readNextElement();
      return std::dynamic_pointer_cast<Node, Element>(elmnt);
    }
  }

  // Error!
  return NodePtr();
}

RelationPtr OsmXmlDiskCache::getRelation(long id)
{
  auto it = _relation2pos.find(id);
  if (it != _relation2pos.end())
  {
    uint64_t pos = it.value();
    _reader.seekAndReset(pos);
    if (_reader.hasMoreElements())
    {
      ElementPtr elmnt = _reader.readNextElement();
      return std::dynamic_pointer_cast<Relation, Element>(elmnt);
    }
  }

  // Error!
  return RelationPtr();
}

WayPtr OsmXmlDiskCache::getWay(long id)
{
  auto it = _way2pos.find(id);
  if (it != _way2pos.end())
  {
    uint64_t pos = it.value();
    _reader.seekAndReset(pos);
    if (_reader.hasMoreElements())
    {
      ElementPtr elmnt = _reader.readNextElement();
      return std::dynamic_pointer_cast<Way, Element>(elmnt);
    }
  }

  // Error!
  return WayPtr();
}

bool OsmXmlDiskCache::containsNode(long id) const
{
  return (_node2pos.find(id) != _node2pos.end());
}

bool OsmXmlDiskCache::containsWay(long id) const
{
  return (_way2pos.find(id) != _way2pos.end());
}

bool OsmXmlDiskCache::containsRelation(long id) const
{
  return (_relation2pos.find(id) != _relation2pos.end());
}

}
