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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmXmlChangesetFileWriter.h"

// hoot
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>

// Qt
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamWriter>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmChangesetFileWriter, OsmXmlChangesetFileWriter)

OsmXmlChangesetFileWriter::OsmXmlChangesetFileWriter() :
_precision(ConfigOptions().getWriterPrecision()),
_addTimestamp(ConfigOptions().getChangesetXmlWriterAddTimestamp()),
_includeDebugTags(ConfigOptions().getWriterIncludeDebugTags()),
_includeCircularErrorTags(ConfigOptions().getWriterIncludeCircularErrorTags())
{
}

void OsmXmlChangesetFileWriter::_initStats()
{
  _stats.clear();
  _stats.resize(Change::Unknown, ElementType::Unknown);
  vector<QString> rows({"Create", "Modify", "Delete"});
  vector<QString> columns({"Node", "Way", "Relation"});
  _stats.setLabels(rows, columns);
}

void OsmXmlChangesetFileWriter::_initIdCounters()
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

void OsmXmlChangesetFileWriter::write(const QString& path,
                                      const ChangesetProviderPtr& changesetProvider)
{
  QList<ChangesetProviderPtr> changesetProviders;
  changesetProviders.append(changesetProvider);
  write(path, changesetProviders);
}

void OsmXmlChangesetFileWriter::write(const QString& path,
                                      const QList<ChangesetProviderPtr>& changesetProviders)
{  
  LOG_DEBUG("Writing changeset to: " << path << "...");

  QString filepath = path;

  _initIdCounters();
  _initStats();
  _parsedChanges.clear();

  long changesetProgress = 1;

  QFile f;
  f.setFileName(filepath);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(path));
  }

  QXmlStreamWriter writer(&f);
  writer.setCodec("UTF-8");
  writer.setAutoFormatting(true);
  writer.writeStartDocument();

  writer.writeStartElement("osmChange");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", HOOT_PACKAGE_NAME);

  Change::ChangeType last = Change::Unknown;

  for (int i = 0; i < changesetProviders.size(); i++)
  {
    LOG_DEBUG(
      "Deriving changes with changeset provider: " << i + 1 << " / " << changesetProviders.size() <<
      "...");

    ChangesetProviderPtr changesetProvider = changesetProviders.at(i);
    LOG_VARD(changesetProvider->hasMoreChanges());
    while (changesetProvider->hasMoreChanges())
    {
      LOG_VART(changesetProvider->hasMoreChanges());
      LOG_TRACE("Reading next XML change...");
      _change = changesetProvider->readNextChange();
      LOG_VART(_change.toString());

      // When multiple changeset providers are passed in sometimes a duplicated change might exist
      // between them, so we're skipping those dupes here. You could make the argument to prevent
      // dupes from occurring before outputting this changeset file, but not quite sure how to do
      // that yet, due to the fact that the changeset providers have a streaming interface.
      if (_parsedChanges.contains(_change))
      {
        LOG_TRACE("Skipping adding duplicated change: " << _change << "...");
        continue;
      }

      LOG_VART(Change::changeTypeToString(last));
      if (_change.getType() != last)
      {
        if (last != Change::Unknown)
        {
          writer.writeEndElement();
          _parsedChanges.append(_change);
        }
        switch (_change.getType())
        {
          case Change::Create:
            LOG_TRACE("Writing create start element...");
            writer.writeStartElement("create");
            break;
          case Change::Delete:
            LOG_TRACE("Writing delete start element...");
            writer.writeStartElement("delete");
            break;
          case Change::Modify:
            LOG_TRACE("Writing modify start element...");
            writer.writeStartElement("modify");
            break;
          case Change::Unknown:
            //see comment in ChangesetDeriver::_nextChange() when
            //_fromE->getElementId() < _toE->getElementId() as to why we do a no-op here.
            break;
          default:
            throw IllegalArgumentException("Unexpected change type.");
        }
        last = _change.getType();
        LOG_VART(last);
      }

      if (_change.getType() != Change::Unknown)
      {
        ElementType::Type type = _change.getElement()->getElementType().getEnum();
        switch (type)
        {
          case ElementType::Node:
            _writeNode(writer, _change.getElement(), _change.getPreviousElement());
            break;
          case ElementType::Way:
            _writeWay(writer, _change.getElement(), _change.getPreviousElement());
            break;
          case ElementType::Relation:
            _writeRelation(writer, _change.getElement(), _change.getPreviousElement());
            break;
          default:
            throw IllegalArgumentException("Unexpected element type.");
        }
        changesetProgress++;
        //  Update the stats
        _stats(last, type)++;
      }
    }

    LOG_VART(Change::changeTypeToString(last));
    if (last != Change::Unknown)
    {
      LOG_TRACE("Writing change end element...");
      writer.writeEndElement();
      last = Change::Unknown;
      _parsedChanges.append(_change);
    }
  }

  LOG_TRACE("Writing root end element...");
  writer.writeEndElement();
  writer.writeEndDocument();

  f.close();

  LOG_DEBUG("Changeset written to: " << path << "...");
}

void OsmXmlChangesetFileWriter::_writeNode(QXmlStreamWriter& writer, ConstElementPtr node,
                                           ConstElementPtr previous)
{
  ConstNodePtr n = dynamic_pointer_cast<const Node>(node);
  ConstNodePtr pn = dynamic_pointer_cast<const Node>(previous);
  LOG_TRACE("Writing change for " << n << "...");

  writer.writeStartElement("node");
  long id = n->getId();
  if (_change.getType() == Change::Create)
  {
    //rails port expects negative ids for new elements; we're starting at -1 to match the convention
    //of iD editor, but that's not absolutely necessary to write the changeset to rails port
    id = _newElementIdCtrs[ElementType::Node] * -1; //assuming no id's = 0
    LOG_TRACE(
      "Converting new element with id: " << n->getElementId() << " to id: " <<
      ElementId(ElementType::Node, id));
    _newElementIdCtrs[ElementType::Node] = _newElementIdCtrs[ElementType::Node] + 1;
    _newElementIdMappings[ElementType::Node].insert(n->getId(), id);
  }
  writer.writeAttribute("id", QString::number(id));
  long version = ElementData::VERSION_EMPTY;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else if (n->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a ") +
      QString("version greater than zero: ") + n->getElementId().toString());
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

  writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
  writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

  if (_addTimestamp)
  {
    if (n->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(n->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  Tags tags = n->getTags();
  _writeTags(writer, tags, n.get());

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::_writeWay(QXmlStreamWriter& writer, ConstElementPtr way,
                                          ConstElementPtr previous)
{
  ConstWayPtr w = dynamic_pointer_cast<const Way>(way);
  ConstWayPtr pw = dynamic_pointer_cast<const Way>(previous);
  LOG_TRACE("Writing change for " << w << "...");

  writer.writeStartElement("way");
  long id = w->getId();
  if (_change.getType() == Change::Create)
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
  long version = ElementData::VERSION_EMPTY;
  // for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else if (w->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a ") +
      QString("version greater than zero: ")  + w->getElementId().toString());
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
  if (_addTimestamp)
  {
    if (w->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(w->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    writer.writeStartElement("nd");
    long nodeRefId = w->getNodeId(j);
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

  Tags tags = w->getTags();
  _writeTags(writer, tags, w.get());

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::_writeRelation(QXmlStreamWriter& writer, ConstElementPtr relation,
                                               ConstElementPtr previous)
{
  ConstRelationPtr r = dynamic_pointer_cast<const Relation>(relation);
  ConstRelationPtr pr = dynamic_pointer_cast<const Relation>(previous);
  LOG_TRACE("Writing change for " << r << "...");

  writer.writeStartElement("relation");
  long id = r->getId();
  if (_change.getType() == Change::Create)
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
  long version = ElementData::VERSION_EMPTY;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else if (r->getVersion() < 1)
  {
    throw HootException(
      QString("Elements being modified or deleted in an .osc changeset must always have a ") +
      QString("version greater than zero: ") + r->getElementId().toString());
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
  if (_addTimestamp)
  {
    if (r->getTimestamp() != 0)
      writer.writeAttribute("timestamp", DateTimeUtils::toTimeString(r->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  const vector<RelationData::Entry>& members = r->getMembers();
  for (size_t j = 0; j < members.size(); j++)
  {
    const RelationData::Entry& e = members[j];
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
    writer.writeAttribute(
      "role",
      _invalidCharacterHandler.removeInvalidCharacters(e.role));
    writer.writeEndElement();
  }

  Tags tags = r->getTags();
  _writeTags(writer, tags, r.get());

  if (r->getType() != "")
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "type");
    writer.writeAttribute(
      "v",
      _invalidCharacterHandler.removeInvalidCharacters(r->getType()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _precision = co.getWriterPrecision();
  _addTimestamp = co.getChangesetXmlWriterAddTimestamp();
  _includeDebugTags = co.getWriterIncludeDebugTags();
  _includeCircularErrorTags = co.getWriterIncludeCircularErrorTags();
}

void OsmXmlChangesetFileWriter::_writeTags(QXmlStreamWriter& writer, Tags& tags,
                                           const Element* element)
{
  LOG_TRACE("Writing " << tags.size() << " tags for: " << element->getElementId() << "...");

  if (_includeDebugTags)
  {
    tags.set(MetadataTags::HootStatus(), QString::number(element->getStatus().getEnum()));
    tags.set(MetadataTags::HootId(), QString::number(element->getId()));
    // This just makes sifting through the xml elements a little bit easier during debugging vs
    // having to scroll around looking for the change type for each element.
    tags.set(MetadataTags::HootChangeType(), Change::changeTypeToString(_change.getType()));
  }

  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString key = it.key();
    const QString val = it.value();
    if (key.isEmpty() == false && val.isEmpty() == false)
    {
      //  Ignore 'hoot:hash' for nodes
      if (key == MetadataTags::HootHash() && element->getElementType() == ElementType::Node)
        continue;
      else if (!_includeDebugTags && key.toLower().startsWith("hoot:"))
        continue;

      writer.writeStartElement("tag");
      writer.writeAttribute("k", _invalidCharacterHandler.removeInvalidCharacters(key));
      writer.writeAttribute("v", _invalidCharacterHandler.removeInvalidCharacters(val));
      writer.writeEndElement();
    }
  }

  // Only report the circular error for changesets when debug tags are turned on, circular error
  // tags are turned on, and (for nodes) there are other tags that aren't debug tags.  This is
  // because changesets are meant for non-hoot related databases and circular error is a hoot tag.
  if (_includeCircularErrorTags && element->hasCircularError() &&
      (element->getElementType() != ElementType::Node ||
      (element->getElementType() == ElementType::Node && tags.getNonDebugCount() > 0)) &&
      _includeDebugTags)
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", MetadataTags::ErrorCircular());
    writer.writeAttribute("v", QString("%1").arg(element->getCircularError()));
    writer.writeEndElement();
  }
}

QString OsmXmlChangesetFileWriter::getStatsTable(const ChangesetStatsFormat& format) const
{
  switch (format.getEnum())
  {
    case ChangesetStatsFormat::Text:
      return _stats.toTableString();
    case ChangesetStatsFormat::Json:
      return _stats.toJsonString();
      break;
    default:
      return "";
  }
}

}
