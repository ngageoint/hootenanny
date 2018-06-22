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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmChangesetElement.h"

//  Hoot
#include <hoot/core/VersionDefines.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/Log.h>

//  Qt
#include <QTextStream>

using namespace std;

namespace hoot
{

XmlChangeset::XmlChangeset()
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxChangesetSize(100),
    _sentCount(0),
    _processedCount(0)
{
}

XmlChangeset::XmlChangeset(const QList<QString> &changesets)
  : _nodes(ChangesetType::TypeMax),
    _ways(ChangesetType::TypeMax),
    _relations(ChangesetType::TypeMax),
    _maxChangesetSize(100),
    _sentCount(0),
    _processedCount(0)
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
  QString changeset = FileUtils::readFully(changesetPath);
  QXmlStreamReader reader(changeset);
  //  Make sure that the XML provided starts with the <diffResult> tag
  QXmlStreamReader::TokenType type = reader.readNext();
  if (type == QXmlStreamReader::StartDocument)
    type = reader.readNext();
  if (type == QXmlStreamReader::StartElement && reader.name() != "osmChange")
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
      if (name == "create")
        loadElements(reader, ChangesetType::TypeCreate);
      else if (name == "modify")
        loadElements(reader, ChangesetType::TypeModify);
      else if (name == "delete")
        loadElements(reader, ChangesetType::TypeDelete);
    }
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
        _nodesToWays[id].push_back(element->id());
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
  QStringRef name = reader.name();
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
      long version = -1;
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

bool XmlChangeset::isSent(XmlElement* element)
{
  if (element == NULL)
    return false;
  else
    return element->getStatus() == XmlElement::ElementStatus::Finalized;
}

bool XmlChangeset::canSend(XmlNode* node)
{
  if (node == NULL)
    return false;
  else
    return node->getStatus() == XmlElement::Available;
}

bool XmlChangeset::canSend(XmlWay* way)
{
  if (way == NULL)
    return false;

  else if (way->getStatus() != XmlElement::Available)
    return false;
  else
  {
    //  All nodes have to be Available or Finalized
    for (int i = 0; i < way->getNodeCount(); ++i)
    {
      long id = way->getNode(i);
      if (_allNodes.find(id) != _allNodes.end() &&
          _allNodes.at(way->getNode(i))->getStatus() == XmlElement::Sent)
        return false;
    }
  }
  return true;
}

bool XmlChangeset::canSend(XmlRelation* relation)
{
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
        //  If the node exists in the list and
        //  it hasn't been sent yet and
        //  it can't be sent yet
        //  then we can't send this relation
        if (_allNodes.find(member.getRef()) != _allNodes.end() &&
            !isSent(_allNodes[member.getRef()].get()) &&
            !canSend(dynamic_cast<XmlNode*>(_allNodes[member.getRef()].get())))
        {
          return false;
        }
      }
      else if (member.isWay())
      {
        if (_allWays.find(member.getRef()) != _allWays.end() &&
            !isSent(_allWays[member.getRef()].get()) &&
            !canSend(dynamic_cast<XmlWay*>(_allWays[member.getRef()].get())))
        {
          return false;
        }
      }
      else if (member.isRelation())
      {
        if (_allRelations.find(member.getRef()) != _allWays.end() &&
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
    element->setStatus(XmlElement::Buffering);
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
//  TEMPORARY:
//#define RELATION_TO_NODE
#define WAY_RELATION_NODE
#ifdef RELATION_TO_NODE
    //  TODO: At some point we should test if the relation/way/node order should be reversed
    //  to figure out which one is faster
    //  Start with the relations
    addRelations(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the ways
    addWays(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the nodes
    addNodes(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
#elif defined WAY_RELATION_NODE
    //  Start with the ways
    addWays(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the relations
    addRelations(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
    //  Then the nodes
    addNodes(changeset, type);
    //  Break out of the loop once the changeset is big enough
    if (changeset->size() >= (size_t)_maxChangesetSize)
      continue;
#else
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
#endif
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
  //  Start with relations and try to split
  //  Then move to ways
  //  Finally nodes

  //  Return the second changeset to be added to the queue
  return split;
}

QString XmlChangeset::getChangesetString(ChangesetInfoPtr changeset, long changeset_id)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  //  OSM Changeset tag
  ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
     << "<osmChange version=\"0.6\" generator=\""<< HOOT_NAME << "\">\n";
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

QString XmlChangeset::getChangeset(ChangesetInfoPtr changeset, long id, ChangesetType type)
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  XmlElementMap& nodes = _nodes[type];
  XmlElementMap& ways = _ways[type];
  XmlElementMap& relations = _relations[type];
  QString category;
  if (type == ChangesetType::TypeCreate)
    category = "create";
  else if (type == ChangesetType::TypeModify)
    category = "modify";
  else if (type == ChangesetType::TypeDelete)
    category = "delete";
  if (nodes.size() > 0 || ways.size() > 0 || relations.size() > 0)
  {
    ts << "\t<" << category << ">\n";
    for (ElementIdToIdMap::iterator it = _idMap.begin(ElementType::Node); it != _idMap.end(ElementType::Node); ++it)
    {
      if (nodes.find(it->second) != nodes.end() && changeset->contains(ElementType::Node, type, it->second))
        ts << nodes.at(it->second)->toString(id);
    }
    for (ElementIdToIdMap::iterator it = _idMap.begin(ElementType::Way); it != _idMap.end(ElementType::Way); ++it)
    {
      if (ways.find(it->second) != ways.end() && changeset->contains(ElementType::Way, type, it->second))
        ts << ways.at(it->second)->toString(id);
    }
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

  XmlElementMap& type = map[changeset_type];
  XmlElementMap::iterator position = type.find(old_id);
  if (position != type.end())
  {
    XmlElementPtr element = type[old_id];
    element->setStatus(XmlElement::Finalized);
    if (changeset_type == ChangesetType::TypeCreate)
      _idMap.updateId(element->getType(), old_id, new_id);
    if (version >= 0)
      element->setVersion(version);
    _processedCount++;
  }
}

XmlElement::XmlElement(const XmlObject& object, ElementIdToIdMap* idMap)
  : _type(ElementType::Unknown),
    _id(object.second.value("id").toString().toLong()),
    _version(object.second.value("version").toString().toLong()),
    _object(object),
    _idMap(idMap),
    _status(XmlElement::Available)
{
}

void XmlElement::addTag(const XmlObject& tag)
{
  if (tag.first == "tag")
    _tags.push_back(tag);
}

QString XmlElement::toString(const QXmlStreamAttributes& attributes, long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  for (int i = 0; i < attributes.size(); ++i)
  {
    const QXmlStreamAttribute& attribute = attributes.at(i);
    QStringRef name = attribute.name();
    ts << name.toString() << "=\"";
    if (name == "changeset")
      ts << changesetId;
    else if (name == "id")
      ts << (_idMap ? _idMap->getNewId(_type, _id) : _id);
    else if (name == "version")
      ts << _version;
    else
      ts << attribute.value().toString();
    ts << "\" ";
  }
  if (!attributes.hasAttribute("changeset"))
    ts << "changeset=\"" << changesetId << "\"";
  return ts.readAll();
}

QString& XmlElement::escapeString(QString& value) const
{
  return value.replace("&", "&amp;")
              .replace("\"", "&quot;")
              .replace("\n", "&#10;")
              .replace(">", "&gt;")
              .replace("<", "&lt;");
}

QString XmlElement::toTagString(const QXmlStreamAttributes& attributes) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  QString value(attributes.value("v").toString());
  ts << "\t\t\t<tag k=\"" << attributes.value("k").toString() << "\" v=\"" << escapeString(value) << "\"/>\n";
  return ts.readAll();
}

void XmlElement::setStatus(ElementStatus status)
{
  _status = status;
}

XmlNode::XmlNode(const XmlObject& node, ElementIdToIdMap* idMap)
  : XmlElement(node, idMap)
{
  _type = ElementType::Node;
}

QString XmlNode::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<node " << XmlElement::toString(_object.second, changesetId);
  if (_tags.size() > 0)
  {
    ts << ">\n";
    for (QVector<XmlObject>::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(it->second);
    ts << "\t\t</node>\n";
  }
  else
    ts << "/>\n";
  return ts.readAll();
}

XmlWay::XmlWay(const XmlObject& way, ElementIdToIdMap* idMap)
  : XmlElement(way, idMap)
{
  _type = ElementType::Way;
}

QString XmlWay::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<way " << XmlElement::toString(_object.second, changesetId) << ">\n";
  for (QList<long>::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
    ts << "\t\t\t<nd ref=\"" << (_idMap ? _idMap->getNewId(ElementType::Node, *it) : *it) << "\"/>\n";
  if (_tags.size() > 0)
  {
    for (QVector<XmlObject>::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(it->second);
  }
  ts << "\t\t</way>\n";
  return ts.readAll();
}

XmlRelation::XmlRelation(const XmlObject& relation, ElementIdToIdMap* idMap)
  : XmlElement(relation, idMap)
{
  _type = ElementType::Relation;
}

void XmlRelation::addMember(const QXmlStreamAttributes& member)
{
  XmlMember m(member, _idMap);
  _members.append(m);
}

QString XmlRelation::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<relation " << XmlElement::toString(_object.second, changesetId) << ">\n";
  for (QList<XmlMember>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    ts << it->toString();
  if (_tags.size() > 0)
  {
    for (QVector<XmlObject>::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(it->second);
  }
  ts << "\t\t</relation>\n";
  return ts.readAll();
}

XmlMember& XmlRelation::getMember(int index)
{
  return _members[index];
}

int XmlRelation::getMemberCount()
{
  return _members.size();
}

XmlMember::XmlMember(const QXmlStreamAttributes& member, ElementIdToIdMap* idMap)
  : _type(member.value("type").toString()),
    _ref(member.value("ref").toString().toLong()),
    _role(member.value("role").toString()),
    _idMap(idMap)
{
}

QString XmlMember::toString() const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t\t<member type=\"" << _type << "\" ref=\""
     << (_idMap ? _idMap->getNewId(ElementType::fromString(_type),_ref) : _ref)
     << "\" role=\"" << _role << "\"/>\n";
  return ts.readAll();
}


}
