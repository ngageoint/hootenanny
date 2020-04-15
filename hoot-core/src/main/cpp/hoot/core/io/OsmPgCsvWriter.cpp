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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmPgCsvWriter.h"

#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <QFileInfo>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmPgCsvWriter)

OsmPgCsvWriter::OsmPgCsvWriter()
  : _precision(ConfigOptions().getWriterPrecision()),
    _separator(","),
    _endl("\n")
{
}

OsmPgCsvWriter::~OsmPgCsvWriter()
{
}

void OsmPgCsvWriter::open(const QString& url)
{
  QFileInfo path(url);
  QString base = QString("%1/%2").arg(path.absolutePath()).arg(path.baseName());
  //  Create new filenames, i.e. /path/filename.pgcsv turns into /path/filename-nodes.pgcsv
  array<QString, FileType::MaxFileType> filenames;
  filenames[FileType::Nodes] = base + QString("-nodes.") + path.completeSuffix();
  filenames[FileType::Ways] = base + QString("-ways.") + path.completeSuffix();
  filenames[FileType::Relations] = base + QString("-relations.") + path.completeSuffix();
  filenames[FileType::WayNodes] = base + QString("-waynodes.") + path.completeSuffix();
  filenames[FileType::RelationMembers] = base + QString("-relationmembers.") + path.completeSuffix();

  for (int i = 0; i < FileType::MaxFileType; ++i)
  {
    _files[i].reset(new QFile(filenames[i]));
    if (!_files[i]->open(QIODevice::WriteOnly | QIODevice::Text))
      throw HootException(QString("Error opening %1 for writing").arg(filenames[i]));
    _streams[i].setDevice(_files[i].get());
  }
  //  Initialize the file headers
  _initFiles();
}

QString OsmPgCsvWriter::toString(const ConstOsmMapPtr& map)
{
  OsmPgCsvWriter writer;
  array<QString, FileType::MaxFileType> buffers;
  for (int i = 0; i < FileType::MaxFileType; ++i)
    writer._streams[i].setString(&buffers[i]);
  //  Initialize the file headers
  writer._initFiles();
  //  Write the map to the string
  writer.write(map);
  //  Flush the streams to the buffers
  writer.close();
  //  Everything is written to the buffers
  for (int i = 1; i < FileType::MaxFileType; ++i)
    writer._streams[0] << writer._endl + buffers[i];
  return buffers[0];
}


void OsmPgCsvWriter::_initFiles()
{
  //  Write out the nodes file header
  _streams[FileType::Nodes].setCodec("UTF-8");
  _streams[FileType::Nodes]
      << "node_id" << _separator
      << "latitude" << _separator
      << "longitude" << _separator
      << "changeset_id" << _separator
      << "visible" << _separator
      << "timestamp" << _separator
      << "version" << _separator
      << "tags" << _endl;
  //  Write out the ways file header
  _streams[FileType::Ways].setCodec("UTF-8");
  _streams[FileType::Ways]
      << "way_id" << _separator
      << "changeset_id" << _separator
      << "timestamp" << _separator
      << "version" << _separator
      << "visible" << _separator
      << "tags" << _endl;
  //  Write out the waynodes file header
  _streams[FileType::WayNodes].setCodec("UTF-8");
  _streams[FileType::WayNodes]
      << "way_id" << _separator
      << "node_id" << _separator
      << "version" << _separator
      << "sequence_id" << _endl;
  //  Write out the relations file header
  _streams[FileType::Relations].setCodec("UTF-8");
  _streams[FileType::Relations]
      << "relation_id" << _separator
      << "changeset_id" << _separator
      << "timestamp" << _separator
      << "version" << _separator
      << "visible" << _separator
      << "tags" << _endl;
  //  Write out the rlation members file header
  _streams[FileType::RelationMembers].setCodec("UTF-8");
  _streams[FileType::RelationMembers]
      << "relation_id" << _separator
      << "member_type" << _separator
      << "member_id" << _separator
      << "member_role" << _separator
      << "version" << _separator
      << "sequence_id" << _endl;
}

void OsmPgCsvWriter::close()
{
  //  Flush the stream then flush and close the file
  for (int i = 0; i < FileType::MaxFileType; ++i)
  {
    _streams[i].flush();
    if (_files[i].get())
    {
      _files[i]->flush();
      _files[i]->close();
    }
  }
}

void OsmPgCsvWriter::write(const ConstOsmMapPtr& map)
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

void OsmPgCsvWriter::writePartial(const hoot::ConstNodePtr& n)
{
  //  Node
  //  node_id,latitude,longitude,changeset_id,visible,timestamp,version,tags
  _streams[FileType::Nodes]
      << n->getId() << _separator
      << QString::number(n->getY(), 'f', _precision) << _separator
      << QString::number(n->getX(), 'f', _precision) << _separator
      << n->getChangeset() << _separator
      << (n->getVisible() ? 't' : 'f') << _separator
      << (n->getTimestamp() != ElementData::TIMESTAMP_EMPTY ? DateTimeUtils::toTimeString(n->getTimestamp()) : "") << _separator
      << n->getVersion() << _separator
      << _getTags(n) << _endl;
}

void OsmPgCsvWriter::writePartial(const hoot::ConstWayPtr& w)
{
  //  Way
  //  way_id,changeset_id,timestamp,version,visible,tags
  _streams[FileType::Ways]
      << w->getId() << _separator
      << w->getChangeset() << _separator
      << (w->getTimestamp() != ElementData::TIMESTAMP_EMPTY ? DateTimeUtils::toTimeString(w->getTimestamp()) : "") << _separator
      << w->getVersion() << _separator
      << (w->getVisible() ? 't' : 'f') << _separator
      << _getTags(w) << _endl;
  //  WayNodes
  //  way_id,node_id,version,sequence_id
  for (size_t i = 0; i < w->getNodeIds().size(); ++i)
  {
    _streams[FileType::WayNodes]
        << w->getId() << _separator
        << w->getNodeIds()[i] << _separator
        << w->getVersion() << _separator
        << i + 1 << _endl;
  }
}

void OsmPgCsvWriter::writePartial(const hoot::ConstRelationPtr& r)
{
  //  Relation
  //  relation_id,changeset_id,timestamp,version,visible,tags
  _streams[FileType::Relations]
      << r->getId() << _separator
      << r->getChangeset() << _separator
      << (r->getTimestamp() != ElementData::TIMESTAMP_EMPTY ? DateTimeUtils::toTimeString(r->getTimestamp()) : "") << _separator
      << r->getVersion() << _separator
      << (r->getVisible() ? 't' : 'f') << _separator
      << _getTags(r) << _endl;
  //  Relation Members
  //  relation_id,member_type,member_id,member_role,version,sequence_id
  for (size_t i = 0; i < r->getMembers().size(); ++i)
  {
    _streams[FileType::RelationMembers]
        << r->getId() << _separator
        << r->getMembers()[i].getElementId().getType().toString() << _separator
        << r->getMembers()[i].getElementId().getId() << _separator
        << r->getMembers()[i].getRole() << _separator
        << r->getVersion() << _separator
        << i + 1 << _endl;
  }
}

QString OsmPgCsvWriter::_getTags(const ConstElementPtr& e)
{
  QString buffer;
  QTextStream stream(&buffer);
  stream.setCodec("UTF-8");
  const Tags& tags = e->getTags();
  QRegExp regex("[\"=>, -]");
  for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
  {
    //  Comma separated list
    if (it != tags.constBegin())
      stream << ",";
    //  Surround the key with quotes if it contains a special character requiring it
    if (it.key().contains(regex))
      stream << "\"" << QString(it.key()).replace("\"", "\\\"") << "\"";
    else
      stream << it.key();
    stream<< "=>";
    //  Surround the value with quotes if it contains a special character requiring it
    if (it.value().contains(regex))
      stream << "\"" << QString(it.value()).replace("\"", "\\\"") << "\"";
    else
      stream << it.value();
  }
  return stream.readAll();
}

void OsmPgCsvWriter::finalizePartial()
{
  close();
}

void OsmPgCsvWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions config(conf);
  _precision = config.getWriterPrecision();
}

}
