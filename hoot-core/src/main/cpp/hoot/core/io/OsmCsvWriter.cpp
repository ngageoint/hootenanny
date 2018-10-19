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

#include "OsmCsvWriter.h"

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/OsmUtils.h>

#include <QFileInfo>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmCsvWriter)

OsmCsvWriter::OsmCsvWriter()
  : _precision(ConfigOptions().getWriterPrecision()),
    _separator(","),
    _endl("\n")
{
}

OsmCsvWriter::~OsmCsvWriter()
{
  close();
}

void OsmCsvWriter::open(QString url)
{
  QFileInfo path(url);
  QString base = QString("%1/%2").arg(path.absolutePath()).arg(path.baseName());
  //  Create three new filenames, i.e. /path/filename.csv turns into /path/filename-nodes.csv
  QString nodes = base + QString("-nodes.") + path.completeSuffix();
  QString ways = base + QString("-ways.") + path.completeSuffix();
  QString waynodes = base + QString("-waynodes.") + path.completeSuffix();
  //  Open the files
  _n.reset(new QFile(nodes));
  if (!_n->open(QIODevice::WriteOnly | QIODevice::Text))
    throw HootException(QString("Error opening %1 for writing").arg(nodes));
  _nodes.setDevice(_n.get());
  _w.reset(new QFile(ways));
  if (!_w->open(QIODevice::WriteOnly | QIODevice::Text))
    throw HootException(QString("Error opening %1 for writing").arg(ways));
  _ways.setDevice(_w.get());
  _wn.reset(new QFile(waynodes));
  if (!_wn->open(QIODevice::WriteOnly | QIODevice::Text))
    throw HootException(QString("Error opening %1 for writing").arg(waynodes));
  _waynodes.setDevice(_wn.get());
  //  Initialize the file headers
  _initFiles();
}

QString OsmCsvWriter::toString(const ConstOsmMapPtr& map)
{
  OsmCsvWriter writer;
  QString nodeBuffer;
  QString wayBuffer;
  QString waynodeBuffer;
  writer._nodes.setString(&nodeBuffer);
  writer._ways.setString(&wayBuffer);
  writer._waynodes.setString(&waynodeBuffer);
  //  Initialize the file headers
  writer._initFiles();
  //  Write the map to the string
  writer.write(map);
  //  Flush the streams to the buffers
  writer.close();
  //  Everything is written to the buffers
  return nodeBuffer + writer._endl + wayBuffer + writer._endl + waynodeBuffer;
}


void OsmCsvWriter::_initFiles()
{
  //  Write out the nodes file header
  _nodes.setCodec("UTF-8");
  _nodes    << "node_id" << _separator
            << "latitude" << _separator
            << "longitude" << _separator
            << "changeset_id" << _separator
            << "visible" << _separator
            << "timestamp" << _separator
            << "tile" << _separator
            << "version" << _separator
            << "redaction_id" << _endl;
  //  Write out the ways file header
  _ways.setCodec("UTF-8");
  _ways     << "way_id" << _separator
            << "changeset_id" << _separator
            << "timestamp" << _separator
            << "version" << _separator
            << "visible" << _separator
            << "redaction_id" << _separator
            << "tags" << _endl;
  //  Write out the waynodes file header
  _waynodes.setCodec("UTF-8");
  _waynodes << "way_id" << _separator
            << "node_id" << _separator
            << "version" << _separator
            << "sequence_id" << _endl;
}

void OsmCsvWriter::close()
{
  //  Flush and close the nodes
  _nodes.flush();
  if (_n.get())
  {
    _n->flush();
    _n->close();
  }
  //  Flush and close the ways
  _ways.flush();
  if (_w.get())
  {
    _w->flush();
    _w->close();
  }
  //  Flush and close the nodes
  _waynodes.flush();
  if (_wn.get())
  {
    _wn->flush();
    _wn->close();
  }
}

void OsmCsvWriter::write(ConstOsmMapPtr map)
{
  QList<long> ids;
  //  Start with the nodes
  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    ids.append(it->first);
  //  Sort the values to give consistent results.
  qSort(ids.begin(), ids.end(), qLess<long>());
  for (int i = 0; i < ids.size(); i++)
    writePartial(map->getNode(ids[i]));
  //  Next are the ways
  ids.clear();
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
    ids.append(it->first);
  //  Sort the values to give consistent results.
  qSort(ids.begin(), ids.end(), qLess<long>());
  for (int i = 0; i < ids.size(); i++)
    writePartial(map->getWay(ids[i]));
  //  Finally the relations
  ids.clear();
  const RelationMap& relations = map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
    ids.append(it->first);
  //  Sort the values to give consistent results.
  qSort(ids.begin(), ids.end(), qLess<long>());
  for (int i = 0; i < ids.size(); i++)
    writePartial(map->getRelation(ids[i]));
}

void OsmCsvWriter::writePartial(const hoot::ConstNodePtr& n)
{
  //  Node
  //  node_id,latitude,longitude,changeset_id,visible,timestamp,tile,version,redaction_id
  _nodes << n->getId() << _separator
         << QString::number(n->getY(), 'f', _precision) << _separator
         << QString::number(n->getX(), 'f', _precision) << _separator
         << n->getChangeset() << _separator
         << (n->getVisible() ? 't' : 'f') << _separator
         << OsmUtils::toTimeString(n->getTimestamp()) << _separator
         /*<< tile?! */ << _separator
         << n->getVersion() << _separator
         /*<< redaction_id?! */ << _endl;
}

void OsmCsvWriter::writePartial(const hoot::ConstWayPtr& w)
{
  //  Way
  //  way_id,changeset_id,timestamp,version,visible,redaction_id,tags
  _ways << w->getId() << _separator
        << w->getChangeset() << _separator
        << OsmUtils::toTimeString(w->getTimestamp()) << _separator
        << w->getVersion() << _separator
        << (w->getVisible() ? 't' : 'f') << _separator
        /*<< redaction_id?! */ << _separator
        << _getTags(w) << _endl;
  //  WayNodes
  //  way_id,node_id,version,sequence_id
  for (size_t i = 0; i < w->getNodeIds().size(); ++i)
  {
    _waynodes << w->getId() << _separator
              << w->getNodeIds()[i] << _separator
              << w->getVersion() << _separator
              << i + 1 << _endl;
  }
}

QString OsmCsvWriter::_getTags(const ConstWayPtr& w)
{
  QString buffer;
  QTextStream stream(&buffer);
  stream.setCodec("UTF-8");
  const Tags& tags = w->getTags();
  QRegExp regex("[\"=>, -]");
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    if (it != tags.constBegin())
      stream << ",";
    if (it.key().contains(regex))
      stream << "\"" << QString(it.key()).replace("\"", "\\\"") << "\"";
    else
      stream << it.key();
    stream<< "=>";
    if (it.value().contains(regex))
      stream << "\"" << QString(it.value()).replace("\"", "\\\"") << "\"";
    else
      stream << it.value();
  }
  return stream.readAll();
}

void OsmCsvWriter::writePartial(const hoot::ConstRelationPtr& /*r*/)
{
  //  Right now we don't do anything here
}

void OsmCsvWriter::finalizePartial()
{
  close();
}

void OsmCsvWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  _precision = config.getWriterPrecision();
}

}
