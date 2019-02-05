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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmApiChangeset.h"

//  Hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

//  Standard
#include <vector>

//  Qt
#include <QTextStream>

using namespace std;

namespace hoot
{

XmlChangeset::XmlChangeset()
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxChangesetSize(ConfigOptions().getChangesetApidbMaxSize()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0)
{
}

XmlChangeset::XmlChangeset(const QList<QString> &changesets)
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxChangesetSize(ConfigOptions().getChangesetApidbMaxSize()),
    _sentCount(0),
    _processedCount(0),
    _failedCount(0)
{
  for (QList<QString>::const_iterator it = changesets.begin(); it != changesets.end(); ++it)
    loadChangeset(*it);
}

void XmlChangeset::loadChangeset(const QString &changesetPath)
{
  /* <osmChange version="0.6" generator="acme osm editor">
   *   <create|modify|delete>
   *     <node|way|relation.../>
   *     ...
   *   </create|modify|delete>
   *   ...
   * </osmChange>
   */
  QFile file(changesetPath);
  if (!file.open(QIODevice::ReadOnly))
  {
    LOG_ERROR("Unable to read file at: " << changesetPath);
    return;
  }
  //  Open the XML stream reader and attach it to the file
  QXmlStreamReader reader(&file);
  //  Make sure that the XML provided starts with the <diffResult> tag
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  //  Read the OSC changeset
  if (changesetPath.endsWith(".osc"))
  {
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
  else  //  .osm file
  {
    if (type == QXmlStreamReader::StartElement && reader.name() != "osm")
    {
      LOG_ERROR("Unknown OSM XML file format.");
      return;
    }
    //  Force load all of the elements as 'create' elements
    loadElements(reader, ChangesetType::TypeCreate);
  }
}

void XmlChangeset::loadElements(QXmlStreamReader& reader, ChangesetType changeset_type)
{
  XmlElementPtr element;
  while (!reader.atEnd() && !reader.hasError())
  {
    QXmlStreamReader::TokenType type = reader.readNext();
    QStringRef name = reader.name();
    //  Start element for nodes/ways/relations and tags/nds/members
    if (type == QXmlStreamReader::StartElement)
    {
      if (name == "node")
      {
        element.reset(new XmlNode(XmlObject("node", reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Node, id);
      }
      else if (name == "way")
      {
        element.reset(new XmlWay(XmlObject("way", reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Way, id);
      }
      else if (name == "relation")
      {
        element.reset(new XmlRelation(XmlObject("relation", reader.attributes()), &_idMap));
        long id = reader.attributes().value("id").toString().toLong();
        _idMap.addId(ElementType::Relation, id);
      }
      else if (name == "tag")
        element->addTag(XmlObject("tag", reader.attributes()));
      else if (name == "nd")
      {
        long id = reader.attributes().value("ref").toString().toLong();
        boost::dynamic_pointer_cast<XmlWay>(element)->addNode(id);
      }
      else if (name == "member")
        boost::dynamic_pointer_cast<XmlRelation>(element)->addMember(reader.attributes());
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
    for (XmlElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      XmlWay* way = dynamic_cast<XmlWay*>(it->second.get());
      while (way->getNodeCount() > maxWayNodes)
      {
        //  Create a copy of the way
        XmlWayPtr newWay(new XmlWay(*way));
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
  for (XmlElementMap::iterator it = _relations[TypeCreate].begin(); it != _relations[TypeCreate].end(); ++it)
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
  for (XmlElementMap::iterator it = _ways[TypeCreate].begin(); it != _ways[TypeCreate].end(); ++it)
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
  for (XmlElementMap::iterator it = _nodes[TypeCreate].begin(); it != _nodes[TypeCreate].end(); ++it)
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
    for (XmlElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
    {
      long node_id = it->first;
      //  Set the node's status to failed if negative
      if (node_id < 1)
        failNode(node_id);
    }
    for (XmlElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
    {
      long way_id = it->first;
      //  Set the node's status to failed if negative
      if (way_id < 1)
        failWay(way_id);
    }
    for (XmlElementMap::iterator it = _relations[type].begin(); it != _relations[type].end(); ++it)
    {
      long relation_id = it->first;
      //  Set the node's status to failed if negative
      if (relation_id < 1)
        failRelation(relation_id);
    }
  }
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
  for (XmlElementMap::iterator it = _nodes[type].begin(); it != _nodes[type].end(); ++it)
  {
    //  Add nodes up until the max changeset
    if (changeset->size() < (size_t)_maxChangesetSize)
      added |= addNode(changeset, type, dynamic_cast<XmlNode*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addNode(ChangesetInfoPtr& changeset, ChangesetType type, XmlNode* node)
{
  //  Only add the nodes that are "sendable"
  if (canSend(node))
  {
    //  Add create nodes if the ID map's ID is negative, modify and delete IDs don't matter
    if ((type == ChangesetType::TypeCreate && _idMap.getNewId(ElementType::Node, node->id()) < 0) ||
         type != ChangesetType::TypeCreate)
    {
      //  Add the node
      changeset->add(ElementType::Node, type, node->id());
      markBuffered(node);
    }
    return true;
  }
  else
    return false;
}

bool XmlChangeset::moveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlNode* node)
{
  //  Add the node to the destination and remove from the source
  destination->add(ElementType::Node, type, node->id());
  source->remove(ElementType::Node, type, node->id());
  return true;
}

bool XmlChangeset::canMoveNode(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType /*type*/, XmlNode* /*node*/)
{
  return source->size() != 1;
}

bool XmlChangeset::addWays(ChangesetInfoPtr& changeset, ChangesetType type)
{
  bool added = false;
  //  Iterate all of the ways of "type" in the changeset
  for (XmlElementMap::iterator it = _ways[type].begin(); it != _ways[type].end(); ++it)
  {
    //  Add ways up until the max changeset
    if (changeset->size() < (size_t)_maxChangesetSize)
      added |= addWay(changeset, type, dynamic_cast<XmlWay*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addWay(ChangesetInfoPtr& changeset, ChangesetType type, XmlWay* way)
{
  if (canSend(way))
  {
    //  Add the way
    changeset->add(ElementType::Way, type, way->id());
    markBuffered(way);
    //  Only creates/modifies require more processing
    if (type != ChangesetType::TypeDelete)
    {
      //  Add any nodes that need to be created
      for (int i = 0; i < way->getNodeCount(); ++i)
      {
        //  Negative IDs from the ID map are for created nodes
        if (_idMap.getNewId(ElementType::Node, way->getNode(i)) < 0)
        {
          XmlNode* node = dynamic_cast<XmlNode*>(_allNodes[way->getNode(i)].get());
          addNode(changeset, ChangesetType::TypeCreate, node);
        }
      }
    }
    return true;
  }
  else
    return false;
}

bool XmlChangeset::moveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlWay* way)
{
  //  Add the way to the destination and remove from the source
  destination->add(ElementType::Way, type, way->id());
  source->remove(ElementType::Way, type, way->id());
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
          moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<XmlNode*>(_allNodes[id].get()));
      }
    }
  }
  return true;
}

bool XmlChangeset::canMoveWay(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType type, XmlWay* way)
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
  for (XmlElementMap::iterator it = _relations[type].begin(); it != _relations[type].end(); ++it)
  {
    //  Add relations up until the max changeset
    if (changeset->size() < (size_t)_maxChangesetSize)
      added |= addRelation(changeset, type, dynamic_cast<XmlRelation*>(it->second.get()));
  }
  //  Return true if something was added
  return added;
}

bool XmlChangeset::addRelation(ChangesetInfoPtr& changeset, ChangesetType type, XmlRelation* relation)
{
  if (canSend(relation))
  {
    //  Add the relation
    changeset->add(ElementType::Relation, type, relation->id());
    markBuffered(relation);
    //  Deletes require no more processing
    if (type != ChangesetType::TypeDelete)
    {
      //  Add any relation members that need to be added
      for (int i = 0; i < relation->getMemberCount(); ++i)
      {
        XmlMember& member = relation->getMember(i);
        //  Negative IDs are for added members
        if (member.getRef() < 0)
        {
          //  Add the types differently
          if (member.isNode())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getNewId(ElementType::Node, member.getRef()) < 0)
            {
              XmlNode* node = dynamic_cast<XmlNode*>(_allNodes[member.getRef()].get());
              addNode(changeset, type, node);
            }
          }
          else if (member.isWay())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getNewId(ElementType::Way, member.getRef()) < 0)
            {
              XmlWay* way = dynamic_cast<XmlWay*>(_allWays[member.getRef()].get());
              addWay(changeset, type, way);
            }
          }
          else if (member.isRelation())
          {
            //  Make sure that the ID is negative (create) in the ID map
            if (_idMap.getNewId(ElementType::Relation, member.getRef()) < 0)
            {
              XmlRelation* relation = dynamic_cast<XmlRelation*>(_allRelations[member.getRef()].get());
              addRelation(changeset, type, relation);
            }
          }
        }
      }
    }
    return true;
  }
  else
    return false;
}

bool XmlChangeset::moveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& destination, ChangesetType type, XmlRelation* relation)
{
  //  Add the way to the destination and remove from the source
  destination->add(ElementType::Relation, type, relation->id());
  source->remove(ElementType::Relation, type, relation->id());
  //  Don't worry about the contents of a delete operation
  if (type != ChangesetType::TypeDelete)
  {
    //  Iterate all of the nodes that exist in the changeset and move them
    for (int i = 0; i < relation->getMemberCount(); ++i)
    {
      XmlMember& member = relation->getMember(i);
      long id = member.getRef();
      //  Each member type iterates the changeset types looking for this element, then moves it
      //  using the corresponding move function
      if (member.isNode())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Node, (ChangesetType)current_type, id))
            moveNode(source, destination, (ChangesetType)current_type, dynamic_cast<XmlNode*>(_allNodes[id].get()));
        }
      }
      else if (member.isWay())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Way, (ChangesetType)current_type, id))
            moveWay(source, destination, (ChangesetType)current_type, dynamic_cast<XmlWay*>(_allWays[id].get()));
        }
      }
      else if (member.isRelation())
      {
        for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
        {
          if (source->contains(ElementType::Relation, (ChangesetType)current_type, id))
            moveRelation(source, destination, (ChangesetType)current_type, dynamic_cast<XmlRelation*>(_allRelations[id].get()));
        }
      }
    }
  }
  return true;
}

bool XmlChangeset::canMoveRelation(ChangesetInfoPtr& source, ChangesetInfoPtr& /*destination*/, ChangesetType type, XmlRelation* relation)
{
  //  Deleting a relation will only contain the relation itself
  if (type == ChangesetType::TypeDelete)
    return source->size() == 1;
  //  Get the count of nodes, ways, and relations that make up this "change"
  size_t count = getObjectCount(source, relation);
  //  Compare that count to the size left in the source changeset
  return source->size() != count;
}

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& /*changeset*/, XmlNode* node)
{
  if (node == NULL)
    return 0;
  //  Nodes count as one object
  return 1;
}

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& changeset, XmlWay* way)
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

size_t XmlChangeset::getObjectCount(ChangesetInfoPtr& changeset, XmlRelation* relation)
{
  if (relation == NULL)
    return 0;
  //  Get the count of nodes, ways, and relations that make up this "change"
  size_t count = 1;
  for (int i = 0; i < relation->getMemberCount(); ++i)
  {
    XmlMember& member = relation->getMember(i);
    long id = member.getRef();
    //  Each member type iterates the changeset types looking for this element, then counts it
    if (member.isNode())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Node, (ChangesetType)current_type, id))
          count += getObjectCount(changeset, dynamic_cast<XmlNode*>(_allNodes[id].get()));
      }
    }
    else if (member.isWay())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Way, (ChangesetType)current_type, id))
          count += getObjectCount(changeset, dynamic_cast<XmlWay*>(_allWays[id].get()));
      }
    }
    else if (member.isRelation())
    {
      for (int current_type = ChangesetType::TypeCreate; current_type != ChangesetType::TypeMax; ++current_type)
      {
        if (changeset->contains(ElementType::Relation, (ChangesetType)current_type, id))
          count += getObjectCount(changeset, dynamic_cast<XmlRelation*>(_allRelations[id].get()));
      }
    }
  }
  return count;
}

bool XmlChangeset::isSent(XmlElement* element)
{
  if (element == NULL)
    return false;
  else
    //  Sent means Buffering, Sent, or Finalized
    return element->getStatus() == XmlElement::ElementStatus::Buffering ||
           element->getStatus() == XmlElement::ElementStatus::Sent ||
           element->getStatus() == XmlElement::ElementStatus::Finalized;
}

bool XmlChangeset::canSend(XmlNode* node)
{
  //  Able to send means Available
  if (node == NULL)
    return false;
  else
    return node->getStatus() == XmlElement::Available;
}

bool XmlChangeset::canSend(XmlWay* way)
{
  //  Able to send means Available
  if (way == NULL)
    return false;
  else if (way->getStatus() != XmlElement::Available)
    return false;
  else
  {
    //  All nodes have to be Available, Buffered, or Finalized
    for (int i = 0; i < way->getNodeCount(); ++i)
    {
      long id = way->getNode(i);
      if (_allNodes.find(id) != _allNodes.end() &&
          !isSent(_allNodes[id].get()) &&
          !canSend(dynamic_cast<XmlNode*>(_allNodes[id].get())))
        return false;
    }
  }
  return true;
}

bool XmlChangeset::canSend(XmlRelation* relation)
{
  //  Able to send means Available
  if (relation == NULL)
    return false;
  else if (relation->getStatus() != XmlElement::Available)
    return false;
  else
  {
    //  All members have to be Available or Finalized
    for (int i = 0; i < relation->getMemberCount(); ++i)
    {
      XmlMember& member = relation->getMember(i);
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
            !canSend(dynamic_cast<XmlNode*>(_allNodes[member.getRef()].get())))
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
            !canSend(dynamic_cast<XmlWay*>(_allWays[member.getRef()].get())))
        {
          return false;
        }
      }
      else if (member.isRelation())
      {
        //  Special case, relation doesn't exist in changeset, it may in database, send it
        if (member.getRef() > 0 && _allRelations.find(member.getRef()) == _allRelations.end())
          return true;
        //  Check if the relation exists and can't be sent
        else if (_allRelations.find(member.getRef()) != _allWays.end() &&
            !isSent(_allRelations[member.getRef()].get()) &&
            !canSend(dynamic_cast<XmlRelation*>(_allRelations[member.getRef()].get())))
        {
          return false;
        }
      }
    }
  }
  return true;
}

void XmlChangeset::markBuffered(XmlElement* element)
{
  if (element != NULL)
  {
    //  Mark buffering
    element->setStatus(XmlElement::Buffering);
    //  Add to the buffer for lookup within this subset
    _sendBuffer.push_back(element);
    _sentCount++;
  }
}

bool XmlChangeset::calculateChangeset(ChangesetInfoPtr& changeset)
{
  //  Create the changeset info object if there isn't one
  if (!changeset)
    changeset.reset(new ChangesetInfo());
  changeset->clear();
  //  Build up the changeset to be around the MAX changeset size
  ChangesetType type = ChangesetType::TypeCreate;
  while (type != ChangesetType::TypeMax &&
         changeset->size() < (size_t)_maxChangesetSize &&
         hasElementsToSend())
  {
    /**
     *  Changesets are created by first adding nodes, then ways, and
     *  finally relations. In testing this order was found to be 4%-7%
     *  faster than any other interpolation of the ordering of nodes,
     *  ways, and relations.
     */
    //  Start with the nodes
    addNodes(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the ways
    addWays(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the relations
    addRelations(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Go to the next type and loop back around
    type = static_cast<ChangesetType>(type + 1);
  }
  //  Move all elements from buffered to sending
  for (vector<XmlElement*>::iterator it = _sendBuffer.begin(); it != _sendBuffer.end(); ++it)
    (*it)->setStatus(XmlElement::Sent);
  _sendBuffer.clear();
  //  Return true if there is anything in this changeset
  return changeset->size() > 0;
}

ChangesetInfoPtr XmlChangeset::splitChangeset(ChangesetInfoPtr changeset)
{
  ChangesetInfoPtr split(new ChangesetInfo());
  //  If there is only one element then splitting only marks that element as failed
  if (changeset->size() == 1)
  {
    updateFailedChangeset(changeset);
    return split;
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
      XmlRelation* relation = dynamic_cast<XmlRelation*>(_allRelations[id].get());

      if (canMoveRelation(changeset, split, (ChangesetType)current_type, relation))
      {
        //  Move the relation and anything associated
        moveRelation(changeset, split, (ChangesetType)current_type, relation);
      }
      else
      {
        //  Remove only the relation from the changeset, not its members
        changeset->remove(ElementType::Relation, (ChangesetType)current_type, relation->id());
        //  Set the relation to available
        relation->setStatus(XmlElement::ElementStatus::Available);
      }
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
      XmlWay* way = dynamic_cast<XmlWay*>(_allWays[id].get());
      if (canMoveWay(changeset, split, (ChangesetType)current_type, way))
      {
        //  Move the way and anything associated
        moveWay(changeset, split, (ChangesetType)current_type, way);
      }
      else
      {
        //  Remove only the way from the changeset, not its nodes
        changeset->remove(ElementType::Way, (ChangesetType)current_type, way->id());
        //  Set the way to available
        way->setStatus(XmlElement::ElementStatus::Available);
      }
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
      XmlNode* node = dynamic_cast<XmlNode*>(_allNodes[id].get());
      if (canMoveNode(changeset, split, (ChangesetType)current_type, node))
      {
        //  Move the node
        moveNode(changeset, split, (ChangesetType)current_type, node);
      }
      else
      {
        //  Remove only the node
        changeset->remove(ElementType::Node, (ChangesetType)current_type, node->id());
        //  Set the node to available
        node->setStatus(XmlElement::ElementStatus::Available);
      }
      //  If the split is big enough, end the operation
      if (split->size() >= splitSize)
        return split;
    }
  }
  //  Return the second changeset to be added to the queue
  return split;
}

void XmlChangeset::updateFailedChangeset(ChangesetInfoPtr changeset)
{
  //  Only set the failed status on single element changesets
  if (changeset->size() != 1)
    return;
  //  Don't set the elements to failed yet, until after they are tried
  if (!changeset->getChangesetIssuesResolved())
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
    for (XmlElementMap::iterator it = _nodes[current_type].begin(); it != _nodes[current_type].end(); ++it)
    {
      XmlNode* node = dynamic_cast<XmlNode*>(it->second.get());
      //  Add only the failed nodes
      if (node->getStatus() == XmlElement::ElementStatus::Failed)
        changeset->add(ElementType::Node, (ChangesetType)current_type, node->id());
    }
    //  Iterate all of the ways in the changeset looking for failed elements
    for (XmlElementMap::iterator it = _ways[current_type].begin(); it != _ways[current_type].end(); ++it)
    {
      XmlWay* way = dynamic_cast<XmlWay*>(it->second.get());
      //  Add only the failed ways
      if (way->getStatus() == XmlElement::ElementStatus::Failed)
        changeset->add(ElementType::Way, (ChangesetType)current_type, way->id());
    }
    //  Iterate all of the relations in the changeset looking for failed elements
    for (XmlElementMap::iterator it = _relations[current_type].begin(); it != _relations[current_type].end(); ++it)
    {
      XmlRelation* relation = dynamic_cast<XmlRelation*>(it->second.get());
      //  Add only the failed relations
      if (relation->getStatus() == XmlElement::ElementStatus::Failed)
        changeset->add(ElementType::Relation, (ChangesetType)current_type, relation->id());
    }
  }
  //  Return the changeset string using changeset ID 0 to indicate it was a failure
  return getChangesetString(changeset, 0);
}

QString XmlChangeset::getChangeset(ChangesetInfoPtr changeset, long id, ChangesetType type)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  XmlElementMap& nodes = _nodes[type];
  XmlElementMap& ways = _ways[type];
  XmlElementMap& relations = _relations[type];
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
    //  Nodes got first in each category
    for (ElementIdToIdMap::iterator it = _idMap.begin(ElementType::Node); it != _idMap.end(ElementType::Node); ++it)
    {
      if (nodes.find(it->second) != nodes.end() && changeset->contains(ElementType::Node, type, it->second))
        ts << nodes.at(it->second)->toString(id);
    }
    //  Followed by ways
    for (ElementIdToIdMap::iterator it = _idMap.begin(ElementType::Way); it != _idMap.end(ElementType::Way); ++it)
    {
      if (ways.find(it->second) != ways.end() && changeset->contains(ElementType::Way, type, it->second))
        ts << ways.at(it->second)->toString(id);
    }
    //  Relations bring up the rear
    for (ElementIdToIdMap::iterator it = _idMap.begin(ElementType::Relation); it != _idMap.end(ElementType::Relation); ++it)
    {
      if (relations.find(it->second) != relations.end() && changeset->contains(ElementType::Relation, type, it->second))
        ts << relations.at(it->second)->toString(id);
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
  XmlElementMap& type = map[changeset_type];
  XmlElementMap::iterator position = type.find(old_id);
  if (position != type.end())
  {
    XmlElementPtr element = type[old_id];
    //  Finalize the element
    element->setStatus(XmlElement::Finalized);
    //  Update the ID in the map with a new ID for elements that were created
    if (changeset_type == ChangesetType::TypeCreate)
      _idMap.updateId(element->getType(), old_id, new_id);
    //  Update the version if necessary
    if (version >= 0)
      element->setVersion(version);
    _processedCount++;
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
    XmlElementMap::iterator position = map[type].find(id);
    if (position != map[type].end())
    {
      //  Found the element, now update it
      XmlElementPtr element = map[type][id];
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
      QXmlStreamAttributes attributes;
      for (QMap<QString, QString>::iterator it = tags.begin(); it != tags.end(); ++it)
        attributes.append("", it.key(), it.value());
      if (attributes.size() > 0)
        element->addTag(XmlObject("tag", QXmlStreamAttributes()));
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

void XmlChangeset::failNode(long id)
{
  //  Set the node's status to failed
  _allNodes[id]->setStatus(XmlElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
}

void XmlChangeset::failWay(long id)
{
  //  Set the way's status to failed
  _allWays[id]->setStatus(XmlElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
}

void XmlChangeset::failRelation(long id)
{
  //  Set the relation's status to failed
  _allRelations[id]->setStatus(XmlElement::ElementStatus::Failed);
  //  Update the failed count once
  ++_failedCount;
}

}
