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

#include "OsmApiChangesetElement.h"

//  Qt
#include <QTextStream>

namespace hoot
{

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
  //  Make sure that the object is in fact a tag before adding it
  if (tag.first == "tag")
    _tags.push_back(tag);
}

QString XmlElement::toString(const QXmlStreamAttributes& attributes, long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  //  Setting the codec to UTF-8 is the special sauce for foreign characters
  ts.setCodec("UTF-8");
  for (int i = 0; i < attributes.size(); ++i)
  {
    const QXmlStreamAttribute& attribute = attributes.at(i);
    QStringRef name = attribute.name();
    ts << name.toString() << "=\"";
    //  Some attributes need to be overridden
    if (name == "changeset")    //  Changeset ID is provided by the OSM API
      ts << changesetId;
    else if (name == "id")      //  Element ID could be different than provided in the file, could be from the API
      ts << (_idMap ? _idMap->getNewId(_type, _id) : _id);
    else if (name == "version") //  Versions are updated by the API
      ts << _version;
    else
      ts << attribute.value().toString();
    ts << "\" ";
  }
  //  Add the changeset attribute if the original data didn't include it
  if (!attributes.hasAttribute("changeset"))
    ts << "changeset=\"" << changesetId << "\"";
  return ts.readAll();
}

QString& XmlElement::escapeString(QString& value) const
{
  //  Simple XML encoding of some problematic characters
  return value.replace("&", "&amp;")
              .replace("\"", "&quot;")
              .replace("'", "&apos;")
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
  //  Make sure to XML encode the value
  ts << "\t\t\t<tag k=\"" << attributes.value("k").toString() << "\" v=\"" << escapeString(value) << "\"/>\n";
  return ts.readAll();
}

XmlNode::XmlNode(const XmlObject& node, ElementIdToIdMap* idMap)
  : XmlElement(node, idMap)
{
  //  Override the type
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
  //  Override the type
  _type = ElementType::Way;
}

QString XmlWay::toString(long changesetId) const
{
  QString buffer;
  QTextStream ts(&buffer);
  ts.setCodec("UTF-8");
  ts << "\t\t<way " << XmlElement::toString(_object.second, changesetId) << ">\n";
  for (QVector<long>::const_iterator it = _nodes.begin(); it != _nodes.end(); ++it)
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
  //  Override the type
  _type = ElementType::Relation;
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
