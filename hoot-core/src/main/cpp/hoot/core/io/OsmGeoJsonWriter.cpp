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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmGeoJsonWriter.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Exception.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
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

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGeoJsonWriter)

OsmGeoJsonWriter::OsmGeoJsonWriter(int precision)
  : OsmJsonWriter(precision),
    _useTaskingManagerFormat(ConfigOptions().getJsonOutputTaskingManagerAoi())
{
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

void OsmGeoJsonWriter::setConfiguration(const Settings& conf)
{
  OsmJsonWriter::setConfiguration(conf);
  _useTaskingManagerFormat = ConfigOptions(conf).getJsonOutputTaskingManagerAoi();
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

void OsmGeoJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;
  if (_out->isWritable() == false)
  {
    throw HootException("Please open the file before attempting to write.");
  }

  _write("{");
  _writeKvp("generator", "Hootenanny"); _write(",");
  _writeKvp("type", "FeatureCollection"); _write(",");
  _write("\"bbox\": "); _write(_getBbox()); _write(",");
  if (_useTaskingManagerFormat && map->getSource() != "")
  {
    _writeKvp(MetadataTags::Source(), map->getSource());
    _write(",");
  }
  _write("\"features\": [", true);
  _firstElement = true;
  _writeNodes();
  _writeWays();
  _writeRelations();
  _writeLn("]");
  _writeLn("}");
  close();
}

// Taken directly from OsmXmlWriter.
QString OsmGeoJsonWriter::toString(const ConstOsmMapPtr& map)
{
  QBuffer buf;

  if (!buf.open(QBuffer::WriteOnly))
  {
    throw InternalErrorException(QObject::tr("Error opening QBuffer for writing. Odd."));
  }

  _out = &buf;
  write(map);
  _out = 0;

  return QString::fromUtf8(buf.buffer());
}

QString OsmGeoJsonWriter::_getBbox()
{
  Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_map);
  return QString("[%1, %2, %3, %4]").arg(QString::number(bounds.getMinX(), 'g', 5))
                                    .arg(QString::number(bounds.getMinY(), 'g', 5))
                                    .arg(QString::number(bounds.getMaxX(), 'g', 5))
                                    .arg(QString::number(bounds.getMaxY(), 'g', 5));
}

void OsmGeoJsonWriter::_writeMeta(ConstElementPtr e)
{
  _write("\"meta\": {");
  _writeKvp("timestamp", DateTimeUtils::toTimeString(e->getTimestamp())); _write(",");
  _writeKvp("version", e->getVersion()); _write(",");
  _writeKvp("visible", (e->getVisible() ? "true" : "false"));
  _write("}");
}

void OsmGeoJsonWriter::_writeGeometry(ConstNodePtr n)
{
  vector<long> node;
  node.push_back(n->getId());
  _writeGeometry(node, "Point");
}

void OsmGeoJsonWriter::_writeGeometry(ConstWayPtr w)
{
  const vector<long>& nodes = w->getNodeIds();
  string geoType = "LineString";
  if (_useTaskingManagerFormat)
    geoType = "MultiPolygon";
  else if (AreaCriterion().isSatisfied(w) || (nodes.size() > 0 && nodes[0] == nodes[nodes.size() - 1]))
    geoType = "Polygon";
  _writeGeometry(nodes, geoType);
}

void OsmGeoJsonWriter::_writeGeometry(ConstRelationPtr r)
{
  vector<RelationData::Entry> members = r->getMembers();
  _writeKvp("type", "GeometryCollection"); _write(",");
  _write("\"geometries\": [");
  bool first = true;
  for (vector<RelationData::Entry>::iterator it = members.begin(); it != members.end(); ++it)
  {
    ConstElementPtr e = _map->getElement(it->getElementId());
    if (e.get() == NULL)
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

void OsmGeoJsonWriter::_writeGeometry(ConstElementPtr e)
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

void OsmGeoJsonWriter::_writeGeometry(const vector<long>& nodes, string type)
{
  //  Build a temporary vector of valid nodes
  vector<long> temp_nodes;
  for (vector<long>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (_map->getNode(*it).get() != NULL)
      temp_nodes.push_back(*it);
  }
  //  Empty nodes list should output an empty coordinate array
  if (temp_nodes.size() == 0)
  {
    _writeKvp("type", type.c_str());
    _write(",");
    _write("\"coordinates\": []");
    return;
  }
  //  Update the geometry type if necessary
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
  bool multipoly = (type.compare("MultiPolygon") == 0);
  if (!point)
    _write("[");
  if (polygon)
    _write("[");
  if (multipoly)
    _write("[[");
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
  if (multipoly)
    _write("]]");
  if (polygon)
    _write("]");
  if (!point)
    _write("]");
}

void OsmGeoJsonWriter::_writeFeature(ConstElementPtr e)
{
  _writeKvp("type", "Feature");
  if (_writeHootFormat)
  {
    _write(",");
    _writeKvp("id", QString::number(e->getId()));
    _write(",");
    _writeKvp("type", _typeName(e->getElementType()));
  }
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

void OsmGeoJsonWriter::_writeNodes()
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
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    _writeNode(_map->getNode(nids[i]));

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeNode(ConstNodePtr node)
{
  if (node.get() == NULL)
    return;
  _write("{");
  _writeFeature(node);
  _write(",");
  _write("\"geometry\": {");
  _writeGeometry(node);
  _write("}");
  _write("}", false);
}

void OsmGeoJsonWriter::_writeWays()
{
  const WayMap& ways = _map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr w = it->second;
    //  Skip any ways that have parents
    set<ElementId> parents = _map->getParents(w->getElementId());
    if (parents.size() > 0)
      continue;
    if (w.get() == NULL)
      continue;
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    //  Make sure that building ways are "complete"
    const vector<long>& nodes = w->getNodeIds();
    bool valid = true;
    if (AreaCriterion().isSatisfied(w))
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = _map->getNode(*nodeIt);
        if (node.get() == NULL)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in geojson if valid
    if (valid)
      _writeWay(w);
    else
    {
      for (vector<long>::const_iterator nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
      {
        ConstNodePtr node = _map->getNode(*nodeIt);
        if (node.get() != NULL)
        {
          if (_firstElement)
            _firstElement = false;
          else
            _write(",", true);
          _writeNode(node);
        }
      }
    }

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeWay(ConstWayPtr way)
{
  if (way.get() == NULL)
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

void OsmGeoJsonWriter::_writeRelations()
{
  const RelationMap& relations = _map->getRelations();
  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    if (_firstElement)
      _firstElement = false;
    else
      _write(",", true);
    ConstRelationPtr r = it->second;
    //  Write out the relation (and all children) in geojson
    _write("{");
    _writeFeature(r);
    _write(",");
    _write("\"geometry\": {");
    _writeGeometry(r);
    _write("}");
    _write("}", false);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO(
        "Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeRelationInfo(ConstRelationPtr r)
{
  _writeKvp("relation-type", r->getType()); _write(",");
  QString roles = _buildRoles(r).c_str();
  _writeKvp("roles", roles);
}

string OsmGeoJsonWriter::_buildRoles(ConstRelationPtr r)
{
  bool first = true;
  return _buildRoles(r, first);
}

string OsmGeoJsonWriter::_buildRoles(ConstRelationPtr r, bool& first)
{
  stringstream ss;
  const vector<RelationData::Entry>& members = r->getMembers();
  //  Iterate all members and concatenate the roles separated by a semicolon
  for (vector<RelationData::Entry>::const_iterator it = members.begin(); it != members.end(); ++it)
  {
    ConstElementPtr e = _map->getElement(it->getElementId());
    if (e.get() == NULL)
      continue;
    if (first)
      first = false;
    else
      ss << ";";
    ss << it->getRole();
    //  Recursively get the roles of the sub-relation that is valid
    if (it->getElementId().getType() == ElementType::Relation &&
        _map->getRelation(it->getElementId().getId()).get() != NULL)
    {
      ss << ";" << _buildRoles(_map->getRelation(it->getElementId().getId()), first);
    }
  }
  return ss.str();
}

}
