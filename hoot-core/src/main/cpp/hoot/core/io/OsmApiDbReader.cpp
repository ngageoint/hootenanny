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
#include "OsmApiDbReader.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/io/ApiDb.h>
#include <hoot/core/util/DbUtils.h>

// Qt
#include <QUrl>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmApiDbReader)

OsmApiDbReader::OsmApiDbReader() :
_database(new OsmApiDb())
{
  setConfiguration(conf());
}

OsmApiDbReader::~OsmApiDbReader()
{
  close();
}

void OsmApiDbReader::open(QString urlStr)
{
  if (!isSupported(urlStr))
  {
    throw HootException("An unsupported URL was passed into OsmApiDbReader: " + urlStr);
  }
  initializePartial();

  QUrl url(urlStr);
  LOG_DEBUG("Opening database for reader at: " << url.path() << "...");

  _database->open(url);

  //see comment in similar location in HootApiDbReader::open
  _database->transaction();
  _open = true;
}

void OsmApiDbReader::_parseAndSetTagsOnElement(ElementPtr element)
{
  //We should see if these tags can be read out at the same time the element itself is read out...

  QStringList tags;
  boost::shared_ptr<QSqlQuery> tagItr;
  switch (element->getElementType().getEnum())
  {
    case ElementType::Node:
      tagItr = _database->selectTagsForNode(element->getId());
      break;
    case ElementType::Way:
      tagItr = _database->selectTagsForWay(element->getId());
      break;
    case ElementType::Relation:
      tagItr = _database->selectTagsForRelation(element->getId());
      break;
    default:
      throw HootException("Invalid element type.");
  }
  while (tagItr->next())
  {
    // test for blank tag
    QString val1 = tagItr->value(1).toString();
    QString val2 = tagItr->value(2).toString();
    QString tag = "";
    if (val1!="" || val2!="")
    {
      tag = "\""+val1+"\"=>\""+val2+"\"";
    }
    if (tag != "")
    {
      tags << tag;
    }
  }
  if (tags.size() > 0)
  {
    element->setTags(ApiDb::unescapeTags(tags.join(", ")));
  }
}

NodePtr OsmApiDbReader::_resultToNode(const QSqlQuery& resultIterator, OsmMap& map)
{
  LOG_TRACE("Converting query result to node...");

  const long rawId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << rawId);
  const long nodeId = _mapElementId(map, ElementId::node(rawId)).getId();
  LOG_VART(nodeId);
  const double lat =
    resultIterator.value(ApiDb::NODES_LATITUDE).toLongLong() / (double)ApiDb::COORDINATE_SCALE;
  const double lon =
    resultIterator.value(ApiDb::NODES_LONGITUDE).toLongLong() / (double)ApiDb::COORDINATE_SCALE;

  NodePtr node(
    Node::newSp(
      _status,
      nodeId,
      lon,
      lat,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::NODES_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::NODES_VERSION).toLongLong(),
      resultIterator.value(ApiDb::NODES_TIMESTAMP).toUInt()));

  _parseAndSetTagsOnElement(node);
  _updateMetadataOnElement(node);
  //we want the reader's status to always override any existing status
  if (!ConfigOptions().getReaderKeepFileStatus() && _status != Status::Invalid)
  {
    node->setStatus(_status);
  }
  LOG_VART(node->getStatus());

  LOG_VART(node->getVersion());

  return node;
}

WayPtr OsmApiDbReader::_resultToWay(const QSqlQuery& resultIterator, OsmMap& map)
{
  LOG_TRACE("Converting query result to way...");

  const long wayId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << wayId);
  const long newWayId = _mapElementId(map, ElementId::way(wayId)).getId();
  LOG_TRACE("Reading way with ID: " << wayId);
  if (newWayId != wayId)
  {
    LOG_VARD(newWayId);
  }

  WayPtr way(
    new Way(
      _status,
      newWayId,
      ConfigOptions().getCircularErrorDefaultValue(),
      resultIterator.value(ApiDb::WAYS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::WAYS_VERSION).toLongLong(),
      resultIterator.value(ApiDb::WAYS_TIMESTAMP).toUInt()));

  // if performance becomes an issue, try reading these out in batch at the same time
  // the element results are read
  vector<long> nodeIds = _database->selectNodeIdsForWay(wayId);
  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    nodeIds[i] = _mapElementId(map, ElementId::node(nodeIds[i])).getId();
  }
  way->addNodes(nodeIds);

  _parseAndSetTagsOnElement(way);
  _updateMetadataOnElement(way);
  //we want the reader's status to always override any existing status
  if (!ConfigOptions().getReaderKeepFileStatus() && _status != Status::Invalid)
  {
    way->setStatus(_status);
  }
  LOG_VART(way->getStatus());

  return way;
}

RelationPtr OsmApiDbReader::_resultToRelation(const QSqlQuery& resultIterator, const OsmMap& map)
{
  LOG_TRACE("Converting query result to relation...");

  const long relationId = resultIterator.value(0).toLongLong();
  LOG_TRACE("raw ID: " << relationId);
  const long newRelationId = _mapElementId(map, ElementId::relation(relationId)).getId();
  LOG_TRACE("Reading relation with ID: " << relationId);
  if (newRelationId != relationId)
  {
    LOG_VART(newRelationId);
  }

  RelationPtr relation(
    new Relation(
      _status,
      newRelationId,
      ConfigOptions().getCircularErrorDefaultValue(),
      "",
      resultIterator.value(ApiDb::RELATIONS_CHANGESET).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_VERSION).toLongLong(),
      resultIterator.value(ApiDb::RELATIONS_TIMESTAMP).toUInt()));

  _parseAndSetTagsOnElement(relation);

  // These could be read out in batch at the same time the element results are read.
  vector<RelationData::Entry> members = _database->selectMembersForRelation(relationId);
  for (size_t i = 0; i < members.size(); ++i)
  {
    members[i].setElementId(_mapElementId(map, members[i].getElementId()));
  }
  relation->setMembers(members);

  _parseAndSetTagsOnElement(relation);
  _updateMetadataOnElement(relation);
  //we want the reader's status to always override any existing status
  if (!ConfigOptions().getReaderKeepFileStatus() && _status != Status::Invalid)
  {
    relation->setStatus(_status);
  }
  LOG_VART(relation->getStatus());

  return relation;
}

void OsmApiDbReader::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);
  setUserEmail(configOptions.getApiDbEmail());
  setBoundingBox(configOptions.getConvertBoundingBox());
  setOverrideBoundingBox(configOptions.getConvertBoundingBoxOsmApiDatabase());
  setSortById(configOptions.getApiDbReaderSortById());
}

}
