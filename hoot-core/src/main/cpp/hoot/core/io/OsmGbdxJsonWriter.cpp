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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "OsmGbdxJsonWriter.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGbdxJsonWriter)

OsmGbdxJsonWriter::OsmGbdxJsonWriter(int precision)
  : OsmJsonWriter(precision),
    _fileNumber(0)
{
  _writeHootFormat = false;
}

void OsmGbdxJsonWriter::open(const QString& path)
{
  QFileInfo fi(path);
  _outputDir = fi.absoluteDir();
  _outputFileName = fi.baseName();

  if (!_outputDir.exists() && !FileUtils::makeDir(_outputDir.path()))
  {
    throw HootException("Error creating directory for writing.");
  }
}

void OsmGbdxJsonWriter::_newOutputFile()
{
  // Close the old file and open a new one
  if (_fp.isOpen())
  {
    close();
  }

  // The output has had a few changes.....
  QString url = _outputDir.filePath(QString("%1_00_%2.json").arg(_outputFileName).arg(_fileNumber++));

  // If the file exists, increment the middle _00_ in the name.
  // NOTE: This assumes that there can be a maximum of 10 copies of a filename....
  if (QFile::exists(url))
  {
    int inc = 0;
    while (QFile::exists(url))
    {
      inc++;
      url = _outputDir.filePath(QString("%1_%2_%3.json").arg(_outputFileName).arg(inc,2,10,QChar('0')).arg(_fileNumber));
    }
  }

  _fp.setFileName(url);

  if (!_fp.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw HootException(QObject::tr("Error opening %1 for writing").arg(url));
  }

  _out = &_fp;
}

void OsmGbdxJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;

  _writeNodes();
  _writeWays();
  _writeRelations();

  // This will move out of here eventually.
  close();
}

void OsmGbdxJsonWriter::_writeGeometry(ConstNodePtr n)
{
  vector<long> node;
  node.push_back(n->getId());
  _writeGeometry(node, "Point");
}

void OsmGbdxJsonWriter::_writeGeometry(ConstWayPtr w)
{
  const vector<long>& nodes = w->getNodeIds();
  const bool isPolygon = AreaCriterion().isSatisfied(w) || nodes[0] == nodes[nodes.size() - 1];
  _writeGeometry(nodes, isPolygon ? "Polygon" : "LineString");
}

void OsmGbdxJsonWriter::_writeGeometry(ConstRelationPtr r)
{
  vector<RelationData::Entry> members = r->getMembers();
  // NOTE: _write DOES NOT add a carriage return unless "true" is the second argument
  // See OsmJsonWriter.cpp, lines 189-194
  _writeKvp("type", "GeometryCollection");
  _write(",");
  _write("\"geometries\": [");
  bool first = true;
  for (vector<RelationData::Entry>::iterator it = members.begin(); it != members.end(); ++it)
  {
    ConstElementPtr e = _map->getElement(it->getElementId());
    if (e.get() == nullptr)
      continue;
    if (first)
      first = false;
    else
      _write(",");
    _write("{");
    _writeGeometry(e);
    _write("}");
  }
  _write("]");
}

void OsmGbdxJsonWriter::_writeGeometry(ConstElementPtr e)
{
  switch(e->getElementType().getEnum())
  {
  case ElementType::Node:
    _writeGeometry(std::dynamic_pointer_cast<const Node>(e));
    break;
  case ElementType::Way:
    _writeGeometry(std::dynamic_pointer_cast<const Way>(e));
    break;
  case ElementType::Relation:
    _writeGeometry(std::dynamic_pointer_cast<const Relation>(e));
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(e->getElementType().toString()));
  }
}

void OsmGbdxJsonWriter::_writeGeometry(const vector<long> &nodes, string type)
{
  //  Build a temporary vector of valid nodes
  vector<long> temp_nodes;
  for (vector<long>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (_map->getNode(*it).get() != nullptr)
      temp_nodes.push_back(*it);
  }
  if (temp_nodes.size() < 2 && type.compare("Point") != 0)
    type = "Point";
  if (temp_nodes.size() < 4 && type.compare("Polygon") == 0)
    type = "LineString";
  if (type.compare("Polygon") == 0 && temp_nodes[0] != temp_nodes[temp_nodes.size() - 1])
    type = "LineString";
  _writeKvp("type", type.c_str());
  _write(",");
  _write("\"coordinates\": ");
  bool point = (type.compare("Point") == 0);
  bool polygon = (type.compare("Polygon") == 0);
  if (!point)
    _write("[");
  if (polygon)
    _write("[");
  bool first = true;
  for (vector<long>::const_iterator it = temp_nodes.begin(); it != temp_nodes.end(); ++it)
  {
    if (first)
      first = false;
    else
      _write(",");
    ConstNodePtr n = _map->getNode(*it);
    _write(QString("[%1, %2]").arg(QString::number(n->getX(), 'g', _precision)).arg(QString::number(n->getY(), 'g', _precision)));
  }
  if (polygon)
    _write("]");
  if (!point)
    _write("]");
}

void OsmGbdxJsonWriter::_writeFeature(ConstElementPtr e)
{
  _writeKvp("type", "Feature");
  if (_hasTags(e))
  {
    _write(",");
    _writeTags(e);
  }
  if (e->getElementType() == ElementType::Relation)
  {
    _write(",");
    _writeRelationInfo(std::dynamic_pointer_cast<const Relation>(e));
  }
}

void OsmGbdxJsonWriter::_writeNodes()
{
  NoInformationCriterion crit;
  QList<long> nids;
  const NodeMap& nodes = _map->getNodes();

  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (!crit.isSatisfied(_map->getNode(it->first)))
      nids.append(it->first);
  }

  // sort the values to give consistent results.
  qSort(nids.begin(), nids.end(), qGreater<long>());
  for (int i = 0; i < nids.size(); i++)
  {
    _newOutputFile();
    _writeNode(_map->getNode(nids[i]));
  }
}

void OsmGbdxJsonWriter::_writeNode(ConstNodePtr node)
{
  if (node.get() == nullptr)
    return;
  _write("{");
  _writeFeature(node);
  _write(",");
  _write("\"geometry\": {");
  _writeGeometry(node);
  _write("}");
  _write("}", false);
}

void OsmGbdxJsonWriter::_writeWays()
{
  const WayMap& ways = _map->getWays();

  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr w = it->second;
    //  Skip any ways that have parents
    set<ElementId> parents = _map->getParents(w->getElementId());
    if (!parents.empty())
      continue;
    if (w.get() == nullptr)
      continue;

    //  Make sure that building ways are "complete"
    const vector<long>& nodes = w->getNodeIds();
    bool valid = true;
    if (AreaCriterion().isSatisfied(w))
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = _map->getNode(*nodeIt);
        if (node.get() == nullptr)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in Gbdxjson if valid
    if (valid)
    {
      _newOutputFile();
      _writeWay(w);
    }
    else
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = _map->getNode(*nodeIt);
        if (node.get() != nullptr)
        {
          _newOutputFile();
          _writeNode(node);
        }
      }
    }
  }
}

void OsmGbdxJsonWriter::_writeWay(ConstWayPtr way)
{
  if (way.get() == nullptr)
    return;
  //  Write out the way in geojson
  _write("{");
  _writeFeature(way);
  _write(",");
  _write("\"geometry\": {");
  _writeGeometry(way);
  _write("}");
  _write("}", false);

}

void OsmGbdxJsonWriter::_writeRelations()
{
  const RelationMap& relations = _map->getRelations();

  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr r = it->second;
    //  Write out the relation (and all children) in geojson
    _newOutputFile();
    _write("{");
    _writeFeature(r);
    _write(",");
    _write("\"geometry\": {");
    _writeGeometry(r);
    _write("}");
    _write("}", false);
  }
}

void OsmGbdxJsonWriter::_writeRelationInfo(ConstRelationPtr r)
{
  _writeKvp("relation-type", r->getType()); _write(",");
  QString roles = _buildRoles(r).c_str();
  _writeKvp("roles", roles);
}

string OsmGbdxJsonWriter::_buildRoles(ConstRelationPtr r)
{
  bool first = true;
  return _buildRoles(r, first);
}

string OsmGbdxJsonWriter::_buildRoles(ConstRelationPtr r, bool& first)
{
  stringstream ss;
  const vector<RelationData::Entry>& members = r->getMembers();
  //  Iterate all members and concatenate the roles separated by a semicolon
  for (vector<RelationData::Entry>::const_iterator it = members.begin(); it != members.end(); ++it)
  {
    ConstElementPtr e = _map->getElement(it->getElementId());
    if (e.get() == nullptr)
      continue;
    if (first)
      first = false;
    else
      ss << ";";
    ss << it->getRole();
    //  Recursively get the roles of the sub-relation that is valid
    if (it->getElementId().getType() == ElementType::Relation &&
        _map->getRelation(it->getElementId().getId()).get() != nullptr)
    {
      ss << ";" << _buildRoles(_map->getRelation(it->getElementId().getId()), first);
    }
  }
  return ss.str();
}

}
