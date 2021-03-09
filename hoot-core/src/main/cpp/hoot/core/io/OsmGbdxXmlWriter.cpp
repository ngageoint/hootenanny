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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmGbdxXmlWriter.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/UuidHelper.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int OsmGbdxXmlWriter::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGbdxXmlWriter)

OsmGbdxXmlWriter::OsmGbdxXmlWriter() :
_formatXml(ConfigOptions().getWriterXmlFormat()),
_precision(ConfigOptions().getWriterPrecision()),
_encodingErrorCount(0),
_fileNumber(0)
{
}

OsmGbdxXmlWriter::~OsmGbdxXmlWriter()
{
  close();
}

QString OsmGbdxXmlWriter::removeInvalidCharacters(const QString& s)
{
  // See Redmine #3553 for an explanation.

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
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Found an invalid character in string: '" << s << "'");
      LOG_WARN("  UCS-4 version of the string: " << s.toUcs4());
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  return result;
}

void OsmGbdxXmlWriter::open(const QString& url)
{
//  if (url.toLower().endsWith(".gxml"))
//  {
//    url.remove(url.size() - 5, url.size());
//  }

//  _outputDir = QDir(url);
//  _outputDir.makeAbsolute();
//  _outputFileName = _outputDir.dirName();

  QFileInfo fi(url);
  _outputDir = fi.absoluteDir();
  _outputFileName = fi.baseName();

  if (_outputDir.exists() == false)
  {
    if (FileUtils::makeDir(_outputDir.path()) == false)
    {
      throw HootException("Error creating directory for writing.");
    }
  }
  _bounds.init();
}

void OsmGbdxXmlWriter::_newOutputFile()
{
  // Close the old file and open a new one
  if (_fp.get())
  {
    // Calling this so that the XML elements & document get closed
    close();
  }

  // The output has had a few changes.....
//  QString url = _outputDir.filePath(UuidHelper::createUuid().toString().replace("{", "").replace("}", "") + ".xml");
//  QString url = _outputDir.filePath(QString("%1_%2.xml").arg(_outputFileName,QString::number(_fileNumber++).rightJustified(4,'0')));
//  QString url = _outputDir.filePath(QString("%1_%2.xml").arg(_outputFileName).arg(_fileNumber++));
  QString url = _outputDir.filePath(QString("%1_00_%2.xml").arg(_outputFileName).arg(_fileNumber++));

  // If the file exists, increment the middle _00_ in the name.
  // NOTE: This assumes that there can be a maximum of 10 copies of a filename....
  if (QFile::exists(url))
  {
//    LOG_ERROR("Clash: Orig Filename: " + url);
    int inc = 0;
    while (QFile::exists(url))
    {
      inc++;
      url = _outputDir.filePath(QString("%1_%2_%3.xml").arg(_outputFileName).arg(inc,2,10,QChar('0')).arg(_fileNumber));
    }
//    LOG_ERROR("Final name: " + url);
  }

  QFile* f = new QFile();
  _fp.reset(f);
  f->setFileName(url);

  if (!_fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw Exception(QObject::tr("Error opening %1 for writing").arg(url));
  }

  _writer.reset(new QXmlStreamWriter(_fp.get()));
  _writer->setCodec("UTF-8");

  if (_formatXml)
  {
    _writer->setAutoFormatting(true);
  }

  _writer->writeStartDocument();
  _writer->writeStartElement("metadata");

}

void OsmGbdxXmlWriter::close()
{
  if (_writer.get())
  {
    _writer->writeEndElement();
    _writer->writeEndDocument();
  }

  if (_fp.get())
  {
    _fp->close();
  }
}

QString OsmGbdxXmlWriter::toString(const ConstOsmMapPtr& map, const bool formatXml)
{
  OsmGbdxXmlWriter writer;
  writer.setFormatXml(formatXml);
  // this will be deleted by the _fp std::shared_ptr
  QBuffer* buf = new QBuffer();
  writer._fp.reset(buf);
  if (!writer._fp->open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw InternalErrorException(QObject::tr("Error opening QBuffer for writing. Odd."));
  }
  writer.write(map);
  return QString::fromUtf8(buf->data(), buf->size());
}

QString OsmGbdxXmlWriter::_typeName(ElementType e)
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

void OsmGbdxXmlWriter::write(const ConstOsmMapPtr& map, const QString& path)
{
  open(path);
  write(map);
}

void OsmGbdxXmlWriter::write(const ConstOsmMapPtr& map)
{
  _writeNodes(map);
  _writeWays(map);
  _writeRelations(map);

  close();
}

void OsmGbdxXmlWriter::_writeTags(const ConstElementPtr& element)
{

  // GBDX XML format:
  //   <M_Lang>English</M_Lang>

  //  const ElementType type = element->getElementType();
  //  assert(type != ElementType::Unknown);

  const Tags& tags = element->getTags();

  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    const QString key = it.key();
    const QString val = it.value().trimmed();

    // Skip the empty stuff
    if (val.isEmpty()) continue;

    // Skip the Detection ID. This goes in the geometery area
    if (key == "Det_id") continue;

    // Dump the raw GBDX attributes into a CDATA block
    if (key == "raw_gbdx")
    {
      _writer->writeStartElement(removeInvalidCharacters(key));
      _writer->writeCDATA(removeInvalidCharacters(val));
      _writer->writeEndElement();
      continue;
    }

    // Image, Platform and Instrument can be a list
//    if (key == "Src_imgid" || key == "Pltfrm_id" || key == "Ins_Type")
//    {
//      QStringList l = val.split(";");

//      _writer->writeStartElement(key);
//      _writer->writeStartElement("t1");
//      _writer->writeCharacters(removeInvalidCharacters(l[0]));
//      _writer->writeEndElement();
//      _writer->writeStartElement("t2");
//      // Look for a second value
//      if (l.size() == 2)
//      {
//        _writer->writeCharacters(removeInvalidCharacters(l[1]));
//      }
//      else
//      {
//        _writer->writeCharacters("NULL");
//      }
//      _writer->writeEndElement(); // t2
//      _writer->writeEndElement(); // key
//      continue;
//    }

    // Keywords can be a list
    if (key == "Kywrd" || key == "Src_imgid" || key == "Pltfrm_id" || key == "Ins_Type")
    {
      _writer->writeStartElement(key);
      QStringList l = val.split(";");
      for (int i = 0; i < l.size(); ++i)
      {
        _writer->writeStartElement("value");
        _writer->writeCharacters(removeInvalidCharacters(l[i]));
        _writer->writeEndElement();
      }
      _writer->writeEndElement();
      continue;
    }

    // Write out the Tag
    _writer->writeStartElement(removeInvalidCharacters(key));
    _writer->writeCharacters(removeInvalidCharacters(val));
    _writer->writeEndElement();
  } // End tag loop

//  // This is the next to fix.
//  if (type == ElementType::Relation)
//  {
//    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(element);
//    if (relation->getType() != "")
//    {
//      _writer->writeStartElement("Got Relation");
//      _writer->writeAttribute("k", "type");
//      _writer->writeAttribute("v", removeInvalidCharacters(relation->getType()));
//      _writer->writeEndElement();
//    }
//  }

}

void OsmGbdxXmlWriter::_writeNodes(ConstOsmMapPtr map)
{
  NoInformationCriterion crit;
  QList<long> nids;
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (!crit.isSatisfied(map->getNode(it->first)))
        nids.append(it->first);
  }

  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qLess<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    _newOutputFile();
    writePartial(map->getNode(nids[i]));
  }
}

void OsmGbdxXmlWriter::_writeWays(ConstOsmMapPtr map)
{
  const WayMap& ways = map->getWays();

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr w = it->second;

    // Skip if null
    if (w.get() == NULL)
      continue;

    //  Skip any ways that have parents
    set<ElementId> parents = map->getParents(w->getElementId());
    if (!parents.empty())
      continue;

    // Make sure that building ways are "complete"
    const vector<long>& nodes = w->getNodeIds();
    bool valid = true;
    if (AreaCriterion().isSatisfied(w))
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = map->getNode(*nodeIt);
        if (node.get() == NULL)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in GbdxXml if valid
    if (valid)
    {
      _newOutputFile();
      _writeWayWithPoints(w,map);
    }
    else
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = map->getNode(*nodeIt);
        if (node.get() != NULL)
        {
          LOG_INFO("Writing Nodes XXX");
          _newOutputFile();
          writePartial(node);
        }
      }
    }
  }

}

void OsmGbdxXmlWriter::_writeRelations(ConstOsmMapPtr map)
{
  QList<long> rids;
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    rids.append(it->first);
  }

  // sort the values to give consistent results.
  qSort(rids.begin(), rids.end(), qLess<long>());
  for (int i = 0; i < rids.size(); i++)
  {
    _newOutputFile();
    _writeRelationWithPoints(map->getRelation(rids[i]),map);
  }
}

void OsmGbdxXmlWriter::_writeBounds(const Envelope& bounds)
{
  _writer->writeStartElement("bounds");
  _writer->writeAttribute("minlat", QString::number(bounds.getMinY(), 'g', _precision));
  _writer->writeAttribute("minlon", QString::number(bounds.getMinX(), 'g', _precision));
  _writer->writeAttribute("maxlat", QString::number(bounds.getMaxY(), 'g', _precision));
  _writer->writeAttribute("maxlon", QString::number(bounds.getMaxX(), 'g', _precision));
  _writer->writeEndElement();
}

void OsmGbdxXmlWriter::writePartial(const ConstNodePtr& n)
{
  LOG_VART(n);

  // WKT Format:
  //  POINT (30 10)

  _writer->writeStartElement("Location");
  _writer->writeCharacters(QString("POINT (%1 %2)").arg(QString::number(n->getX(), 'f', _precision)).arg(QString::number(n->getY(), 'f', _precision)));
  _writer->writeEndElement();

  _writeTags(n);
}

void OsmGbdxXmlWriter::_writeWayWithPoints(const ConstWayPtr& w, ConstOsmMapPtr map)
{
  LOG_VART(w);

// GBDX XML Format:
//  <Det_Val>
//      <features>
//        <geometry>
//          <coordinates>62.7477649302</coordinates>
//          <coordinates>29.2851062801</coordinates>
//          <type>Point</type>
//        </geometry>
//        <id>0</id>
//        <properties>
//          <count>5</count>
//          <mgrs>41RMN755396</mgrs>
//        </properties>
//        <type>Feature</type>
//      </features>

  _writer->writeStartElement("Det_Val");
  _writer->writeStartElement("features");
  _writer->writeStartElement("geometry");
  _writer->writeStartElement("WKT");

  // WKT Format:
  // LINESTRING (30 10, 10 30, 40 40)
  // POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))
  // POLYGON ((35 10, 45 45, 15 40, 10 20, 35 10),(20 30, 35 35, 30 20, 20 30))

  QString endBracket;

  const vector<long>& nodes = w->getNodeIds();
  if (AreaCriterion().isSatisfied(w) || nodes[0] == nodes[nodes.size() - 1])
  {
    endBracket = "))";
    _writer->writeCharacters(QString("POLYGON (("));
  }
  else
  {
    endBracket = ")";
    _writer->writeCharacters(QString("LINESTRING ("));
  }

  bool first = true;
  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    if (first)
      first = false;
    else
      _writer->writeCharacters(QString(", "));

    const long nid = w->getNodeId(j);
    ConstNodePtr n = map->getNode(nid);
    _writer->writeCharacters(QString("%1 %2").arg(QString::number(n->getX(), 'f', _precision)).arg(QString::number(n->getY(), 'f', _precision)));
  }
  _writer->writeCharacters(endBracket);

  _writer->writeEndElement(); // WKT

  _writer->writeEndElement(); // geometry

  // Add the Det_id from the Tag
  _writer->writeStartElement("id");
  _writer->writeCharacters(w->getTags()["Det_id"]);
  _writer->writeEndElement();

  _writer->writeEndElement(); // features

  _writer->writeEndElement(); // Det_Val

  _writeTags(w);
}

void OsmGbdxXmlWriter::writePartial(const ConstWayPtr& w)
{
  LOG_VARI(w);

  _writer->writeStartElement("Partial way");
  _writer->writeAttribute("visible", "true");
  _writer->writeAttribute("id", QString::number(w->getId()));

  for (size_t j = 0; j < w->getNodeCount(); j++)
  {
    _writer->writeStartElement("nd");
    _writer->writeAttribute("ref", QString::number(w->getNodeId(j)));
    _writer->writeEndElement();
  }

  _writeTags(w);

  _writer->writeEndElement();
}

void OsmGbdxXmlWriter::writePartial(const ConstRelationPtr& r)
{
  LOG_VART(r);

  _writer->writeStartElement("XX relation");
  _writer->writeAttribute("visible", "true");
  _writer->writeAttribute("id", QString::number(r->getId()));
  _writer->writeAttribute("type", r->getType());

  const vector<RelationData::Entry>& members = r->getMembers();
  for (size_t j = 0; j < members.size(); j++)
  {
    const RelationData::Entry& e = members[j];
    _writer->writeStartElement("member");
    _writer->writeAttribute("type", _typeName(e.getElementId().getType()));
    _writer->writeAttribute("ref", QString::number(e.getElementId().getId()));
    _writer->writeAttribute("role", removeInvalidCharacters(e.role));
    _writer->writeEndElement();
  }

  _writeTags(r);

  _writer->writeEndElement();
}

void OsmGbdxXmlWriter::_writeRelationWithPoints(const ConstRelationPtr& r,  ConstOsmMapPtr map)
{
  LOG_VART(r);

  _writer->writeStartElement("Det_Val");

  _writer->writeStartElement("features");
  _writer->writeStartElement("geometry");
  _writer->writeStartElement("WKT");

  // The format is:
  // MULTIPOINT (10 40, 40 30, 20 20, 30 10)
  // MULTILINESTRING ((10 10, 20 20, 10 40),(40 40, 30 30, 40 20, 30 10))
  // MULTIPOLYGON (((30 20, 45 40, 10 40, 30 20)),((15 5, 40 10, 10 20, 5 10, 15 5)))
  // MULTIPOLYGON (((40 40, 20 45, 45 30, 40 40)),((20 35, 10 30, 10 10, 30 5, 45 20, 20 35),(30 20, 20 15, 20 25, 30 20)))


  QString featureGeometry(r->getType());
  QString startBracket;
  QString endBracket;

  if (featureGeometry == "multipolygon")
  {
    featureGeometry = "MultiPolygon";
    startBracket = "((";
    endBracket = "))";
  }
  else if (featureGeometry == "multilinestring")
  {
    featureGeometry = "MultiLineString";
    startBracket = "(";
    endBracket = ")";
  }
  else if (featureGeometry == "multipoint")
  {
    featureGeometry = "MultiPoint";
    startBracket = "";
    endBracket = "";
  }

  _writer->writeCharacters(featureGeometry.toUpper() + QString(" ("));
  //    _writer->writeStartElement("member");
  //    _writer->writeAttribute("type", _typeName(e.getElementId().getType()));
  //    _writer->writeAttribute("ref", QString::number(e.getElementId().getId()));
  //    _writer->writeAttribute("role", removeInvalidCharacters(e.role));
  //    _writer->writeEndElement();

  bool firstRel = true;
  const vector<RelationData::Entry>& members = r->getMembers();
  for (vector<RelationData::Entry>::const_iterator it = members.begin(); it != members.end(); ++it)
  {
    ConstElementPtr elm = map->getElement(it->getElementId());
    if (elm.get() == NULL)
      continue;

    if (firstRel)
      firstRel = false;
    else
      _writer->writeCharacters(QString(", "));

    // Sort out ways first. Nodes later
    if (elm->getElementType() == ElementType::Way)
    {
      ConstWayPtr w = map->getWay(elm->getElementId());

      _writer->writeCharacters(startBracket);
      bool firstWay = true;
      for (size_t j = 0; j < w->getNodeCount(); j++)
      {
        if (firstWay)
          firstWay = false;
        else
          _writer->writeCharacters(QString(", "));

        const long nid = w->getNodeId(j);
        ConstNodePtr n = map->getNode(nid);
        _writer->writeCharacters(QString("%1 %2").arg(QString::number(n->getX(), 'f', _precision)).arg(QString::number(n->getY(), 'f', _precision)));
      }
      _writer->writeCharacters(endBracket);
    } // End way

  } // End relation member

  _writer->writeCharacters(QString(")"));
  _writer->writeEndElement(); // WKT

//  _writer->writeStartElement("type");
//  _writer->writeCharacters(featureGeometry);
//  _writer->writeEndElement();

  _writer->writeEndElement(); // geometry

  // Add the Det_id from the Tag
  _writer->writeStartElement("id");
  _writer->writeCharacters(r->getTags()["Det_id"]);
  _writer->writeEndElement();

//  _writer->writeStartElement("type");
//  _writer->writeCharacters("Feature");
//  _writer->writeEndElement();

  _writer->writeEndElement(); // features

  _writer->writeEndElement(); // Det_Val

  _writeTags(r);

  _writer->writeEndElement();
}

void OsmGbdxXmlWriter::finalizePartial()
{
  //osmosis chokes on the bounds being written at the end of the file, so not writing it at all
  //_writeBounds(_bounds);
  close();
}

}
