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
 * @copyright Copyright (C) 2017-2023 Maxar (http://www.maxar.com/)
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

//  GEOS
#include <geos/geom/GeometryFactory.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, OsmGeoJsonWriter)

OsmGeoJsonWriter::OsmGeoJsonWriter(int precision)
  : OsmJsonWriter(precision),
    _useTaskingManagerFormat(ConfigOptions().getJsonOutputTaskingManagerAoi()),
    _useThematicLayers(ConfigOptions().getWriterThematicStructure()),
    _cropFeaturesCrossingBounds(ConfigOptions().getWriterCropFeaturesCrossingBounds())
{
  setConfiguration(conf());
}

void OsmGeoJsonWriter::open(const QString& url)
{
  //  Open the writer
  _writer.open(url);
  //  Initialize the translator if we have one
  if (!_scriptPath.isEmpty())
    initTranslator();
}

void OsmGeoJsonWriter::write(const ConstOsmMapPtr& map)
{
  _map = map;
  if (!_writer.isOpen())
    throw HootException("Please open the file before attempting to write.");

  initializePartial();

  _writeNodes();
  _writeWays();
  _writeRelations();

  finalizePartial();
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
  bool try_split = options.getGeojsonWriteSplitFileStructure();

  //  Thematic structure requires a translator
  QString script = options.getSchemaTranslationScript();
  if (script.isEmpty())
  {
    if (try_split)
    {
      LOG_ERROR("OsmGeoJsonWriter requires schema translation script when used with '" <<
                ConfigOptions::getGeojsonWriteSplitFileStructureKey() << "'. Reverting to single file output.");
    }
  }
  else
  {
    try
    {
      //  Set the Translation
      SchemaUtils::validateTranslationUrl(script);
      setSchemaTranslationScript(script);
      //  Translated files shouldn't include circular error values
      setIncludeCircularError(false);
      //  Spliting files requires a translation
      if (try_split)
      {
        t = MultiFileWriter::MultiThematic;
        _writeSplitFiles = true;
      }
    }
    catch (const IllegalArgumentException& e)
    {
      LOG_ERROR(e.getWhat() << "  Reverting to single file output.");
    }
  }
  _writer.setWriterType(t);
  //  Set the OsmGeoJsonWriter configuration
  _useTaskingManagerFormat = options.getJsonOutputTaskingManagerAoi();
  _cropFeaturesCrossingBounds = options.getWriterCropFeaturesCrossingBounds();
  //  Set the bounds for cropped lines and polygons
  setBounds(Boundable::loadBounds(options));
}

void OsmGeoJsonWriter::initializePartial()
{
  //  Setup the header section
  _writer.setHeaderSection();
  _write("{");
  _writeKvp("generator", "Hootenanny");
  _write(",");
  _writeKvp("type", "FeatureCollection");
  _write(",");
  if (_map)
  {
    _writeKvp("bbox", _getBbox());
    _write(",");
  }
  if (_useTaskingManagerFormat && _map && _map->getSource() != "")
  {
    _writeKvp(MetadataTags::Source(), _map->getSource());
    _write(",");
  }
  _write("\"features\": [", true);
  //  Write the nodes/ways/relations to the body
  _writer.setBodySection();
}

void OsmGeoJsonWriter::finalizePartial()
{
  //  Finish up with the footer
  _writer.setFooterSection();
  _writeLn("");
  _writeLn("]");
  _writeLn("}");

}

void OsmGeoJsonWriter::writePartial(const ConstNodePtr& n)
{
  //  Add the element to the cache before trying to write the object
  _addElementToCache(n);
  ElementProviderPtr provider = _getElementProvider();
  _writePartial(provider, n);
}

void OsmGeoJsonWriter::_writePartial(const ElementProviderPtr& provider, const ConstNodePtr& n)
{
  if (NoInformationCriterion().isSatisfied(n))
    return;
  //  Translate the element
  std::shared_ptr<geos::geom::Geometry> geometry = _translateElement(provider, n);
  geometry = _cropGeometryToBounds(geometry);
  if (!geometry || geometry->isEmpty())
    return;
  //  Iterate all translations
  for (auto translation_it = _translatedElementMap.begin(); translation_it != _translatedElementMap.end(); ++translation_it)
  {
    _writer.setCurrentFileIndex(translation_it->first);
    if (_writer.isCurrentIndexWritten())
      _write(",", true);

    _writeElement(provider, n, translation_it->second, geometry);
  }
}

void OsmGeoJsonWriter::writePartial(const ConstWayPtr& w)
{
  //  Add the element to the cache before trying to write the object
  _addElementToCache(w);
  ElementProviderPtr provider = _getElementProvider();
  _writePartial(provider, w);
}

void OsmGeoJsonWriter::_writePartial(const ElementProviderPtr& provider, const ConstWayPtr& w)
{
  //  Translate the element
  std::shared_ptr<geos::geom::Geometry> geometry = _translateElement(provider, w);
  geometry = _cropGeometryToBounds(geometry);
  if (!geometry || geometry->isEmpty())
    return;
  //  Iterate all translations
  for (auto translation_it = _translatedElementMap.begin(); translation_it != _translatedElementMap.end(); ++translation_it)
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(translation_it->second);
    if (!way)
      continue;
    //  Make sure that building ways are "complete"
    const vector<long>& nodes = way->getNodeIds();
    bool valid = true;
    if (_areaCriterion.isSatisfied(way))
    {
      for (auto node_id : nodes)
      {
        //  No need to translate the node, it is for validation only
        ConstNodePtr node = provider->getNode(node_id);
        if (!node)
        {
          valid = false;
          break;
        }
      }
    }
    //  Write out the way in geojson if valid
    if (valid)
    {
      //  Set the current index
      _writer.setCurrentFileIndex(translation_it->first);
      if (_writer.isCurrentIndexWritten())
        _write(",", true);
      _writeElement(provider, w, way, geometry);
    }
    else
    {
      for (auto node_id : nodes)
      {
        //  Translate the element
        ConstNodePtr n = provider->getNode(node_id);
        geometry = _translateElement(provider, n);
        geometry = _cropGeometryToBounds(geometry);
        if (!geometry || geometry->isEmpty())
          continue;
        //  Iterate all translations
        for (auto node_translation_it = _translatedElementMap.begin(); node_translation_it != _translatedElementMap.end(); ++node_translation_it)
        {
          ConstNodePtr node = std::dynamic_pointer_cast<const Node>(node_translation_it->second);
          if (!node)
          {
            _writer.setCurrentFileIndex(node_translation_it->first);
            if (_writer.isCurrentIndexWritten())
              _write(",", true);
            _writeElement(provider, n, node, geometry);
          }
        }
      }
    }
  }
}

void OsmGeoJsonWriter::writePartial(const ConstRelationPtr& r)
{
  //  Add the element to the cache before trying to write the object
  _addElementToCache(r);
  ElementProviderPtr provider = _getElementProvider();
  _writePartial(provider, r);
}

void OsmGeoJsonWriter::_writePartial(const ElementProviderPtr& provider, const ConstRelationPtr& r)
{
  //  Translate the element
  std::shared_ptr<geos::geom::Geometry> geometry = _translateElement(provider, r);
  geometry = _cropGeometryToBounds(geometry);
  if (!geometry || geometry->isEmpty())
    return;
  //  Iterate all translations
  for (auto translation_it = _translatedElementMap.begin(); translation_it != _translatedElementMap.end(); ++translation_it)
  {
    ConstRelationPtr relation = std::dynamic_pointer_cast<const Relation>(translation_it->second);
    if (!relation)
      continue;

    _writer.setCurrentFileIndex(translation_it->first);

    if (_writer.isCurrentIndexWritten())
      _write(",", true);

    _writeElement(provider, r, relation, geometry);
  }
}

QString OsmGeoJsonWriter::_getBbox() const
{
  Envelope bounds = CalculateMapBoundsVisitor::getGeosBounds(_map);
  if (_bounds)
    bounds = *_bounds->getEnvelopeInternal();
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
  ElementProviderPtr provider(std::const_pointer_cast<ElementProvider>(std::dynamic_pointer_cast<const ElementProvider>(_map)));
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
    ConstNodePtr n = std::dynamic_pointer_cast<const Node>(_map->getNode(node_id));
    if (!n)
      continue;

    _writePartial(provider, n);

    _numWritten++;
    if (_numWritten % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeWays()
{
  ElementProviderPtr provider(std::const_pointer_cast<ElementProvider>(std::dynamic_pointer_cast<const ElementProvider>(_map)));
  const WayMap& ways = _map->getWays();
  for (auto it = ways.begin(); it != ways.end(); ++it)
  {
    ConstWayPtr way = it->second;

    _writePartial(provider, way);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeRelations()
{
  ElementProviderPtr provider(std::const_pointer_cast<ElementProvider>(std::dynamic_pointer_cast<const ElementProvider>(_map)));
  const RelationMap& relations = _map->getRelations();
  for (auto it = relations.begin(); it != relations.end(); ++it)
  {
    ConstRelationPtr r = it->second;

    _writePartial(provider, r);

    _numWritten++;
    if (_numWritten % (_statusUpdateInterval) == 0)
    {
      PROGRESS_INFO("Wrote " << StringUtils::formatLargeNumber(_numWritten) << " elements to output.");
    }
  }
}

void OsmGeoJsonWriter::_writeGeometry(ConstNodePtr /*n*/, const std::shared_ptr<geos::geom::Geometry>& geometry)
{
  _writePointGeometry(geometry.get());
}

void OsmGeoJsonWriter::_writeGeometry(ConstWayPtr w, const std::shared_ptr<geos::geom::Geometry>& geometry)
{
  size_t count = w->getNodeCount();
  if (count == 1 || (count == 2 && w->getNodeId(0) == w->getNodeId(1)))
    _writePointGeometry(geometry.get());
  else if (_useTaskingManagerFormat)
    _writeMultiPolygonGeometry(geometry.get());
  else if (AreaCriterion().isSatisfied(w) && count > 3)
    _writePolygonGeometry(geometry.get());
  else
    _writeLineStringGeometry(geometry.get());
}

void OsmGeoJsonWriter::_writeGeometry(ConstRelationPtr /*r*/, const std::shared_ptr<geos::geom::Geometry>& geometry)
{
  switch (geometry->getGeometryTypeId())
  {
  case GeometryTypeId::GEOS_POINT:
    _writePointGeometry(geometry.get());
    break;
  case GeometryTypeId::GEOS_LINESTRING:
  case GeometryTypeId::GEOS_LINEARRING:
    _writeLineStringGeometry(geometry.get());
    break;
  case GeometryTypeId::GEOS_POLYGON:
    _writePolygonGeometry(geometry.get());
    break;
  case GeometryTypeId::GEOS_MULTILINESTRING:
    _writeMultiLineStringGeometry(geometry.get());
    break;
  case GeometryTypeId::GEOS_MULTIPOLYGON:
    _writeMultiPolygonGeometry(geometry.get());
    break;
  case GeometryTypeId::GEOS_GEOMETRYCOLLECTION:
    _writeGeometryCollectionGeometry(geometry.get());
    break;
  default:
    LOG_ERROR("Unknown geometry type: " << geometry->getGeometryType() << "...skipping");
    break;
  }
}

void OsmGeoJsonWriter::_writeGeometryCollectionGeometry(const geos::geom::Geometry* geometry)
{
  _writeKvp("type", "GeometryCollection");
  _write(",");
  _write("\"geometries\": [");
  bool first = true;
  size_t count = geometry->getNumGeometries();
  for (size_t index = 0; index < count; ++index)
  {
    const Geometry* sub = geometry->getGeometryN(index);
    if (!sub)
      continue;
    if (first)
      first = false;
    else
      _write(",");
    _write("{");
    switch (sub->getGeometryTypeId())
    {
    case GeometryTypeId::GEOS_POINT:
      _writePointGeometry(sub);
      break;
    case GeometryTypeId::GEOS_LINESTRING:
    case GeometryTypeId::GEOS_LINEARRING:
      _writeLineStringGeometry(sub);
      break;
    case GeometryTypeId::GEOS_POLYGON:
      _writePolygonGeometry(sub);
      break;
    case GeometryTypeId::GEOS_MULTILINESTRING:
      _writeMultiLineStringGeometry(sub);
      break;
    case GeometryTypeId::GEOS_MULTIPOLYGON:
      _writeMultiPolygonGeometry(sub);
      break;
    case GeometryTypeId::GEOS_GEOMETRYCOLLECTION:
      _writeGeometryCollectionGeometry(sub);
      break;
    default:
      LOG_ERROR("Unknown geometry type: " << sub->getGeometryType() << "...skipping");
      break;
    }
    _write("}");
  }
  _write("]");

}

void OsmGeoJsonWriter::_writePointGeometry(const geos::geom::Geometry* geometry)
{
  _writeGeometry(geometry, GeometryTypeId::GEOS_POINT);
}

void OsmGeoJsonWriter::_writeLineStringGeometry(const geos::geom::Geometry* geometry)
{
  GeometryTypeId type = geometry->getGeometryTypeId();
  switch (type)
  {
  default:
  case GEOS_LINESTRING:
    _writeGeometry(geometry, type);
    break;
  case GEOS_MULTILINESTRING:
    _writeMultiGeometry(geometry, type);
    break;
  }
}

void OsmGeoJsonWriter::_writePolygonGeometry(const geos::geom::Geometry* geometry)
{
  GeometryTypeId type = geometry->getGeometryTypeId();
  switch (type)
  {
  default:
  case GEOS_POLYGON:
    _writeGeometry(geometry, type);
    break;
  case GEOS_MULTIPOLYGON:
    _writeMultiGeometry(geometry, type);
    break;
  }
}

void OsmGeoJsonWriter::_writeGeometry(const geos::geom::Geometry* geometry, geos::geom::GeometryTypeId type)
{
  QString typeName, openBracket, closeBracket;
  bool isPoint = false;
  bool isLine = false;
  switch (type)
  {
  case GeometryTypeId::GEOS_POINT:
    typeName = "Point";
    isPoint = true;
    break;
  case GeometryTypeId::GEOS_LINESTRING:
    typeName = "LineString";
    openBracket = "[";
    closeBracket = "]";
    isLine = true;
    break;
  case GeometryTypeId::GEOS_POLYGON:
    typeName = "Polygon";
    openBracket = "[[";
    closeBracket = "]]";
    break;
  default:
    LOG_ERROR("Unknown Multi-Geometry type, skipping.");
    return;
  }
  _writeKvp("type", typeName);
  _write(",");
  _write("\"coordinates\": ");
  _write(openBracket);
  std::shared_ptr<geos::geom::CoordinateSequence> cs(geometry->getCoordinates().release());
  if (isPoint)
    _writeCoordinate(cs->getAt(0));
  else if (isLine)
    _writeCoordinateSequence(cs);
  else
    _writePolygonCoordinateSequence(cs);
  _write(closeBracket);

}

void OsmGeoJsonWriter::_writeMultiLineStringGeometry(const geos::geom::Geometry* geometry)
{
  _writeMultiGeometry(geometry, GeometryTypeId::GEOS_MULTILINESTRING);
}

void OsmGeoJsonWriter::_writeMultiPolygonGeometry(const geos::geom::Geometry* geometry)
{
  _writeMultiGeometry(geometry, GeometryTypeId::GEOS_MULTIPOLYGON);
}

void OsmGeoJsonWriter::_writeMultiGeometry(const geos::geom::Geometry* geometry, geos::geom::GeometryTypeId type)
{
  QString typeName, openBracket, closeBracket;
  bool isPolygon = false;
  switch (type)
  {
  case GeometryTypeId::GEOS_MULTILINESTRING:
    typeName = "MultiLineString";
    openBracket = "[";
    closeBracket = "]";
    break;
  case GeometryTypeId::GEOS_MULTIPOLYGON:
    typeName = "MultiPolygon";
    openBracket = "[[";
    closeBracket = "]]";
    isPolygon = true;
    break;
  default:
    LOG_ERROR("Unknown Multi-Geometry type, skipping.");
    return;
  }
  _writeKvp("type", typeName);
  _write(",");
  _write("\"coordinates\": [");
  bool first = true;
  for (size_t index = 0; index < geometry->getNumGeometries(); ++index)
  {
    const Geometry* line = geometry->getGeometryN(index);
    std::shared_ptr<geos::geom::CoordinateSequence> cs(line->getCoordinates().release());
    if (first)
      first = false;
    else
      _write(",");
    _write(openBracket);
    if (isPolygon)
      _writePolygonCoordinateSequence(cs);
    else
      _writeCoordinateSequence(cs);
    _write(closeBracket);
  }
  _write("]");

}

void OsmGeoJsonWriter::_writeCoordinateSequence(const std::shared_ptr<geos::geom::CoordinateSequence>& coordinates)
{
  bool first = true;
  for (size_t i = 0; i < coordinates->getSize(); ++i)
  {
    if (first)
      first = false;
    else
      _write(",");
    _writeCoordinate(coordinates->getAt(i));
  }
}

void OsmGeoJsonWriter::_writeCoordinate(const geos::geom::Coordinate& coordinate)
{
  _write(QString("[%1, %2]").arg(QString::number(coordinate.x, 'g', _precision), QString::number(coordinate.y, 'g', _precision)));
}

void OsmGeoJsonWriter::_writePolygonCoordinateSequence(const std::shared_ptr<geos::geom::CoordinateSequence>& coordinates)
{
  bool first = true;
  bool divide = false;
  double first_x = std::numeric_limits<double>::max();
  double first_y = std::numeric_limits<double>::max();
  size_t start_index = 0;
  for (size_t i = 0; i < coordinates->getSize(); ++i)
  {
    double x = coordinates->getAt(i).x;
    double y = coordinates->getAt(i).y;
    if (first)
    {
      first = false;
      first_x = x;
      first_y = y;
      if (divide)
      {
        divide = false;
        _write(",[");
      }
    }
    else
      _write(",");
    _writeCoordinate(coordinates->getAt(i));
    if (first_x == x && first_y == y && i != coordinates->getSize() - 1 && i != start_index)
    {
      divide = true;
      first = true;
      start_index = i + 1;
      _write("]");
    }
  }
}

void OsmGeoJsonWriter::_writeGeometry(ConstElementPtr e, const std::shared_ptr<geos::geom::Geometry>& geometry)
{
  switch(e->getElementType().getEnum())
  {
  case ElementType::Node:
    _writeGeometry(std::dynamic_pointer_cast<const Node>(e), geometry);
    break;
  case ElementType::Way:
    _writeGeometry(std::dynamic_pointer_cast<const Way>(e), geometry);
    break;
  case ElementType::Relation:
    _writeGeometry(std::dynamic_pointer_cast<const Relation>(e), geometry);
    break;
  default:
    throw HootException(QString("Unexpected element type: %1").arg(e->getElementType().toString()));
  }
}

void OsmGeoJsonWriter::_writeFeature(const ElementProviderPtr& provider, ConstElementPtr e)
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
    _writeRelationInfo(provider, std::dynamic_pointer_cast<const Relation>(e));
  }
}

void OsmGeoJsonWriter::_writeElement(const ElementProviderPtr& provider, ConstElementPtr element, ConstElementPtr translated_element, const std::shared_ptr<geos::geom::Geometry>& geometry)
{
  if (!element)
    return;
  //  Write out the element in geojson
  _write("{");
  _writeFeature(provider, translated_element);  //  Use the translated element to write out the tags
  _write(",");
  _write("\"geometry\": {");
  _writeGeometry(element, geometry);  //  Use the original element to specify the geometry
  _write("}");
  _write("}", false);
}

void OsmGeoJsonWriter::_writeRelationInfo(const ElementProviderPtr& provider, ConstRelationPtr r)
{
  _writeKvp("relation-type", r->getType());
  QString roles = _buildRoles(provider, r).c_str();
  if (!roles.isEmpty())
  {
    _write(",");
    _writeKvp("roles", roles);
  }
}

string OsmGeoJsonWriter::_buildRoles(const ElementProviderPtr& provider, ConstRelationPtr r)
{
  bool first = true;
  return _buildRoles(provider, r, first);
}

string OsmGeoJsonWriter::_buildRoles(const ElementProviderPtr& provider, ConstRelationPtr r, bool& first)
{
  stringstream ss;
  const vector<RelationData::Entry>& members = r->getMembers();
  //  Iterate all members and concatenate the roles separated by a semicolon
  for (const auto& member : members)
  {
    ConstElementPtr e = provider->getElement(member.getElementId());
    if (!e)
      continue;
    QString role = member.getRole();
    if (role.isEmpty())
    {
      if (member.getElementId().getType() != ElementType::Relation)
        continue;
      if (_buildRoles(provider, provider->getRelation(member.getElementId().getId()), first).empty())
        continue;
    }
    if (first)
      first = false;
    else
      ss << ";";
    ss << member.getRole();
    //  Recursively get the roles of the sub-relation that is valid
    if (member.getElementId().getType() == ElementType::Relation && provider->getRelation(member.getElementId().getId()).get() != nullptr)
      ss << ";" << _buildRoles(provider, provider->getRelation(member.getElementId().getId()), first);
  }
  return ss.str();
}

std::shared_ptr<geos::geom::Geometry> OsmGeoJsonWriter::_translateElement(const ElementProviderPtr& provider, const ConstElementPtr& e)
{
  std::shared_ptr<geos::geom::Geometry> geometry;
  std::vector<ScriptToOgrSchemaTranslator::TranslatedFeature> feature;
  //  Clear out the map of
  _translatedElementMap.clear();
  //  Don't translate without a translator
  if (!_translator)
  {
    QString table = _getLayerName(e, nullptr);
    _translatedElementMap[table] = e;
    return convertGeometry(provider, e);
  }
  //  Translate the element and get the
  //  Translate the feature
  translateToFeatures(provider, e, geometry, feature);
  if (feature.empty())
  {
    QString table = _getLayerName(e, geometry);
    _translatedElementMap[table] = e;
    return geometry;
  }
  //  Iterate all features and put them into the translated map
  for (const auto& f : feature)
  {
    if (!f.feature)
    {
      QString table = _getLayerName(e, geometry);
      _translatedElementMap[table] = e;
      continue;
    }
    ElementPtr c = e->clone();
    Tags t;
    const QVariantMap& vm = f.feature->getValues();
    for (auto it = vm.constBegin(); it != vm.constEnd(); ++it)
    {
      const QVariant& v = it.value();
      QByteArray ba = it.key().toUtf8();
      //  Convert the variant values, don't insert "empty" values (i.e. -999999 or 'No Information')
      switch (v.type())
      {
      case QVariant::Invalid:
        break;
      case QVariant::Int:
      {
        int value = v.toInt();
        if (value != -999999)
          t.set(ba.constData(), value);
        break;
      }
      case QVariant::LongLong:
      {
        qlonglong value = v.toLongLong();
        if (value != -999999)
          t.set(ba.constData(), QString::number(value));
        break;
      }
      case QVariant::Double:
      {
        double value = v.toDouble();
        if (value != -999999.0)
          t.set(ba.constData(), value);
        break;
      }
      case QVariant::String:
      {
        QString value = v.toString();
        if (!value.isEmpty() && value != "noInformation" && value != "No Information")
          t.set(ba.constData(), v.toString());
        break;
      }
      default:
        LOG_WARN("Can't convert the provided value into an OGR value. (" << v.toString() << ")");
        break;
      }
    }
    c->setTags(t);
    QString layer = "";
    if (_writeSplitFiles)
      layer = _getLayerName(e, f, geometry);
    _translatedElementMap[layer] = c;
  }
  return geometry;
}

QString OsmGeoJsonWriter::_getLayerName(const ConstElementPtr& e, const ScriptToOgrSchemaTranslator::TranslatedFeature& feature,
                                        const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  if (!feature.tableName.isEmpty())
    return feature.tableName;
  else
    return _getLayerName(e, geometry);
}

QString OsmGeoJsonWriter::_getLayerName(const ConstElementPtr& e, const std::shared_ptr<geos::geom::Geometry>& geometry) const
{
  if (!_writeSplitFiles)
    return "";
  //  Review and restriction relations have their own layer names
  if (e->getElementType() ==  ElementType::Relation)
  {
    ConstRelationPtr r = std::static_pointer_cast<const Relation>(e);
    if (r->getType() == MetadataTags::RelationReview())
      return _useThematicLayers ? "Review" : "REVIEW";
    else if (r->getType() == MetadataTags::RelationRestriction())
      return _useThematicLayers ? "Restriction" : "RESTRICTION";
  }
  //  Unknown feature types get slightly different unknown layer names for thematic
  if (_useThematicLayers)
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

std::shared_ptr<geos::geom::Geometry> OsmGeoJsonWriter::_cropGeometryToBounds(const std::shared_ptr<geos::geom::Geometry>& geometry, bool validate) const
{
  try
  {
    //  Treat bounded vs unbounded differently
    if (geometry && _bounds)
    {
      if (geometry->intersects(_bounds.get()))
      {
        if (_cropFeaturesCrossingBounds)
        {
          //  Get the intersection of the geometry with the bounding envelope
          std::unique_ptr<geos::geom::Geometry> intersection = geometry->intersection(_bounds.get());
          return std::shared_ptr<geos::geom::Geometry>(intersection.release());
        }
      }
      else  //  Geometry is outside of the bounds, eliminate it
        return nullptr;
    }
  }
  catch (const std::runtime_error& e)
  {
    if (validate)
    {
      std::shared_ptr<geos::geom::Geometry> g(GeometryUtils::validateGeometry(geometry.get()));
      return _cropGeometryToBounds(g, false);
    }
    else
    {
      LOG_WARN(e.what());
      return nullptr;
    }
  }
  return geometry;
}

}
