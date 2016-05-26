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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmJsonReader.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QStringList>

// Standard
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
using namespace std;

namespace hoot
{

// Default constructor
OsmJsonReader::OsmJsonReader()
{
  // Do nothing
}

// We allow the use of single quotes, for ease of coding
// test strings into c++. Single quotes within string literals
// should be escaped as \'
void scrubQuotes(QString &jsonStr)
{
  // Detect if they are using single quotes or doubles
  if (jsonStr.indexOf("\"node\"", Qt::CaseInsensitive) > -1)
    return; // No need to scrub
  else
  {
    // Convert single quotes to double quotes
    // First change escaped singles
    jsonStr.replace("\\'", "\"\"\"");
    // Replace singles with doubles
    jsonStr.replace("'", "\"");
    // Revert escaped singles
    jsonStr.replace("\"\"\"", "'");
  }
}

// Throws HootException on error
void OsmJsonReader::_loadJSON(QString jsonStr)
{
  scrubQuotes(jsonStr);

  // Convert string to stringstream
  stringstream ss(jsonStr.toUtf8().constData(), ios::in);

  if (!ss.good())
  {
    throw HootException(QString("Error reading from JSON string:\n%1").arg(jsonStr));
  }

  try
  {
    pt::read_json(ss, _propTree);
  }
  catch (pt::json_parser::json_parser_error e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());
    throw HootException("Error parsing JSON " + reason + "(line " + line + ")");
  }
  catch (std::exception e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
}

OsmMapPtr OsmJsonReader::loadFromString(QString jsonStr)
{
  _loadJSON(jsonStr);
  return _parseOverpassJson();
}

OsmMapPtr OsmJsonReader::_parseOverpassJson()
{
  // Overpass has 4 top level items: version, generator, osm3s, elements
  _version = _propTree.get("version", string(""));
  _generator = _propTree.get("generator", string(""));
  _timestamp_base = _propTree.get("osm3s.timestamp_osm_base", string(""));
  _copyright = _propTree.get("osm3s.copyright", string(""));

  // Make a map, and iterate through all of our elements, adding them
  OsmMapPtr pMap(new OsmMap());
  pt::ptree elements = _propTree.get_child("elements");
  pt::ptree::const_iterator elementIt = elements.begin();
  while (elementIt != elements.end())
  {
    // Type can be node, way, or relation
    string typeStr = elementIt->second.get("type", string("--"));

    if ("node" == typeStr)
    {
      parseOverpassNode(elementIt->second, pMap);
    }
    else if ("way" == typeStr)
    {
      parseOverpassWay(elementIt->second, pMap);
    }
    else if ("relation" == typeStr)
    {
      parseOverpassRelation(elementIt->second, pMap);
    }
    else
    {
      // Log unknown element
      LOG_WARN("Unknown JSON elment type (" << typeStr << ") when parsing json osm");
    }
    ++elementIt;
  }

  return pMap;
}

void OsmJsonReader::parseOverpassNode(const pt::ptree &item, OsmMapPtr pMap)
{
  // Get info we need to construct our node
  long id = item.get("id", -1l); // default to -1
  double lat = item.get("lat", 0.0);
  double lon = item.get("lon", 0.0);
  Meters circErr = 15.0; // Appears to be default used (e.g., see PbfReader)
  Status s;

  // Construct node
  NodePtr pNode(new Node(s, id, lon, lat, circErr));

  // Add tags
  addTags(item, pNode);

  // Add node to map
  pMap->addNode(pNode);
}

void OsmJsonReader::parseOverpassWay(const pt::ptree &item, OsmMapPtr pMap)
{
  // Get info we need to construct our way
  long id = item.get("id", -1l); // default to -1
  Meters circErr = 15.0; // Appears to be default used (e.g., see PbfReader)
  Status s;

  // Construct Way
  WayPtr pWay(new Way(s, id, circErr));

  // Add nodes
  if (item.not_found() != item.find("nodes"))
  {
    pt::ptree nodes = item.get_child("nodes");
    pt::ptree::const_iterator nodeIt = nodes.begin();
    while (nodeIt != nodes.end())
    {
      string k = nodeIt->first;
      long v = nodeIt->second.get_value<long>();
      pWay->addNode(v);
      ++nodeIt;
    }
  }

  // Add tags
  addTags(item, pWay);

  // Add way to map
  pMap->addWay(pWay);
}

void OsmJsonReader::parseOverpassRelation(const pt::ptree &item, OsmMapPtr pMap)
{
  // Get info we need to construct our way
  long id = item.get("id", -1l); // default to -1
  Meters circErr = 15.0; // Appears to be default used (e.g., see PbfReader)
  Status s;

  // Construct Relation
  RelationPtr pRelation(new Relation(s, id, circErr));

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
  }

  // Add tags
  addTags(item, pRelation);

  // Add relation to map
  pMap->addRelation(pRelation);
}

void OsmJsonReader::addTags(const boost::property_tree::ptree &item, hoot::ElementPtr pElement)
{
  // Find tags and add them
  if (item.not_found() != item.find("tags"))
  {
    pt::ptree tags = item.get_child("tags");
    pt::ptree::const_iterator tagIt = tags.begin();
    while (tagIt != tags.end())
    {
      string k = tagIt->first;
      string v = tagIt->second.get_value<string>();

      // If we are "error:circular", need to set it on the element object,
      // rather than add it as a tag
      if (k == "error:circular")
      {
        pElement->setCircularError(Meters(QString::fromStdString(v).toInt()));
      }
      else
      {
        pElement->setTag(QString::fromStdString(k), QString::fromStdString(v));
      }
      ++tagIt;
    }
  }
}

} // namespace hoot

