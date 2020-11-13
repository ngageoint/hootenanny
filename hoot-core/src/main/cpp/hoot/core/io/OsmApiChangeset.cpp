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
#include <hoot/core/util/StringUtils.h>

//  Standard
#include <algorithm>
#include <stack>
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
    _maxChangesetSize(ConfigOptions().getChangesetMaxSize()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0),
    _cleanupCount(0)
{
}

XmlChangeset::XmlChangeset(const QString& changeset)
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _maxChangesetSize(ConfigOptions().getChangesetMaxSize()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0),
    _cleanupCount(0)
{
  loadChangeset(changeset);
}

XmlChangeset::XmlChangeset(const QList<QString>& changesets)
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxPushSize(ConfigOptions().getChangesetApidbSizeMax()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0),
    _cleanupCount(0)
{
  for (QList<QString>::const_iterator it = changesets.begin(); it != changesets.end(); ++it)
    loadChangeset(*it);
}

void XmlChangeset::loadChangeset(const QString& changeset)
{
  QFileInfo fi(changeset);
  if (fi.exists())
  {
    if (fi.isDir())
      loadChangesetDirectory(changeset);
    else
      loadChangesetFile(changeset);
  }
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

void XmlChangeset::loadChangesetXml(const QString& changesetXml)
{
  //  Load the XML directly into the reader
  QXmlStreamReader reader(changesetXml);
  //  Load the changeset formatted XML
  loadChangeset(reader);
}

void XmlChangeset::loadChangesetDirectory(const QString& changesetDirectory)
{
  //  Iterate all of the files in the directory, loading them one by one
  QDir dir(changesetDirectory);
  dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
  dir.setSorting(QDir::Name | QDir::IgnoreCase);
  QStringList filters;
  filters << "*.osc";
  dir.setNameFilters(filters);

  QFileInfoList files = dir.entryInfoList();
  for (int i = 0; i < files.size(); ++i)
  {
    QFileInfo fileInfo = files.at(i);
    QString filepath = fileInfo.absoluteFilePath();
    //  Check if this file is in the request/response debug output format (including the error file)
    if (filepath.contains("-Request--") && filepath.endsWith("000.osc", Qt::CaseInsensitive))
    {
      //  Only load the requests that were successful, i.e. response 200
      QString response = filepath;
      response.replace("-Request--", "-Response-").replace("000.osc", "200.osc");
      if (QFile::exists(response))
        loadChangesetFile(filepath);
    }
    else if (filepath.contains("-Response-") && filepath.endsWith("200.osc", Qt::CaseInsensitive))
    {
      //  Read in the update for new IDs
      updateChangeset(FileUtils::readFully(filepath));
    }
    else if (filepath.endsWith("-error.osc", Qt::CaseInsensitive))
    {
      //  The changeset error file
      loadChangesetFile(filepath);
    }
    else
    {
      //  Output the filename to the log
      LOG_DEBUG("Skipping file in folder: " << filepath);
    }
  }
}

void XmlChangeset::loadChangeset(QXmlStreamReader& reader)
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
      //  Nodes/Ways/Relations cannot be both modified and deleted in the same changeset
      //  Fix that here
      if (name == "node")
        insertElement(element, changeset_type, _nodes, _allNodes);
      else if (name == "way")
        insertElement(element, changeset_type, _ways, _allWays);
      else if (name == "relation")
        insertElement(element, changeset_type, _relations, _allRelations);
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
  //  Way and relation adds cannot have members that are negative that don't exist in this changeset
  for (ChangesetElementMap::iterator it = _relations[TypeCreate].begin(); it != _relations[TypeCreate].end(); ++it)
  {
    ChangesetRelationPtr relation(std::dynamic_pointer_cast<ChangesetRelation>(it->second));
    std::stack<int> remove_members;
    for (int i = 0; i < relation->getMemberCount(); ++i)
    {
      ChangesetRelationMember& m = relation->getMember(i);
      if (m.getRef() < 0)
      {
        //  Mark for removal all nodes/ways/relations with negative IDs that don't exist
        if (m.isNode() && _allNodes.find(m.getRef()) == _allNodes.end())
          remove_members.push(i);
        else if (m.isWay() && _allWays.find(m.getRef()) == _allWays.end())
          remove_members.push(i);
        else if (m.isRelation() && _allRelations.find(m.getRef()) == _allRelations.end())
          remove_members.push(i);
      }
    }
    //  Remove any member with a negative ID (add) that doesn't exist in this changeset
    while (!remove_members.empty())
    {
      relation->removeMember(remove_members.top());
      remove_members.pop();
    }
  }
  for (ChangesetElementMap::iterator it = _ways[TypeCreate].begin(); it != _ways[TypeCreate].end(); ++it)
  {
    ChangesetWayPtr way(std::dynamic_pointer_cast<ChangesetWay>(it->second));
    std::stack<int> remove_nodes;
    for (int i = 0; i < way->getNodeCount(); ++i)
    {
      //  Mark for removal all nodes with negative IDs that don't exist
      long nodeId = way->getNode(i);
      if (nodeId < 0 && _allNodes.find(nodeId) == _allNodes.end())
        remove_nodes.push(nodeId);
    }
    //  Remove any node with a negative ID (add) that doesn't exist in this changeset
    while (!remove_nodes.empty())
    {
      way->removeNodes(remove_nodes.top(), 1);
      remove_nodes.pop();
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

QString XmlChangeset::getString(ChangesetType type)
{
  switch (type)
  {
  case ChangesetType::TypeCreate:   return "<create>";
  case ChangesetType::TypeModify:   return "<modify>";
  case ChangesetType::TypeDelete:   return "<delete>";
  default:                          return "";
  }
}

void XmlChangeset::updateChangeset(const QString& changes)
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

bool XmlChangeset::addNodes(const ChangesetInfoPtr& changeset, ChangesetType type)
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

bool XmlChangeset::addNode(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetNode* node)
{
  //  Only add the nodes that are "sendable"
  if (canSend(node))
  {
    //  No need to getObjectCount() like addWay and addRelation because it is either 0 or 1
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
        set<long> relationIds = _nodeIdsToRelations[node->id()];
        sendable = addParentRelations(changeset, relationIds);
      }
      if (_nodeIdsToWays.find(node->id()) != _nodeIdsToWays.end())
      {
        set<long> wayIds = _nodeIdsToWays[node->id()];
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

void XmlChangeset::moveOrRemoveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node)
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

bool XmlChangeset::moveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node, bool /*failing*/)
{
  //  Add the node to the destination and remove from the source
  destination->add(ElementType::Node, type, node->id());
  source->remove(ElementType::Node, type, node->id());
  return true;
}

bool XmlChangeset::canMoveNode(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetNode* node)
{
  //  Deleting a node can only happen when all ways and relations referencing this node are in this changeset
  if (type == ChangesetType::TypeDelete)
  {
    //  Iterate all relations for this node
    if (_nodeIdsToRelations.find(node->id()) != _nodeIdsToRelations.end())
    {
      set<long> relationIds = _nodeIdsToRelations[node->id()];
      for (set<long>::const_iterator it = relationIds.begin(); it != relationIds.end(); ++it)
      {
        if (!destination->contains(ElementType::Relation, *it))
          return false;
      }
    }
    //  Iterate all ways for this node
    if (_nodeIdsToWays.find(node->id()) != _nodeIdsToWays.end())
    {
      set<long> wayIds = _nodeIdsToWays[node->id()];
      for (set<long>::const_iterator it = wayIds.begin(); it != wayIds.end(); ++it)
      {
        if (!destination->contains(ElementType::Way, *it))
          return false;
      }
    }
    //  All parent ways and relations are in the destination, so this node can move
  }
  //  Creating or Modifying a node can always move
  return source->size() != 1;
}

bool XmlChangeset::addWays(const ChangesetInfoPtr& changeset, ChangesetType type)
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

bool XmlChangeset::addWay(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetWay* way)
{
  //  Check if the way is able to be sent
  if (canSend(way))
  {
    //  The number of elements in this way (fully "hydrated") cannot exceed the max size in a changeset
    ElementCountSet elements(ElementType::Max);
    size_t count = getObjectCount(way, elements, false);
    if (count + changeset->size() > (size_t)_maxChangesetSize)
      return false;
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
        set<long> relationIds = _wayIdsToRelations[way->id()];
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

bool XmlChangeset::addParentWays(const ChangesetInfoPtr& changeset, const set<long>& way_ids)
{
  bool sendable = true;
  for (set<long>::iterator it = way_ids.begin(); it != way_ids.end(); ++it)
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

void XmlChangeset::moveOrRemoveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way)
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

bool XmlChangeset::moveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way, bool failing)
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
        {
          bool allowMove = true;
          //  Don't move this node if it isn't free and clear when failing (and also an add)
          if (failing && _nodes[TypeCreate].find(id) != _nodes[TypeCreate].end())
          {
            set<long> wayIds = _nodeIdsToWays[id];
            //  Nodes that intersect multiple ways need further checking
            if (wayIds.size() > 1)
            {
              //  Iterate all of the parent ways looking for a valid (non-error) way
              for (set<long>::iterator it = wayIds.begin(); it != wayIds.end(); ++it)
              {
                //  Ignore this way (hasn't been errored out yet) and look for a valid way
                if (*it != way->id() && way->getStatus() != ChangesetElement::ElementStatus::Failed)
                  allowMove = false;
              }
            }
          }
          //  Only move the node if it is allowed
          if (allowMove)
            moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetNode*>(_allNodes[id].get()));
        }
      }
    }
  }
  //  Add the way to the destination and remove from the source
  destination->add(ElementType::Way, type, way->id());
  source->remove(ElementType::Way, type, way->id());
  return true;
}

bool XmlChangeset::canMoveWay(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetWay* way)
{
  //  Deleting a way will only work if the parent relation is there too
  if (type == ChangesetType::TypeDelete)
  {
    if (_wayIdsToRelations.find(way->id()) != _wayIdsToRelations.end())
    {
      set<long> relations = _wayIdsToRelations[way->id()];
      for (set<long>::iterator it = relations.begin(); it != relations.end(); ++it)
      {
        if (!destination->contains(ElementType::Way, *it))
          return false;
      }
    }
    return source->size() != 1;
  }
  //  Get the count of nodes and way that make up this "change"
  ElementCountSet elements(ElementType::Max);
  size_t count = getObjectCount(source, way, elements);
  //  Compare that count to the size left in the source changeset
  return source->size() != count;
}

bool XmlChangeset::addRelations(const ChangesetInfoPtr& changeset, ChangesetType type)
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

bool XmlChangeset::addRelation(const ChangesetInfoPtr& changeset, ChangesetType type, ChangesetRelation* relation)
{
  //  Check if the relation is able to be sent
  if (canSend(relation))
  {
    //  The number of elements in this relation (fully "hydrated") cannot exceed the max size in a changeset
    ElementCountSet elements(ElementType::Max);
    size_t count = getObjectCount(relation, elements, false);
    if (count + changeset->size() > (size_t)_maxChangesetSize)
      return false;
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
              if (relation_member && relation->id() != relation_member->id())
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
        set<long> relationIds = _relationIdsToRelations[relation->id()];
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

bool XmlChangeset::addParentRelations(const ChangesetInfoPtr& changeset, const set<long>& relation_ids)
{
  bool sendable = true;
  for (set<long>::iterator it = relation_ids.begin(); it != relation_ids.end(); ++it)
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

void XmlChangeset::moveOrRemoveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation)
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

bool XmlChangeset::moveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation, bool failing)
{
  //  Don't worry about the contents of a delete operation
  if (type != ChangesetType::TypeDelete)
  {
    //  Iterate all of the members that exist in the changeset and move them
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
            moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetNode*>(_allNodes[id].get()), failing);
        }
      }
      else if (member.isWay())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Way, (ChangesetType)current_type, id))
            moveWay(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetWay*>(_allWays[id].get()), failing);
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
              moveRelation(source, destination, (ChangesetType)current_type, dynamic_cast<ChangesetRelation*>(_allRelations[id].get()), failing);
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

bool XmlChangeset::canMoveRelation(const ChangesetInfoPtr& source, const ChangesetInfoPtr& destination, ChangesetType type, ChangesetRelation* relation)
{
  //  Deleting a relation requires any parent relation
  if (type == ChangesetType::TypeDelete)
  {
    if (_relationIdsToRelations.find(relation->id()) != _relationIdsToRelations.end())
    {
      set<long> relations = _relationIdsToRelations[relation->id()];
      for (set<long>::iterator it = relations.begin(); it != relations.end(); ++it)
      {
        if (!destination->contains(ElementType::Relation, *it))
          return false;
      }
    }
    return source->size() == 1;
  }
  //  Get the count of nodes, ways, and relations that make up this "change"
  ElementCountSet elements(ElementType::Max);
  size_t count = getObjectCount(source, relation, elements);
  //  Compare that count to the size left in the source changeset
  return source->size() != count;
}

size_t XmlChangeset::getObjectCount(ChangesetNode* node, ElementCountSet& elements, bool countSent)
{
  ChangesetInfoPtr empty;
  return getObjectCount(empty, node, elements, countSent);
}

size_t XmlChangeset::getObjectCount(ChangesetWay* way, ElementCountSet& elements, bool countSent)
{
  ChangesetInfoPtr empty;
  return getObjectCount(empty, way, elements, countSent);
}

size_t XmlChangeset::getObjectCount(ChangesetRelation* relation, ElementCountSet& elements, bool countSent)
{
  ChangesetInfoPtr empty;
  return getObjectCount(empty, relation, elements, countSent);
}

size_t XmlChangeset::getObjectCount(const ChangesetInfoPtr& /*changeset*/, ChangesetNode* node, ElementCountSet& elements, bool countSent)
{
  //  Cannot count NULL nodes
  if (node == NULL)
    return 0;
  //  Do not recount nodes already counted
  if (elements[ElementType::Node].find(node->id()) != elements[ElementType::Node].end())
    return 0;
  //  Do not count non-available nodes
  if (!countSent && node->getStatus() != ChangesetElement::Available)
    return 0;
  //  Record the node ID in the node elements set
  elements[ElementType::Node].insert(node->id());
  //  Nodes count as one object
  return 1;
}

size_t XmlChangeset::getObjectCount(const ChangesetInfoPtr& changeset, ChangesetWay* way, ElementCountSet& elements, bool countSent)
{
  if (way == NULL)
    return 0;
  //  Get the count of nodes and way that make up this "change"
  size_t count = 0;
  //  Do not count non-available ways
  if (!countSent && way->getStatus() != ChangesetElement::Available)
    return 0;
  //  Increment the count if it isn't already found in the element set
  if (elements[ElementType::Way].find(way->id()) == elements[ElementType::Way].end())
  {
    elements[ElementType::Way].insert(way->id());
    ++count;
  }
  //  Iterate all of the nodes
  for (int i = 0; i < way->getNodeCount(); ++i)
  {
    long id = way->getNode(i);
    //  Look for the node in the list of nodes
    for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
    {
      //  Do not recount this node
      if (!changeset || changeset->contains(ElementType::Node, (ChangesetType)current_type, id))
      {
        ChangesetNode* node = NULL;
        if (_allNodes.find(id) != _allNodes.end())
          node = dynamic_cast<ChangesetNode*>(_allNodes[id].get());
        count += getObjectCount(changeset, node, elements);
      }
    }
  }
  return count;
}

size_t XmlChangeset::getObjectCount(const ChangesetInfoPtr& changeset, ChangesetRelation* relation, ElementCountSet& elements, bool countSent)
{
  if (relation == NULL)
    return 0;
  //  Get the count of nodes, ways, and relations that make up this "change"
  size_t count = 0;
  //  Do not count non-available relations
  if (!countSent && relation->getStatus() != ChangesetElement::Available)
    return 0;
  //  Increment the count if the relation isn't already found in the element set
  if (elements[ElementType::Relation].find(relation->id()) == elements[ElementType::Relation].end())
  {
    elements[ElementType::Relation].insert(relation->id());
    ++count;
  }
  //  Iterate all of the relation elements
  for (int i = 0; i < relation->getMemberCount(); ++i)
  {
    ChangesetRelationMember& member = relation->getMember(i);
    long id = member.getRef();
    //  Each member type iterates the changeset types looking for this element, then counts it
    if (member.isNode())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (!changeset || changeset->contains(ElementType::Node, (ChangesetType)current_type, id))
        {
          ChangesetNode* node = NULL;
          if (_allNodes.find(id) != _allNodes.end())
            node = dynamic_cast<ChangesetNode*>(_allNodes[id].get());
          count += getObjectCount(changeset, node, elements, countSent);
        }
      }
    }
    else if (member.isWay())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (!changeset || changeset->contains(ElementType::Way, (ChangesetType)current_type, id))
        {
          ChangesetWay* way = NULL;
          if (_allWays.find(id) != _allWays.end())
            way = dynamic_cast<ChangesetWay*>(_allWays[id].get());
          count += getObjectCount(changeset, way, elements, countSent);
        }
      }
    }
    else if (member.isRelation())
    {
      //  Don't recount circular referenced relations
      if (id != relation->id())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (!changeset || changeset->contains(ElementType::Relation, (ChangesetType)current_type, id))
          {
            ChangesetRelation* relation = NULL;
            if (_allRelations.find(id) != _allRelations.end())
              relation = dynamic_cast<ChangesetRelation*>(_allRelations[id].get());
            count += getObjectCount(changeset, relation, elements, countSent);
          }
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

ChangesetInfoPtr XmlChangeset::splitChangeset(const ChangesetInfoPtr& changeset, const QString& splitHint)
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
    //  See if the hint is something like:
    //   Placeholder node not found for reference -145213 in way -5687
    if (_failureCheck.matchesPlaceholderFailure(splitHint, member_id, member_type, element_id, element_type))
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
            if (current_type == ChangesetType::TypeCreate)
            {
              //  Move all nodes to be created with this way to the split
              moveWay(changeset, split, (ChangesetType)current_type, way, true);
            }
            else
            {
              split->add(element_type, (ChangesetType)current_type, way->id());
              changeset->remove(element_type, (ChangesetType)current_type, way->id());
            }
            split->setError();
            return split;
          }
          else if (element_type == ElementType::Relation)
          {
            ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
            //  Add the relation to the split and remove from the changeset
            split->add(element_type, (ChangesetType)current_type, relation->id());
            changeset->remove(element_type, (ChangesetType)current_type, relation->id());
            //  If one element doesn't exist, fail the relation
            split->setError();
            return split;
          }
        }
      }
    }
    //  See if the hint is something like:
    //   Relation with id  cannot be saved due to Relation with id 1707699
    else if (_failureCheck.matchesRelationFailure(splitHint, element_id, member_id, member_type))
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
    //  See if the hint is something like:
    //   Relation with id -2 requires the relations with id in 1707148,1707249, which either do not exist, or are not visible.
    else if (_failureCheck.matchesMultiElementFailure(splitHint, element_id, element_type, member_ids, member_type))
    {
      //  If there is a relation id, move just that relation to the split
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(element_type, (ChangesetType)current_type, element_id))
        {
          if (element_type == ElementType::Way)
          {
            ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[element_id].get());
            //  Add the way to the split and remove from the changeset
            if (current_type == ChangesetType::TypeCreate)
            {
              moveWay(changeset, split, (ChangesetType)current_type, way, true);
              split->setError();
            }
            else
            {
              split->add(element_type, (ChangesetType)current_type, way->id());
              changeset->remove(element_type, (ChangesetType)current_type, way->id());
            }
            return split;
          }
          else if (element_type == ElementType::Relation)
          {
            ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[element_id].get());
            //  Add the relation to the split and remove from the changeset
            split->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
            changeset->remove(ElementType::Relation, (ChangesetType)current_type, relation->id());
            split->setError();
            return split;
          }
        }
      }
    }
    //  See if the hint is something like:
    //   Changeset precondition failed: Precondition failed: Node 5 is still used by ways 67,91
    else if (_failureCheck.matchesChangesetDeletePreconditionFailure(splitHint, element_id, element_type, member_ids, member_type))
    {
      //  In this case the node 5 cannot be deleted because ways 67 and 91 are still using it.  Ways 67 and 91
      //  must be modified or deleted first here we figure out how to make that happen
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(element_type, (ChangesetType)current_type, element_id))
        {
          //  Remove the offending change from this changeset
          split->add(element_type, (ChangesetType)current_type, element_id);
          changeset->remove(element_type, (ChangesetType)current_type, element_id);
          //  Try to add the blocking element to the split changeset
          for (int blocking_type = ChangesetType::TypeCreate; blocking_type != ChangesetType::TypeMax; ++blocking_type)
          {
            if (member_type == ElementType::Way)
            {
              for (size_t i = 0; i < member_ids.size(); ++i)
              {
                //  Add the way to the split so that they can be processed together
                if (_allWays.find(member_ids[i]) != _allWays.end())
                {
                  ChangesetWay* way = dynamic_cast<ChangesetWay*>(_allWays[member_ids[i]].get());
                  moveWay(changeset, split, (ChangesetType)blocking_type, way);
                }
              }
            }
            else if (member_type == ElementType::Relation)
            {
              for (size_t i = 0; i < member_ids.size(); ++i)
              {
                //  Add the relation to the split so that they can be processed together
                if (_allRelations.find(member_ids[i]) != _allRelations.end())
                {
                  ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(_allRelations[member_ids[i]].get());
                  moveRelation(changeset, split, (ChangesetType)blocking_type, relation);
                }
              }
            }
          }
          //  Don't send the element again if the blocking elements aren't in the split
          if (split->size() != member_ids.size() + 1) //  +1 includes element reported
          {
            if (split->size() != 1)
            {
              //  When some of the containing elements but not all are present,
              //  move the element to a new changeset info object to fail it
              ChangesetInfoPtr failing(new ChangesetInfo());
              failing->add(element_type, (ChangesetType)current_type, element_id);
              split->remove(element_type, (ChangesetType)current_type, element_id);
              //  Fail only the element, but not the other elements in split
              failing->setError();
              failChangeset(failing);
            }
            else
              split->setError();
          }
          //  Split out the offending element and the associated blocking element if possible
          return split;
        }
      }
    }
    //  See if the hint is something like:
    //   The node with the id 12345 has already been deleted
    else if (_failureCheck.matchesElementGoneDeletedFailure(splitHint, element_id, element_type))
    {
      //  This should only occur for deletes
      if (changeset->contains(element_type, ChangesetType::TypeDelete, element_id))
      {
        //  Get the element from the correct element map
        ChangesetElement* element = NULL;
        if (element_type == ElementType::Node)
          element = _allNodes[element_id].get();
        else if (element_type == ElementType::Way)
          element = _allWays[element_id].get();
        else if (element_type == ElementType::Relation)
          element = _allRelations[element_id].get();
        //  If it was found, update the status to finalized because it doesn't need to be deleted twice
        if (element)
        {
          changeset->remove(element_type, ChangesetType::TypeDelete, element->id());
          element->setStatus(ChangesetElement::Finalized);
          _processedCount++;
          //  Return an empty split so that the rest of the changeset is just pushed back
          //  on the work queue to continue on
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
      {
        //  Remove any nodes that were orphaned by the split
        return fixOrphanedNodesSplit(changeset, split);
      }
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
      {
        //  Remove any nodes that were orphaned by the split
        return fixOrphanedNodesSplit(changeset, split);
      }
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
      {
        //  Remove any nodes that were orphaned by the split
        return fixOrphanedNodesSplit(changeset, split);
      }
    }
  }
  //  Remove any nodes that were orphaned by the split
  return fixOrphanedNodesSplit(changeset, split);
}

void XmlChangeset::updateFailedChangeset(const ChangesetInfoPtr& changeset, bool forceFailure)
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

QString XmlChangeset::getChangesetString(const ChangesetInfoPtr& changeset, long changeset_id)
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
  //  Add any of the clean-up elements that weren't cleaned up
  if (getCleanupCount() > 0)
    changeset->append(_cleanup);
  //  Return the changeset string using changeset ID 0 to indicate it was a failure
  return getChangesetString(changeset, 0);
}

QString XmlChangeset::getChangeset(const ChangesetInfoPtr& changeset, long changeset_id, ChangesetType type)
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

void XmlChangeset::updateLastElement(LastElementInfo& last)
{
  //  Update the ID if it is negative
  ElementType::Type type = last._id.getType().getEnum();
  long id  = last._id.getId();
  if (id < 0)
  {
    if (_idMap.containsId(type, id))
      last._id = ElementId(type, _idMap.getId(type, id));
  }
  //  Get the current version
  switch(type)
  {
  case ElementType::Node:
    last._version = _allNodes[id]->getVersion();
    break;
  case ElementType::Way:
    last._version = _allWays[id]->getVersion();
    break;
  case ElementType::Relation:
    last._version = _allRelations[id]->getVersion();
    break;
  default:
    LOG_WARN("Unknown element type found updating last element.");
    break;
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
      if (element->getVersion() != version && version >= 1)
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

void XmlChangeset::failChangeset(const ChangesetInfoPtr& changeset)
{
  if (!changeset)
    return;
  if (!changeset->getError())
    return;
  for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
  {
    //  Iterate all of the nodes and fail them
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Node, (ChangesetType)current_type);
         it != changeset->end(ElementType::Node, (ChangesetType)current_type); ++it)
    {
      failNode(*it);
    }
    //  Iterate all of the ways and fail them
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Way, (ChangesetType)current_type);
         it != changeset->end(ElementType::Way, (ChangesetType)current_type); ++it)
    {
      failWay(*it);
    }
    //  Iterate all of the relations and fail them
    for (ChangesetInfo::iterator it = changeset->begin(ElementType::Relation, (ChangesetType)current_type);
         it != changeset->end(ElementType::Relation, (ChangesetType)current_type); ++it)
    {
      failRelation(*it);
    }
  }
  //  Write out any failed elements
  writeErrorFile();
}

void XmlChangeset::failNode(long id, bool beforeSend)
{
  //  Validate the ID
  if (_allNodes.find(id) == _allNodes.end() || !_allNodes[id])
    return;
  //  Don't set it twice
  if (_allNodes[id]->getStatus() == ChangesetElement::ElementStatus::Failed)
    return;
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend || _allNodes[id]->getStatus() == ChangesetElement::ElementStatus::Available)
    ++_sentCount;
  //  Set the node's status to failed
  _allNodes[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  When a node fails to be created, any parent ways must fail also
  //  Along with any parent relations
  if (_nodes[TypeCreate].find(id) != _nodes[TypeCreate].end())
  {
    //  Fail parent ways
    if (_nodeIdsToWays.find(id) != _nodeIdsToWays.end())
    {
      const set<long>& parents = _nodeIdsToWays[id];
      for (set<long>::const_iterator it = parents.begin(); it != parents.end(); ++it)
        failWay(*it, beforeSend);
    }
    //  Fail parent relations
    if (_nodeIdsToRelations.find(id) != _nodeIdsToRelations.end())
    {
      const set<long>& parents = _nodeIdsToRelations[id];
      for (set<long>::const_iterator it = parents.begin(); it != parents.end(); ++it)
        failRelation(*it, beforeSend);
    }
  }
}

void XmlChangeset::failWay(long id, bool beforeSend)
{
  //  Validate the ID
  if (_allWays.find(id) == _allWays.end() || !_allWays[id])
    return;
  //  Don't set it twice
  if (_allWays[id]->getStatus() == ChangesetElement::ElementStatus::Failed)
    return;
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend || _allWays[id]->getStatus() == ChangesetElement::ElementStatus::Available)
    ++_sentCount;
  //  Set the way's status to failed
  _allWays[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  When a way fails to be created, any parent relations must fail also
  if (_ways[TypeCreate].find(id) != _ways[TypeCreate].end() &&
      _wayIdsToRelations.find(id) != _wayIdsToRelations.end())
  {
    const set<long>& parents = _wayIdsToRelations[id];
    for (set<long>::const_iterator it = parents.begin(); it != parents.end(); ++it)
      failRelation(*it, beforeSend);
  }
}

void XmlChangeset::failRelation(long id, bool beforeSend)
{
  //  Validate the ID
  if (_allRelations.find(id) == _allRelations.end() || !_allRelations[id])
    return;
  //  Don't set it twice
  if (_allRelations[id]->getStatus() == ChangesetElement::ElementStatus::Failed)
    return;
  //  Update the failed count once
  ++_failedCount;
  //  Update sent count as if we already sent it and it failed
  if (beforeSend || _allRelations[id]->getStatus() == ChangesetElement::ElementStatus::Available)
    ++_sentCount;
  //  Set the relation's status to failed
  _allRelations[id]->setStatus(ChangesetElement::ElementStatus::Failed);
  //  When a relation fails to be created, any parent relations must fail also
  if (_relations[TypeCreate].find(id) != _relations[TypeCreate].end() &&
      _relationIdsToRelations.find(id) != _relationIdsToRelations.end())
  {
    const set<long>& parents = _relationIdsToRelations[id];
    for (set<long>::const_iterator it = parents.begin(); it != parents.end(); ++it)
      failRelation(*it, beforeSend);
  }
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
    ts << elements.at(_idMap.getId(elementType, *it))->toString(changeset_id, type);
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

bool XmlChangeset::calculateRemainingChangeset(ChangesetInfoPtr& changeset)
{
  //  Create the changeset info object if there isn't one
  if (!changeset)
    changeset.reset(new ChangesetInfo());
  changeset->clear();
  //  This is the last changeset of the bunch because of the error state
  changeset->setFinished();
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
    //  Write the file with changeset ID of zero
    FileUtils::writeFully(getRemainingFilename(), this->getChangesetString(changeset, 0));
  }
  //  Return true if there is anything in the changeset
  return !empty_changeset;
}

void XmlChangeset::updateRemainingChangeset()
{
  //  Remove the "remaining" file once it comes back successfully
  QFile::remove(getRemainingFilename());
}

bool XmlChangeset::isMatch(const XmlChangeset& changeset)
{
  bool isEqual = true;
  //  Check counts first
  if (_allNodes.size() != changeset._allNodes.size())
  {
    LOG_WARN("Node count not equal (ref: " << _allNodes.size() << ", test: " << changeset._allNodes.size() << ")");
    isEqual = false;
  }
  if (_allWays.size() != changeset._allWays.size())
  {
    LOG_WARN("Way count not equal (ref: " << _allWays.size() << ", test: " << changeset._allWays.size() << ")");
    isEqual = false;
  }
  if (_allRelations.size() != changeset._allRelations.size())
  {
    LOG_WARN("Relation count not equal (ref: " << _allRelations.size() << ", test: " << changeset._allRelations.size() << ")");
    isEqual = false;
  }
  //  Now check each type, create, modify, delete
  for (int changeset_type = ChangesetType::TypeCreate; changeset_type != ChangesetType::TypeMax; ++changeset_type)
  {
    ChangesetType type = static_cast<ChangesetType>(changeset_type);
    //  Iterate all of the nodes of "type" in the changeset
    QSet<long> missingNodes;
    for (ChangesetElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
    {
      ChangesetNode* node = dynamic_cast<ChangesetNode*>(it->second.get());
      ChangesetElementMap::const_iterator found = changeset._nodes[type].find(node->id());
      if (found != changeset._nodes[type].end())
      {
        //  Compare the two nodes
        ChangesetNode* node2 = dynamic_cast<ChangesetNode*>(found->second.get());
        QString output;
        if (!node->diff(*node2, output))
        {
          //  Display the node diff
          output.chop(1);
          LOG_WARN("Node ID " << node->id() << "\n" << output);
        }
      }
      else
        missingNodes.insert(node->id());
    }
    //  Output missing nodes in full
    if (missingNodes.size() > 0)
    {
      QString buffer;
      QTextStream ts(&buffer);
      ts.setCodec("UTF-8");
      for (QSet<long>::iterator it = missingNodes.begin(); it != missingNodes.end(); ++it)
        ts << _nodes[type][*it]->toString(0, type);
      buffer.chop(1);
      buffer.replace("\n", "\n>");
      LOG_WARN("Missing nodes: " << getString(type) << " - " << missingNodes << "\n>" << buffer);
      isEqual = false;
    }
    //  Iterate all of the ways of "type" in the changeset
    QSet<long> missingWays;
    for (ChangesetElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      ChangesetWay* way = dynamic_cast<ChangesetWay*>(it->second.get());
      ChangesetElementMap::const_iterator found = changeset._ways[type].find(way->id());
      if (found != changeset._ways[type].end())
      {
        //  Compare the two ways
        ChangesetWay* way2 = dynamic_cast<ChangesetWay*>(found->second.get());
        QString output;
        if (!way->diff(*way2, output))
        {
          //  Display the way diff
          output.chop(1);
          LOG_WARN("Way ID " << way->id() << "\n" << output);
        }
      }
      else
        missingWays.insert(way->id());
    }
    //  Output missing ways in full
    if (missingWays.size() > 0)
    {
      QString buffer;
      QTextStream ts(&buffer);
      ts.setCodec("UTF-8");
      for (QSet<long>::iterator it = missingWays.begin(); it != missingWays.end(); ++it)
        ts << _ways[type][*it]->toString(0, type);
      buffer.chop(1);
      buffer.replace("\n", "\n>");
      LOG_WARN("Missing ways: " << getString(type) << " - " << missingWays << "\n>" << buffer);
      isEqual = false;
    }
    //  Iterate all of the relations of "type" in the changeset
    QSet<long> missingRelations;
    for (ChangesetElementMap::iterator it = _relations[changeset_type].begin(); it != _relations[changeset_type].end(); ++it)
    {
      ChangesetRelation* relation = dynamic_cast<ChangesetRelation*>(it->second.get());
      ChangesetElementMap::const_iterator found = changeset._relations[changeset_type].find(relation->id());
      if (found != changeset._relations[changeset_type].end())
      {
        //  Compare the two ways
        ChangesetRelation* relation2 = dynamic_cast<ChangesetRelation*>(found->second.get());
        QString output;
        if (!relation->diff(*relation2, output))
        {
          //  Display the relation diff
          output.chop(1);
          LOG_WARN("Relation ID " << relation->id() << "\n" << output);
        }
      }
      else
        missingRelations.insert(relation->id());
    }
    //  Output missing relations in full
    if (missingRelations.size() > 0)
    {
      QString buffer;
      QTextStream ts(&buffer);
      ts.setCodec("UTF-8");
      for (QSet<long>::iterator it = missingRelations.begin(); it != missingRelations.end(); ++it)
        ts << _relations[type][*it]->toString(0, type);
      buffer.chop(1);
      buffer.replace("\n", "\n>");
      LOG_WARN("Missing relations: " << getString(type) << " - " << missingRelations << "\n>" << buffer);
      isEqual = false;
    }
  }
  return isEqual;
}

void XmlChangeset::failRemainingChangeset()
{
  //  Fail all remaining nodes
  failRemainingElements(_allNodes);
  //  Fail all remaining ways
  failRemainingElements(_allWays);
  //  Fail all remaining relations
  failRemainingElements(_allRelations);
}

void XmlChangeset::failRemainingElements(const ChangesetElementMap& elements)
{
  //  Iterate all elements
  for (ChangesetElementMap::const_iterator it = elements.begin(); it != elements.end(); ++it)
  {
    ChangesetElementPtr element = it->second;
    //  Validate elements before using them
    if (element)
    {
      //  Anything that isn't finalized or failed is now failed
      ChangesetElement::ElementStatus status = element->getStatus();
      if (status != ChangesetElement::Finalized && status != ChangesetElement::Failed)
      {
        element->setStatus(ChangesetElement::Failed);
        ++_failedCount;
      }
    }
  }
}

QString XmlChangeset::getRemainingFilename()
{
  //  Replace error with remaining in the error pathname
  QString pathname = _errorPathname;
  pathname.replace("error", "remaining");
  return pathname;
}

ChangesetInfoPtr XmlChangeset::fixOrphanedNodesSplit(const ChangesetInfoPtr& changeset, const ChangesetInfoPtr& split)
{
  set<long> nodes;
  //  Iterate all of the nodes in the remaining changeset looking for deleted shared nodes
  for (ChangesetInfo::iterator it = changeset->begin(ElementType::Node, ChangesetType::TypeDelete);
       it != changeset->end(ElementType::Node, ChangesetType::TypeDelete); ++it)
  {
    //  Remove any node found in a relation in the split changeset
    if (_nodeIdsToRelations.find(*it) != _nodeIdsToRelations.end())
    {
      set<long> relations = _nodeIdsToRelations[*it];
      for (set<long>::iterator r = relations.begin(); r != relations.end(); ++r)
      {
        if (split->contains(ElementType::Relation, *r))
          nodes.insert(*it);
      }
    }
    //  Remove any node found in a way in the split changeset
    if (_nodeIdsToWays.find(*it) != _nodeIdsToWays.end())
    {
      set<long> ways = _nodeIdsToWays[*it];
      for (set<long>::iterator w = ways.begin(); w != ways.end(); ++w)
      {
        if (split->contains(ElementType::Way, *w))
          nodes.insert(*it);
      }
    }
  }
  //  Remove any of the nodes from the changeset so that they are blocked for upload later
  ChangesetInfoPtr orphanedNodes(new ChangesetInfo());
  for (set<long>::iterator n = nodes.begin(); n != nodes.end(); ++n)
  {
    ChangesetElement* node = _allNodes[*n].get();
    if (node)
    {
      changeset->remove(ElementType::Node, ChangesetType::TypeDelete, *n);
      orphanedNodes->add(ElementType::Node, ChangesetType::TypeDelete, *n);
    }
  }
  //  Add the orphaned nodes to the
  std::lock_guard<std::mutex> lock(_cleanupMutex);
  _cleanupCount += orphanedNodes->size();
  if (!_cleanup)
    _cleanup = orphanedNodes;
  else
    _cleanup->append(orphanedNodes);
  //  Return the split
  return split;
}

ChangesetInfoPtr XmlChangeset::getCleanupElements()
{
  //  Lock the mutex and return the cleanup changeset
  std::lock_guard<std::mutex> lock(_cleanupMutex);
  return _cleanup;
}

void XmlChangeset::clearCleanupElements()
{
  std::lock_guard<std::mutex> lock(_cleanupMutex);
  _cleanupCount = 0;
  _cleanup.reset();
}

void XmlChangeset::insertElement(const ChangesetElementPtr& element, ChangesetType type, ChangesetTypeMap& elementMap, ChangesetElementMap& all)
{
  if (type == ChangesetType::TypeDelete && elementMap[ChangesetType::TypeModify].find(element->id()) != elementMap[ChangesetType::TypeModify].end())
  {
    //  Ignore the delete because the modify already exists
  }
  else if (type == ChangesetType::TypeModify && elementMap[ChangesetType::TypeDelete].find(element->id()) != elementMap[ChangesetType::TypeDelete].end())
  {
    //  Remove the delete element
    elementMap[ChangesetType::TypeDelete].erase(element->id());
    //  Add the modify element
    elementMap[type][element->id()] = element;
    all[element->id()] = element;
  }
  else
  {
    //  Add the element to the changeset
    elementMap[type][element->id()] = element;
    all[element->id()] = element;
  }
}

ChangesetInfo::ChangesetInfo()
  : _attemptedResolveChangesetIssues(false),
    _numFailureRetries(0),
    _numVersionRetries(0),
    _finished(false),
    _isError(false)
{
}

void ChangesetInfo::add(ElementType::Type element_type, ChangesetType changeset_type, long id)
{
  _changeset[element_type][changeset_type].insert(id);
  //  Changes in the changeset cause the retries to start over
  _numFailureRetries = 0;
}

void ChangesetInfo::remove(ElementType::Type element_type, ChangesetType changeset_type, long id)
{
  container& selectedSet = _changeset[element_type][changeset_type];
  if (selectedSet.find(id) != selectedSet.end())
    selectedSet.erase(id);
  //  Changes in the changeset cause the retries to start over
  _numFailureRetries = 0;
}

long ChangesetInfo::getFirst(ElementType::Type element_type, ChangesetType changeset_type)
{
  return *(_changeset[element_type][changeset_type].begin());
}

void ChangesetInfo::clear()
{
  for (int i = 0; i < ElementType::Unknown; ++i)
  {
    for (int j = 0; j < ChangesetType::TypeMax; ++j)
      _changeset[i][j].clear();
  }
  _numFailureRetries = 0;
}

bool ChangesetInfo::contains(ElementType::Type element_type, long id)
{
  return _changeset[element_type][ChangesetType::TypeCreate].find(id) != end(element_type, ChangesetType::TypeCreate) ||
         _changeset[element_type][ChangesetType::TypeModify].find(id) != end(element_type, ChangesetType::TypeModify) ||
         _changeset[element_type][ChangesetType::TypeDelete].find(id) != end(element_type, ChangesetType::TypeDelete);
}

bool ChangesetInfo::contains(ElementType::Type element_type, ChangesetType changeset_type, long id)
{
  return _changeset[element_type][changeset_type].find(id) != end(element_type, changeset_type);
}

ChangesetInfo::iterator ChangesetInfo::begin(ElementType::Type element_type, ChangesetType changeset_type)
{
  return _changeset[element_type][changeset_type].begin();
}

ChangesetInfo::iterator ChangesetInfo::end(ElementType::Type element_type, ChangesetType changeset_type)
{
  return _changeset[element_type][changeset_type].end();
}

size_t ChangesetInfo::size(ElementType::Type elementType, ChangesetType changesetType)
{
  return _changeset[elementType][changesetType].size();
}

size_t ChangesetInfo::size(ChangesetType changesetType)
{
  size_t s = 0;
  //  Iterate element types
  for (int i = 0; i < (int)ElementType::Unknown; ++i)
    s += _changeset[i][changesetType].size();
  return s;
}


size_t ChangesetInfo::size()
{
  size_t s = 0;
  //  Iterate element types
  for (int i = 0; i < (int)ElementType::Unknown; ++i)
  {
    //  Sum up all counts for each changeset type
    for (int j = 0; j < ChangesetType::TypeMax; ++j)
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

bool ChangesetInfo::canRetryFailure()
{
  return _numFailureRetries < MAX_FAILURE_RETRIES;
}

void ChangesetInfo::retryFailure()
{
  //  Increment the failure retry count
  _numFailureRetries++;
  //  A retry was attempted, set the attempted flag
  _attemptedResolveChangesetIssues = true;
}

bool ChangesetInfo::canRetryVersion()
{
  return _numVersionRetries < MAX_VERSION_RETRIES;
}

void ChangesetInfo::retryVersion()
{
  //  Increment the version retry count
  _numVersionRetries++;
}

void ChangesetInfo::append(const std::shared_ptr<ChangesetInfo>& info)
{
  if (!info)
    return;
  //  Iterate all of the types and modes appending 'info' to this object
  for (int i = 0; i < (int)ElementType::Unknown; ++i)
  {
    for (int j = 0; j < ChangesetType::TypeMax; ++j)
      _changeset[i][j].insert(info->_changeset[i][j].begin(), info->_changeset[i][j].end());
  }
}

LastElementInfo ChangesetInfo::getLastElement()
{
  LastElementInfo last;
  //  Iterate backwards to get the last element
  for (int i = (int)ElementType::Relation; i >= (int)ElementType::Node; --i)
  {
//    for (int j = ChangesetType::TypeDelete; j >= ChangesetType::TypeCreate; --j)
    //  Temporary reporting fix for UI overpass sync-fix, ignore relation deletes for last element reporting
    int j = (i == (int)ElementType::Relation) ? ChangesetType::TypeModify : ChangesetType::TypeDelete;
    for (; j >= ChangesetType::TypeCreate; --j)
    {
      if (_changeset[i][j].size() > 0)
      {
        long id = (*_changeset[i][j].begin());
        last._id = ElementId((ElementType::Type)i, id);
        last._type = (ChangesetType)j;
        return last;
      }
    }
  }
  return last;
}

}
