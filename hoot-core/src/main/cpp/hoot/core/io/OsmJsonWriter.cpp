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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */
#include "OsmJsonWriter.h"

// Hoot
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

namespace pt = boost::property_tree;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmJsonWriter)

OsmJsonWriter::OsmJsonWriter(int precision)
  : _precision(precision),
    _writeHootFormat(ConfigOptions().getJsonFormatHootenanny()),
    _numWritten(0),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10),
    _writeSplitFiles(false),
    _sortTags(ConfigOptions().getWriterSortTagsByKey()),
    _includeIds(ConfigOptions().getWriterIncludeIdTag()),
    _includeCompatibilityTags(true),
    _pretty(ConfigOptions().getJsonPrettyPrint()),
    _writeEmptyTags(ConfigOptions().getJsonPerserveEmptyTags())
{
}

void OsmJsonWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  _precision = options.getWriterPrecision();
  _sortTags = options.getWriterSortTagsByKey();
  _includeDebug = options.getWriterIncludeDebugTags();
  _includeIds= options.getWriterIncludeIdTag();
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
  if ((s.startsWith("{") && s.endsWith("}")) || (s.startsWith("[") && s.endsWith("]")) || s == "null")
    return s;
  else
  {
    if (s.startsWith("\"") && s.endsWith("\"") && s.count("\"") == 2)
      s = s.mid(1, s.length() - 2);
    s.replace('\\', "\\\\");
    s.replace('\"', "\\\"");
    return "\"" % s % "\"";
  }
}

void OsmJsonWriter::open(const QString& url)
{
  //  JSON files are always single files
  _writer.setWriterType(MultiFileWriter::MultiFileWriterType::SingleFile);
  //  Open the object
  _writer.open(url);
}

QString OsmJsonWriter::toString(const ConstOsmMapPtr& map)
{
  _writer.setWriterType(MultiFileWriter::MultiFileWriterType::SingleBuffer);
  _writer.open();
  write(map);
  return _writer.getBuffer();
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
  if (!_writer.isOpen())
    throw HootException("Please open the file before attempting to write.");

  //  Write the header
  initializePartial();
  //  Write all elements
  _writeNodes();
  _writeWays();
  _writeRelations();
  //  Write the footer
  finalizePartial();

  close();
}

void OsmJsonWriter::initializePartial()
{
  //  Write the following to the header of the file
  _writer.setHeaderSection();
  _write("{");
  _write("\"version\": 0.6,");
  _write("\"generator\": \"Hootenanny\",");
  _write("\"elements\": [", true);
  _firstElement = true;
  //  Write the following to the body of the file
  _writer.setBodySection();
}

void OsmJsonWriter::finalizePartial()
{
  //  Lastly write the footer of the file
  _writer.setFooterSection();
  _writeLn("]");
  _writeLn("}");

  close();
}

void OsmJsonWriter::writePartial(const ConstNodePtr& n)
{
  if (_writer.isCurrentIndexWritten())
    _write(",", true);

  _write("{");
  _writeKvp("type", "node");
  _write(",");
  _writeKvp("id", n->getId());
  _write(",");

  _writeMetadata(*n);

  _writeKvp("lat", n->getY());
  _write(",");
  _writeKvp("lon", n->getX());
  if (_hasTags(n))
    _write(",");
  _writeTags(n);
  _write("}", false);
}

void OsmJsonWriter::writePartial(const ConstWayPtr& w)
{
  if (_writer.isCurrentIndexWritten())
    _write(",", true);

  _write("{");
  _writeKvp("type", "way");
  _write(",");
  _writeKvp("id", w->getId());
  _write(",");

  _writeMetadata(*w);

  _write("\"nodes\":[");
  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    _write(QString::number(w->getNodeId(static_cast<int>(j))), false);
    if (j != w->getNodeCount() - 1)
      _write(",");
  }
  _write("]");
  if (_hasTags(w))
    _write(",");
  _writeTags(w);
  _write("}", false);
}

void OsmJsonWriter::writePartial(const ConstRelationPtr& r)
{
  if (_writer.isCurrentIndexWritten())
    _write(",", true);

  _write("{");
  _writeKvp("type", "relation");
  _write(",");
  _writeKvp("id", r->getId());
  _write(",");

  _writeMetadata(*r);

  const vector<RelationData::Entry>& members = r->getMembers();
  _write("\"members\":[");
  for (size_t j = 0; j < members.size(); j++)
  {
    const RelationData::Entry& e = members[j];
    if (j != 0)
      _write(",", true);
    else
      _write("", true);

    _write("{");
    _writeKvp("type", _typeName(e.getElementId().getType()));
    _write(",");
    _writeKvp("ref", e.getElementId().getId());
    _write(",");
    _writeKvp("role", e.getRole());
    _write("}");
  }
  _write("]");
  if (_hasTags(r))
    _write(",");
  _writeTags(r);
  _write("}", false);
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
    _writeKvp("timestamp", DateTimeUtils::toTimeString(element.getTimestamp()));
    _write(",");
    long version = element.getVersion();
    if (version == ElementData::VERSION_EMPTY)
      version = 1;
    _writeKvp("version", version);
    _write(",");
  }
  else
  {
    if (element.getTimestamp() != ElementData::TIMESTAMP_EMPTY)
    {
      _writeKvp("timestamp", DateTimeUtils::toTimeString(element.getTimestamp()));
      _write(",");
    }
    if (element.getVersion() != ElementData::VERSION_EMPTY)
    {
      _writeKvp("version", element.getVersion());
      _write(",");
    }
  }
  //  Negative IDs are considered "new" elements and shouldn't have a changeset
  if (element.getChangeset() != ElementData::CHANGESET_EMPTY && element.getId() > 0)
  {
    _writeKvp("changeset", element.getChangeset());
    _write(",");
  }
  if (element.getUser() != ElementData::USER_EMPTY)
  {
    _writeKvp("user", element.getUser());
    _write(",");
  }
  if (element.getUid() != ElementData::UID_EMPTY)
  {
    _writeKvp("uid", element.getUid());
    _write(",");
  }
}

void OsmJsonWriter::_writeNodes()
{
  vector<long> nids;
  const NodeMap& nodes = _map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
    nids.push_back(it->first);
  // sort the values to give consistent results.
  sort(nids.begin(), nids.end(), less<long>());
  for (auto node_id : nids)
  {
    ConstNodePtr n = _map->getNode(node_id);
    writePartial(n);
    _numWritten++;
    if (_numWritten % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

bool OsmJsonWriter::_hasTags(const ConstElementPtr& e) const
{
  return !e->getTags().empty() ||
          e->getElementType() != ElementType::Node ||
         (e->getCircularError() >= 0 && e->getTags().getInformationCount() > 0) ||
         _includeDebug || _includeIds;
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
  ElementPtr eClone = e->clone();
  _addExportTagsVisitor.visit(eClone);

  bool firstTag = true;
  const Tags& tags = eClone->getTags();
  //  Sort the keys for output
  QList<QString> keys = tags.keys();
  if (_sortTags)
    keys.sort();

  //  Write out the tags with their key/value pairs
  for (const auto& key : qAsConst(keys))
  {
    QString value = tags.get(key).trimmed();
    if (!value.isEmpty())
    {
      if (key == "uuid")
        value = value.replace("{", "").replace("}", "");
      else if (value.contains("{") || value.contains("["))
        value = _validateJsonString(value);
      _writeTag(key, value, firstTag);
    }
  }

  if (firstTag == false)
    _write("}");
}

void OsmJsonWriter::_writeWays()
{
  vector<long> wids;
  const WayMap& ways = _map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
    wids.push_back(it->first);

  // sort the values to give consistent results.
  sort(wids.begin(), wids.end(), less<long>());

  for (auto way_id : wids)
  {
    ConstWayPtr w = _map->getWay(way_id);
    writePartial(w);
    _numWritten++;
    if (_numWritten % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmJsonWriter::_writeRelations()
{
  vector<long> rids;
  const RelationMap& relations = _map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
    rids.push_back(it->first);

  // sort the values to give consistent results.
  sort(rids.begin(), rids.end(), less<long>());

  for (auto relation_id : rids)
  {
    ConstRelationPtr r = _map->getRelation(relation_id);
    writePartial(r);
    _numWritten++;
    if (_numWritten % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

QString OsmJsonWriter::_validateJsonString(const QString &value) const
{
  bool isJson = false;

  boost::property_tree::ptree t;
  // Convert string to stringstream
  stringstream ss(value.toUtf8().constData(), ios::in);

  if (ss.good())
  {
    try
    {
      //  Attempt to read and parse the JSON
      pt::read_json(ss, t);
      isJson = true;
    }
    catch (const std::exception&)
    {
      isJson = false;
    }
  }
  //  JSON data can be returned, otherwise it is book-ended with quotes
  if (isJson)
    return value;
  else
    return QString("\"%1\"").arg(value);
}

}
