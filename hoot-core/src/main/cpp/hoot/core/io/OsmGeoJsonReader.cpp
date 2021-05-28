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

#include "OsmGeoJsonReader.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

// Qt
#include <QTextStream>
#include <QTextCodec>
#include <QRegExp>

// Standard
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace pt = boost::property_tree;
using namespace std;
using namespace geos::geom;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmGeoJsonReader)

bool OsmGeoJsonReader::isSupported(const QString& url)
{
  QUrl myUrl(url);

  const bool isRelativeUrl = myUrl.isRelative();
  const bool isLocalFile =  myUrl.isLocalFile();

  //  Is it a file?
  if (isRelativeUrl || isLocalFile)
  {
    if (url.endsWith(".geojson", Qt::CaseInsensitive) &&
        !url.startsWith("http", Qt::CaseInsensitive))
    {
      return true;
    }
  }

  //  Is it a web address?
  if (myUrl.host().toLower() != ConfigOptions().getOverpassApiHost() &&
      ("http" == myUrl.scheme() || "https" == myUrl.scheme()))
  {
    return true;
  }

  //  Default to not supported
  return false;
}

void OsmGeoJsonReader::read(const OsmMapPtr& map)
{
  _map = map;
  _map->appendSource(_url);
  if (_isFile)
  {
    QTextStream instream(&_file);
    _results.append(instream.readAll());
  }
  else
    _readFromHttp();
  //  Process all of the result strings;
  for (int i = 0; i < _results.size(); ++i)
  {
    // This will throw a hoot exception if JSON is invalid
    _loadJSON(_results[i]);
    _parseGeoJson();
  }
}

void OsmGeoJsonReader::loadFromString(const QString& jsonStr, const OsmMapPtr& map)
{
  _loadJSON(jsonStr);
  _map = map;
  _parseGeoJson();
  _map.reset();
}

OsmMapPtr OsmGeoJsonReader::loadFromFile(const QString& path)
{
  QFile infile(path);
  if (!infile.open(QFile::ReadOnly | QFile::Text))
  {
    throw HootException("Unable to read GeoJSON file: " + path);
  }

  QTextStream instream(&infile);
  QString jsonStr = instream.readAll();
  _loadJSON(jsonStr);
  _map.reset(new OsmMap());
  _parseGeoJson();
  return _map;
}

void OsmGeoJsonReader::_parseGeoJson()
{
  _generator = QString::fromStdString(_propTree.get("generator", string("")));
  QString type = QString::fromStdString(_propTree.get("type", string("")));
  if (_propTree.not_found() != _propTree.find("bbox"))
  {
    /*Envelope env = */_parseBbox(_propTree.get_child("bbox"));
  }
  if (_propTree.not_found() != _propTree.find(MetadataTags::Source().toStdString()))
  {
    _map->replaceSource(QString(_propTree.get_child(MetadataTags::Source().toStdString()).data().c_str()));
  }

  // The GeoJSON spec doesn't support the crs prop anymore and expects all data to be in WGS84.
  // This is here for backward compatibility with older data.
  _parseCoordSys();

  // If we don't have a "features" child then we should just have a single feature
  if (_propTree.not_found() != _propTree.find("features"))
  {
    // Iterate through all of our features, adding them to the map
    pt::ptree features = _propTree.get_child("features");
    for (pt::ptree::const_iterator featureIt = features.begin(); featureIt != features.end(); ++featureIt)
        _parseGeoJsonFeature(featureIt->second);
  }
  else
  {
    // Single Feature
    _parseGeoJsonFeature(_propTree);
  }
}

void OsmGeoJsonReader::_parseCoordSys()
{
  if (_propTree.not_found() != _propTree.find("crs"))
  {
    // Since there's no spec for this, not exactly sure what to expect here. Below is what I've seen
    // so far. Its possible this code isn't quite extensible enough.
    // "crs": { "type": "name", "properties": { "name": "urn:ogc:def:crs:EPSG::32618" } }

    int crs = -1;
    bool ok = false;

    boost::property_tree::ptree crsProp = _propTree.get_child("crs");
    if (crsProp.not_found() != crsProp.find("properties"))
    {
      pt::ptree crsProps = crsProp.get_child("properties");
      if (crsProps.not_found() != crsProps.find("name"))
      {
        const QString crsName = QString::fromStdString(crsProps.get("name", std::string("")));
        LOG_VART(crsName);

        QRegExp crsNameRegex("EPSG::([0-9]+)", Qt::CaseInsensitive);
        const int index = crsNameRegex.indexIn(crsName);
        LOG_VART(index);
        const QStringList matches = crsNameRegex.capturedTexts();
        LOG_VART(matches);
        if (matches.size() == 2)
        {
          const QString crsStr = matches[1];
          LOG_VART(crsStr);
          crs = crsStr.toInt(&ok);
        }
      }
    }

    if (ok && crs != -1)
    {
      LOG_DEBUG("Setting map projection to EPSG: " << crs);
      std::shared_ptr<OGRSpatialReference> spatRef(new OGRSpatialReference());
      spatRef->importFromEPSG(crs);
      _map->setProjection(spatRef);
    }
  }
}

void OsmGeoJsonReader::_parseGeoJsonFeature(const boost::property_tree::ptree& feature)
{
  string id = feature.get("id", string(""));
  //  Some IDs may be strings that start with "node/" or "way/", remove that
  size_t pos = id.find("/");
  if (pos != string::npos)
    id = id.erase(0, pos + 1);
  if (feature.not_found() != feature.find("properties"))
  {
    pt::ptree properties = feature.get_child("properties");

    if (feature.not_found() != feature.find("geometry"))
    {
      pt::ptree geometry = feature.get_child("geometry");
      //  Type can be node, way, or relation
      string typeStr = properties.get("type", string("--"));
      string geoType = geometry.get("type", string("--"));
      if ("node" == typeStr ||
          "Point" == geoType)
      {
        bool useOld = _useDataSourceIds;
        if (id == "")
          _useDataSourceIds = false;
        _parseGeoJsonNode(id, properties, geometry);
        _useDataSourceIds = useOld;
      }
      else if ("way" == typeStr ||
               "Polygon" == geoType ||
               "LineString" == geoType)
      {
        bool useOld = _useDataSourceIds;
        if (id == "")
          _useDataSourceIds = false;
        _parseGeoJsonWay(id, properties, geometry);
        _useDataSourceIds = useOld;
      }
      else if ("relation" == typeStr ||
               "GeometryCollection" == geoType ||
               "MultiPoint" == geoType ||
               "MultiLineString" == geoType ||
               "MultiPolygon" == geoType)
      {
        bool useOld = _useDataSourceIds;
        if (id == "")
          _useDataSourceIds = false;
        _parseGeoJsonRelation(id, properties, geometry);
        _useDataSourceIds = useOld;
        //  Empty the roles if necessary
        while (!_roles.empty())
          _roles.pop();
      }
      else
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Unknown JSON elment type (" << typeStr << ") when parsing GeoJSON");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }
}

geos::geom::Envelope OsmGeoJsonReader::_parseBbox(const boost::property_tree::ptree& bbox) const
{
  pt::ptree::const_iterator bboxIt = bbox.begin();
  if (bboxIt == bbox.end())
    return Envelope();
  double minX = bboxIt->second.get_value<double>();
  bboxIt++;
  double minY = bboxIt->second.get_value<double>();
  bboxIt++;
  double maxX = bboxIt->second.get_value<double>();
  bboxIt++;
  double maxY = bboxIt->second.get_value<double>();
  return Envelope(minX, maxX, minY, maxY);
}

void OsmGeoJsonReader::_parseGeoJsonNode(const string& id, const pt::ptree& properties,
                                         const pt::ptree& geometry)
{
  //  Get info we need to construct our node
  long node_id = -1;
  if (_useDataSourceIds)
  {
    //  Try to cast the ID to a long, if unsuccessful create a new ID
    try
    {
      node_id = boost::lexical_cast<long>(id);
    }
    catch (const boost::bad_lexical_cast&)
    {
      node_id = _map->createNextNodeId();
    }
  }
  else
    node_id = _map->createNextNodeId();
  //  Parse the geometry
  vector<Coordinate> coords = _parseGeometry(geometry);

  // Defensive: We have seen files with empty coordinate arrays
  if (coords.empty())
  {
    LOG_INFO("Empty Coordinates. Skipping feature");
    return;
  }

  double lat = coords[0].y;
  double lon = coords[0].x;

  long version = ElementData::VERSION_EMPTY;
  version = properties.get("@version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = properties.get("@changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = properties.get("@timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = properties.get("@user", user);
  long uid = ElementData::UID_EMPTY;
  uid = properties.get("@uid", uid);

  //  Construct node
  NodePtr pNode(
    new Node(
      _defaultStatus, node_id, lon, lat, _defaultCircErr, changeset, version, timestamp,
      QString::fromStdString(user), uid));
  //  Add tags
  _addTags(properties, pNode);
  LOG_VART(pNode);
  //  Add node to map
  _map->addNode(pNode);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmGeoJsonReader::_parseGeoJsonWay(const string& id, const pt::ptree& properties,
                                        const pt::ptree& geometry)
{
  vector<Coordinate> coords = _parseGeometry(geometry);

  // Defensive: We have seen files with empty coordinate arrays
  if (coords.empty())
  {
    LOG_INFO("Empty Coordinates. Skipping feature");
    return;
  }

  //  Get info we need to construct our way
  long way_id = -1;
  if (_useDataSourceIds)
  {
    //  Try to cast the ID to a long, if unsuccessful create a new ID
    try
    {
      way_id = boost::lexical_cast<long>(id);
    }
    catch (const boost::bad_lexical_cast&)
    {
      way_id = _map->createNextWayId();
    }
  }
  else
    way_id = _map->createNextWayId();

  long version = ElementData::VERSION_EMPTY;
  version = properties.get("@version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = properties.get("@changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = properties.get("@timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = properties.get("@user", user);
  long uid = ElementData::UID_EMPTY;
  uid = properties.get("@uid", uid);

  //  Construct Way
  WayPtr way(
    new Way(
      _defaultStatus, way_id, _defaultCircErr, changeset, version, timestamp,
      QString::fromStdString(user), uid));
  bool isPoly = (geometry.get("type", "").compare("Polygon") == 0);

  //  Add nodes
  for (vector<Coordinate>::iterator it = coords.begin(); it != coords.end(); ++it)
  {
    if (isPoly && (it + 1) == coords.end())
    {
      //  Don't create another node to close the polygon, just use the first one
      way->addNode(way->getNodeId(0));
    }
    else
    {
      long node_id = _map->createNextNodeId();
      NodePtr node(new Node(_defaultStatus, node_id, *it, _defaultCircErr));
      _map->addNode(node);
      way->addNode(node_id);
    }
  }
  //  Add tags
  _addTags(properties, way);
  LOG_VART(way);
  //  Add way to map
  _map->addWay(way);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmGeoJsonReader::_parseGeoJsonRelation(const string& id, const pt::ptree& properties,
                                             const pt::ptree& geometry)
{
  //  Get info we need to construct our relation
  long relation_id = -1;
  if (_useDataSourceIds)
  {
    //  Try to cast the ID to a long, if unsuccessful create a new ID
    try
    {
      relation_id = boost::lexical_cast<long>(id);
    }
    catch (const boost::bad_lexical_cast&)
    {
      relation_id = _map->createNextRelationId();
    }
  }
  else
    relation_id = _map->createNextRelationId();

  long version = ElementData::VERSION_EMPTY;
  version = properties.get("@version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = properties.get("@changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = properties.get("@timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = properties.get("@user", user);
  long uid = ElementData::UID_EMPTY;
  uid = properties.get("@uid", uid);

  //  Create an empty set of properties
  pt::ptree empty;
  //  Construct Relation
  RelationPtr relation(
    new Relation(
      _defaultStatus, relation_id, _defaultCircErr, "", changeset, version, timestamp,
      QString::fromStdString(user), uid));

  //  Add the relation type and parse the roles
  // NOTE: This may be empty which will cause errors later. If it is empty, we add a type
  // later when we sort out what the geometry is - MultiPolygon etc
  string relation_type = properties.get("relation-type", "");
  relation->setType(relation_type.c_str());

  if (_roles.empty())
  {
    //  Get the roles and tokenize them by semicolon
    string roles_values = properties.get("roles", "");
    if (roles_values.compare("") != 0)
    {
      using _tokenizer = boost::tokenizer<boost::char_separator<char>>;
      _tokenizer tokens(roles_values, boost::char_separator<char>(";", nullptr, boost::keep_empty_tokens));
      for (_tokenizer::iterator it = tokens.begin(); it != tokens.end(); ++it)
        _roles.push(*it);
    }
    else
      _roles.push("");
  }
  //  Make sure that we have the 'GeometryCollection' with the 'geometries' tree inside
  string geo_type = geometry.get("type", "");
  if (geo_type == "GeometryCollection")
  {
    if (geometry.not_found() != geometry.find("geometries"))
    {
      bool use_old = _useDataSourceIds;
      _useDataSourceIds = true;
      pt::ptree geometries = geometry.get_child("geometries");
      long points = 0,
           polys = 0,
           linestrings = 0,
           other = 0;
      for (pt::ptree::const_iterator it = geometries.begin(); it != geometries.end(); ++it)
      {
        pt::ptree geo = it->second;
        string type = geo.get("type", "");
        //  Make sure that there is always at least a blank role
        if (_roles.empty())
          _roles.push("");
        QString role(_roles.front().c_str());
        _roles.pop();
        if (type == "Point")
        {
          //  Node
          long node_id = _map->createNextNodeId();
          string node = boost::lexical_cast<string>(node_id);
          _parseGeoJsonNode(node, empty, geo);
          relation->addElement(role, ElementType::Node, node_id);
          points++;
        }
        else if (type == "Polygon" ||
                 type == "LineString")
        {
          //  Way
          long way_id = _map->createNextWayId();
          string way = boost::lexical_cast<string>(way_id);
          _parseGeoJsonWay(way, empty, geo);
          relation->addElement(role, ElementType::Way, way_id);
          if (type == "Polygon")
            polys++;
          else
            linestrings++;
        }
        else if (type == "GeometryCollection")
        {
          //  Relation
          long r_id = _map->createNextRelationId();
          string rel = boost::lexical_cast<string>(r_id);
          _parseGeoJsonRelation(rel, empty, geo);
          relation->addElement(role, ElementType::Relation, r_id);
          other++;
        }
        else if (type == "MultiPoint" ||
                 type == "MultiLineString" ||
                 type == "MultiPolygon")
        {
          //  Also a Relation but cannot be a relation of relations
          long rid = _map->createNextRelationId();
          //  Construct Relation
          RelationPtr r(new Relation(_defaultStatus, rid, _defaultCircErr));
          if (type == "MultiPoint")
            _parseMultiPointGeometry(geo, r);
          else if (type == "MultiLineString")
            _parseMultiLineGeometry(geo, r);
          else if (type == "MultiPolygon")
            _parseMultiPolygonGeometry(geo, r);
          _map->addRelation(r);
          relation->addElement(role, ElementType::Relation, rid);
          other++;
        }
        else
        {
          LOG_WARN("Unsupported JSON geometry type (" << type << ") when parsing GeoJSON");
          other++;
        }
      }
      _useDataSourceIds = use_old;
      //  Update the relation type if needed
      if (relation_type == "")
      {
        if (points > 0 && polys == 0 && linestrings == 0 && other == 0)
          relation->setType(MetadataTags::RelationMultiPoint());
        else if (points == 0 && polys > 0 && linestrings == 0 && other == 0)
          relation->setType(MetadataTags::RelationMultiPolygon());
        else if (points == 0 && polys == 0 && linestrings > 0 && other == 0)
          relation->setType(MetadataTags::RelationMultilineString());
        else
          relation->setType(MetadataTags::RelationCollection());
      }
    }
  }
  else if (geo_type == "MultiPoint")
  {
    if (relation->getType() == "")
      relation->setType(MetadataTags::RelationMultiPoint());
    _parseMultiPointGeometry(geometry, relation);
  }
  else if (geo_type == "MultiLineString")
  {
    if (relation->getType() == "")
      relation->setType(MetadataTags::RelationMultilineString());
    _parseMultiLineGeometry(geometry, relation);
  }
  else if (geo_type == "MultiPolygon")
  {
    if (relation->getType() == "")
      relation->setType(MetadataTags::RelationMultiPolygon());
    _parseMultiPolygonGeometry(geometry, relation);
  }
  else
  {
    LOG_WARN("Unsupported JSON geometry type (" << geo_type << ") when parsing GeoJSON");
  }
  //  Add tags
  _addTags(properties, relation);
  LOG_VART(relation);
  //  Add relation to map
  _map->addRelation(relation);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmGeoJsonReader::_parseMultiPointGeometry(const boost::property_tree::ptree& geometry,
                                                const RelationPtr& relation)
{
  vector<JsonCoordinates> multigeo = _parseMultiGeometry(geometry);
  vector<JsonCoordinates>::const_iterator multi = multigeo.begin();
  for (JsonCoordinates::const_iterator coord = multi->begin(); coord != multi->end(); ++coord)
  {
    long node_id = _map->createNextNodeId();
    double lat = coord[0].y;
    double lon = coord[0].x;
    //  Construct node
    NodePtr pNode(new Node(_defaultStatus, node_id, lon, lat, _defaultCircErr));
    //  Add node to map
    _map->addNode(pNode);
    relation->addElement("", ElementType::Node, node_id);
  }
}

void OsmGeoJsonReader::_parseMultiLineGeometry(const boost::property_tree::ptree& geometry,
                                               const RelationPtr& relation)
{
  vector<JsonCoordinates> multigeo = _parseMultiGeometry(geometry);
  for (vector<JsonCoordinates>::const_iterator multi = multigeo.begin(); multi != multigeo.end(); ++multi)
  {
    long way_id = _map->createNextWayId();
    WayPtr way(new Way(_defaultStatus, way_id, _defaultCircErr));
    for (JsonCoordinates::const_iterator coord = multi->begin(); coord != multi->end(); ++coord)
    {
      long node_id = _map->createNextNodeId();
      double lat = coord[0].y;
      double lon = coord[0].x;
      //  Construct node
      NodePtr node(new Node(_defaultStatus, node_id, lon, lat, _defaultCircErr));
      //  Add node to map
      _map->addNode(node);
      way->addNode(node_id);
    }
    relation->addElement("", ElementType::Way, way_id);
  }
}

void OsmGeoJsonReader::_parseMultiPolygonGeometry(const boost::property_tree::ptree& geometry,
                                                  const RelationPtr& relation)
{
  vector<JsonCoordinates> multigeo = _parseMultiGeometry(geometry);
  for (vector<JsonCoordinates>::const_iterator multi = multigeo.begin(); multi != multigeo.end(); ++multi)
  {
    long way_id = _map->createNextWayId();
    WayPtr way(new Way(_defaultStatus, way_id, _defaultCircErr));
    for (JsonCoordinates::const_iterator coord = multi->begin(); coord != multi->end(); ++coord)
    {
      if ((coord + 1) == multi->end())
      {
        //  Don't create another node to close the polygon, just use the first one
        way->addNode(way->getNodeId(0));
      }
      else
      {
        long node_id = _map->createNextNodeId();
        NodePtr node(new Node(_defaultStatus, node_id, *coord, _defaultCircErr));
        _map->addNode(node);
        way->addNode(node_id);
      }
    }
    _map->addWay(way);
    relation->addElement("", ElementType::Way, way_id);
  }
}

JsonCoordinates OsmGeoJsonReader::_parseGeometry(const pt::ptree& geometry)
{
  JsonCoordinates results;
  string type = geometry.get("type", "");
  if (type == "Point")
  {
    //  Single coordinate should only have two elements, lat/lon
    pt::ptree coordinates = geometry.get_child("coordinates");
    pt::ptree::const_iterator it = coordinates.begin();
    double x = it->second.get_value<double>();
    ++it;
    double y = it->second.get_value<double>();
    results.emplace_back(x, y);
  }
  else if (type == "LineString")
  {
    //  Line string is a single array of coordinates (array)
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {    
      std::shared_ptr<Coordinate> pCoord = ReadCoordinate(it->second);
      if (pCoord)
        results.push_back(*pCoord);
    }
  }
  else if (type == "Polygon")
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      for (pt::ptree::const_iterator array = it->second.begin(); array != it->second.end(); ++array)
      {
        std::shared_ptr<Coordinate> pCoord = ReadCoordinate(array->second);
        if (pCoord)
          results.push_back(*pCoord);
      }
    }
  }
  else
  {
    LOG_WARN("Unsupported simple JSON geometry type (" << type << ") when parsing GeoJSON");
  }

  return results;
}

vector<JsonCoordinates> OsmGeoJsonReader::_parseMultiGeometry(const pt::ptree& geometry)
{
  vector<JsonCoordinates> results;
  string type = geometry.get("type", "");
  if (type == "MultiPoint")
  {
    //  MultiPoint is a single array of coordinates (array)
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      JsonCoordinates point;
      std::shared_ptr<Coordinate> pCoord = ReadCoordinate(it->second);
      if (pCoord)
        point.push_back(*pCoord);
      results.push_back(point);
    }
  }
  else if (type == "MultiLineString")
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      JsonCoordinates line;
      for (pt::ptree::const_iterator array = it->second.begin(); array != it->second.end(); ++array)
      {
        std::shared_ptr<Coordinate> pCoord = ReadCoordinate(array->second);
        if (pCoord)
          line.push_back(*pCoord);
      }
      results.push_back(line);
    }
  }
  else if (type == "MultiPolygon")
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      JsonCoordinates polygon;
      for (pt::ptree::const_iterator array = it->second.begin(); array != it->second.end(); ++array)
      {
        for (pt::ptree::const_iterator poly = array->second.begin(); poly != array->second.end(); ++poly)
        {
          std::shared_ptr<Coordinate> pCoord = ReadCoordinate(poly->second);
          if (pCoord)
            polygon.push_back(*pCoord);
        }
      }
      results.push_back(polygon);
    }
  }
  else
  {
    LOG_WARN("Unsupported multi JSON geometry type (" << type << ") when parsing GeoJSON");
  }
  return results;
}

void OsmGeoJsonReader::_addTags(const pt::ptree& item, const ElementPtr& element)
{
  //  Starts with the "properties" tree, use the "tags" subtree if it exists
  //  otherwise just use the "properties" tree as tags
  if (item.not_found() != item.find("tags"))
    OsmJsonReader::_addTags(item, element);
  else
  {
    for (pt::ptree::const_iterator tagIt = item.begin(); tagIt != item.end(); ++tagIt)
    {
      QString key = QString::fromStdString(tagIt->first).trimmed();
      //LOG_VART(key);
      // We've already parsed properties that have keys starting with '@' as metadata for the
      // feature.
      if (!key.startsWith("@"))
      {
        QString value;
        if (tagIt->second.begin() != tagIt->second.end())
          value = QString::fromStdString(_parseSubTags(tagIt->second)).trimmed();
        else
          value = QString::fromStdString(tagIt->second.get_value<string>()).trimmed();
        element->setTag(key, value);
      }
    }
  }
}

string OsmGeoJsonReader::_parseSubTags(const pt::ptree& item)
{
  stringstream ss;
  bool isObject = false;
  bool isArray = true;
  for (pt::ptree::const_iterator it = item.begin(); it != item.end(); ++it)
  {
    if (it != item.begin())
      ss << ",";
    else if (it->first != "")
    {
      isObject = true;
      isArray = false;
    }
    if (!isArray)
      ss << "\"" << it->first << "\":";
    if (it->second.get_value<string>() != "")
      ss << "\"" << it->second.get_value<string>() << "\"";
    else
      ss << _parseSubTags(it->second);
  }
  if (isObject)
    return "{" + ss.str() + "}";
  else if (isArray)
    return "[" + ss.str() + "]";
  else
    return ss.str();
}

std::shared_ptr<Coordinate> OsmGeoJsonReader::ReadCoordinate(const pt::ptree& coordsIt) const
{
  std::shared_ptr<Coordinate> pCoord;

  double x = 0;
  double y = 0;

  pt::ptree::const_iterator coord = coordsIt.begin();

  // according to GeoJson specs, RFC7946, Section 9 we need to expect x,y,z and unc values but we
  // only care about x and y
  if (coord != coordsIt.end())
  {
    x = coord->second.get_value<double>();
    ++coord;

    if (coord != coordsIt.end())
    {
      y = coord->second.get_value<double>();
      pCoord = std::shared_ptr<Coordinate>( new Coordinate(x, y) );
    }
  }

  return pCoord;
}

}

