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

#include "OsmApiChangesetElement.h"

#include <hoot/core/util/HootException.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>

namespace hoot
{

/** Custom ID sort order */
bool id_sort_order(long lhs, long rhs)
{
  if (lhs > 0 && rhs > 0)       return lhs < rhs; //  Positive numbers count up
  else if (lhs < 0 && rhs < 0)  return lhs > rhs; //  Negative numbers count down
  else                          return lhs < rhs; //  Negative numbers come before positive
}

ChangesetElement::ChangesetElement(const XmlObject& object, ElementIdToIdMap* idMap)
  : _type(ElementType::Unknown),
    _id(object.second.value("id").toString().toLong()),
    _version(object.second.value("version").toString().toLong()),
    _idMap(idMap),
    _status(ChangesetElement::Available)
{
  for (QXmlStreamAttributes::const_iterator it = object.second.begin(); it != object.second.end(); ++it)
  {
    //  Ignore the changeset attribute because it will be replaced later even if it does exist
    if (it->name().compare(QString("changeset")) != 0)
      _object.push_back(std::make_pair(it->name().toString(), it->value().toString()));
  }
}

ChangesetElement::ChangesetElement(const ChangesetElement& element)
  : _type(element._type),
    _id(element._id),
    _version(element._version),
    _object(element._object),
    _tags(element._tags),
    _idMap(element._idMap),
    _status(element._status)
{
}

void ChangesetElement::addTag(const XmlObject& tag)
{
  //  Make sure that the object is in fact a tag before adding it
  if (tag.first == "tag")
  {
    QString key;
    QString value;
    for (QXmlStreamAttributes::const_iterator it = tag.second.begin(); it != tag.second.end(); ++it)
    {
      if (it->name() == "k")
        key = it->value().toString();
      else if (it->name() == "v")
        value = it->value().toString();
    }
    if (key != "" and value != "")
      _tags.push_back(std::make_pair(key, value));
  }
}

QString ChangesetElement::getTagKey(int index)
{
  if (index < 0 || index >= (int)_tags.size())
    throw IllegalArgumentException();
  return _tags[index].first;
}

QString ChangesetElement::getTagValue(int index)
{
  if (index < 0 || index >= (int)_tags.size())
    throw IllegalArgumentException();
  return _tags[index].second;
}

QString ChangesetElement::toString(const ElementAttributes& attributes, long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  //  Setting the codec to UTF-8 is the special sauce for foreign characters
  ts.setCodec("UTF-8");
  bool hasChangeset = false;
  for (ElementAttributes::const_iterator it = attributes.begin(); it != attributes.end(); ++it)
  {
    QString name = it->first;
    if (name == "action")
      continue;
    ts << name << "=\"";
    //  Some attributes need to be overridden
    if (name == "changeset")    //  Changeset ID is provided by the OSM API
    {
      ts << changesetId;
      hasChangeset = true;
    }
    else if (name == "id")      //  Element ID could be different than provided in the file, could be from the API
      ts << (_idMap ? _idMap->getId(_type, _id) : _id);
    else if (name == "version") //  Versions are updated by the API
      ts << _version;
    else
      ts << it->second;
    ts << "\" ";
  }
  //  Add the changeset attribute if the original data didn't include it
  if (!hasChangeset)
    ts << "changeset=\"" << changesetId << "\"";
  return ts.readAll();
}

QString ChangesetElement::escapeString(const QString& value) const
{
  //  Simple XML encoding of some problematic characters
  QString escape = value;
  return escape.replace("&", "&amp;")
               .replace("\"", "&quot;")
               .replace("\n", "&#10;")
               .replace(">", "&gt;")
               .replace("<", "&lt;");
}

QString ChangesetElement::toTagString(const ElementTag& tag) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  QString key(escapeString(tag.first));
  QString value(escapeString(tag.second));
  //  Tag values of length 255 need to be truncated
  QString newValue(ApiTagTruncateVisitor::truncateTag(key, value));

  //  Make sure to XML encode the value
  ts << "\t\t\t<tag k=\"" << key << "\" v=\"";
  if (newValue != "")
    ts << newValue;
  else
    ts << value;
  ts << "\"/>\n";
  return ts.readAll();
}

bool ChangesetElement::diffElement(const ChangesetElement* element, QTextStream& ts1, QTextStream& ts2) const
{
  bool success = true;
  //  Compare the IDs
  if (_id != element->_id)
  {
    ts1 << "< id = " << _id << "\n";
    ts2 << "> id = " << element->_id << "\n";
    success = false;
  }
  //  Compare the attributes
  if (!diffAttributes(element->_object, ts1, ts2))
    success = false;
  //  Compare the tags
  if (!diffTags(element->_tags, ts1, ts2))
    success = false;
  return success;
}

bool ChangesetElement::diffAttributes(const ElementAttributes& attributes, QTextStream& ts1, QTextStream& ts2) const
{
  bool success = true;
  //  Compare the attributes by count first
  if (_object.size() != attributes.size())
  {
    ts1 << "< attribute count = " << _object.size() << "\n";
    for (size_t index = 0; index < _object.size(); ++index)
      ts1 << "< " << _object[index].first << " = " << _object[index].second << "\n";
    ts2 << "> attribute count = " << attributes.size() << "\n";
    for (size_t index = 0; index < attributes.size(); ++index)
      ts2 << "> " << attributes[index].first << " = " << attributes[index].second << "\n";
    success = false;
  }
  else
  {
    //  Compare attributes one by one
    for (size_t index = 0; index < _object.size(); ++index)
    {
      QString name1 = _object[index].first;
      QString value1 = _object[index].second;
      QString name2 = attributes[index].first;
      QString value2 = attributes[index].second;
      //  Some attributes can have different values and still be equivalent
      if (name1 == "action" || name1 == "version" || name1 == "changeset")
        continue;
      //  Compare names and values
      if (name1 != name2 && value1 != value2)
      {
        ts1 << "< " << name1 << " = " << value1 << "\n";
        ts2 << "> " << name2 << " = " << value2 << "\n";
        success = false;
      }
    }
  }
  return success;
}

bool ChangesetElement::diffTags(const ElementTags& tags,  QTextStream& ts1, QTextStream& ts2) const
{
  bool success = true;
  //  Compare the tags by count first
  if (_tags.size() != tags.size())
  {
    ts1 << "< tag count = " << _tags.size() << "\n";
    for (size_t index = 0; index < _tags.size(); ++index)
      ts1 << "< " << _tags[index].first << " = " << _tags[index].second << "\n";
    ts2 << "> tag count = " << tags.size() << "\n";
    for (size_t index = 0; index < tags.size(); ++index)
      ts2 << "> " << tags[index].first << " = " << tags[index].second << "\n";
    success = false;
  }
  else
  {
    //  Compare tags one by one
    for (size_t index = 0; index < _tags.size(); ++index)
    {
      QString key1 = _tags[index].first;
      QString value1 = _tags[index].second;
      QString key2 = tags[index].first;
      QString value2 = tags[index].second;
      //  Ignore any tags here?
      //  Compare keys and values
      if (key1 != key2 && value1 != value2)
      {
        ts1 << "< " << key1 << " = " << value1 << "\n";
        ts2 << "> " << key2 << " = " << value2 << "\n";
        success = false;
      }
    }
  }
  return success;
}

ChangesetNode::ChangesetNode(const XmlObject& node, ElementIdToIdMap* idMap)
  : ChangesetElement(node, idMap)
{
  //  Override the type
  _type = ElementType::Node;
}

ChangesetNode::ChangesetNode(const ChangesetNode &node)
  : ChangesetElement(node)
{
}

QString ChangesetNode::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<node " << ChangesetElement::toString(_object, changesetId);
  if (_tags.size() > 0)
  {
    ts << ">\n";
    for (ElementTags::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(*it);
    ts << "\t\t</node>\n";
  }
  else
    ts << "/>\n";
  return ts.readAll();
}

bool ChangesetNode::diff(const ChangesetNode& node, QString& diffOutput) const
{
  bool success = true;
  QString buffer1;
  QTextStream ts1(&buffer1);
  ts1.setCodec("UTF-8");
  QString buffer2;
  QTextStream ts2(&buffer2);
  ts2.setCodec("UTF-8");
  //  Compare the common element data
  if (!diffElement(&node, ts1, ts2))
    success = false;
  //  Create the diff when the two nodes are different
  if (!success)
  {
    ts1 << "--------------------\n" << ts2.readAll();
    diffOutput = ts1.readAll();
  }
  //  Return success variable
  return success;
}

ChangesetWay::ChangesetWay(const XmlObject& way, ElementIdToIdMap* idMap)
  : ChangesetElement(way, idMap)
{
  //  Override the type
  _type = ElementType::Way;
}

ChangesetWay::ChangesetWay(const ChangesetWay &way)
  : ChangesetElement(way),
    _nodes(way._nodes)
{
}

void ChangesetWay::removeNodes(int position, int count)
{
  if (position < 0 || count == 0 || _nodes.size() < 1)
    return;
  if (count < 0 || count >= _nodes.size())
    count = _nodes.size() - position;
  _nodes.remove(position, count);
}

QString ChangesetWay::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<way " << ChangesetElement::toString(_object, changesetId) << ">\n";
  for (QVector<long>::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
    ts << "\t\t\t<nd ref=\"" << (_idMap ? _idMap->getId(ElementType::Node, *it) : *it) << "\"/>\n";
  if (_tags.size() > 0)
  {
    for (ElementTags::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(*it);
  }
  ts << "\t\t</way>\n";
  return ts.readAll();
}

bool ChangesetWay::diff(const ChangesetWay& way, QString& diffOutput) const
{
  bool success = true;
  QString buffer1;
  QTextStream ts1(&buffer1);
  ts1.setCodec("UTF-8");
  QString buffer2;
  QTextStream ts2(&buffer2);
  ts2.setCodec("UTF-8");
  //  Compare the common element data
  if (!diffElement(&way, ts1, ts2))
    success = false;
  //  Compare the way nodes
  if (_nodes.size() != way._nodes.size())
  {
    ts1 << "< node count = " << _nodes.size() << "\n";
    for (int index = 0; index < _nodes.size(); ++index)
      ts1 << "< " << _nodes[index] << "\n";
    ts2 << "> node count = " << way._nodes.size() << "\n";
    for (int index = 0; index < way._nodes.size(); ++index)
      ts2 << "> " << way._nodes[index] << "\n";
    success = false;
  }
  else
  {
    //  Compare node IDs one by one
    for (int index = 0; index < _nodes.size(); ++index)
    {
      //  Node IDs
      if (_nodes[index] != way._nodes[index] && _nodes[index] != way._idMap->getId(ElementType::Node, way._nodes[index]))
      {
        ts1 << "< nodes = ";
        for (QVector<long>::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
          ts1 << *it << " ";
        ts1 << "\n";
        ts2 << "> nodes = ";
        for (QVector<long>::const_iterator it = way._nodes.begin(); it != way._nodes.end(); ++it)
          ts2 << *it << " ";
        ts2 << "\n";
        success = false;
        break;
      }
    }
  }
  //  Create the diff when the two nodes are different
  if (!success)
  {
    ts1 << "--------------------\n" << ts2.readAll();
    diffOutput = ts1.readAll();
  }
  //  Return success variable
  return success;
}

ChangesetRelation::ChangesetRelation(const XmlObject& relation, ElementIdToIdMap* idMap)
  : ChangesetElement(relation, idMap)
{
  //  Override the type
  _type = ElementType::Relation;
}

ChangesetRelation::ChangesetRelation(const ChangesetRelation &relation)
  : ChangesetElement(relation),
    _members(relation._members)
{
}

bool ChangesetRelation::hasMember(ElementType::Type type, long id)
{
  //  Iterate all of the members
  for (QList<ChangesetRelationMember>::iterator it = _members.begin(); it != _members.end(); ++it)
  {
    switch (type)
    {
    case ElementType::Node:
      //  Node matches node, and ID matches ID
      if (it->isNode() && it->getRef() == id)
        return true;
      break;
    case ElementType::Way:
      //  Way matches way, and ID matches ID
      if (it->isWay() && it->getRef() == id)
        return true;
      break;
    case ElementType::Relation:
      //  Relation matches relation, and ID matches ID
      if (it->isRelation() && it->getRef() == id)
        return true;
      break;
    default:
      break;
    }
  }
  return false;
}

QString ChangesetRelation::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<relation " << ChangesetElement::toString(_object, changesetId) << ">\n";
  for (QList<ChangesetRelationMember>::const_iterator it = _members.begin(); it != _members.end(); ++it)
    ts << it->toString();
  if (_tags.size() > 0)
  {
    for (ElementTags::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(*it);
  }
  ts << "\t\t</relation>\n";
  return ts.readAll();
}

bool ChangesetRelation::diff(const ChangesetRelation& relation, QString& diffOutput) const
{
  bool success = true;
  QString buffer1;
  QTextStream ts1(&buffer1);
  ts1.setCodec("UTF-8");
  QString buffer2;
  QTextStream ts2(&buffer2);
  ts2.setCodec("UTF-8");
  //  Compare the common element data
  if (!diffElement(&relation, ts1, ts2))
    success = false;
  //  Compare the members
  if (_members.size() != relation._members.size())
  {
    ts1 << "< member count = " << _members.size() << "\n";
    ts2 << "> member count = " << relation._members.size() << "\n";
    success = false;
  }
  else
  {
    //  Compare members one by one
    for (int index = 0; index < _members.size(); ++index)
    {
      if (!_members[index].diff(relation._members[index], ts1, ts2))
        success = false;
    }
  }
  //  Create the diff when the two nodes are different
  if (!success)
  {
    ts1 << "--------------------\n" << ts2.readAll();
    diffOutput = ts1.readAll();
  }
  //  Return success variable
  return success;
}

ChangesetRelationMember::ChangesetRelationMember(const QXmlStreamAttributes& member, ElementIdToIdMap* idMap)
  : _type(ElementType::fromString(member.value("type").toString().toLower())),
    _ref(member.value("ref").toString().toLong()),
    _role(member.value("role").toString()),
    _idMap(idMap)
{
}

QString ChangesetRelationMember::toString() const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t\t<member type=\"" << ElementType(_type).toString().toLower() << "\" ref=\""
     << (_idMap ? _idMap->getId(_type, _ref) : _ref)
     << "\" role=\"" << _role << "\"/>\n";
  return ts.readAll();
}

bool ChangesetRelationMember::diff(const ChangesetRelationMember& member, QTextStream& ts1, QTextStream& ts2) const
{
  bool success = true;
  //  Compare the ref IDs
  if (_ref != member._ref && _ref != member._idMap->getId(member._type, member._ref))
  {
    ts1 << "< ref = " << _ref << "\n";
    ts2 << "> ref = " << member._ref << "\n";
    success = false;
  }
  //  Compare the member type
  if (_type != member._type)
  {
    ts1 << "< type = " << _type << "\n";
    ts2 << "> type = " << member._type << "\n";
    success = false;
  }
  //  Compare the member role
  if (_role != member._role)
  {
    ts1 << "< role = " << _role << "\n";
    ts2 << "> role = " << member._role << "\n";
    success = false;
  }
  return success;
}

}
