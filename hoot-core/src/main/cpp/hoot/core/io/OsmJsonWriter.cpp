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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmJsonWriter.h"

// Hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmJsonWriter)

OsmJsonWriter::OsmJsonWriter(int precision)
  : _includeDebug(ConfigOptions().getWriterIncludeDebugTags()),
    _includeCompatibilityTags(true),
    _precision(precision),
    _out(0),
    _pretty(ConfigOptions().getJsonPrettyPrint()),
    _writeEmptyTags(ConfigOptions().getJsonPerserveEmptyTags()),
    _writeHootFormat(ConfigOptions().getJsonFormatHootenanny()),
    _numWritten(0),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10)
{
}

void OsmJsonWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _precision = options.getWriterPrecision();
  _includeDebug = options.getWriterIncludeDebugTags();
  _writeHootFormat = options.getJsonFormatHootenanny();
  _pretty = options.getJsonPrettyPrint();
  _writeEmptyTags = options.getJsonPerserveEmptyTags();
  _statusUpdateInterval = options.getTaskStatusUpdateInterval() * 10;
}

QString OsmJsonWriter::markupString(const QString& str)
{
  QString s = str;
  s.replace('\n', "\\n");
  s.replace('\t', "\\t");
  s.replace('\r', "\\r");
  //  Don't add quotes around JSON values
  if (s.startsWith("{") || s.startsWith("[") || s == "null")
    return s;
  else
  {
    s.replace('\\', "\\\\");
    s.replace('\"', "\\\"");
    return "\"" % s % "\"";
  }
}

void OsmJsonWriter::open(const QString& url)
{
  _fp.setFileName(url);
  if (!_fp.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw Exception(QObject::tr("Error opening %1 for writing").arg(url));
  }
  _out = &_fp;
}

QString OsmJsonWriter::toString(const ConstOsmMapPtr& map)
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

void OsmJsonWriter::write(const ConstOsmMapPtr& map, const QString& path)
{
  open(path);
  write(map);
}

void OsmJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;
  if (_out->isWritable() == false)
  {
    throw HootException("Please open the file before attempting to write.");
  }

  _write("{");
  _write("\"version\": 0.6,");
  _write("\"generator\": \"Hootenanny\",");
  _write("\"elements\": [", true);
  _firstElement = true;
  _writeNodes();
  _writeWays();
  _writeRelations();
  _writeLn("]");
  _writeLn("}");

  close();
}

void OsmJsonWriter::_writeKvp(const QString& key, const QString& value)
{
  _write(markupString(key) % ":" % markupString(value), false);
}

void OsmJsonWriter::_writeKvp(const QString& key, long value)
{
  _write(markupString(key) % ":" % QString::number(value), false);
}

void OsmJsonWriter::_writeKvp(const QString& key, double value)
{
  _write(markupString(key) % ":" % QString::number(value, 'g', _precision), false);
}

void OsmJsonWriter::_writeMetadata(const Element& element)
{
  if (_includeCompatibilityTags)
  {
    _writeKvp("timestamp", DateTimeUtils::toTimeString(element.getTimestamp())); _write(",");
    long version = element.getVersion();
    if (version == ElementData::VERSION_EMPTY)
    {
      version = 1;
    }
    _writeKvp("version", version); _write(",");
  }
  else
  {
    if (element.getTimestamp() != ElementData::TIMESTAMP_EMPTY)
    {
      _writeKvp("timestamp", DateTimeUtils::toTimeString(element.getTimestamp())); _write(",");
    }
    if (element.getVersion() != ElementData::VERSION_EMPTY)
    {
      _writeKvp("version", element.getVersion()); _write(",");
    }
  }
  if (element.getChangeset() != ElementData::CHANGESET_EMPTY &&
      //  Negative IDs are considered "new" elements and shouldn't have a changeset
      element.getId() > 0)
  {
    _writeKvp("changeset", element.getChangeset()); _write(",");
  }
  if (element.getUser() != ElementData::USER_EMPTY)
  {
    _writeKvp("user", element.getUser()); _write(",");
  }
  if (element.getUid() != ElementData::UID_EMPTY)
  {
    _writeKvp("uid", element.getUid()); _write(",");
  }
}

void OsmJsonWriter::_writeNodes()
{
  const long debugId = 0;

  QList<long> nids;
  const NodeMap& nodes = _map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    nids.append(it->first);
  }
  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qLess<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    ConstNodePtr n = _map->getNode(nids[i]);
    const QString msg = "Writing node: " + n->toString() + "...";
    if (n->getId() == debugId)
    {
      LOG_VARD(msg);
    }
    else
    {
      LOG_VART(msg);
    }

    if (!_firstElement) _write(",", true);
    _firstElement = false;

    _write("{");
    _writeKvp("type", "node"); _write(",");
    _writeKvp("id", n->getId()); _write(",");

    _writeMetadata(*n);

    _writeKvp("lat", n->getY()); _write(",");
    _writeKvp("lon", n->getX());
    if (_hasTags(n)) _write(",");
    _writeTags(n);
    _write("}", false);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmJsonWriter::_write(const QString& str, bool newLine)
{
  _out->write(str.toUtf8());
  if (newLine)
    _out->write(QString("\n").toUtf8());
}

bool OsmJsonWriter::_hasTags(const ConstElementPtr& e)
{
  return !e->getTags().empty() ||
         e->getElementType() != ElementType::Node ||
        (e->getCircularError() >= 0 && e->getTags().getInformationCount() > 0) ||
         _includeDebug;
}

void OsmJsonWriter::_writeTag(const QString& key, const QString& value, bool& firstTag)
{
  if (key.isEmpty() == false && (value.isEmpty() == false || _writeEmptyTags))
  {
    if (firstTag)
    {
      if (_writeHootFormat)
        _write("\"tags\":{");
      else
        _write("\"properties\":{");
      firstTag = false;
    }
    else
      _write(",");
    _writeKvp(key, value);
  }
}

void OsmJsonWriter::_writeTags(const ConstElementPtr& e)
{
  ElementPtr eClone(e->clone());
  _addExportTagsVisitor.visit(eClone);

  bool firstTag = true;
  const Tags& tags = eClone->getTags();
  if (!tags.empty())
  {
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
    {
      QString key = it.key();
      QString value = it.value();
      if (key == "uuid")
        value = value.replace("{", "").replace("}", "");
      _writeTag(key, value, firstTag);
    }
  }

  if (firstTag == false)
  {
    _write("}");
  }
}

void OsmJsonWriter::_writeWays()
{
  const long debugId = 0;

  QList<long> wids;
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    wids.append(it->first);
  }
  // sort the values to give consistent results.
  qSort(wids.begin(), wids.end(), qLess<long>());

  for (int i = 0; i < wids.size(); i++)
  {
    ConstWayPtr w = _map->getWay(wids[i]);
    const QString msg = "Writing way: " + w->toString() + "...";

    if (w->getId() == debugId)
    {
      LOG_VARD(msg);
    }
    else
    {
      LOG_VART(msg);
    }

    if (!_firstElement) _write(",", true);
    _firstElement = false;

    _write("{");
    _writeKvp("type", "way"); _write(",");
    _writeKvp("id", w->getId()); _write(",");

    _writeMetadata(*w);

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
    if (_hasTags(w)) _write(",");
    _writeTags(w);
    _write("}", false);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmJsonWriter::_writeRelations()
{
  const long debugId = 0;

  QList<long> rids;
  const RelationMap& relations = _map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    rids.append(it->first);
  }

  // sort the values to give consistent results.
  qSort(rids.begin(), rids.end(), qLess<long>());

  for (int i = 0; i < rids.size(); i++)
  {
    ConstRelationPtr r = _map->getRelation(rids[i]);
    const QString msg = "Writing relation: " + r->toString() + "...";
    if (r->getId() == debugId)
    {
      LOG_VARD(msg);
    }
    else
    {
      LOG_VART(msg);
    }

    if (!_firstElement) _write(",", true);
    _firstElement = false;

    _write("{");
    _writeKvp("type", "relation"); _write(",");
    _writeKvp("id", r->getId()); _write(",");

    _writeMetadata(*r);

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
    if (_hasTags(r)) _write(",");
    _writeTags(r);
    _write("}", false);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

}
