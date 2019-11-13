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

#include "OsmApiChangesetElement.h"

#include <hoot/core/util/HootException.h>
#include <hoot/core/visitors/ApiTagTruncateVisitor.h>

//  Qt
#include <QTextStream>

namespace hoot
{

ChangesetElement::ChangesetElement(const XmlObject& object, ElementIdToIdMap* idMap)
  : _type(ElementType::Unknown),
    _id(object.second.value("id").toString().toLong()),
    _version(object.second.value("version").toString().toLong()),
    _idMap(idMap),
    _status(ChangesetElement::Available)
{
  for (QXmlStreamAttributes::const_iterator it = object.second.begin(); it != object.second.end(); ++it)
    _object.push_back(std::make_pair(it->name().toString(), it->value().toString()));
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
      ts << (_idMap ? _idMap->getNewId(_type, _id) : _id);
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

QString& ChangesetElement::escapeString(QString& value) const
{
  //  Simple XML encoding of some problematic characters
  return value.replace("&", "&amp;")
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
  QString key(tag.first);
  QString value(tag.second);
  //  Tag values of length 255 need to be truncated
  QString newValue(ApiTagTruncateVisitor::truncateTag(key, value));

  //  Make sure to XML encode the value
  ts << "\t\t\t<tag k=\"" << key << "\" v=\"";
  if (newValue != "")
    ts << escapeString(newValue);
  else
    ts << escapeString(value);
  ts << "\"/>\n";
  return ts.readAll();
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
    ts << "\t\t\t<nd ref=\"" << (_idMap ? _idMap->getNewId(ElementType::Node, *it) : *it) << "\"/>\n";
  if (_tags.size() > 0)
  {
    for (ElementTags::const_iterator it = _tags.begin(); it != _tags.end(); ++it)
      ts << toTagString(*it);
  }
  ts << "\t\t</way>\n";
  return ts.readAll();
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
     << (_idMap ? _idMap->getNewId(_type, _ref) : _ref)
     << "\" role=\"" << _role << "\"/>\n";
  return ts.readAll();
}

}
