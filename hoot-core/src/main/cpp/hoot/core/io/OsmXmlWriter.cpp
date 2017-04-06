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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmXmlWriter.h"

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
#include <hoot/core/util/OsmUtils.h>
#include <hoot/core/elements/Tags.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>

namespace hoot
{

unsigned int OsmXmlWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmXmlWriter)

OsmXmlWriter::OsmXmlWriter() :
_formatXml(ConfigOptions().getOsmMapWriterFormatXml()),
_includeIds(false),
_includeDebug(ConfigOptions().getWriterIncludeDebug()),
_includePointInWays(false),
_includeCompatibilityTags(true),
_textStatus(ConfigOptions().getWriterTextStatus()),
_osmSchema(ConfigOptions().getOsmMapWriterSchema()),
_precision(round(ConfigOptions().getWriterPrecision())),
_encodingErrorCount(0)
{

}

QString OsmXmlWriter::removeInvalidCharacters(const QString& s)
{
  // See #3553 for an explanation.

  QString result;
  result.reserve(s.size());

  bool foundError = false;
  for (int i = 0; i < s.size(); i++)
  {
    QChar c = s[i];
    // See http://stackoverflow.com/questions/730133/invalid-characters-in-xml
    if (c < 0x20 && c != 0x9 && c != 0xA && c != 0xD)
    {
      foundError = true;
    }
    else
    {
      result.append(c);
    }
  }

  if (foundError)
  {
    _encodingErrorCount++;
    if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN("Found an invalid character in string: '" << s << "'");
      LOG_WARN("  UCS-4 version of the string: " << s.toUcs4());
    }
    else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  return result;
}

void OsmXmlWriter::open(QString url)
{
  QFile* f = new QFile();
  _fp.reset(f);
  f->setFileName(url);
  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw Exception(QObject::tr("Error opening %1 for writing").arg(url));
  }
}

void OsmXmlWriter::setIncludeCompatibilityTags(bool includeCompatibility)
{
  _includeCompatibilityTags = includeCompatibility;
}

QString OsmXmlWriter::toString(const ConstOsmMapPtr& map)
{
  OsmXmlWriter writer;
  // this will be deleted by the _fp auto_ptr
  QBuffer* buf = new QBuffer();
  writer._fp.reset(buf);
  if (!writer._fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw InternalErrorException(QObject::tr("Error opening QBuffer for writing. Odd."));
  }
  writer.write(map);
  return QString::fromUtf8(buf->data(), buf->size());
}

QString OsmXmlWriter::_typeName(ElementType e)
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

void OsmXmlWriter::write(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  open(path);
  write(map);
}

void OsmXmlWriter::write(boost::shared_ptr<const OsmMap> map)
{
  if (!_fp.get() || _fp->isWritable() == false)
  {
    throw HootException("Please open the file before attempting to write.");
  }
  QXmlStreamWriter writer(_fp.get());
  writer.setCodec("UTF-8");

  if (_formatXml)
  {
    writer.setAutoFormatting(true);
  }

  writer.writeStartDocument();

  writer.writeStartElement("osm");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", "hootenanny");

  int epsg = map->getProjection()->GetEPSGGeogCS();
  if (epsg > -1)
  {
    writer.writeAttribute("srs", QString("+epsg:%1").arg(epsg));
  }
  else
  {
    char *wkt;
    map->getProjection()->exportToWkt(&wkt);
    writer.writeAttribute("srs", wkt);
    free(wkt);
  }

  if (_osmSchema != "")
  {
    writer.writeAttribute("schema", _osmSchema);
  }

  _timestamp = "1970-01-01T00:00:00Z";

  _writeNodes(map, writer);
  _writeWays(map, writer);
  _writeRelations(map, writer);

  writer.writeEndElement();
  writer.writeEndDocument();

  _fp->close();
}

void OsmXmlWriter::_writeMetadata(QXmlStreamWriter& writer, const Element *e)
{
  LOG_VART(e->getElementId());

  if (_includeCompatibilityTags)
  {
    writer.writeAttribute("timestamp", OsmUtils::toTimeString(e->getTimestamp()));
    long version = e->getVersion();
    if (version == ElementData::VERSION_EMPTY)
    {
      version = 1;
    }
    writer.writeAttribute("version", QString::number(version));
  }
  else
  {
    //TODO: This comparison seems to be still unequal when I set an element's timestamp to
    //ElementData::TIMESTAMP_EMPTY.  See RemoveAttributeVisitor
    if (e->getTimestamp() != ElementData::TIMESTAMP_EMPTY)
    {
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(e->getTimestamp()));
    }
    if (e->getVersion() != ElementData::VERSION_EMPTY)
    {
      writer.writeAttribute("version", QString::number(e->getVersion()));
    }
  }
  if (e->getChangeset() != ElementData::CHANGESET_EMPTY)
  {
    writer.writeAttribute("changeset", QString::number(e->getChangeset()));
  }
  if (e->getUser() != ElementData::USER_EMPTY)
  {
    writer.writeAttribute("user", e->getUser());
  }
  if (e->getUid() != ElementData::UID_EMPTY)
  {
    writer.writeAttribute("uid", QString::number(e->getUid()));
  }
}

void OsmXmlWriter::_writeNodes(boost::shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  QList<long> nids;
  NodeMap::const_iterator it = map->getNodes().begin();
  while (it != map->getNodes().end())
  {
    nids.append(it->first);
    ++it;
  }

  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qLess<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    const Node* n = map->getNode(nids[i]).get();
    writer.writeStartElement("node");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(n->getId()));
    _writeMetadata(writer, n);
    writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
    writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

    const Tags& tags = n->getTags();

    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
    {
      if (it.key().isEmpty() == false && it.value().isEmpty() == false)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", removeInvalidCharacters(it.key()));
        if (it.key() == MetadataTags::HootStatus() && n->getStatus() != Status::Invalid)
        {
          if (_textStatus)
          {
            writer.writeAttribute("v", n->getStatus().toTextStatus());
          }
          else
          {
            writer.writeAttribute("v", QString("%1").arg(n->getStatus().getEnum()));
          }
        }
        else
        {
          writer.writeAttribute("v", removeInvalidCharacters(it.value()));
        }
        writer.writeEndElement();
      }
    }

    if (! tags.contains(MetadataTags::HootStatus()))
    {
      if (_textStatus && tags.getNonDebugCount() > 0)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        writer.writeAttribute("v", n->getStatus().toTextStatus());
        writer.writeEndElement();
      }
      else if (_includeDebug)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        if (_textStatus)
        {
          writer.writeAttribute("v", n->getStatus().toTextStatus());
        }
        else
        {
          writer.writeAttribute("v", QString("%1").arg(n->getStatus().getEnum()));
        }
        writer.writeEndElement();
      }
    }

    // turn this on when we start using node circularError.
    if (n->hasCircularError() && tags.getNonDebugCount() > 0 &&
        ConfigOptions().getWriterIncludeCircularError())
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::ErrorCircular());
      writer.writeAttribute("v", QString("%1").arg(n->getCircularError()));
      writer.writeEndElement();
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::HootId());
      writer.writeAttribute("v", QString("%1").arg(n->getId()));
      writer.writeEndElement();
    }
    writer.writeEndElement();
  }
}

void OsmXmlWriter::_writeWays(boost::shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  QList<long> wids;
  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end())
  {
    wids.append(it->first);
    ++it;
  }

  // sort the values to give consistent results.
  qSort(wids.begin(), wids.end(), qLess<long>());
  for (int i = 0; i < wids.size(); i++)
  {
    const Way* w = map->getWay(wids[i]).get();
    writer.writeStartElement("way");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(w->getId()));

    _writeMetadata(writer, w);

    for (size_t j = 0; j < w->getNodeCount(); j++)
    {
      writer.writeStartElement("nd");
      long nid = w->getNodeId(j);
      writer.writeAttribute("ref", QString::number(w->getNodeId(j)));
      if (_includePointInWays)
      {
        boost::shared_ptr<const Node> n = map->getNode(nid);
        writer.writeAttribute("x", QString::number(n->getX(), 'g', _precision));
        writer.writeAttribute("y", QString::number(n->getY(), 'g', _precision));
      }
      writer.writeEndElement();
    }

    const Tags& tags = w->getTags();

    for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
    {
      if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", removeInvalidCharacters(tit.key()));

        if (tit.key() == MetadataTags::HootStatus() && w->getStatus() != Status::Invalid)
        {
          if (_textStatus)
          {
            writer.writeAttribute("v", w->getStatus().toTextStatus());
          }
          else
          {
            writer.writeAttribute("v", QString("%1").arg(w->getStatus().getEnum()));
          }
        }
        else
        {
          writer.writeAttribute("v", removeInvalidCharacters(tit.value()));
        }
        writer.writeEndElement();
      }
    }

    // Logic: If we already have a "hoot:status" tag, make sure it contains the actual
    // status of the element. See writeNodes for more info
    if (! tags.contains(MetadataTags::HootStatus()))
    {
      if (_textStatus)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        writer.writeAttribute("v", w->getStatus().toTextStatus());
        writer.writeEndElement();
      }
      else if (_includeDebug)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        writer.writeAttribute("v", QString("%1").arg(w->getStatus().getEnum()));
        writer.writeEndElement();
      }
    }

    if (w->hasCircularError() && ConfigOptions().getWriterIncludeCircularError())
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::ErrorCircular());
      writer.writeAttribute("v", QString("%1").arg(w->getCircularError()));
      writer.writeEndElement();
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::HootId());
      writer.writeAttribute("v", QString("%1").arg(w->getId()));
      writer.writeEndElement();
    }

    writer.writeEndElement();
  }
}

void OsmXmlWriter::_writeRelations(boost::shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  QList<long> rids;
  RelationMap::const_iterator it = map->getRelations().begin();
  while (it != map->getRelations().end())
  {
    rids.append(it->first);
    ++it;
  }

  // sort the values to give consistent results.
  qSort(rids.begin(), rids.end(), qLess<long>());
  for (int i = 0; i < rids.size(); i++)
  {
    const boost::shared_ptr<const Relation> r = map->getRelation(rids[i]);
    writer.writeStartElement("relation");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(r->getId()));

    _writeMetadata(writer, r.get());

    const vector<RelationData::Entry>& members = r->getMembers();
    for (size_t j = 0; j < members.size(); j++)
    {
      const RelationData::Entry& e = members[j];
      writer.writeStartElement("member");
      writer.writeAttribute("type", _typeName(e.getElementId().getType()));
      writer.writeAttribute("ref", QString::number(e.getElementId().getId()));
      writer.writeAttribute("role", removeInvalidCharacters(e.role));
      writer.writeEndElement();
    }

    const Tags& tags = r->getTags();

    for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
    {
      if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", removeInvalidCharacters(tit.key()));

        // Does this need && r->getStatus() != Status::Invalid
        if (tit.key() == MetadataTags::HootStatus())
        {
          if (_textStatus)
          {
            writer.writeAttribute("v", r->getStatus().toTextStatus());
          }
          else
          {
            writer.writeAttribute(
              "v", removeInvalidCharacters(QString::number(r->getStatus().getEnum())));
          }
        }
        else
        {
          writer.writeAttribute("v", removeInvalidCharacters(tit.value()));
        }
        writer.writeEndElement();
      }
    }

    if (r->getType() != "")
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "type");
      writer.writeAttribute("v", removeInvalidCharacters(r->getType()));
      writer.writeEndElement();
    }

    if (r->hasCircularError() && ConfigOptions().getWriterIncludeCircularError())
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::ErrorCircular());
      writer.writeAttribute("v", QString("%1").arg(r->getCircularError()));
      writer.writeEndElement();
    }

    if (! tags.contains(MetadataTags::HootStatus()))
    {
      if (_textStatus)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        writer.writeAttribute("v", r->getStatus().toTextStatus());
        writer.writeEndElement();
      }
      else if (_includeDebug)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", MetadataTags::HootStatus());
        writer.writeAttribute("v", QString("%1").arg(r->getStatus().getEnum()));
        writer.writeEndElement();
      }
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", MetadataTags::HootId());
      writer.writeAttribute("v", QString("%1").arg(r->getId()));
      writer.writeEndElement();
    }

    writer.writeEndElement();
  }
}

}
