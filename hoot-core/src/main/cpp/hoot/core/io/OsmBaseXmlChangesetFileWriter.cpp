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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmBaseXmlChangesetFileWriter.h"

// hoot
#include <hoot/core/conflate/ConflateUtils.h>
#include <hoot/core/criterion/InBoundsCriterion.h>
#include <hoot/core/elements/RelationMemberUtils.h>
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/ConfigUtils.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QXmlStreamWriter>

using namespace std;

namespace hoot
{

//  DON'T REGISTER THIS CLASS WITH THE FACTORY, REGISTER DERIVED CLASSES
//HOOT_FACTORY_REGISTER()

OsmBaseXmlChangesetFileWriter::OsmBaseXmlChangesetFileWriter()
  : OsmChangesetFileWriter(),
    _precision(ConfigOptions().getWriterPrecision()),
    _addTimestamp(ConfigOptions().getChangesetXmlWriterAddTimestamp()),
    _sortTags(ConfigOptions().getWriterSortTagsByKey())
{
}

void OsmBaseXmlChangesetFileWriter::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _precision = co.getWriterPrecision();
  _addTimestamp = co.getChangesetXmlWriterAddTimestamp();
  _includeDebugTags = co.getWriterIncludeDebugTags();
  _includeCircularErrorTags = co.getWriterIncludeCircularErrorTags();
  _changesetIgnoreBounds = co.getChangesetIgnoreBounds();
  _sortTags = co.getWriterSortTagsByKey();

}

void OsmBaseXmlChangesetFileWriter::_initStats()
{
  _stats.clear();
  _stats.resize(static_cast<size_t>(Change::ChangeType::Unknown),
                static_cast<size_t>(ElementType::Unknown));
  vector<QString> rows({"Create", "Modify", "Delete"});
  vector<QString> columns({"Node", "Way", "Relation"});
  _stats.setLabels(rows, columns);
}

void OsmBaseXmlChangesetFileWriter::_initIdCounters()
{
  _newElementIdCtrs.clear();
  _newElementIdCtrs[ElementType::Node] = 1;
  _newElementIdCtrs[ElementType::Way] = 1;
  _newElementIdCtrs[ElementType::Relation] = 1;

  _newElementIdMappings.clear();
  _newElementIdMappings[ElementType::Node] = QMap<long, long>();
  _newElementIdMappings[ElementType::Way] = QMap<long, long>();
  _newElementIdMappings[ElementType::Relation] = QMap<long, long>();
}

void OsmBaseXmlChangesetFileWriter::write(const QString& path, const ChangesetProviderPtr& changesetProvider)
{
  QList<ChangesetProviderPtr> changesetProviders;
  changesetProviders.append(changesetProvider);
  write(path, changesetProviders);
}

void OsmBaseXmlChangesetFileWriter::write(const QString& path, const QList<ChangesetProviderPtr>& changesetProviders)
{  
  LOG_DEBUG("Writing changeset to: " << path << "...");

  LOG_VARD(_map1List.size());
  LOG_VARD(_map2List.size());
  assert(_map1List.size() == _map2List.size());

  QString filepath = path;

  _initIdCounters();
  _initStats();
  _parsedChangeIds.clear();

  long changesetProgress = 1;

  QFile f;
  f.setFileName(filepath);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    throw HootException(QString("Error opening %1 for writing").arg(path));

  QXmlStreamWriter writer(&f);
  writer.setCodec("UTF-8");
  writer.setAutoFormatting(true);

  writer.writeStartDocument();

  //  Write out the header
  _writeXmlFileHeader(writer);

  Change::ChangeType last = Change::ChangeType::Unknown;

  for (int i = 0; i < changesetProviders.size(); i++)
  {
    LOG_DEBUG("Deriving changes with changeset provider: " << i + 1 << " of " << changesetProviders.size() << "...");

    // Bounds checking requires a map. Grab the two input maps if they were passed in...one for
    // each dataset, before changes and after.
    ConstOsmMapPtr map1;
    ConstOsmMapPtr map2;
    if (!_map1List.empty())
      map1 = _map1List.at(i);

    if (!_map2List.empty())
      map2 = _map2List.at(i);

    ChangesetProviderPtr changesetProvider = changesetProviders.at(i);
    LOG_VARD(changesetProvider->hasMoreChanges());
    ConstElementPtr changeElement;
    while (changesetProvider->hasMoreChanges())
    {
      LOG_VART(changesetProvider->hasMoreChanges());
      LOG_TRACE("Reading next XML change...");
      _change = changesetProvider->readNextChange();
      LOG_VART(_change);
      changeElement = _change.getElement();

      if (!changeElement)
        continue;

      // When multiple changeset providers are passed in, sometimes multiple changes for the same
      // element may exist as a result of combining their results together, so we're skipping those
      // dupes here. Formerly, we only checked for exact duplicate statements and now we're more
      // lenient not allowing multiple changes for the same element. Keeping the first change and
      // throwing out subsequent dupes is a bit arbitrary, but will go with it for now, as it seems
      // to work well. Other options could be to favor certain change types over others. This check
      // must be here and not in ChangesetDeriver, as the problem has only been seen when combining
      // multiple derivers. You could make the argument to prevent these types of dupes from
      // occurring before outputting the changeset file with this writer, but not quite sure how to
      // do that yet, due to the fact that the changeset providers have a streaming interface.
      if (_parsedChangeIds.contains(changeElement->getElementId()))
      {
        LOG_TRACE("Skipping change for element ID already having change: " << _change << "...");
        continue;
      }

      // If a bounds was specified for calculating the changeset, honor it.
      if (!_changesetIgnoreBounds && ConfigUtils::boundsOptionEnabled() && _failsBoundsCheck(changeElement, map1, map2, _change.getType()))
        continue;

      if (_change.getType() != last && last != Change::ChangeType::Unknown)
          _parsedChangeIds.append(changeElement->getElementId());

      _writeXmlFileSectionHeader(writer, last);

      //  Update the last type to now be the current type
      if (_change.getType() != last)
        last = _change.getType();
      if (_change.getType() != Change::ChangeType::Unknown)
      {
        ElementType::Type type = changeElement->getElementType().getEnum();
        switch (type)
        {
        case ElementType::Node:
          _writeNode(writer, changeElement, _change.getPreviousElement());
          break;
        case ElementType::Way:
          _writeWay(writer, changeElement, _change.getPreviousElement());
          break;
        case ElementType::Relation:
          _writeRelation(writer, changeElement, _change.getPreviousElement());
          break;
        default:
          throw IllegalArgumentException("Unexpected element type.");
        }
        changesetProgress++;
        //  Update the stats
        if (last != Change::ChangeType::NoChange)
          _stats(static_cast<size_t>(last), type)++;
      }
    }

    LOG_VART(Change::changeTypeToString(last));
    if (last != Change::ChangeType::Unknown)
    {
      LOG_TRACE("Writing change end element...");
      writer.writeEndElement();
      last = Change::ChangeType::Unknown;
      _parsedChangeIds.append(changeElement->getElementId());
    }
  }

  LOG_TRACE("Writing root end element...");
  writer.writeEndElement();
  writer.writeEndDocument();

  f.close();

  LOG_DEBUG("Changeset written to: " << path << "...");
}

void OsmBaseXmlChangesetFileWriter::_writeNode(QXmlStreamWriter& writer, ConstElementPtr node, ConstElementPtr previous)
{
  ConstNodePtr n = dynamic_pointer_cast<const Node>(node);
  ConstNodePtr pn = dynamic_pointer_cast<const Node>(previous);
  LOG_TRACE("Writing change for " << n << "...");

  writer.writeStartElement("node");
  long id = n->getId();
  if (_change.getType() == Change::ChangeType::Create)
  {
    // rails port expects negative ids for new elements; we're starting at -1 to match the
    // convention of iD editor, but that's not absolutely necessary to write the changeset to rails
    // port
    id = _newElementIdCtrs[ElementType::Node] * -1; // assuming no id's = 0
    LOG_TRACE(
      "Converting new element with id: " << n->getElementId() << " to id: " <<
      ElementId(ElementType::Node, id));
    _newElementIdCtrs[ElementType::Node] = _newElementIdCtrs[ElementType::Node] + 1;
    _newElementIdMappings[ElementType::Node].insert(n->getId(), id);
  }
  writer.writeAttribute("id", QString::number(id));
  long version;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::ChangeType::Create)
    version = 0;
  else if (n->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a "
              "version greater than zero: %1").arg(n->getElementId().toString()));
  }
  else if (pn && pn->getVersion() < n->getVersion())
  {
    //  Previous node contains a version from the API and should be preserved
    version = pn->getVersion();
  }
  else
    version = n->getVersion();
  LOG_VART(version);
  writer.writeAttribute("version", QString::number(version));
  //  Write the action attribute
  _writeXmlActionAttribute(writer);
  //  Write the lat/lon values
  writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
  writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));
  //  Add the timestamp if desired
  if (_addTimestamp)
  {
    if (n->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(n->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }
  //  Copy tags to allow _writeTags to add additional information
  Tags tags = n->getTags();
  _writeTags(writer, tags, n.get());

  writer.writeEndElement();
}

void OsmBaseXmlChangesetFileWriter::_writeWay(QXmlStreamWriter& writer, ConstElementPtr way, ConstElementPtr previous)
{
  ConstWayPtr w = dynamic_pointer_cast<const Way>(way);
  ConstWayPtr pw = dynamic_pointer_cast<const Way>(previous);
  LOG_TRACE("Writing change for " << w << "...");

  writer.writeStartElement("way");
  long id = w->getId();
  if (_change.getType() == Change::ChangeType::Create)
  {
    //see corresponding note in _writeNode
    id = _newElementIdCtrs[ElementType::Way] * -1;
    LOG_TRACE(
      "Converting new element with id: " << w->getElementId() << " to id: " <<
      ElementId(ElementType::Way, id));
    _newElementIdCtrs[ElementType::Way] = _newElementIdCtrs[ElementType::Way] + 1;
    _newElementIdMappings[ElementType::Way].insert(w->getId(), id);
  }
  writer.writeAttribute("id", QString::number(id));
  long version;
  // for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::ChangeType::Create)
    version = 0;
  else if (w->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a "
              "version greater than zero: %1").arg(w->getElementId().toString()));
  }
  else if (pw && pw->getVersion() < w->getVersion())
  {
    //  Previous way contains a version from the API and should be preserved
    version = pw->getVersion();
  }
  else
    version = w->getVersion();
  LOG_VART(version);
  writer.writeAttribute("version", QString::number(version));
  //  Write the action attribute
  _writeXmlActionAttribute(writer);
  //  Add the timestamp if desired
  if (_addTimestamp)
  {
    if (w->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(w->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  for (auto nodeRefId : w->getNodeIds())
  {
    writer.writeStartElement("nd");
    if (_newElementIdMappings[ElementType::Node].contains(nodeRefId))
    {
      const long newNodeRefId = _newElementIdMappings[ElementType::Node][nodeRefId];
      LOG_TRACE(
        "Converting new node ref with id: " << nodeRefId << " to id: " <<
        ElementId(ElementType::Node, newNodeRefId));
      nodeRefId = newNodeRefId;
    }
    writer.writeAttribute("ref", QString::number(nodeRefId));
    writer.writeEndElement();
  }
  //  Copy tags to allow _writeTags to add additional information
  Tags tags = w->getTags();
  _writeTags(writer, tags, w.get());

  writer.writeEndElement();
}

void OsmBaseXmlChangesetFileWriter::_writeRelation(QXmlStreamWriter& writer, ConstElementPtr relation, ConstElementPtr previous)
{
  ConstRelationPtr r = dynamic_pointer_cast<const Relation>(relation);
  ConstRelationPtr pr = dynamic_pointer_cast<const Relation>(previous);
  LOG_TRACE("Writing change for " << r << "...");

  writer.writeStartElement("relation");
  long id = r->getId();
  if (_change.getType() == Change::ChangeType::Create)
  {
    //see corresponding note in _writeNode
    id = _newElementIdCtrs[ElementType::Relation] * -1;
    LOG_TRACE(
      "Converting new element with id: " << r->getElementId() << " to id: " <<
      ElementId(ElementType::Relation, id));
    _newElementIdCtrs[ElementType::Relation] = _newElementIdCtrs[ElementType::Relation] + 1;
    _newElementIdMappings[ElementType::Relation].insert(r->getId(), id);
  }
  writer.writeAttribute("id", QString::number(id));
  long version;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::ChangeType::Create)
    version = 0;
  else if (r->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a version greater than zero: ")
        .arg(r->getElementId().toString()));
  }
  else if (pr && pr->getVersion() < r->getVersion())
  {
    //  Previous relation contains a version from the API and should be preserved
    version = pr->getVersion();
  }
  else
    version = r->getVersion();
  LOG_VART(version);
  writer.writeAttribute("version", QString::number(version));
  //  Write the action attribute
  _writeXmlActionAttribute(writer);
  //  Add the timestamp if desired
  if (_addTimestamp)
  {
    if (r->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(r->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  const vector<RelationData::Entry>& members = r->getMembers();
  for (const auto& e : members)
  {
    writer.writeStartElement("member");
    const ElementType elementType = e.getElementId().getType();
    writer.writeAttribute("type", elementType.toString().toLower());
    long memberId = e.getElementId().getId();
    if (_newElementIdMappings[elementType.getEnum()].contains(memberId))
    {
      const long newMemberId = _newElementIdMappings[elementType.getEnum()][memberId];
      LOG_TRACE(
        "Converting new member with id: " << memberId << " to id: " <<
        ElementId(elementType, newMemberId));
      memberId = newMemberId;
    }
    writer.writeAttribute("ref", QString::number(memberId));
    writer.writeAttribute("role", _invalidCharacterHandler.removeInvalidCharacters(e.getRole()));
    writer.writeEndElement();
  }
  //  Copy tags to allow _writeTags to add additional information
  Tags tags = r->getTags();
  if (r->getType() != "")
    tags.set("type", r->getType());
  _writeTags(writer, tags, r.get());

  writer.writeEndElement();
}

void OsmBaseXmlChangesetFileWriter::_writeTags(QXmlStreamWriter& writer, Tags& tags, const Element* element)
{
  LOG_TRACE("Writing " << tags.size() << " tags for: " << element->getElementId() << "...");
  //  Remove all of the hoot tags
  tags.removeHootTags();
  //  Add back any tags needed
  _getOptionalTags(tags, element);
  //  Sort the keys for output
  QList<QString> keys = tags.keys();
  if (_sortTags)
    keys.sort();

  for (const auto& key : qAsConst(keys))
  {
    QString val = tags.get(key).trimmed();
    if (!key.isEmpty() && !val.isEmpty())
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", _invalidCharacterHandler.removeInvalidCharacters(key));
      writer.writeAttribute("v", _invalidCharacterHandler.removeInvalidCharacters(val));
      writer.writeEndElement();
    }
  }
}

QString OsmBaseXmlChangesetFileWriter::getStatsTable(const ChangesetStatsFormat& format) const
{
  switch (format.getEnum())
  {
  case ChangesetStatsFormat::TextFormat:
    return _stats.toTableString();
  case ChangesetStatsFormat::JsonFormat:
    return _stats.toJsonString();
  default:
    return "";
  }
}

}
