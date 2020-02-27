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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiChangeset.h"

//  Hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

//  Standard
#include <algorithm>
#include <vector>

//  Qt
#include <QRegularExpression>

using namespace std;

namespace hoot
{

XmlChangeset::XmlChangeset()
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0)
{
}

XmlChangeset::XmlChangeset(const QList<QString> &changesets)
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0)
{
  for (QList<QString>::const_iterator it = changesets.begin(); it != changesets.end(); ++it)
    loadChangeset(*it);
}

void XmlChangeset::loadChangeset(const QString &changeset)
{
  if (QFile::exists(changeset))
    loadChangesetFile(changeset);
  else
    loadChangesetXml(changeset);
}

void XmlChangeset::loadChangesetFile(const QString& path)
{
  //  Attempt to open the file
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
  {
    LOG_ERROR("Unable to read file at: " << path);
    return;
  }
  //  Open the XML stream reader and attach it to the file
  QXmlStreamReader reader(&file);
  //  Read the OSC changeset
  if (path.endsWith(".osc"))
    loadChangeset(reader);
  else  //  .osm file
    loadOsmAsChangeset(reader);
}

void XmlChangeset::loadChangesetXml(const QString &changesetXml)
{
  //  Load the XML directly into the reader
  QXmlStreamReader reader(changesetXml);
  //  Load the changeset formatted XML
  loadChangeset(reader);
}

void XmlChangeset::loadChangeset(QXmlStreamReader &reader)
{
  //  Make sure that the XML provided starts with the <diffResult> tag
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  if (type == QXmlStreamReader::StartElement && reader.name() != "osmChange")
  {
    LOG_ERROR("Unknown changeset file format.");
    return;
  }
  //  Iterate all of updates and record them
  while (!reader.atEnd() && !reader.hasError())
  {
    type = reader.readNext();
    if (type == QXmlStreamReader::StartElement)
    {
      QStringRef name = reader.name();
      if (name == "create")
        loadElements(reader, ChangesetType::TypeCreate);
      else if (name == "modify")
        loadElements(reader, ChangesetType::TypeModify);
      else if (name == "delete")
        loadElements(reader, ChangesetType::TypeDelete);
    }
  }
}

void XmlChangeset::loadOsmAsChangeset(QXmlStreamReader& reader)
{
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  if (type == QXmlStreamReader::StartElement && reader.name() != "osm")
  {
    LOG_ERROR("Unknown OSM XML file format.");
    return;
  }
  //  Force load all of the elements as 'create' elements
  loadElements(reader, ChangesetType::TypeCreate);
}

void XmlChangeset::loadElements(QXmlStreamReader& reader, ChangesetType changeset_type)
{
  ChangesetElementPtr element;
  while (!reader.atEnd() && !reader.hasError())
  {
    QXmlStreamReader::TokenType type = reader.readNext();
    QString name = reader.name().toString().toLower();
    //  Start element for nodes/ways/relations and tags/nds/members
    if (type == QXmlStreamReader::StartElement)
    {
      if (name == "node")
      {
        element.reset(new ChangesetNode(XmlObject(name, reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Node, id);
      }
      else if (name == "way")
      {
        element.reset(new ChangesetWay(XmlObject(name, reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Way, id);
      }
      else if (name == "relation")
      {
        element.reset(new ChangesetRelation(XmlObject(name, reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Relation, id);
      }
      else if (name == "tag")
        element->addTag(XmlObject(name, reader.attributes()));
      else if (name == "nd")
      {
        long id = reader.attributes().value("ref").toString().toLong();
        std::dynamic_pointer_cast<ChangesetWay>(element)->addNode(id);
        //  Update the node to way map
        _nodeIdsToWays[id].insert(element->id());
      }
      else if (name == "member")
      {
        std::shared_ptr<ChangesetRelation> relation = std::dynamic_pointer_cast<ChangesetRelation>(element);
        relation->addMember(reader.attributes());
        ChangesetRelationMember& member = relation->getMember(relation->getMemberCount() - 1);
        //  Update the node/way/relation to relation maps
        if (member.isNode())
          _nodeIdsToRelations[member.getRef()].insert(element->id());
        else if (member.isWay())
          _wayIdsToRelations[member.getRef()].insert(element->id());
        else if (member.isRelation())
          _relationIdsToRelations[member.getRef()].insert(element->id());
      }
    }
    //  End element for create/modify/delete
    else if (type == QXmlStreamReader::EndElement)
    {
      if (name == "node")
      {
        _nodes[changeset_type][element->id()] = element;
        _allNodes[element->id()] = element;
      }
      else if (name == "way")
      {
        _ways[changeset_type][element->id()] = element;
        _allWays[element->id()] = element;
      }
      else if (name == "relation")
      {
        _relations[changeset_type][element->id()] = element;
        _allRelations[element->id()] = element;
      }
      else if (name == "create" || name == "modify" || name == "delete")
        return;
    }
  }
}

void XmlChangeset::splitLongWays(long maxWayNodes)
{
  //  Negative numbers means there is no max set
  if (maxWayNodes < 0)
    return;
  //  Iterate all of the changeset types except delete
  for (int type = TypeCreate; type < TypeDelete; ++type)
  {
    for (ChangesetElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(it->second.get());
      while (way->getNodeCount() > maxWayNodes)
      {
        //  Create a copy of the way
        ChangesetWayPtr newWay(new ChangesetWay(*way));
        newWay->changeId(getNextWayId());
        //  Remove maxWayNodes from the original and add them to this way
        way->removeNodes(0, maxWayNodes - 1);
        newWay->removeNodes(maxWayNodes);
        //  Save the new way
        _idMap.addId(ElementType::Way, newWay->id());
        _ways[TypeCreate][newWay->id()] = newWay;
        _allWays[newWay->id()] = newWay;
      }
    }
  }
}

void XmlChangeset::fixMalformedInput()
{
  //  Element adds cannot have a positive ID, must be negative
  for (ChangesetElementMap::iterator it = _relations[TypeCreate].begin(); it != _relations[TypeCreate].end(); ++it)
  {
    long old_id = it->first;
    if (old_id > 0)
    {
      //  Get a new negative relation ID
      long new_id = getNextRelationId();
      //  Replace old_id everywhere with new_id, in the relation and in other relations
      replaceRelationId(old_id, new_id);
    }
  }
  for (ChangesetElementMap::iterator it = _ways[TypeCreate].begin(); it != _ways[TypeCreate].end(); ++it)
  {
    long old_id = it->first;
    if (old_id > 0)
    {
      //  Get a new negative way ID
      long new_id = getNextWayId();
      //  Replace old_id everywhere with new_id, in the way and in any relations
      replaceWayId(old_id, new_id);
    }
  }
  for (ChangesetElementMap::iterator it = _nodes[TypeCreate].begin(); it != _nodes[TypeCreate].end(); ++it)
  {
    long old_id = it->first;
    if (old_id > 0)
    {
      //  Get a new negative node ID
      long new_id = getNextNodeId();
      //  Replace old_id everywhere with new_id, in the node, and in any ways and relations
      replaceNodeId(old_id, new_id);
    }
  }
  //  Element modifies/deletes cannot have a negative ID, must be positive
  //  Nothing can be done about an element modify/delete if the ID is negative
  for (int type = TypeModify; type < TypeMax; ++type)
  {
    for (ChangesetElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
    {
      long node_id = it->first;
      //  Set the node's status to failed if negative
      if (node_id < 1)
        failNode(node_id, true);
    }
    for (ChangesetElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      long way_id = it->first;
      //  Set the node's status to failed if negative
      if (way_id < 1)
        failWay(way_id, true);
    }
    for (ChangesetElementMap::iterator it = _relations[type].begin(); it != _relations[type].end(); ++it)
    {
      long relation_id = it->first;
      //  Set the node's status to failed if negative
      if (relation_id < 1)
        failRelation(relation_id, true);
    }
  }
  //  Output the error file if there are errors
  writeErrorFile();
}

void XmlChangeset::updateChangeset(const QString &changes)
{
  /* <diffResult generator="OpenStreetMap Server" version="0.6">
   *   <node|way|relation old_id="#" new_id="#" new_version="#"/>
   *   ...
   * </diffResult>
   */
  QXmlStreamReader reader(changes);
  //  Make sure that the XML provided starts with the <diffResult> tag
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::Invalid)
  {
    LOG_ERROR("Invalid changeset response.");
    return;
  }
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  if (type == QXmlStreamReader::StartElement && reader.name() != "diffResult")
  {
    LOG_WARN("Unknown changeset response format.");
    return;
  }
  //  Iterate all of updates and record them
  while (!reader.atEnd() && !reader.hasError())
  {
    type = reader.readNext();
    if (type == QXmlStreamReader::StartElement)
    {
      QStringRef name = reader.name();
      QXmlStreamAttributes attributes = reader.attributes();
      //  Get the old and new IDs
      long old_id = 0;
      long new_id = 0;
      long version = ElementData::VERSION_EMPTY;
      if (attributes.hasAttribute("old_id"))
          old_id = attributes.value("old_id").toString().toLong();
      if (attributes.hasAttribute("new_id"))
        new_id = attributes.value("new_id").toString().toLong();
      if (attributes.hasAttribute("new_version"))
        version = attributes.value("new_version").toString().toLong();
      //  Update the old ID to the new ID and the version
      if (name == "node")
        updateElement(_nodes, old_id, new_id, version);
      else if (name == "way")
        updateElement(_ways, old_id, new_id, version);
      else if (name == "relation")
        updateElement(_relations, old_id, new_id, version);
      if (old_id == 0)
      {
        LOG_ERROR("Element cannot be updated. No ID given.");
      }
    }
  }
}

bool XmlChangeset::fixChangeset(const QString& update)
{
  /* <osm>
   *  <node|way|relation id="#" lat="#" lon="#" version="#" changeset="#" user="#" uid="#" visible="#" timestamp="#">
   *   <tag k="..." v="..."/>
   *   ...
   *  </node>
   * </osm>
   */
  bool success = false;
  QXmlStreamReader reader(update);
  //  Make sure that the XML provided starts with the <osm> tag
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  if (type == QXmlStreamReader::StartElement && reader.name() != "osm")
  {
    LOG_WARN("Unknown element fix format.");
    return success;
  }
  long id = 0;
  long version = 0;
  QMap<QString, QString> tags;
  //  Iterate all of updates and record them
  while (!reader.atEnd() && !reader.hasError())
  {
    type = reader.readNext();
    QStringRef name = reader.name();
    QXmlStreamAttributes attributes = reader.attributes();
    if (type == QXmlStreamReader::StartElement)
    {
      if (name == "node" || name == "way" || name == "relation")
      {
        if (attributes.hasAttribute("id"))
          id = attributes.value("id").toString().toLong();
        if (attributes.hasAttribute("version"))
          version = attributes.value("version").toString().toLong();
      }
      else if (name == "tag" && attributes.hasAttribute("k") && attributes.hasAttribute("v"))
        tags[attributes.value("k").toString()] = attributes.value("v").toString();
    }
    else if (type == QXmlStreamReader::EndElement)
    {
      bool reset = false;
      //  Fix the element
      if (name == "node")
      {
        success |= fixElement(_nodes, id, version, tags);
        reset = true;
      }
      else if (name == "way")
      {
        success |= fixElement(_ways, id, version, tags);
        reset = true;
      }
      else if (name == "relation")
      {
        success |= fixElement(_relations, id, version, tags);
        reset = true;
      }
      //  Reset the id, version, and tags
      if (reset)
      {
        id = 0;
        version = 0;
        tags.clear();
      }
    }
  }
  return success;
}

bool XmlChangeset::addNodes(ChangesetInfoPtr& changeset, ChangesetType type)
{
  bool added = false;
  //  Iterate all of the nodes of "type" in the changeset
  for (ChangesetElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
  {
    //  Add nodes up until the max changeset
    if (changeset->size() < (size_t)_maxPushSize)
      added |= addNode(changeset, type, dynamic_cast<ChangesetNode*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addNode(ChangesetInfoPtr& changeset, ChangesetType type, ChangesetNode* node)
{
  //  Only add the nodes that are "sendable"
  if (canSend(node))
  {
    //  Add create nodes if the ID map's ID is negative, modify IDs don't matter
    if ((type == ChangesetType::TypeCreate && _idMap.getId(ElementType::Node, node->id()) < 0) ||
         type != ChangesetType::TypeDelete)
    {
      //  Add the node
      changeset->add(ElementType::Node, type, node->id());
      markBuffered(node);
      return true;
    }
    else if (type == ChangesetType::TypeDelete)
    {
      bool sendable = true;
      //  Deleting nodes can't happen until the ways and relations that have the node are deleted
      if (_nodeIdsToRelations.find(node->id()) != _nodeIdsToRelations.end())
      {
        std::set<long> relationIds = _nodeIdsToRelations[node->id()];
        sendable = addParentRelations(changeset, relationIds);
      }
      if (_nodeIdsToWays.find(node->id()) != _nodeIdsToWays.end())
      {
        std::set<long> wayIds = _nodeIdsToWays[node->id()];
        sendable = addParentWays(changeset, wayIds);
      }
      if (sendable)
      {
        //  Add the node
        changeset->add(ElementType::Node, type, node->id());
        markBuffered(node);
        return true;
      }
    }
  }
  return false;
}

void XmlChangeset::moveOrRemoveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node)
{
  //  Move the node from source to destination if possible, or remove it from the source
  if (canMoveNode(source, destination, type, node))
  {
    //  Move the node
    moveNode(source, destination, type, node);
  }
  else
  {
    //  Remove only the node
    source->remove(ElementType::Node, type, node->id());
    //  Set the node to available
    node->setStatus(ChangesetElement::ElementStatus::Available);
  }
}

bool XmlChangeset::moveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node)
{
  //  Add the node to the destination and remove from the source
  destination->add(ElementType::Node, type, node->id());
  source->remove(ElementType::Node, type, node->id());
  return true;
}

bool XmlChangeset::canMoveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType /*type*/, ChangesetNode* /*node*/)
{
  return source->size() != 1;
}

bool XmlChangeset::addWays(ChangesetInfoPtr& changeset, ChangesetType type)
{
  bool added = false;
  //  Iterate all of the ways of "type" in the changeset
  for (ChangesetElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
  {
    //  Add ways up until the max changeset
    if (changeset->size() < (size_t)_maxPushSize)
      added |= addWay(changeset, type, dynamic_cast<ChangesetWay*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addWay(ChangesetInfoPtr& changeset, ChangesetType type, ChangesetWay* way)
{
  if (canSend(way))
  {
    bool sendable = true;
    //  Only creates/modifies require pre-processing
    if (type != ChangesetType::TypeDelete)
    {
      //  Add any nodes that need to be created
      for (int i = 0; i < way->getNodeCount(); ++i)
      {
        //  Negative IDs from the ID map are for created nodes
        if (_idMap.getId(ElementType::Node, way->getNode(i)) < 0)
        {
          ChangesetNode* node = dynamic_cast<ChangesetNode*>(_allNodes[way->getNode(i)].get());
          addNode(changeset, ChangesetType::TypeCreate, node);
        }
      }
    }
    else
    {
      if (_wayIdsToRelations.find(way->id()) != _wayIdsToRelations.end())
      {
        std::set<long> relationIds = _wayIdsToRelations[way->id()];
        sendable = addParentRelations(changeset, relationIds);
      }
    }
    //  Add the way last
    if (sendable)
    {
      changeset->add(ElementType::Way, type, way->id());
      markBuffered(way);
      return true;
    }
  }
  return false;
}

bool XmlChangeset::addParentWays(ChangesetInfoPtr& changeset, const std::set<long>& way_ids)
{
  bool sendable = true;
  for (std::set<long>::iterator it = way_ids.begin(); it != way_ids.end(); ++it)
  {
    long wayId = *it;
    //  The relation is either a modify or a delete, add it to the changeset
    if (_ways[ChangesetType::TypeModify].find(wayId) != _ways[ChangesetType::TypeModify].end())
    {
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[wayId].get());
      //  Check ways that aren't already in this changeset or finished
      if (!changeset->contains(ElementType::Way, ChangesetType::TypeModify, way->id()) &&
          way->getStatus() != ChangesetElement::Finalized)
      {
        if (canSend(way))
        {
          changeset->add(ElementType::Way, ChangesetType::TypeModify, wayId);
          markBuffered(way);
        }
        else if (way->getStatus() != ChangesetElement::Finalized)
          sendable = false;
      }
    }
    else if (_ways[ChangesetType::TypeDelete].find(wayId) != _ways[ChangesetType::TypeDelete].end())
    {
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[wayId].get());
      //  Check ways that aren't already in this changeset or finished
      if (!changeset->contains(ElementType::Way, ChangesetType::TypeDelete, way->id()) &&
          way->getStatus() != ChangesetElement::Finalized)
      {
        if (canSend(way))
        {
          changeset->add(ElementType::Way, ChangesetType::TypeDelete, wayId);
          markBuffered(way);
        }
        else
          sendable = false;
      }
    }
  }
  return sendable;
}

void XmlChangeset::moveOrRemoveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way)
{
  if (canMoveWay(source, destination, type, way))
  {
    //  Move the way and anything associated
    moveWay(source, destination, type, way);
  }
  else
  {
    //  Remove only the way from the changeset, not its nodes
    source->remove(ElementType::Way, type, way->id());
    //  Set the way to available
    way->setStatus(ChangesetElement::ElementStatus::Available);
  }
}

bool XmlChangeset::moveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way)
{
  //  Don't worry about the contents of a delete operation
  if (type != ChangesetType::TypeDelete)
  {
    //  Iterate all of the nodes that exist in the changeset and move them
    for (int i = 0; i < way->getNodeCount(); ++i)
    {
      long id = way->getNode(i);
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (source->contains(ElementType::Node, (ChangesetType)current_type, id))
          moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetNode*>(_allNodes[id].get()));
      }
    }
  }
  //  Add the way to the destination and remove from the source
  destination->add(ElementType::Way, type, way->id());
  source->remove(ElementType::Way, type, way->id());
  return true;
}

bool XmlChangeset::canMoveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType type, ChangesetWay* way)
{
  //  Deleting a way will only contain the way itself
  if (type == ChangesetType::TypeDelete)
    return source->size() == 1;
  //  Get the count of nodes and way that make up this "change"
  size_t count = getObjectCount(source, way);
  //  Compare that count to the size left in the source changeset
  return source->size() != count;
}

bool XmlChangeset::addRelations(ChangesetInfoPtr& changeset, ChangesetType type)
{
  bool added = false;
  //  Iterate all of the ways of "type" in the changeset
  for (ChangesetElementMap::iterator it = _relations[type].begin(); it != _relations[type].end(); ++it)
  {
    //  Add relations up until the max changeset
    if (changeset->size() < (size_t)_maxPushSize)
      added |= addRelation(changeset, type, dynamic_cast<ChangesetRelation*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addRelation(ChangesetInfoPtr& changeset, ChangesetType type, ChangesetRelation* relation)
{
  if (canSend(relation))
  {
    bool sendable = true;
    //  Deletes require no pre-processing
    if (type != ChangesetType::TypeDelete)
    {
      //  Add any relation members that need to be added
      for (int i = 0; i < relation->getMemberCount(); ++i)
      {
        ChangesetRelationMember& member = relation->getMember(i);
        //  Negative IDs are for added members
        if (member.getRef() < 0)
        {
          //  Add the types differently
          if (member.isNode())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getId(ElementType::Node, member.getRef()) < 0)
            {
              ChangesetNode* node = dynamic_cast<ChangesetNode*>(_allNodes[member.getRef()].get());
              addNode(changeset, type, node);
            }
          }
          else if (member.isWay())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getId(ElementType::Way, member.getRef()) < 0)
            {
              ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[member.getRef()].get());
              addWay(changeset, type, way);
            }
          }
          else if (member.isRelation())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getId(ElementType::Relation, member.getRef()) < 0)
            {
              ChangesetRelation* relation_member = dynamic_cast<ChangesetRelation*>(_allRelations[member.getRef()].get());
              //  Don't re-add self referencing relations
              if (relation->id() != relation_member->id())
                addRelation(changeset, type, relation_member);
            }
          }
        }
      }
    }
    else
    {
      if (_relationIdsToRelations.find(relation->id()) != _relationIdsToRelations.end())
      {
        std::set<long> relationIds = _relationIdsToRelations[relation->id()];
        sendable = addParentRelations(changeset, relationIds);
      }
    }
    //  Add the relation last
    if (sendable)
    {
      changeset->add(ElementType::Relation, type, relation->id());
      markBuffered(relation);
      return true;
    }
  }
  return false;
}

bool XmlChangeset::addParentRelations(ChangesetInfoPtr& changeset, const std::set<long>& relation_ids)
{
  bool sendable = true;
  for (std::set<long>::iterator it = relation_ids.begin(); it != relation_ids.end(); ++it)
  {
    long relationId = *it;
    //  The relation is either a modify or a delete, add it to the changeset
    if (_relations[ChangesetType::TypeModify].find(relationId) != _relations[ChangesetType::TypeModify].end())
    {
      ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[relationId].get());
      //  Relations in this changeset or ones that are done don't need to be added
      if (!changeset->contains(ElementType::Relation, ChangesetType::TypeModify, relation->id()) &&
          relation->getStatus() != ChangesetElement::Finalized)
      {
        if (canSend(relation))
        {
          changeset->add(ElementType::Relation, ChangesetType::TypeModify, relationId);
          markBuffered(relation);
        }
        else
          sendable = false;
      }
    }
    else if (_relations[ChangesetType::TypeDelete].find(relationId) != _relations[ChangesetType::TypeDelete].end())
    {
      ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[relationId].get());
      //  Relations in this changeset or ones that are done don't need to be added
      if (!changeset->contains(ElementType::Relation, ChangesetType::TypeDelete, relation->id()) &&
          relation->getStatus() != ChangesetElement::Finalized)
      {
        if (canSend(relation))
        {
          changeset->add(ElementType::Relation, ChangesetType::TypeDelete, relationId);
          markBuffered(relation);
        }
        else
          sendable = false;
      }
    }
  }
  return sendable;

}

void XmlChangeset::moveOrRemoveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation)
{
  if (canMoveRelation(source, destination, type, relation))
  {
    //  Move the relation and anything associated
    moveRelation(source, destination, type, relation);
  }
  else
  {
    //  Remove only the relation from the changeset, not its members
    source->remove(ElementType::Relation, type, relation->id());
    //  Set the relation to available
    relation->setStatus(ChangesetElement::ElementStatus::Available);
  }
}

bool XmlChangeset::moveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation)
{
  //  Don't worry about the contents of a delete operation
  if (type != ChangesetType::TypeDelete)
  {
    //  Iterate all of the nodes that exist in the changeset and move them
    for (int i = 0; i < relation->getMemberCount(); ++i)
    {
      ChangesetRelationMember& member = relation->getMember(i);
      long id = member.getRef();
      //  Each member type iterates the changeset types looking for this element, then moves it
      //  using the corresponding move function
      if (member.isNode())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Node, (ChangesetType)current_type, id))
            moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetNode*>(_allNodes[id].get()));
        }
      }
      else if (member.isWay())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Way, (ChangesetType)current_type, id))
            moveWay(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetWay*>(_allWays[id].get()));
        }
      }
      else if (member.isRelation())
      {
        //  Don't attempt to move circular relation references
        if (id != relation->id())
        {
          for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
          {
            if (source->contains(ElementType::Relation, (ChangesetType)current_type, id))
              moveRelation(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetRelation*>(_allRelations[id].get()));
          }
        }
      }
    }
  }
  //  Add the way to the destination and remove from the source
  destination->add(ElementType::Relation, type, relation->id());
  source->remove(ElementType::Relation, type, relation->id());
  return true;
}

bool XmlChangeset::canMoveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType type, ChangesetRelation* relation)
{
  //  Deleting a relation will only contain the relation itself
  if (type == ChangesetType::TypeDelete)
    return source->size() == 1;
  //  Get the count of nodes, ways, and relations that make up this "change"
  size_t count = getObjectCount(source, relation);
  //  Compare that count to the size left in the source changeset
  return source->size() != count;
}

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& /*changeset*/, ChangesetNode* node)
{
  if (node == NULL)
    return 0;
  //  Nodes count as one object
  return 1;
}

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& changeset, ChangesetWay* way)
{
  if (way == NULL)
    return 0;
  //  Get the count of nodes and way that make up this "change"
  size_t count = 1;
  for (int i = 0; i < way->getNodeCount(); ++i)
  {
    long id = way->getNode(i);
    for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
    {
      if (changeset->contains(ElementType::Node, (ChangesetType)current_type, id))
        ++count;
    }
  }
  return count;
}

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& changeset, ChangesetRelation* relation)
{
  if (relation == NULL)
    return 0;
  //  Get the count of nodes, ways, and relations that make up this "change"
  size_t count = 1;
  for (int i = 0; i < relation->getMemberCount(); ++i)
  {
    ChangesetRelationMember& member = relation->getMember(i);
    long id = member.getRef();
    //  Each member type iterates the changeset types looking for this element, then counts it
    if (member.isNode())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Node, (ChangesetType)current_type, id))
          count += getObjectCount(changeset, dynamic_cast<ChangesetNode*>(_allNodes[id].get()));
      }
    }
    else if (member.isWay())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Way, (ChangesetType)current_type, id))
          count += getObjectCount(changeset, dynamic_cast<ChangesetWay*>(_allWays[id].get()));
      }
    }
    else if (member.isRelation())
    {
      //  Don't recount circular referenced relations
      if (id != relation->id())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (changeset->contains(ElementType::Relation, (ChangesetType)current_type, id))
            count += getObjectCount(changeset, dynamic_cast<ChangesetRelation*>(_allRelations[id].get()));
        }
      }
    }
  }
  return count;
}

bool XmlChangeset::isSent(ChangesetElement* element)
{
  if (element == NULL)
    return false;
  else
    //  Sent means Buffering, Sent, or Finalized
    return element->getStatus() == ChangesetElement::ElementStatus::Buffering ||
           (element->getStatus() == ChangesetElement::ElementStatus::Sent && element->id() > 0) ||
           element->getStatus() == ChangesetElement::ElementStatus::Finalized;
}

bool XmlChangeset::canSend(ChangesetNode* node)
{
  //  Able to send means Available
  if (node == NULL)
    return false;
  else
    return node->getStatus() == ChangesetElement::Available;
}

bool XmlChangeset::canSend(ChangesetWay* way)
{
  //  Able to send means Available
  if (way == NULL)
    return false;
  else if (way->getStatus() != ChangesetElement::Available)
    return false;
  else
  {
    //  All nodes have to be Available, Buffered, or Finalized
    for (int i = 0; i < way->getNodeCount(); ++i)
    {
      long id = way->getNode(i);
      if (_allNodes.find(id) != _allNodes.end() &&
          !isSent(_allNodes[id].get()) &&
          !canSend(dynamic_cast<ChangesetNode*>(_allNodes[id].get())))
        return false;
    }
  }
  return true;
}

bool XmlChangeset::canSend(ChangesetRelation* relation)
{
  //  Able to send means Available
  if (relation == NULL)
    return false;
  else if (relation->getStatus() != ChangesetElement::Available)
    return false;
  else
  {
    //  All members have to be Available or Finalized
    for (int i = 0; i < relation->getMemberCount(); ++i)
    {
      ChangesetRelationMember& member = relation->getMember(i);
      //  First check the member type, these are separated to reduce the comparisons
      //  since these are called frequently
      if (member.isNode())
      {
        //  Special case, node doesn't exist in changeset, it may in database, send it
        if (member.getRef() > 0 && _allNodes.find(member.getRef()) == _allNodes.end())
          return true;
        //  If the node exists in the list and
        //  it hasn't been sent yet and
        //  it can't be sent yet
        //  then we can't send this relation
        else if (_allNodes.find(member.getRef()) != _allNodes.end() &&
            !isSent(_allNodes[member.getRef()].get()) &&
            !canSend(dynamic_cast<ChangesetNode*>(_allNodes[member.getRef()].get())))
        {
          return false;
        }
      }
      else if (member.isWay())
      {
        //  Special case, way doesn't exist in changeset, it may in database, send it
        if (member.getRef() > 0 && _allWays.find(member.getRef()) == _allWays.end())
          return true;
        //  Check if the way exists and can't be sent
        else if (_allWays.find(member.getRef()) != _allWays.end() &&
            !isSent(_allWays[member.getRef()].get()) &&
            !canSend(dynamic_cast<ChangesetWay*>(_allWays[member.getRef()].get())))
        {
          return false;
        }
      }
      else if (member.isRelation())
      {
        //  Special case, relation doesn't exist in changeset, it may in database, send it
        if (member.getRef() > 0 && _allRelations.find(member.getRef()) == _allRelations.end())
          return true;
        //  Special case, relation has a member relation that is itself, send it
        else if (member.getRef() == relation->id())
          return true;
        //  Check if the relation exists and can't be sent
        else if (_allRelations.find(member.getRef()) != _allWays.end() &&
            !isSent(_allRelations[member.getRef()].get()) &&
            !canSend(dynamic_cast<ChangesetRelation*>(_allRelations[member.getRef()].get())))
        {
          return false;
        }
      }
    }
  }
  return true;
}

void XmlChangeset::markBuffered(ChangesetElement* element)
{
  if (element != NULL)
  {
    //  Mark buffering
    element->setStatus(ChangesetElement::Buffering);
    //  Add to the buffer for lookup within this subset
    _sendBuffer.push_back(element);
    ++_sentCount;
  }
}

bool XmlChangeset::calculateChangeset(ChangesetInfoPtr& changeset)
{
  //  Create the changeset info object if there isn't one
  if (!changeset)
    changeset.reset(new ChangesetInfo());
  changeset->clear();
  //  Build up the changeset to be around the MAX changeset push size
  ChangesetType type = ChangesetType::TypeCreate;
  while (type != ChangesetType::TypeMax &&
         changeset->size() < (size_t)_maxPushSize &&
         hasElementsToSend())
  {
    /**
     *  Changesets are created by first adding nodes, then ways, and
     *  finally relations. In testing this order was found to be 4%-7%
     *  faster than any other interpolation of the ordering of nodes,
     *  ways, and relations.  BUT deleting must go in the opposite order
     *  so we'll do relations, ways, and finally nodes.
     */
    //  Start with the relations
    addRelations(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxPushSize)
      continue;
    //  Then the ways
    addWays(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxPushSize)
      continue;
    //  Finally the nodes
    addNodes(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxPushSize)
      continue;
    //  Go to the next type and loop back around
    type = static_cast<ChangesetType>(type + 1);
  }
  //  Move all elements from buffered to sending
  for (vector<ChangesetElement*>::iterator it = _sendBuffer.begin(); it != _sendBuffer.end(); ++it)
    (*it)->setStatus(ChangesetElement::Sent);
  _sendBuffer.clear();
  //  Return true if there is anything in this changeset
  return changeset->size() > 0;
}

bool XmlChangeset::matchesPlaceholderFailure(const QString& hint,
                                             long& member_id, ElementType::Type& member_type,
                                             long& element_id, ElementType::Type& element_type)
{
  //  Placeholder node not found for reference -145213 in way -5687
  //  Placeholder Way not found for reference -12257 in Relation -51
  QRegularExpression reg("Placeholder (node|way|relation) not found for reference (-?[0-9]+) in (node|way|relation) (-?[0-9]+)",
                         QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  if (match.hasMatch())
  {
    //  Get the node/way/relation type and id that caused the failure
    member_type = ElementType::fromString(match.captured(1).toLower());
    bool success = false;
    member_id = match.captured(2).toLong(&success);
    if (!success)
      return success;
    //  Get the node/way/relation type and id that failed
    element_type = ElementType::fromString(match.captured(3));
    element_id = match.captured(4).toLong(&success);
    return success;
  }
  return false;
}

bool XmlChangeset::matchesRelationFailure(const QString& hint, long& element_id, long& member_id, ElementType::Type& member_type)
{
  //  Relation with id  cannot be saved due to Relation with id 1707699
  QRegularExpression reg("Relation with id (-?[0-9]+)? cannot be saved due to (nodes|way|relation) with id (-?[0-9]+)",
                         QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  if (match.hasMatch())
  {
    QString error = match.captured(1);
    if (error != "")
      element_id = error.toLong();
    //  Get the node/way/relation type and id that failed
    member_type = ElementType::fromString(match.captured(2));
    bool success = false;
    member_id = match.captured(3).toLong(&success);
    return success;
  }
  return false;
}

bool XmlChangeset::matchesMultiRelationFailure(const QString& hint,
                                               long& element_id,
                                               std::vector<long>& member_ids,
                                               ElementType::Type& member_type)
{
  //  Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible.
  QRegularExpression reg("Relation (-?[0-9]+) requires the (nodes|ways|relations) with id in ((-?[0-9]+,)+) (.*)", //-?[0-9]+,).*", //+ which either do not exist, or are not visible.",
                         QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  if (match.hasMatch())
  {
    QString error = match.captured(1);
    if (error != "")
      element_id = error.toLong();
    //  Get the node/way/relation type (remove the 's') and id that failed
    member_type = ElementType::fromString(match.captured(2).left(match.captured(2).length() - 1));
    bool success = false;
    QStringList ids = match.captured(3).split(",", QString::SkipEmptyParts);
    for (int i = 0; i < ids.size(); ++i)
    {
      long id = ids[i].toLong(&success);
      if (success)
        member_ids.push_back(id);
    }
    return success;
  }
  return false;
}

bool XmlChangeset::matchesChangesetPreconditionFailure(const QString& hint,
                                                       long& member_id, ElementType::Type& member_type,
                                                       long& element_id, ElementType::Type& element_type)
{
  //  Precondition failed: Node 55 is still used by ways 123
  QRegularExpression reg(
        "Precondition failed: (Node|Way|Relation) (-?[0-9]+) is still used by (node|way|relation)s (-?[0-9]+)",
        QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  if (match.hasMatch())
  {
    bool success = false;
    member_type = ElementType::fromString(match.captured(1).toLower());
    member_id = match.captured(2).toLong(&success);
    if (!success)
      return success;
    element_type = ElementType::fromString(match.captured(3).toLower());
    element_id = match.captured(4).toLong(&success);
    return success;
  }
  return false;
}

bool XmlChangeset::matchesChangesetConflictVersionMismatchFailure(const QString& hint,
                                                                  long& element_id, ElementType::Type& element_type,
                                                                  long& version_old, long& version_new)
{
  //  Changeset conflict: Version mismatch: Provided 2, server had: 1 of Node 4869875616
  QRegularExpression reg(
        "Changeset conflict: Version mismatch: Provided ([0-9]+), server had: ([0-9]+) of (Node|Way|Relation) ([0-9]+)",
        QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  if (match.hasMatch())
  {
    bool success = false;
    version_old = match.captured(1).toLong(&success);
    if (!success)
      return success;
    version_new = match.captured(2).toLong(&success);
    if (!success)
      return success;
    element_type = ElementType::fromString(match.captured(3).toLower());
    element_id = match.captured(4).toLong(&success);
    return success;
  }
  return false;
}

bool XmlChangeset::matchesChangesetClosedFailure(const QString& hint)
{
  //  Changeset conflict: The changeset 49514098 was closed at 2020-01-08 16:28:56 UTC
  QRegularExpression reg(
        ".*The changeset ([0-9]+) was closed.*",
        QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = reg.match(hint);
  return match.hasMatch();
}

bool XmlChangeset::writeErrorFile()
{
  //  Validate the pathname
  if (_errorPathname.isEmpty())
    return false;
  if (!hasFailedElements())
    return false;
  //  Don't write an empty file
  QString errorChangeset = getFailedChangesetString();
  if (errorChangeset.isEmpty())
    return false;
  //  Lock the mutex for writing
  _errorMutex.lock();
  //  Write out the file
  FileUtils::writeFully(_errorPathname, errorChangeset);
  //  Unlock the mutex
  _errorMutex.unlock();
  return true;
}

ChangesetInfoPtr XmlChangeset::splitChangeset(ChangesetInfoPtr changeset, const QString& splitHint)
{
  ChangesetInfoPtr split(new ChangesetInfo());
  //  If there is only one element then splitting only marks that element as failed
  if (changeset->size() == 1)
  {
    updateFailedChangeset(changeset);
    return split;
  }
  //  Try to use the split hint to split out one failing element
  if (splitHint != "")
  {
    long member_id = 0;
    long element_id = 0;
    std::vector<long> member_ids;
    ElementType::Type member_type = ElementType::Unknown;
    ElementType::Type element_type = ElementType::Unknown;
    //  See if the hint is something like: Placeholder node not found for reference -145213 in way -5687
    if (matchesPlaceholderFailure(splitHint, member_id, member_type, element_id, element_type))
    {
      //  Use the type and id to split the changeset
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(element_type, (ChangesetType)current_type, element_id))
        {
          if (element_type == ElementType::Way)
          {
            ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[element_id].get());
            //  Add the way to the split and remove from the changeset
            split->add(element_type, (ChangesetType)current_type, way->id());
            changeset->remove(element_type, (ChangesetType)current_type, way->id());
            return split;
          }
          else if (element_type == ElementType::Relation)
          {
            ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
            //  Add the relation to the split and remove from the changeset
            split->add(element_type, (ChangesetType)current_type, relation->id());
            changeset->remove(element_type, (ChangesetType)current_type, relation->id());
            return split;
          }
        }
      }
    }
    //  See if the hint is something like: Relation with id  cannot be saved due to Relation with id 1707699
    else if (matchesRelationFailure(splitHint, element_id, member_id, member_type))
    {
      if (element_id != 0)
      {
        //  If there is a relation id, move just that relation to the split
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (changeset->contains(ElementType::Relation, (ChangesetType)current_type, element_id))
          {
            ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
            //  Add the relation to the split and remove from the changeset
            split->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
            changeset->remove(ElementType::Relation, (ChangesetType)current_type, relation->id());
            return split;
          }
        }
      }
      else
      {
        //  If no relation id is found, move all relations that contain the id/type combination to the split
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          for (ChangesetElementMap::iterator it = _relations[current_type].begin(); it != _relations[current_type].end(); ++it)
          {
            ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(it->second.get());
            //  Make sure that the changeset contains this relation and this relation contains the problematic element
            if (relation->hasMember(member_type, member_id) &&
                changeset->contains(ElementType::Relation, (ChangesetType)current_type, relation->id()))
            {
              //  Add the relation to the split and remove from the changeset
              split->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
              changeset->remove(ElementType::Relation, (ChangesetType)current_type, relation->id());
              return split;
            }
          }
        }
      }
    }
    //  See if the hint is something like: Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible.
    else if (matchesMultiRelationFailure(splitHint, element_id, member_ids, member_type))
    {
      //  If there is a relation id, move just that relation to the split
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Relation, (ChangesetType)current_type, element_id))
        {
          ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
          //  Add the relation to the split and remove from the changeset
          split->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
          changeset->remove(ElementType::Relation, (ChangesetType)current_type, relation->id());
          return split;
        }
      }
    }
    //  See if the hint is something like: Changeset precondition failed: Precondition failed: Node 5 is still used by ways 67
    else if (matchesChangesetPreconditionFailure(splitHint, member_id, member_type, element_id, element_type))
    {
      //  In this case the node 5 cannot be deleted because way 67 is still using it.  Way 67 must be modified or deleted first
      //  here we figure out how to make that happen
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(member_type, (ChangesetType)current_type, member_id))
        {
          //  Remove the offending change from this changeset
          split->add(member_type, (ChangesetType)current_type, member_id);
          changeset->remove(member_type, (ChangesetType)current_type, member_id);
          //  Try to add the blocking element to the split changeset
          for (int blocking_type = ChangesetType::TypeCreate; blocking_type != ChangesetType::TypeMax; ++blocking_type)
          {
            if (element_type == ElementType::Way)
            {
              //  Add the way to the split so that they can be processed together
              if (_allWays.find(element_id) != _allWays.end())
              {
                ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[element_id].get());
                addWay(split, (ChangesetType)blocking_type, way);
              }
            }
            else if (element_type == ElementType::Relation)
            {
              //  Add the relation to the split so that they can be processed together
              if (_allRelations.find(element_id) != _allRelations.end())
              {
                ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
                addRelation(split, (ChangesetType)blocking_type, relation);
              }
            }
          }
          //  Split out the offending element and the associated blocking element if possible
          return split;
        }
      }
    }
  }
  //  Split the changeset in half (approximately)
  size_t splitSize = changeset->size() / 2;
  //  Start with relations and try to split
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Because we are modifying the changeset, get the first element in the set and iterate
    while (changeset->size(ElementType::Relation, (ChangesetType)current_type) > 0)
    {
      long id = changeset->getFirst(ElementType::Relation, (ChangesetType)current_type);
      ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[id].get());
      //  Move the relation to the new changeset if possible or remove it and make it available again
      moveOrRemoveRelation(changeset, split, (ChangesetType)current_type, relation);
      //  If the split is big enough, end the operation
      if (split->size() >= splitSize)
        return split;
    }
  }
  //  Then move to ways
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Because we are modifying the changeset, get the first element in the set and iterate
    while (changeset->size(ElementType::Way, (ChangesetType)current_type) > 0)
    {
      long id = changeset->getFirst(ElementType::Way, (ChangesetType)current_type);
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[id].get());
      //  Move the way to the new changeset if possible or remove it and make it available again
      moveOrRemoveWay(changeset, split, (ChangesetType)current_type, way);
      //  If the split is big enough, end the operation
      if (split->size() >= splitSize)
        return split;
    }
  }
  //  Finally nodes
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Because we are modifying the changeset, get the first element in the set and iterate
    while (changeset->size(ElementType::Node, (ChangesetType)current_type) > 0)
    {
      long id = changeset->getFirst(ElementType::Node, (ChangesetType)current_type);
      ChangesetNode* node = dynamic_cast<ChangesetNode*>(_allNodes[id].get());
      //  Move the node to the new changeset if possible or remove it and make it available again
      moveOrRemoveNode(changeset, split, (ChangesetType)current_type, node);
      //  If the split is big enough, end the operation
      if (split->size() >= splitSize)
        return split;
    }
  }
  //  Return the second changeset to be added to the queue
  return split;
}

void XmlChangeset::updateFailedChangeset(ChangesetInfoPtr changeset, bool forceFailure)
{
  //  Only set the failed status on single element changesets
  if (changeset->size() != 1 && !forceFailure)
    return;
  //  Don't set the elements to failed yet, until after they are tried
  if (!changeset->getAttemptedResolveChangesetIssues())
    return;
  //  Iterate the three changeset type arrays looking for elements to mark
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Set the relation's status to failed
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Relation, (ChangesetType)current_type);
         it != changeset->end(ElementType::Relation, (ChangesetType)current_type); ++it)
      failRelation(*it);
    //  Set the way's status to failed
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Way, (ChangesetType)current_type);
         it != changeset->end(ElementType::Way, (ChangesetType)current_type); ++it)
      failWay(*it);
    //  Set the node's status to failed
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Node, (ChangesetType)current_type);
         it != changeset->end(ElementType::Node, (ChangesetType)current_type); ++it)
      failNode(*it);
  }
  //  Re-write the failed changeset to disk each time a new changeset is added
  writeErrorFile();
}

QString XmlChangeset::getChangesetString(ChangesetInfoPtr changeset, long changeset_id)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  //  OSM Changeset tag
  ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
     << "<osmChange version=\"0.6\" generator=\""<< HOOT_PACKAGE_NAME << "\">\n";
  //  Creates should go first
  ts << getChangeset(changeset, changeset_id, ChangesetType::TypeCreate);
  //  Then modifies
  ts << getChangeset(changeset, changeset_id, ChangesetType::TypeModify);
  //  Finally deletes
  ts << getChangeset(changeset, changeset_id, ChangesetType::TypeDelete);
  //  Close OSM Changeset
  ts << "</osmChange>\n";
  //  Return the OSM Changeset
  return ts.readAll();
}

QString XmlChangeset::getFailedChangesetString()
{
  //  Nothing failed, nothing returned
  if (_failedCount == 0)
    return "";
  //  Create a changeset info object to hold all of the failed elements
  ChangesetInfoPtr changeset(new ChangesetInfo());
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Iterate all of the nodes in the changeset looking for failed elements
    for (ChangesetElementMap::iterator it = _nodes[current_type].begin(); it != _nodes[current_type].end(); ++it)
    {
      ChangesetNode* node = dynamic_cast<ChangesetNode*>(it->second.get());
      //  Add only the failed nodes
      if (node->getStatus() == ChangesetElement::ElementStatus::Failed)
        changeset->add(ElementType::Node, (ChangesetType)current_type, node->id());
    }
    //  Iterate all of the ways in the changeset looking for failed elements
    for (ChangesetElementMap::iterator it = _ways[current_type].begin(); it != _ways[current_type].end(); ++it)
    {
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(it->second.get());
      //  Add only the failed ways
      if (way->getStatus() == ChangesetElement::ElementStatus::Failed)
        changeset->add(ElementType::Way, (ChangesetType)current_type, way->id());
    }
    //  Iterate all of the relations in the changeset looking for failed elements
    for (ChangesetElementMap::iterator it = _relations[current_type].begin(); it != _relations[current_type].end(); ++it)
    {
      ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(it->second.get());
      //  Add only the failed relations
      if (relation->getStatus() == ChangesetElement::ElementStatus::Failed)
        changeset->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
    }
  }
  //  Return the changeset string using changeset ID 0 to indicate it was a failure
  return getChangesetString(changeset, 0);
}

QString XmlChangeset::getChangeset(ChangesetInfoPtr changeset, long changeset_id, ChangesetType type)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  QString category;
  //  Get the category tag from the changeset type
  if (type == ChangesetType::TypeCreate)
    category = "create";
  else if (type == ChangesetType::TypeModify)
    category = "modify";
  else if (type == ChangesetType::TypeDelete)
    category = "delete";
  if (changeset->size(ElementType::Node, type) > 0 || changeset->size(ElementType::Way, type) > 0 || changeset->size(ElementType::Relation, type) > 0)
  {
    ts << "\t<" << category << ">\n";
    if (type != ChangesetType::TypeDelete)
    {
      //  Nodes go first in each category
      writeNodes(changeset, ts, type, changeset_id);
      //  Followed by ways
      writeWays(changeset, ts, type, changeset_id);
      //  Relations bring up the rear
      writeRelations(changeset, ts, type, changeset_id);
    }
    else
    {
      //  Relations first for deletes
      writeRelations(changeset, ts, type, changeset_id);
      //  Followed by ways
      writeWays(changeset, ts, type, changeset_id);
      //  Nodes go last
      writeNodes(changeset, ts, type, changeset_id);
    }
    ts << "\t</" << category << ">\n";
  }
  //  Return the OSM Changeset category
  return ts.readAll();
}

void XmlChangeset::updateElement(ChangesetTypeMap& map, long old_id, long new_id, long version)
{
  //  Zero IDs are not valid
  if (old_id == 0)
    return;
  ChangesetType changeset_type;
  if (new_id == 0)            //  No new ID means the element was deleted
    changeset_type = ChangesetType::TypeDelete;
  else if (new_id != old_id)  //  When the old and new IDs are different this was a create, update the node id
    changeset_type = ChangesetType::TypeCreate;
  else                        //  Otherwise it was a modify and nothing needs updating
    changeset_type = ChangesetType::TypeModify;
  //  Find the element by the old ID
  ChangesetElementMap& type = map[changeset_type];
  ChangesetElementMap::iterator position = type.find(old_id);
  if (position != type.end())
  {
    ChangesetElementPtr element = type[old_id];
    //  Finalize the element
    element->setStatus(ChangesetElement::Finalized);
    //  Update the ID in the map with a new ID for elements that were created
    if (changeset_type == ChangesetType::TypeCreate)
      _idMap.updateId(element->getType(), old_id, new_id);
    //  Update the version if necessary
    if (version >= 0)
      element->setVersion(version);
    _processedCount++;
  }
  else
  {
    LOG_ERROR("Element cannot be updated. ID " << old_id);
  }
}

bool XmlChangeset::fixElement(ChangesetTypeMap& map, long id, long version, QMap<QString, QString> tags)
{
  bool success = false;
  //  Negative IDs should never be fixed
  if (id < 1)
    return success;
  //  Find the affected element
  for (int type = 0; type < ChangesetType::TypeMax; ++type)
  {
    ChangesetElementMap::iterator position = map[type].find(id);
    if (position != map[type].end())
    {
      //  Found the element, now update it
      ChangesetElementPtr element = map[type][id];
      //  Only update the version if it is out of sync
      if (element->getVersion() != version)
      {
        element->setVersion(version);
        //  Change was made
        success = true;
      }
      //  Update the tags if they are missing
      for (int i = 0; i < element->getTagCount(); ++i)
      {
        QString key = element->getTagKey(i);
        if (tags.contains(key))
          tags.remove(key);
      }
      //  Add in any tags that are missing
      for (QMap<QString, QString>::iterator it = tags.begin(); it != tags.end(); ++it)
      {
        QXmlStreamAttributes attributes;
        attributes.append("", "k", it.key());
        attributes.append("", "v", it.value());
        element->addTag(XmlObject("tag", attributes));
      }
    }
  }
  return success;
}

long XmlChangeset::getNextNodeId()
{
  //  Get a new node ID from the generator
  long id = _idGen.createNodeId();
  //  Make sure that ID isn't already being used in the changeset
  while (_allNodes.find(id) != _allNodes.end())
    id = _idGen.createNodeId();
  return id;
}

long XmlChangeset::getNextWayId()
{
  //  Get a new way ID from the generator
  long id = _idGen.createWayId();
  //  Make sure that ID isn't already being used in the changeset
  while (_allWays.find(id) != _allWays.end())
    id = _idGen.createWayId();
  return id;
}

long XmlChangeset::getNextRelationId()
{
  //  Get a new relation ID from the generator
  long id = _idGen.createRelationId();
  //  Make sure that ID isn't already being used in the changeset
  while (_allRelations.find(id) != _allRelations.end())
    id = _idGen.createRelationId();
  return id;
}

void XmlChangeset::replaceNodeId(long old_id, long new_id)
{
  //  Update the ID to ID map with the new ID so it is used everywhere that the ID is referenced
  _idMap.updateId(ElementType::Node, old_id, new_id);
}

void XmlChangeset::replaceWayId(long old_id, long new_id)
{
  //  Update the ID to ID map with the new ID so it is used everywhere that the ID is referenced
  _idMap.updateId(ElementType::Way, old_id, new_id);
}

void XmlChangeset::replaceRelationId(long old_id, long new_id)
{
  //  Update the ID to ID map with the new ID so it is used everywhere that the ID is referenced
  _idMap.updateId(ElementType::Relation, old_id, new_id);
}

void XmlChangeset::failNode(long id, bool beforeSend)
{
  //  Set the node's status to failed
  _allNodes[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend)
    ++_sentCount;
}

void XmlChangeset::failWay(long id, bool beforeSend)
{
  //  Set the way's status to failed
  _allWays[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend)
    ++_sentCount;
}

void XmlChangeset::failRelation(long id, bool beforeSend)
{
  //  Set the relation's status to failed
  _allRelations[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend)
    ++_sentCount;
  LOG_TRACE("Failed relation (" << id << ")");
}

void XmlChangeset::writeElements(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id,
                                 ElementType::Type elementType, const ChangesetElementMap& elements)
{
  vector<long> outputElements;
  //  Iterate only those elements that are in the small changeset to be sent and not all of the elements in entire changeset
  for (ChangesetInfo::iterator it = changeset->begin(elementType, type); it != changeset->end(elementType, type); ++it)
  {
    if (elements.find(*it) != elements.end() && _idMap.containsId(elementType, *it))
      outputElements.push_back(_idMap.getId(elementType, *it));
  }
  //  Sort the elements by ID
  std::sort(outputElements.begin(), outputElements.end(), id_sort_order);
  //  Iterate the elements again, writing them to the output stream
  for (vector<long>::iterator it = outputElements.begin(); it != outputElements.end(); ++it)
    ts << elements.at(_idMap.getId(elementType, *it))->toString(changeset_id);
}

void XmlChangeset::writeNodes(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id)
{
  writeElements(changeset, ts, type, changeset_id, ElementType::Node, _nodes[type]);
}

void XmlChangeset::writeWays(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id)
{
  writeElements(changeset, ts, type, changeset_id, ElementType::Way, _ways[type]);
}

void XmlChangeset::writeRelations(const ChangesetInfoPtr& changeset, QTextStream& ts, ChangesetType type, long changeset_id)
{
  writeElements(changeset, ts, type, changeset_id, ElementType::Relation, _relations[type]);
}

bool XmlChangeset::calculateRemainingChangeset(ChangesetInfoPtr &changeset)
{
  //  Create the changeset info object if there isn't one
  if (!changeset)
    changeset.reset(new ChangesetInfo());
  changeset->clear();
  //  This is the last changeset of the bunch because of the error state
  changeset->setLast();
  //  Loop through all remaining elements
  for (ChangesetType type = ChangesetType::TypeCreate; type != ChangesetType::TypeMax; type = static_cast<ChangesetType>(type + 1))
  {
    //  Iterate all of the relations of "type" in the changeset
    for (ChangesetElementMap::iterator it = _relations[type].begin(); it != _relations[type].end(); ++it)
    {
      ChangesetElement* relation = it->second.get();
      if (relation->getStatus() == ChangesetElement::Available)
      {
        //  Add the relation to the changeset
        changeset->add(ElementType::Relation, type, relation->id());
        //  Mark the relation as buffered
        markBuffered(relation);
        //  Mark the relation as sent
        relation->setStatus(ChangesetElement::Sent);
      }
    }
    //  Iterate all of the ways of "type" in the changeset
    for (ChangesetElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      ChangesetElement* way = it->second.get();
      if (way->getStatus() == ChangesetElement::Available)
      {
        //  Add the way to the changeset
        changeset->add(ElementType::Way, type, way->id());
        //  Mark the way as buffered
        markBuffered(way);
        //  Mark the way as sent
        way->setStatus(ChangesetElement::Sent);
      }
    }
    //  Iterate all of the nodes of "type" in the changeset
    for (ChangesetElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
    {
      ChangesetElement* node = it->second.get();
      if (node->getStatus() == ChangesetElement::Available)
      {
        //  Add the node to the changeset
        changeset->add(ElementType::Node, type, node->id());
        //  Mark the node as buffered
        markBuffered(node);
        //  Mark the node as sent
        node->setStatus(ChangesetElement::Sent);
      }
    }
  }
  //  Clear the send buffer
  _sendBuffer.clear();
  bool empty_changeset = changeset->size() == 0;
  //  Output the remaining changeset to a file to inform the user where the issues lie
  if (!empty_changeset && !_errorPathname.isEmpty())
  {
    //  Replace error with remaining in the error pathname
    QString pathname = _errorPathname;
    pathname.replace("error", "remaining");
    //  Write the file with changeset ID of zero
    FileUtils::writeFully(pathname, this->getChangesetString(changeset, 0));
  }
  //  Return true if there is anything in the changeset
  return !empty_changeset;
}

ChangesetInfo::ChangesetInfo()
  : _attemptedResolveChangesetIssues(false),
    _numRetries(0),
    _last(false)
{
}

void ChangesetInfo::add(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
{
  _changeset[element_type][changeset_type].insert(id);
  //  Changes in the changeset cause the retries to start over
  _numRetries = 0;
}

void ChangesetInfo::remove(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
{
  container& selectedSet = _changeset[element_type][changeset_type];
  if (selectedSet.find(id) != selectedSet.end())
    selectedSet.erase(id);
  //  Changes in the changeset cause the retries to start over
  _numRetries = 0;
}

long ChangesetInfo::getFirst(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
{
  return *(_changeset[element_type][changeset_type].begin());
}

void ChangesetInfo::clear()
{
  for (int i = 0; i < (int)ElementType::Unknown; ++i)
  {
    for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
      _changeset[i][j].clear();
  }
  _numRetries = 0;
}

bool ChangesetInfo::contains(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type, long id)
{
  return _changeset[element_type][changeset_type].find(id) != end(element_type, changeset_type);
}

ChangesetInfo::iterator ChangesetInfo::begin(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
{
  return _changeset[element_type][changeset_type].begin();
}

ChangesetInfo::iterator ChangesetInfo::end(ElementType::Type element_type, XmlChangeset::ChangesetType changeset_type)
{
  return _changeset[element_type][changeset_type].end();
}

size_t ChangesetInfo::size(ElementType::Type elementType, XmlChangeset::ChangesetType changesetType)
{
  return _changeset[(int)elementType][(int)changesetType].size();
}

size_t ChangesetInfo::size()
{
  size_t s = 0;
  //  Iterate element types
  for (int i = 0; i < (int)ElementType::Unknown; ++i)
  {
    //  Sum up all counts for each changeset type
    for (int j = 0; j < (int)XmlChangeset::TypeMax; ++j)
      s += _changeset[i][j].size();
  }
  return s;
}

bool ChangesetInfo::getAttemptedResolveChangesetIssues()
{
  return _attemptedResolveChangesetIssues;
}

void ChangesetInfo::setAttemptedResolveChangesetIssues(bool attempted)
{
  _attemptedResolveChangesetIssues = attempted;
}

bool ChangesetInfo::canRetry()
{
  return _numRetries < MAX_RETRIES;
}

void ChangesetInfo::retry()
{
  //  Increment the retry count
  _numRetries++;
  //  Once the retry count reaches MAX_RETRIES set the attempted resolved flag
  _attemptedResolveChangesetIssues = true;
}

}
