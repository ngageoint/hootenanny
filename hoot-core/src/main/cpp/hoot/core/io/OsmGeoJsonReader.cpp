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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmGeoJsonReader.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/util/Factory.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

// Qt
#include <QTextStream>
#include <QTextCodec>
#include <QEventLoop>
#include <QTextStream>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>

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

HOOT_FACTORY_REGISTER(OsmJsonReader, OsmGeoJsonReader)

OsmGeoJsonReader::OsmGeoJsonReader() : OsmJsonReader()
{
}

OsmGeoJsonReader::~OsmGeoJsonReader()
{
}

bool OsmGeoJsonReader::isSupported(QString url)
{
  QUrl myUrl(url);

  // Is it a file?
  if (myUrl.isRelative() || myUrl.isLocalFile())
  {
    QString filename = myUrl.toLocalFile();

    if (QFile::exists(filename) && url.endsWith(".geojson", Qt::CaseInsensitive))
      return true;
  }

  // Is it a web address?
  if ("http" == myUrl.scheme() || "https" == myUrl.scheme())
  {
    return true;
  }

  // Default to not supported
  return false;
}

/**
 * Reads the specified map. When this method is complete
 * the input will likely be closed.
 */
void OsmGeoJsonReader::read(OsmMapPtr map)
{
  _map = map;
  QString jsonStr;
  if (_isFile)
  {
    QTextStream instream(&_file);
    jsonStr = instream.readAll();
  }
  else
  {
    // Do HTTP GET request
    boost::shared_ptr<QNetworkAccessManager> pNAM(new QNetworkAccessManager());
    QNetworkRequest request(_url);
    boost::shared_ptr<QNetworkReply> pReply(pNAM->get(request));

    // Wait for finished signal from reply object
    QEventLoop loop;
    QObject::connect(pReply.get(), SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // Check error status on our reply
    if (QNetworkReply::NoError != pReply->error())
    {
      QString errMsg = pReply->errorString();
      throw HootException(QString("Network error for GET request (%1): %2").arg(_url.toString()).arg(errMsg));
    }

    QByteArray data = pReply->readAll();
    jsonStr = QString::fromAscii(data.data());
  }
  _loadJSON(jsonStr);
  _parseGeoJson();
}

OsmMapPtr OsmGeoJsonReader::loadFromString(QString jsonStr)
{
  _loadJSON(jsonStr);
  _map.reset(new OsmMap());
  _parseGeoJson();
  return _map;
}

OsmMapPtr OsmGeoJsonReader::loadFromFile(QString path)
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
  // Overpass has 4 top level items: version, generator, osm3s, elements
  _generator = QString::fromStdString(_propTree.get("generator", string("")));
  QString type = QString::fromStdString(_propTree.get("type", string("")));
  Envelope env = _parseBbox(_propTree.get_child("bbox"));

  // Make a map, and iterate through all of our features, adding them
  pt::ptree features = _propTree.get_child("features");
  for (pt::ptree::const_iterator featureIt = features.begin(); featureIt != features.end(); ++featureIt)
  {
    const pt::ptree& feature = featureIt->second;
    string id = feature.get("id", string(""));
    if (feature.not_found() != feature.find("properties"))
    {
      pt::ptree properties = feature.get_child("properties");

      if (feature.not_found() != feature.find("geometry"))
      {
        pt::ptree geometry = feature.get_child("geometry");

        // Type can be node, way, or relation
        string typeStr = properties.get("type", string("--"));
        if ("node" == typeStr)
        {
          _parseGeoJsonNode(id, properties, geometry);
        }
        else if ("way" == typeStr)
        {
          _parseGeoJsonWay(id, properties, geometry);
        }
        else if ("relation" == typeStr)
        {
          _parseGeoJsonRelation(id, properties, geometry);
        }
        else
        {
          if (logWarnCount < ConfigOptions().getLogWarnMessageLimit())
          {
            LOG_WARN("Unknown JSON elment type (" << typeStr << ") when parsing GeoJSON");
          }
          else if (logWarnCount == ConfigOptions().getLogWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
      }
    }
  }
}

geos::geom::Envelope OsmGeoJsonReader::_parseBbox(const boost::property_tree::ptree& bbox)
{
  for (pt::ptree::const_iterator bboxIt = bbox.begin(); bboxIt != bbox.end(); ++bboxIt)
  {
    double minX = bboxIt->second.get_value<double>();
    ++bboxIt;
    double minY = bboxIt->second.get_value<double>();
    ++bboxIt;
    double maxX = bboxIt->second.get_value<double>();
    ++bboxIt;
    double maxY = bboxIt->second.get_value<double>();
    return Envelope(minX, minY, maxX, maxY);
  }
  return Envelope();
}

void OsmGeoJsonReader::_parseGeoJsonNode(const string& id, const pt::ptree& properties, const pt::ptree& geometry)
{
  // Get info we need to construct our node
  long node_id = -1;
  if (_useDataSourceIds)
    node_id = boost::lexical_cast<long>(id);
  else
    node_id = _map->createNextNodeId();

  vector<Coordinate> coords = _parseGeometry(geometry);
  double lat = coords[0].y;
  double lon = coords[0].x;

  // Construct node
  NodePtr pNode(new Node(_defaultStatus, node_id, lon, lat, _defaultCircErr));

  // Add tags
  _addTags(properties, pNode);

  // Add node to map
  _map->addNode(pNode);
}

void OsmGeoJsonReader::_parseGeoJsonWay(const string& id, const pt::ptree& properties, const pt::ptree& geometry)
{
  // Get info we need to construct our way
  long way_id = -1;
  if (_useDataSourceIds)
    way_id = boost::lexical_cast<long>(id);
  else
    way_id = _map->createNextWayId();

  // Construct Way
  WayPtr pWay(new Way(_defaultStatus, way_id, _defaultCircErr));

  bool isPoly = (geometry.get("type", "").compare("Polygon") == 0);
  // Add nodes
  vector<Coordinate> coords = _parseGeometry(geometry);
  for (vector<Coordinate>::iterator it = coords.begin(); it != coords.end(); ++it)
  {
    if (isPoly && (it + 1) == coords.end())
    {
      //  Don't create another node to close the polygon, just use the first one
      pWay->addNode(pWay->getNodeId(0));
    }
    else
    {
      long node_id = _map->createNextNodeId();
      NodePtr pNode(new Node(_defaultStatus, node_id, *it, _defaultCircErr));
      _map->addNode(pNode);
      pWay->addNode(node_id);
    }
  }

  // Add tags
  _addTags(properties, pWay);

  // Add way to map
  _map->addWay(pWay);
}

void OsmGeoJsonReader::_parseGeoJsonRelation(const string& id, const pt::ptree& properties, const pt::ptree& geometry)
{
  // Get info we need to construct our relation
  long relation_id = -1;
  if (_useDataSourceIds)
    relation_id = boost::lexical_cast<long>(id);
  else
    relation_id = _map->createNextRelationId();

  // Construct Relation
  RelationPtr pRelation(new Relation(_defaultStatus, relation_id, _defaultCircErr));

  if (geometry.get("type", "").compare("GeometryCollection") == 0)
  {
    if (geometry.not_found() != geometry.find("geometries"))
    {
      pt::ptree geometries = geometry.get_child("geometries");
      for (pt::ptree::const_iterator it = geometries.begin(); it != geometries.end(); ++it)
      {
        pt::ptree geo = it->second;
        string type = geo.get("type", "");
        if (type.compare("Point") == 0)
        {
          //  Node
        }
        else if (type.compare("Polygon") == 0 || type.compare("LineString") == 0)
        {
          //  Way
        }
        else if (type.compare("GeometryCollection") == 0)
        {
          //  Relation
        }
      }
    }
/*
  // Add members
  if (item.not_found() != item.find("members"))
  {
    pt::ptree members = item.get_child("members");
    pt::ptree::const_iterator memberIt = members.begin();
    while (memberIt != members.end())
    {
      string typeStr = memberIt->second.get("type", string(""));
      long ref = memberIt->second.get("ref", -1l); // default -1 ?
      string role = memberIt->second.get("role", string(""));

      pRelation->addElement(QString::fromStdString(role),
                            ElementType::fromString(QString::fromStdString(typeStr)),
                            ref);
      ++memberIt;
    }
*/
  }

  // Add tags
  _addTags(properties, pRelation);

  // Add relation to map
  _map->addRelation(pRelation);
}

vector<Coordinate> OsmGeoJsonReader::_parseGeometry(const pt::ptree& geometry)
{
  vector<Coordinate> results;

  string type = geometry.get("type", "");
  if (type.compare("Point") == 0)
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      double x = it->second.get_value<double>();
      ++it;
      double y = it->second.get_value<double>();
      results.push_back(Coordinate(x, y));
    }
  }
  else if (type.compare("Polygon") == 0)
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      for (pt::ptree::const_iterator array = it->second.begin(); array != it->second.end(); ++array)
      {
        for (pt::ptree::const_iterator coord = array->second.begin(); coord != array->second.end(); ++coord)
        {
          double x = coord->second.get_value<double>();
          ++coord;
          double y = coord->second.get_value<double>();
          results.push_back(Coordinate(x, y));
        }
      }
    }
  }
  else if (type.compare("LineString") == 0)
  {
    pt::ptree coordinates = geometry.get_child("coordinates");
    for (pt::ptree::const_iterator it = coordinates.begin(); it != coordinates.end(); ++it)
    {
      for (pt::ptree::const_iterator coord = it->second.begin(); coord != it->second.end(); ++coord)
      {
        double x = coord->second.get_value<double>();
        ++coord;
        double y = coord->second.get_value<double>();
        results.push_back(Coordinate(x, y));
      }
    }
  }
  else
  {
  }

  return results;
}

} // namespace hoot

