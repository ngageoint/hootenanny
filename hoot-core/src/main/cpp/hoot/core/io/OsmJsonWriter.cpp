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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmJsonWriter.h"

// Boost
using namespace boost;

// Hoot
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Tags.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

namespace hoot {

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmJsonWriter)

OsmJsonWriter::OsmJsonWriter(int precision)
{
  _includeDebug = ConfigOptions().getWriterIncludeDebug();
  _precision = precision;
  _out = 0;
  _pretty = false;
}

QString OsmJsonWriter::_markupString(const QString& str)
{
  QString s = str;
  s.replace('\\', "\\\\");
  s.replace('\"', "\\\"");
  s.replace('\n', "\\\n");
  s.replace('\t', "\\\t");
  s.replace('\r', "\\\r");
  return "\"" % s % "\"";
}

void OsmJsonWriter::open(QString url)
{
  _fp.setFileName(url);
  if (!_fp.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw Exception(QObject::tr("Error opening %1 for writing").arg(url));
  }
  _out = &_fp;
}

QString OsmJsonWriter::toString(boost::shared_ptr<const OsmMap> map)
{
  QBuffer b;
  b.open(QBuffer::WriteOnly);
  _out = &b;
  write(map);
  _out = 0;
  return QString::fromUtf8(b.buffer());
}

QString OsmJsonWriter::_typeName(ElementType e)
{
  switch(e.getEnum())
  {
  case ElementType::Node:
    return "node";
  case ElementType::Way:
    return "way";
  case ElementType::Relation:
    return "relation";
  default:
    throw HootException("Unexpected element type.");
  }
}

void OsmJsonWriter::write(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  open(path);
  write(map);
}

void OsmJsonWriter::write(boost::shared_ptr<const OsmMap> map)
{
  if (_out->isWritable() == false)
  {
    throw HootException("Please open the file before attempting to write.");
  }

  _write("{");
  _write("\"version\": 0.6,");
  _write("\"generator\": \"Hootenanny\",");
  _write("\"elements\": [", true);
  _firstElement = true;
  _writeNodes(map);
  _writeWays(map);
  _writeRelations(map);
  _writeLn("]");
  _writeLn("}");

  _fp.close();
}

void OsmJsonWriter::_writeKvp(const QString& key, const QString& value)
{
  _write(_markupString(key) % ":" % _markupString(value), false);
}

void OsmJsonWriter::_writeKvp(const QString& key, long value)
{
  _write(_markupString(key) % ":" % QString::number(value), false);
}

void OsmJsonWriter::_writeKvp(const QString& key, double value)
{
  _write(_markupString(key) % ":" % QString::number(value, 'g', _precision), false);
}

void OsmJsonWriter::_writeNodes(boost::shared_ptr<const OsmMap> map)
{
  QList<long> nids;
  NodeMap::const_iterator it = map->getNodes().begin();
  while (it != map->getNodes().end()) {
    nids.append(it->first);
    it++;
  }
  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qGreater<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    ConstNodePtr n = map->getNode(nids[i]);
    if (!_firstElement) _write(",", true);
    _firstElement = false;
    _write("{");
    _writeKvp("type", "node"); _write(",");
    _writeKvp("id", n->getId()); _write(",");
    _writeKvp("lat", n->getY()); _write(",");
    _writeKvp("lon", n->getX());
    _writeTags(n);
    _write("}", false);
  }
}

void OsmJsonWriter::_write(const QString& str, bool newLine)
{
  if (newLine)
  {
    _out->write((str + "\n").toUtf8());
  }
  else
  {
    _out->write(str.toUtf8());
  }
}

void OsmJsonWriter::_writeTag(const QString& key, const QString& value, bool& firstTag)
{
  if (key.isEmpty() == false && value.isEmpty() == false)
  {
    _write(",");
    if (firstTag)
    {
      _write("\"tags\":{");
      firstTag = false;
    }
    _writeKvp(key, value);
  }
}

void OsmJsonWriter::_writeTags(ConstElementPtr e)
{
  bool firstTag = true;
  const Tags& tags = e->getTags();
  if (tags.size() > 0)
  {
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
    {
      _writeTag(it.key(), it.value(), firstTag);
    }
  }

  // turn this on when we start using node circularError.
  if (e->getElementType() != ElementType::Node ||
      (e->getCircularError() >= 0 && e->getTags().getInformationCount() > 0))
  {
    _writeTag(MetadataTags::ErrorCircular(), QString::number(e->getCircularError(), 'g', _precision), firstTag);
  }

  if (_includeDebug)
  {
    _writeTag(MetadataTags::HootId(), QString::number(e->getId()), firstTag);
    _writeTag(MetadataTags::HootStatus(), QString::number((int)e->getStatus().getEnum()), firstTag);
  }

  if (firstTag == false)
  {
    _write("}");
  }
}

void OsmJsonWriter::_writeWays(boost::shared_ptr<const OsmMap> map)
{
  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end())
  {
    if (!_firstElement) _write(",", true);
    _firstElement = false;
    ConstWayPtr w = it->second;
    _write("{");
    _writeKvp("type", "way"); _write(",");
    _writeKvp("id", w->getId()); _write(",");

    _write("\"nodes\":[");
    for (size_t j = 0; j < w->getNodeCount(); j++)
    {
      _write(QString::number(w->getNodeId(j)), false);
      if (j != w->getNodeCount() - 1)
      {
        _write(",");
      }
    }
    _write("]");
    _writeTags(w);

    ++it;
  }
}

void OsmJsonWriter::_writeRelations(boost::shared_ptr<const OsmMap> map)
{
  RelationMap::const_iterator it = map->getRelations().begin();
  while (it != map->getRelations().end())
  {
    if (!_firstElement) _write(",", true);
    _firstElement = false;
    ConstRelationPtr r = it->second;

    _write("{");
    _writeKvp("type", "relation"); _write(",");
    _writeKvp("id", r->getId()); _write(",");

    const vector<RelationData::Entry>& members = r->getMembers();
    _write("\"members\":[");
    for (size_t j = 0; j < members.size(); j++)
    {
      const RelationData::Entry& e = members[j];
      if (j != 0)
      {
        _write(",", true);
      }
      else
      {
        _write("", true);
      }
      _write("{");
      _writeKvp("type", _typeName(e.getElementId().getType())); _write(",");
      _writeKvp("ref", e.getElementId().getId()); _write(",");
      _writeKvp("role", e.getRole());
      _write("}");
    }
    _write("]");

    _writeTags(r);

    ++it;
  }
}

}
