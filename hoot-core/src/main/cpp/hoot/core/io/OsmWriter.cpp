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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmWriter.h"

// Boost
using namespace boost;

// Hoot
#include <hoot/core/Exception.h>
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/OsmUtils.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>

namespace hoot {

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmWriter)

OsmWriter::OsmWriter()
{
  _includeIds = false;
  _includeDebug = ConfigOptions().getWriterIncludeDebug();
  _includePointInWays = false;
  _includeCompatibilityTags = true;
  _precision = 16;
  _encodingErrorCount = 0;
}

QString OsmWriter::_e(const QString& s)
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
    if (_encodingErrorCount <= 10)
    {
      LOG_WARN("Found an invalid character in string: '" << s << "'");
      LOG_WARN("  UCS-4 version of the string: " << s.toUcs4());
    }
    else if (_encodingErrorCount == 11)
    {
      LOG_WARN("Found more than 10 strings with invalid characters. No longer reporting.");
    }
  }

  return result;
}

void OsmWriter::open(QString url)
{
  QFile* f = new QFile();
  _fp.reset(f);
  f->setFileName(url);
  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw Exception(QObject::tr("Error opening %1 for writing").arg(url));
  }
}

void OsmWriter::setIncludeCompatibilityTags(bool includeCompatibility)
{
  _includeCompatibilityTags = includeCompatibility;
}

QString OsmWriter::toString(const ConstOsmMapPtr& map)
{
  OsmWriter writer;
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

QString OsmWriter::_typeName(ElementType e)
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

void OsmWriter::write(boost::shared_ptr<const OsmMap> map, const QString& path)
{
  open(path);
  write(map);
}

void OsmWriter::write(boost::shared_ptr<const OsmMap> map)
{
  if (!_fp.get() || _fp->isWritable() == false)
  {
    throw HootException("Please open the file before attempting to write.");
  }
  QXmlStreamWriter writer(_fp.get());
  writer.setCodec("UTF-8");
  writer.setAutoFormatting(true);
  writer.writeStartDocument();

  writer.writeStartElement("osm");
  writer.writeAttribute("version", "0.6");
  writer.writeAttribute("generator", "hootenanny");
  char *wkt;
  map->getProjection()->exportToPrettyWkt(&wkt);
  writer.writeAttribute("srs", wkt);
  free(wkt);

  _timestamp = "1970-01-01T00:00:00Z";

  _writeNodes(map, writer);
  _writeWays(map, writer);
  _writeRelations(map, writer);

  writer.writeEndElement();
  writer.writeEndDocument();

  _fp->close();
}

void OsmWriter::_writeNodes(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  const OsmMap::NodeMap& nodes = map->getNodeMap();
  QList<long> nids = nodes.keys();
  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qGreater<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    const Node* n = map->getNode(nids[i]).get();
    writer.writeStartElement("node");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(n->getId()));
    if (_includeCompatibilityTags)
    {
      //writer.writeAttribute("timestamp", _timestamp);
       // LOG_WARN("timestamp value = ");
       // LOG_WARN(n->getTimestamp());
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(n->getTimestamp()));
      //writer.writeAttribute("version", "1");
      writer.writeAttribute("version", QString::number(n->getVersion()));
      writer.writeAttribute("changeset", QString::number(n->getChangeset()));
    }
    writer.writeAttribute("lat", QString::number(n->getY(), 'f', _precision));
    writer.writeAttribute("lon", QString::number(n->getX(), 'f', _precision));

    const Tags& tags = n->getTags();
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); it++)
    {
      if (it.key().isEmpty() == false && it.value().isEmpty() == false)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", _e(it.key()));
        writer.writeAttribute("v", _e(it.value()));
        writer.writeEndElement();
      }
    }

    // turn this on when we start using node circularError.
    if (n->getCircularError() >= 0 && n->getTags().getNonDebugCount() > 0)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "error:circular");
      writer.writeAttribute("v", QString("%1").arg(n->getCircularError()));
      writer.writeEndElement();
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:id");
      writer.writeAttribute("v", QString("%1").arg(n->getId()));
      writer.writeEndElement();
    }

    if (_includeDebug)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:status");
      writer.writeAttribute("v", QString("%1").arg(n->getStatus().getEnum()));
      writer.writeEndElement();
    }

    writer.writeEndElement();
  }
}

void OsmWriter::_writeWays(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  WayMap::const_iterator it = map->getWays().begin();
  while (it != map->getWays().end())
  {
    const Way* w = it->second.get();
    writer.writeStartElement("way");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(w->getId()));
    if (_includeCompatibilityTags)
    {
      //writer.writeAttribute("timestamp", _timestamp);
      //LOG_WARN("wtimestamp value = ");
      //LOG_WARN(w->getTimestamp());
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(w->getTimestamp()));
      //writer.writeAttribute("version", "1");
      writer.writeAttribute("version", QString::number(w->getVersion()));
      writer.writeAttribute("changeset", QString::number(w->getChangeset()));
    }

    for (size_t j = 0; j < w->getNodeCount(); j++)
    {
      writer.writeStartElement("nd");
      long nid = w->getNodeId(j);
      writer.writeAttribute("ref", QString::number(w->getNodeId(j)));
      if (_includePointInWays)
      {
        shared_ptr<const Node> n = map->getNode(nid);
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
        writer.writeAttribute("k", _e(tit.key()));
        writer.writeAttribute("v", _e(tit.value()));
        writer.writeEndElement();
      }
    }

    if (w->getCircularError() >= 0)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "error:circular");
      writer.writeAttribute("v", QString("%1").arg(w->getCircularError()));
      writer.writeEndElement();
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:id");
      writer.writeAttribute("v", QString("%1").arg(w->getId()));
      writer.writeEndElement();
    }

    if (_includeDebug)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:status");
      writer.writeAttribute("v", QString("%1").arg(w->getStatus().getEnum()));
      writer.writeEndElement();
    }

    writer.writeEndElement();

    ++it;
  }
}

void OsmWriter::_writeRelations(shared_ptr<const OsmMap> map, QXmlStreamWriter& writer)
{
  RelationMap::const_iterator it = map->getRelationMap().begin();
  while (it != map->getRelationMap().end())
  {
    const shared_ptr<Relation>& r = it->second;
    writer.writeStartElement("relation");
    writer.writeAttribute("visible", "true");
    writer.writeAttribute("id", QString::number(r->getId()));
    if (_includeCompatibilityTags)
    {
      //writer.writeAttribute("timestamp", _timestamp);
      //LOG_WARN("rtimestamp value = ");
      //LOG_WARN(r->getTimestamp());
      writer.writeAttribute("timestamp", OsmUtils::toTimeString(r->getTimestamp()));
      //writer.writeAttribute("version", "1");
      writer.writeAttribute("version", QString::number(r->getVersion()));
      writer.writeAttribute("changeset", QString::number(r->getChangeset()));
    }

    const vector<RelationData::Entry>& members = r->getMembers();
    for (size_t j = 0; j < members.size(); j++)
    {
      const RelationData::Entry& e = members[j];
      writer.writeStartElement("member");
      writer.writeAttribute("type", _typeName(e.getElementId().getType()));
      writer.writeAttribute("ref", QString::number(e.getElementId().getId()));
      writer.writeAttribute("role", _e(e.role));
      writer.writeEndElement();
    }

    const Tags& tags = r->getTags();
    for (Tags::const_iterator tit = tags.constBegin(); tit != tags.constEnd(); ++tit)
    {
      if (tit.key().isEmpty() == false && tit.value().isEmpty() == false)
      {
        writer.writeStartElement("tag");
        writer.writeAttribute("k", _e(tit.key()));
        writer.writeAttribute("v", _e(tit.value()));
        writer.writeEndElement();
      }
    }

    if (r->getType() != "")
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "type");
      writer.writeAttribute("v", _e(r->getType()));
      writer.writeEndElement();
    }

    if (r->getCircularError() >= 0)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "error:circular");
      writer.writeAttribute("v", QString("%1").arg(r->getCircularError()));
      writer.writeEndElement();
    }

    if (_includeDebug || _includeIds)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:id");
      writer.writeAttribute("v", QString("%1").arg(r->getId()));
      writer.writeEndElement();
    }

    if (_includeDebug)
    {
      writer.writeStartElement("tag");
      writer.writeAttribute("k", "hoot:status");
      writer.writeAttribute("v", QString("%1").arg(r->getStatus().getEnum()));
      writer.writeEndElement();
    }

    writer.writeEndElement();

    ++it;
  }
}

}
