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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HootApiDbReader.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QUrl>
#include <QDateTime>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, HootApiDbReader)

HootApiDbReader::HootApiDbReader() :
_database(new HootApiDb())
{
  setConfiguration(conf());
}

HootApiDbReader::~HootApiDbReader()
{
  close();
}

Envelope HootApiDbReader::calculateEnvelope() const
{
  assert(_open);
  Envelope result = _database->calculateEnvelope();
  return result;
}

void HootApiDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed in to HootApiDbReader: " + urlStr);
  }
  initializePartial();

  QUrl url(urlStr);
  QStringList pList = url.path().split("/");
  LOG_DEBUG("Opening database for reader at: " << url.path() << "...");
  bool ok;
  _database->open(url);

  long requestedMapId = pList[pList.size() - 1].toLong(&ok);

  if (!ok)
  {
    if (_email == "")
    {
      throw HootException("If a map name is specified then the user email must also be specified "
                          "via: " + ConfigOptions::getApiDbEmailKey());
    }

    QString mapName = pList[pList.size() - 1];
    _database->setUserId(_database->getUserId(_email, false));
    set<long> mapIds = _database->selectMapIds(mapName);
    if (mapIds.size() != 1)
    {
      QString str =
        QString("Expected 1 map with the name '%1' but found %2 maps.")
          .arg(mapName)
          .arg(mapIds.size());
      throw HootException(str);
    }
    requestedMapId = *mapIds.begin();
  }

  if (!_database->mapExists(requestedMapId))
  {
    _database->close();
    throw HootException("No map exists with ID: " + QString::number(requestedMapId));
  }
  _database->setMapId(requestedMapId);

  //using a transaction seems to make sense here, b/c we don't want to read a map being modified
  //in the middle of its modification caused by a changeset upload, which could cause the map to
  //be invalid as a whole
  _database->transaction();
  _open = true;
}

NodePtr HootApiDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  LOG_TRACE("Converting query result to node...");

  long nodeId = _mapElementId(map, ElementId::node(resultIterator.value(0).toLongLong())).getId();
  LOG_VART(ElementId(ElementType::Node, nodeId));

  // Timestamp
  QDateTime dt = resultIterator.value(ApiDb::NODES_TIMESTAMP).toDateTime();
  dt.setTimeSpec(Qt::UTC);

  NodePtr node(
    Node::newSp(
      _status,
      nodeId,
      resultIterator.value(ApiDb::NODES_LONGITUDE).toDouble(),
      resultIterator.value(ApiDb::NODES_LATITUDE).toDouble(),
      -1,
      resultIterator.value(ApiDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::NODES_VERSION).toLongLong(),
      dt.toMSecsSinceEpoch() / 1000));

  node->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::NODES_TAGS)));
  _updateMetadataOnElement(node);

  // We want the reader's status to always override any existing status
  // Unless, we really want to keep the status.
  if (!_keepStatusTag && _status != Status::Invalid)
  {
    node->setStatus(_status);
  }

  LOG_VART(node->getVersion());

  return node;
}

WayPtr HootApiDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  LOG_TRACE("Converting query result to way...");

  const long wayId = resultIterator.value(0).toLongLong();
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  LOG_VART(ElementId(ElementType::Way, wayId));

  // Timestamp
  QDateTime dt = resultIterator.value(ApiDb::WAYS_TIMESTAMP).toDateTime();
  dt.setTimeSpec(Qt::UTC);

  WayPtr way(
    new Way(
      _status,
      newWayId,
      -1,
      resultIterator.value(ApiDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::WAYS_VERSION).toLongLong(),
      dt.toMSecsSinceEpoch() / 1000));

  way->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::WAYS_TAGS)));
  _updateMetadataOnElement(way);
  //we want the reader's status to always override any existing status
  if (!_keepStatusTag && _status != Status::Invalid)
  {
    way->setStatus(_status);
  }
  LOG_VART(way->getStatus());

  // these maybe could be read out in batch at the same time the element results are read...
  vector<long> nodeIds = _database->selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  return way;
}

RelationPtr HootApiDbReader::_resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map)
{
  LOG_TRACE("Converting query result to relation...");

  const long relationId = resultIterator.value(0).toLongLong();
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();
  LOG_VART(ElementId(ElementType::Relation, relationId));

  // Timestamp
  QDateTime dt = resultIterator.value(ApiDb::RELATIONS_TIMESTAMP).toDateTime();
  dt.setTimeSpec(Qt::UTC);

  RelationPtr relation(
    new Relation(
      _status,
      newRelationId,
      -1,
      "",/*MetadataTags::RelationCollection()*/ //services db doesn't support relation "type" yet
      resultIterator.value(ApiDb::RELATIONS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_VERSION).toLongLong(),
      dt.toMSecsSinceEpoch() / 1000));

  relation->setTags(ApiDb::unescapeTags(resultIterator.value(ApiDb::RELATIONS_TAGS)));
  _updateMetadataOnElement(relation);
  //we want the reader's status to always override any existing status
  if (!_keepStatusTag && _status != Status::Invalid)
  {
    relation->setStatus(_status);
  }

  // these maybe could be read out in batch at the same time the element results are read...
  vector<RelationData::Entry> members = _database->selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  return relation;
}

void HootApiDbReader::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setMaxElementsPerMap(configOptions.getMaxElementsPerPartialMap());
  setUserEmail(configOptions.getApiDbEmail());
  setBoundingBox(configOptions.getConvertBoundingBox());
  setOverrideBoundingBox(configOptions.getConvertBoundingBoxHootApiDatabase());
}

}
