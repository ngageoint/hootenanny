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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmXmlChangesetFileWriter.h"

// hoot
#include <hoot/core/io/OsmXmlWriter.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/util/Log.h>

// Qt
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamWriter>

using namespace std;

namespace hoot
{

OsmXmlChangesetFileWriter::OsmXmlChangesetFileWriter()
  : _precision(ConfigOptions().getWriterPrecision()),
    _changesetMaxSize(ConfigOptions().getChangesetMaxSize()),
    _multipleChangesetsWritten(false),
    _addTimestamp(ConfigOptions().getChangesetXmlWriterAddTimestamp()),
    _includeDebugTags(ConfigOptions().getWriterIncludeDebugTags())
{
  _stats.resize(Change::Unknown, ElementType::Unknown);
  vector<QString> rows( {"Create", "Modify", "Delete"} );
  vector<QString> columns( {"Node", "Way", "Relation"} );
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

void OsmXmlChangesetFileWriter::write(QString path, ChangeSetProviderPtr cs)
{
  QFileInfo info(path);
  info.setCaching(false);
  QString file = info.baseName();
  QString dir = info.path();
  QString ext = info.completeSuffix();
  int fileCount = 0;
  QString filepath = path;

  _initIdCounters();

  while (cs->hasMoreChanges())
  {
    long changesetProgress = 1;
    //  Create a new filepath if the file is split in multiple files because of the
    //  changeset.max.size setting
    //   i.e. <filepath>/<filename>-001.<ext>
    if (fileCount > 0)
    {
      filepath =
        QString("%1/%2-%3.%4").arg(dir).arg(file).arg(fileCount, 3, 10, QChar('0')).arg(ext);
      _multipleChangesetsWritten = true;
    }
    LOG_INFO("Writing changeset to " << filepath);

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
    writer.writeAttribute("generator", "hootenanny");

    Change::ChangeType last = Change::Unknown;

    while (cs->hasMoreChanges() && changesetProgress <= _changesetMaxSize)
    {
      LOG_TRACE("Reading next XML change...");
      _change = cs->readNextChange();
      LOG_VART(_change.toString());
      if (_change.getType() != last)
      {
        if (last != Change::Unknown)
        {
          writer.writeEndElement();
        }
        switch (_change.getType())
        {
          case Change::Create:
            writer.writeStartElement("create");
            break;
          case Change::Delete:
            writer.writeStartElement("delete");
            break;
          case Change::Modify:
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
            _writeNode(writer, boost::dynamic_pointer_cast<const Node>(_change.getElement()));
            break;
          case ElementType::Way:
            _writeWay(writer, boost::dynamic_pointer_cast<const Way>(_change.getElement()));
            break;
          case ElementType::Relation:
            _writeRelation(
              writer, boost::dynamic_pointer_cast<const Relation>(_change.getElement()));
            break;
          default:
            throw IllegalArgumentException("Unexpected element type.");
        }
        changesetProgress++;
        //  Update the stats
        _stats(last, type)++;
      }
    }

    if (last != Change::Unknown)
    {
      writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();

    f.close();
    //  Increment the file number if needed for split files
    fileCount++;
  }
}

//TODO: consolidate redundant tag code in these element write methods

void OsmXmlChangesetFileWriter::_writeNode(QXmlStreamWriter& writer, ConstNodePtr n)
{
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
  long version = -1;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else
    version = n->getVersion();
  writer.writeAttribute("version", QString::number(version));

  writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
  writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

  if (_addTimestamp)
  {
    if (n->getTimestamp() != 0)
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(n->getTimestamp()));
    else
      writer.writeAttribute("timestamp", "");
  }

  Tags tags = n->getTags();
  if (_includeDebugTags)
  {
    tags.set(MetadataTags::HootStatus(), QString::number(n->getStatus().getEnum()));
  }
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    if (it.key().isEmpty() == false && it.value().isEmpty() == false &&
        it.key() != MetadataTags::HootHash())
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", _invalidCharacterRemover.removeInvalidCharacters(it.key()));
      writer.writeAttribute("v", _invalidCharacterRemover.removeInvalidCharacters(it.value()));
      writer.writeEndElement();
    }
  }

  // turn this on when we start using node circularError.
  if (n->hasCircularError() && n->getTags().getNonDebugCount() > 0)
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", MetadataTags::ErrorCircular());
    writer.writeAttribute("v", QString("%1").arg(n->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::_writeWay(QXmlStreamWriter& writer, ConstWayPtr w)
{
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
  long version = -1;
  // for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else
    version = w->getVersion();
  writer.writeAttribute("version", QString::number(version));
  if (_addTimestamp)
  {
    if (w->getTimestamp() != 0)
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(w->getTimestamp()));
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
  if (_includeDebugTags)
  {
    tags.set(MetadataTags::HootStatus(), QString::number(w->getStatus().getEnum()));
  }
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", _invalidCharacterRemover.removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", _invalidCharacterRemover.removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (w->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", MetadataTags::ErrorCircular());
    writer.writeAttribute("v", QString("%1").arg(w->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::_writeRelation(QXmlStreamWriter& writer, ConstRelationPtr r)
{
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
  long version = -1;
  //  for xml changeset OSM rails port expects created elements to have version = 0
  if (_change.getType() == Change::Create)
    version = 0;
  else
    version = r->getVersion();
  writer.writeAttribute("version", QString::number(version));
  if (_addTimestamp)
  {
    if (r->getTimestamp() != 0)
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(r->getTimestamp()));
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
    writer.writeAttribute("role", _invalidCharacterRemover.removeInvalidCharacters(e.role));
    writer.writeEndElement();
  }

  Tags tags = r->getTags();
  if (_includeDebugTags)
  {
    tags.set(MetadataTags::HootStatus(), QString::number(r->getStatus().getEnum()));
  }
  for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
  {
    if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", _invalidCharacterRemover.removeInvalidCharacters(tit.key()));
      writer.writeAttribute("v", _invalidCharacterRemover.removeInvalidCharacters(tit.value()));
      writer.writeEndElement();
    }
  }

  if (r->getType() != "")
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", "type");
    writer.writeAttribute("v", _invalidCharacterRemover.removeInvalidCharacters(r->getType()));
    writer.writeEndElement();
  }

  if (r->hasCircularError())
  {
    writer.writeStartElement("tag");
    writer.writeAttribute("k", MetadataTags::ErrorCircular());
    writer.writeAttribute("v", QString("%1").arg(r->getCircularError()));
    writer.writeEndElement();
  }

  writer.writeEndElement();
}

void OsmXmlChangesetFileWriter::setConfiguration(const Settings &conf)
{
  ConfigOptions co(conf);
  _precision = co.getWriterPrecision();
  _changesetMaxSize = co.getChangesetMaxSize();
}

}
