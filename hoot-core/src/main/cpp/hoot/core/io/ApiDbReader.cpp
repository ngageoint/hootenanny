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
#include "ApiDbReader.h"

// Hoot
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/io/TableType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QSet>

namespace hoot
{

unsigned int ApiDbReader::logWarnCount = 0;

ApiDbReader::ApiDbReader() :
_useDataSourceIds(true),
_status(Status::Invalid),
_open(false)
{

}

void ApiDbReader::setBoundingBox(const QString bbox)
{
  if (!bbox.trimmed().isEmpty())
  {
    _bounds = GeometryUtils::envelopeFromConfigString(bbox);
  }
}

void ApiDbReader::setOverrideBoundingBox(const QString bbox)
{
  if (!bbox.trimmed().isEmpty())
  {
    _overrideBounds = GeometryUtils::envelopeFromConfigString(bbox);
  }
}

bool ApiDbReader::_hasBounds()
{
  return _isValidBounds(_bounds) || _isValidBounds(_overrideBounds);
}

ElementId ApiDbReader::_mapElementId(const OsmMap& map, ElementId oldId)
{
  ElementId result;
  LOG_VART(oldId);
  if (_useDataSourceIds)
  {
    result = oldId;
  }
  else
  {
    long id = oldId.getId();
    switch (oldId.getType().getEnum())
    {
    case ElementType::Node:
      if (_nodeIdMap.count(id) > 0)
      {
        result = ElementId::node(_nodeIdMap.at(id));
      }
      else
      {
        long newId = map.createNextNodeId();
        _nodeIdMap[id] = newId;
        result = ElementId::node(newId);
      }
      break;
    case ElementType::Way:
      if (_wayIdMap.count(id) > 0)
      {
        result = ElementId::way(_wayIdMap.at(id));
      }
      else
      {
        long newId = map.createNextWayId();
        _wayIdMap[id] = newId;
        result = ElementId::way(newId);
      }
      break;
    case ElementType::Relation:
      if (_relationIdMap.count(id) > 0)
      {
        result = ElementId::relation(_relationIdMap.at(id));
      }
      else
      {
        long newId = map.createNextRelationId();
        _relationIdMap[id] = newId;
        result = ElementId::relation(newId);
      }
      break;
    default:
      throw IllegalArgumentException("Expected a valid element type, but got: " +
        QString::number(oldId.getType().getEnum()));
    }
  }
  LOG_VART(result);

  return result;
}

void ApiDbReader::_updateMetadataOnElement(ElementPtr element)
{
  bool ok;
  Tags& tags = element->getTags();

  if (tags.contains(MetadataTags::HootStatus()))
  {
    QString statusStr = tags.get(MetadataTags::HootStatus());
    bool ok;
    const int statusInt = statusStr.toInt(&ok);
    Status status = static_cast<Status::Type>(statusInt);
    if (ok && status.getEnum() >= Status::Invalid && status.getEnum() <= Status::Conflated)
    {
      element->setStatus(status);
    }
    else
    {
      if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN("Invalid status: " + statusStr + " for element with ID: " +
                 QString::number(element->getId()));
      }
      else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
    //We don't need to carry this tag around once the value is set on the element...it will
    //be reinstated by some writers, though.
    if (! ConfigOptions().getReaderKeepFileStatus()) { tags.remove(MetadataTags::HootStatus()); }
  }

  if (tags.contains("type"))
  {
    Relation* r = dynamic_cast<Relation*>(element.get());
    if (r)
    {
      r->setType(tags["type"]);
      //I don't think OSM non-hoot metadata tags should be removed here...
      //tags.remove("type");
    }
  }

  if (tags.contains(MetadataTags::ErrorCircular()))
  {
    element->setCircularError(tags.get(MetadataTags::ErrorCircular()).toDouble(&ok));
    if (!ok)
    {
      try
      {
        double tv = tags.getLength(MetadataTags::ErrorCircular()).value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException& /*e*/)
      {
        ok = false;
      }

      if (!ok)
      {
        if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN("Error parsing " + MetadataTags::ErrorCircular() + ".");
        }
        else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    //We don't need to carry this tag around once the value is set on the element...it will
    //be reinstated by some writers, though.
    tags.remove(MetadataTags::ErrorCircular());
  }
  else if (tags.contains(MetadataTags::Accuracy()))
  {
    element->setCircularError(tags.get(MetadataTags::Accuracy()).toDouble(&ok));

    if (!ok)
    {
      try
      {
        double tv = tags.getLength(MetadataTags::Accuracy()).value();
        element->setCircularError(tv);
        ok = true;
      }
      catch (const HootException& /*e*/)
      {
        ok = false;
      }

      if (!ok)
      {
        if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN("Error parsing " + MetadataTags::Accuracy() + ".");
        }
        else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
    //I don't think OSM non-hoot metadata tags should be removed here...
    //tags.remove(MetadataTags::Accuracy());
  }
}

bool ApiDbReader::_isValidBounds(const Envelope& bounds)
{
  if (bounds.isNull() ||
      (bounds.getMinX() == -180.0 && bounds.getMinY() == -90.0 && bounds.getMaxX() == 180.0 &&
       bounds.getMaxY() == 90.0))
  {
    return false;
  }
  return true;
}

void ApiDbReader::_readByBounds(OsmMapPtr map, const Envelope& bounds)
{
  long boundedNodeCount = 0;
  long boundedWayCount = 0;
  long boundedRelationCount = 0;

  LOG_DEBUG("Retrieving node records within the query bounds...");
  boost::shared_ptr<QSqlQuery> nodeItr = _getDatabase()->selectNodesByBounds(bounds);
  QSet<QString> nodeIds;
  while (nodeItr->next())
  {
    const QSqlQuery resultIterator = *nodeItr;
    NodePtr node = _resultToNode(resultIterator, *map);
    LOG_VART(node);
    map->addElement(node);
    boundedNodeCount++;
    //Don't use the mapped id from the node object here, b/c we want don't want to use mapped ids
    //with any queries.  Mapped ids may not exist yet.
    const QString nodeId = QString::number(resultIterator.value(0).toLongLong());
    LOG_VART(nodeId);
    nodeIds.insert(nodeId);
  }
  LOG_VARD(nodeIds.size());

  if (nodeIds.size() > 0)
  {
    LOG_DEBUG("Retrieving way IDs referenced by the selected nodes...");
    QSet<QString> wayIds;
    boost::shared_ptr<QSqlQuery> wayIdItr = _getDatabase()->selectWayIdsByWayNodeIds(nodeIds);
    while (wayIdItr->next())
    {
      const QString wayId = QString::number((*wayIdItr).value(0).toLongLong());
      LOG_VART(wayId);
      wayIds.insert(wayId);
    }
    LOG_VARD(wayIds.size());

    if (wayIds.size() > 0)
    {
      LOG_DEBUG("Retrieving ways by way ID...");
      boost::shared_ptr<QSqlQuery> wayItr =
        _getDatabase()->selectElementsByElementIdList(wayIds, TableType::Way);
      while (wayItr->next())
      {
        //I'm a little confused why this wouldn't cause a problem in that you could be writing ways
        //to the map here whose nodes haven't yet been written to the map yet.  Haven't encountered
        //the problem yet with test data, but will continue to keep an eye on it.
        WayPtr way = _resultToWay(*wayItr, *map);
        map->addElement(way);
        LOG_VART(way);
        boundedWayCount++;
      }

      LOG_DEBUG("Retrieving way node IDs referenced by the selected ways...");
      QSet<QString> additionalWayNodeIds;
      boost::shared_ptr<QSqlQuery> additionalWayNodeIdItr =
        _getDatabase()->selectWayNodeIdsByWayIds(wayIds);
      while (additionalWayNodeIdItr->next())
      {
        const QString nodeId = QString::number((*additionalWayNodeIdItr).value(0).toLongLong());
        LOG_VART(nodeId);
        additionalWayNodeIds.insert(nodeId);
      }

      //subtract nodeIds from additionalWayNodeIds so no dupes get added
      LOG_VARD(nodeIds.size());
      LOG_VARD(additionalWayNodeIds.size());
      additionalWayNodeIds = additionalWayNodeIds.subtract(nodeIds);
      LOG_VARD(additionalWayNodeIds.size());

      if (additionalWayNodeIds.size() > 0)
      {
        nodeIds.unite(additionalWayNodeIds);
        LOG_DEBUG(
          "Retrieving nodes falling outside of the query bounds but belonging to a selected way...");
        boost::shared_ptr<QSqlQuery> additionalWayNodeItr =
          _getDatabase()->selectElementsByElementIdList(additionalWayNodeIds, TableType::Node);
        while (additionalWayNodeItr->next())
        {
          NodePtr node = _resultToNode(*additionalWayNodeItr, *map);
          map->addElement(node);
          LOG_VART(node);
          boundedNodeCount++;
        }
      }
    }

    LOG_DEBUG("Retrieving relation IDs referenced by the selected ways and nodes...");
    QSet<QString> relationIds;
    assert(nodeIds.size() > 0);
    boost::shared_ptr<QSqlQuery> relationIdItr =
      _getDatabase()->selectRelationIdsByMemberIds(nodeIds, ElementType::Node);
    while (relationIdItr->next())
    {
      const QString relationId = QString::number((*relationIdItr).value(0).toLongLong());
      LOG_VART(relationId);
      relationIds.insert(relationId);
    }
    if (wayIds.size() > 0)
    {
      relationIdItr = _getDatabase()->selectRelationIdsByMemberIds(wayIds, ElementType::Way);
      while (relationIdItr->next())
      {
        const QString relationId = QString::number((*relationIdItr).value(0).toLongLong());
        LOG_VART(relationId);
        relationIds.insert(relationId);
      }
    }
    LOG_VARD(relationIds.size());

    if (relationIds.size() > 0)
    {
      LOG_DEBUG("Retrieving relations by relation ID...");
      boost::shared_ptr<QSqlQuery> relationItr =
        _getDatabase()->selectElementsByElementIdList(relationIds, TableType::Relation);
      while (relationItr->next())
      {
        RelationPtr relation = _resultToRelation(*relationItr, *map);
        map->addElement(relation);
        LOG_VART(relation);
        boundedRelationCount++;
      }
    }
  }

  LOG_DEBUG(
    "Bounded query read " << (boundedNodeCount + boundedWayCount + boundedRelationCount) <<
    " total elements.");
  LOG_VARD(boundedNodeCount);
  LOG_VARD(boundedWayCount);
  LOG_VARD(boundedRelationCount);
  LOG_DEBUG("Current map:");
  LOG_VARD(map->getNodes().size());
  LOG_VARD(map->getWays().size());
  LOG_VARD(map->getRelations().size());
}

}
