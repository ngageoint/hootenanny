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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "OsmGeoJsonWriter.h"

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
#include <hoot/core/schema/SchemaUtils.h>
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

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGeoJsonWriter)

OsmGeoJsonWriter::OsmGeoJsonWriter(int precision)
  : OsmJsonWriter(precision),
    _useTaskingManagerFormat(ConfigOptions().getJsonOutputTaskingManagerAoi()),
    _useThematicLayers(ConfigOptions().getWriterThematicStructure())
{
  setConfiguration(conf());
}

void OsmGeoJsonWriter::open(const QString& url)
{
  //  Open the writer
  _writer.open(url);
  //  Initialize the translator for splitting files
  if (_writeSplitFiles)
    initTranslator();
}

void OsmGeoJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;
  if (!_writer.isOpen())
    throw HootException("Please open the file before attempting to write.");

  //  Setup the header section
  _writer.setHeaderSection();
  _write("{");
  _writeKvp("generator", "Hootenanny");
  _write(",");
  _writeKvp("type", "FeatureCollection");
  _write(",");
  _write("\"bbox\": ");
  _write(_getBbox());
  _write(",");
  if (_useTaskingManagerFormat && map->getSource() != "")
  {
    _writeKvp(MetadataTags::Source(), map->getSource());
    _write(",");
  }
  _write("\"features\": [", true);
  //  Write the nodes/ways/relations to the body
  _writer.setBodySection();
  _writeNodes();
  _writeWays();
  _writeRelations();
  //  Finish up with the footer
  _writer.setFooterSection();
  _writeLn("]");
  _writeLn("}");
  close();
}

void OsmGeoJsonWriter::setConfiguration(const Settings& conf)
{
  ConfigOptions options(conf);
  //  Set the OsmJsonWriter configurations
  OsmJsonWriter::setConfiguration(conf);
  //  GeoJSON should always output valid GeoJSON and not the Hoot JSON format
  _writeHootFormat = false;
  //  Set the MultiFileWriter settings
  MultiFileWriter::MultiFileWriterType t = MultiFileWriter::SingleFile;

  if (options.getGeojsonWriteSplitFileStructure())
  {
    //  Thematic structure requires a translator
    QString script = options.getSchemaTranslationScript();
    if (script.isEmpty())
    {
      LOG_ERROR("OsmGeoJsonWriter requires schema translation script when used with '" <<
                ConfigOptions::getGeojsonWriteSplitFileStructureKey() << "'. Reverting to single file output.");
    }
    else
    {
      try
      {
        //  Set the Translation
        SchemaUtils::validateTranslationUrl(script);
        setSchemaTranslationScript(script);
        t = MultiFileWriter::MultiThematic;
        _writeSplitFiles = true;
      }
      catch (const IllegalArgumentException& e)
      {
        LOG_ERROR(e.getWhat() << "  Reverting to single file output.");
      }
    }
  }
  _writer.setWriterType(t);
  //  Set the OsmGeoJsonWriter configuration
  _useTaskingManagerFormat = ConfigOptions(conf).getJsonOutputTaskingManagerAoi();
}

QString OsmGeoJsonWriter::_getBbox() const
{ 
  Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_map);
  return QString("[%1, %2, %3, %4]")
      .arg(QString::number(bounds.getMinX(), 'g', 5), QString::number(bounds.getMinY(), 'g', 5),
           QString::number(bounds.getMaxX(), 'g', 5), QString::number(bounds.getMaxY(), 'g', 5));
}

void OsmGeoJsonWriter::_writeMeta(ConstElementPtr e)
{
  _write("\"meta\": {");
  _writeKvp("timestamp", DateTimeUtils::toTimeString(e->getTimestamp()));
  _write(",");
  _writeKvp("version", e->getVersion());
  _write(",");
  _writeKvp("visible", e->getVisible() ? "true" : "false");
  _write("}");
}

void OsmGeoJsonWriter::_writeNodes()
{
  NoInformationCriterion crit;
  vector<long> nids;
  const NodeMap& nodes = _map->getNodes();
  for (auto it = nodes.begin(); it != nodes.end(); ++it)
  {
    if (!crit.isSatisfied(_map->getNode(it->first)))
      nids.push_back(it->first);
  }
  // sort the values to give consistent results.
  sort(nids.begin(), nids.end(), greater<long>());

  for (auto node_id : nids)
  {
    ConstNodePtr n = _map->getNode(node_id);
    _setWriterIndex(n);
    if (_writer.isCurrentIndexWritten())
      _write(",", true);

    _writeNode(n);

    _numWritten++;
    if (_numWritten % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeWays()
{
  const WayMap& ways = _map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr w = it->second;
    if (w.get() == nullptr)
      continue;
    //  Skip any ways that have parents
    set<ElementId> parents = _map->getParents(w->getElementId());
    if (!parents.empty())
      continue;
    //  Make sure that building ways are "complete"
    const vector<long>& nodes = w->getNodeIds();
    bool valid = true;
    if (_areaCriterion.isSatisfied(w))
    {
      for (auto node_id : nodes)
      {
        ConstNodePtr node = _map->getNode(node_id);
        if (node.get() == nullptr)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in geojson if valid
    if (valid)
    {
      _setWriterIndex(w);
      if (_writer.isCurrentIndexWritten())
        _write(",", true);
      _writeWay(w);
    }
    else
    {
      for (auto node_id : nodes)
      {
        ConstNodePtr node = _map->getNode(node_id);
        if (node.get() != nullptr)
        {
          _setWriterIndex(node);
          if (_writer.isCurrentIndexWritten())
            _write(",", true);
          _writeNode(node);
        }
      }
    }

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeRelations()
{
  const RelationMap& relations = _map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr r = it->second;

    _setWriterIndex(r);

    if (_writer.isCurrentIndexWritten())
      _write(",", true);
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
  else if (AreaCriterion().isSatisfied(w) || (!nodes.empty() && nodes[0] == nodes[nodes.size() - 1]))
    geoType = "Polygon";
  _writeGeometry(nodes, geoType);
}

void OsmGeoJsonWriter::_writeGeometry(ConstRelationPtr r)
{
  const vector<RelationData::Entry>& members = r->getMembers();
  _writeKvp("type", "GeometryCollection");
  _write(",");
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

void OsmGeoJsonWriter::_writeFeature(ConstElementPtr e)
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

void OsmGeoJsonWriter::_writeNode(ConstNodePtr node)
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

void OsmGeoJsonWriter::_writeWay(ConstWayPtr way)
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

void OsmGeoJsonWriter::_writeRelationInfo(ConstRelationPtr r)
{
  _writeKvp("relation-type", r->getType());
  _write(",");
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
    if (member.getElementId().getType() == ElementType::Relation && _map->getRelation(member.getElementId().getId()).get() != nullptr)
      ss << ";" << _buildRoles(_map->getRelation(member.getElementId().getId()), first);
  }
  return ss.str();
}

void OsmGeoJsonWriter::_setWriterIndex(const ConstElementPtr& e)
{
  if (_writeSplitFiles)
  {
    std::shared_ptr<geos::geom::Geometry> geometry;
    std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> feature;
    ElementProviderPtr provider(std::const_pointer_cast<ElementProvider>(std::dynamic_pointer_cast<const ElementProvider>(_map)));
    //  Translate the feature
    translateToFeatures(provider, e, geometry, feature);
    QString layer = _getLayerName(feature, geometry);
    _writer.setCurrentFileIndex(layer);
  }
}

QString OsmGeoJsonWriter::_getLayerName(const std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature>& feature,
                                        const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  //  TODO: Check with Matt about multiple features
  if (feature.size() > 0 && !feature[0].tableName.isEmpty())
      return feature[0].tableName;
  else if (_useThematicLayers)
    return _getThematicUnknown(geometry);
  else
    return _getFcodeUnknown(geometry);
}

QString OsmGeoJsonWriter::_getThematicUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  if (geometry)
  {
    //  getDimension returns (0=point, 1=line, 2=surface)
    switch(geometry->getDimension())
    {
    case 0: //  Point
      return "UnknownPnt";
      break;
    case 1: //  Line
      return "UnknownCrv";
      break;
    case 2: //  Surface
      return "UnknownSrf";
      break;
    default:
      break;
    }
  }
  return "Unknown";
}

QString OsmGeoJsonWriter::_getFcodeUnknown(const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  if (geometry)
  {
    //  getDimension returns (0=point, 1=line, 2=surface)
    switch(geometry->getDimension())
    {
    case 0: //  Point
      return "UNKNOWN_P";
      break;
    case 1: //  Line
      return "UNKNOWN_C";
      break;
    case 2: //  Surface
      return "UNKNOWN_S";
      break;
    default:
      break;
    }
  }
  return "UNKNOWN";
}
}
