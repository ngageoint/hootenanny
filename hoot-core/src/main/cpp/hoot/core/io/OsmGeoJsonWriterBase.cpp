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
 * @copyright Copyright (C) 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmGeoJsonWriterBase.h"

// Hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/elements/ElementData.h>
#include <hoot/core/elements/ElementType.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Qt
#include <QBuffer>
#include <QDateTime>
#include <QXmlStreamWriter>
#include <QtCore/QStringBuilder>

using namespace geos::geom;
using namespace std;

namespace hoot
{

//  Don't register with HOOT_FACTORY_REGISTER()

OsmGeoJsonWriterBase::OsmGeoJsonWriterBase(int precision, bool taskingManagerFormat)
  : OsmJsonWriter(precision),
    _useTaskingManagerFormat(taskingManagerFormat)
{
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

void OsmGeoJsonWriterBase::setConfiguration(const Settings& conf)
{
  OsmJsonWriter::setConfiguration(conf);
  _useTaskingManagerFormat = ConfigOptions(conf).getJsonOutputTaskingManagerAoi();
  if (_useTaskingManagerFormat)
    _writeHootFormat = false;
}

void OsmGeoJsonWriterBase::_writeGeometry(ConstNodePtr n)
{
  vector<long> node;
  node.push_back(n->getId());
  _writeGeometry(node, "Point");
}

void OsmGeoJsonWriterBase::_writeGeometry(ConstWayPtr w)
{
  const vector<long>& nodes = w->getNodeIds();
  string geoType = "LineString";
  if (_useTaskingManagerFormat)
    geoType = "MultiPolygon";
  else if (AreaCriterion().isSatisfied(w) || (!nodes.empty() && nodes[0] == nodes[nodes.size() - 1]))
    geoType = "Polygon";
  _writeGeometry(nodes, geoType);
}

void OsmGeoJsonWriterBase::_writeGeometry(ConstRelationPtr r)
{
  const vector<RelationData::Entry>& members = r->getMembers();
  _writeKvp("type", "GeometryCollection"); _write(",");
  _write("\"geometries\": [");
  bool first = true;
  for (const auto& member : members)
  {
    ConstElementPtr e = _map->getElement(member.getElementId());
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

void OsmGeoJsonWriterBase::_writeGeometry(ConstElementPtr e)
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

void OsmGeoJsonWriterBase::_writeGeometry(const vector<long>& nodes, string type)
{
  //  Build a temporary vector of valid nodes
  vector<long> temp_nodes;
  for (auto node_id : nodes)
  {
    if (_map->getNode(node_id).get() != nullptr)
      temp_nodes.push_back(node_id);
  }
  //  Empty nodes list should output an empty coordinate array
  if (temp_nodes.empty())
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
  for (auto node_id : temp_nodes)
  {
    if (first)
      first = false;
    else
      _write(",");
    ConstNodePtr n = _map->getNode(node_id);
    _write(QString("[%1, %2]").arg(QString::number(n->getX(), 'g', _precision), QString::number(n->getY(), 'g', _precision)));
  }
  if (multipoly)
    _write("]]");
  if (polygon)
    _write("]");
  if (!point)
    _write("]");
}

void OsmGeoJsonWriterBase::_writeFeature(ConstElementPtr e)
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

void OsmGeoJsonWriterBase::_writeNode(ConstNodePtr node)
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

void OsmGeoJsonWriterBase::_writeWay(ConstWayPtr way)
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

void OsmGeoJsonWriterBase::_writeRelationInfo(ConstRelationPtr r)
{
  _writeKvp("relation-type", r->getType()); _write(",");
  QString roles = _buildRoles(r).c_str();
  _writeKvp("roles", roles);
}

string OsmGeoJsonWriterBase::_buildRoles(ConstRelationPtr r)
{
  bool first = true;
  return _buildRoles(r, first);
}

string OsmGeoJsonWriterBase::_buildRoles(ConstRelationPtr r, bool& first)
{
  stringstream ss;
  const vector<RelationData::Entry>& members = r->getMembers();
  //  Iterate all members and concatenate the roles separated by a semicolon
  for (const auto& member : members)
  {
    ConstElementPtr e = _map->getElement(member.getElementId());
    if (e.get() == nullptr)
      continue;
    if (first)
      first = false;
    else
      ss << ";";
    ss << member.getRole();
    //  Recursively get the roles of the sub-relation that is valid
    if (member.getElementId().getType() == ElementType::Relation &&
        _map->getRelation(member.getElementId().getId()).get() != nullptr)
    {
      ss << ";" << _buildRoles(_map->getRelation(member.getElementId().getId()), first);
    }
  }
  return ss.str();
}

}
