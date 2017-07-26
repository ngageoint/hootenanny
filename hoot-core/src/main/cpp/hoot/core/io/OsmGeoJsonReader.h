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

#ifndef OSM_GEOJSON_READER_H
#define OSM_GEOJSON_READER_H

// Qt
#include <QUrl>
#include <QFile>
#include <QString>

// Standard
#include <queue>
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmJsonReader.h>

namespace hoot
{

/**
 * This class is intended to create an OsmMap from a given GeoJSON string.
 *
 * The input string must be well-formed GeoJSON.  Also, be aware that this
 * class doesn't do anything clever to handle large datasets - it simply keeps
 * everything in memory. Be careful if you want to use it with large datasets.
 */

typedef std::vector<geos::geom::Coordinate> JsonCoordinates;

class OsmGeoJsonReader : public OsmJsonReader
{
public:

  static std::string className() { return "hoot::OsmGeoJsonReader"; }

  OsmGeoJsonReader();
  virtual ~OsmGeoJsonReader();

  /**
   * @brief isSupported returns true if the URL is likely supported. This isn't
   *        an exhaustive check - it just looks to see if the URL appears to be
   *        a file, or web address
   * @param url
   * @return
   */
  virtual bool isSupported(QString url);

  /**
   * @brief read Reads the data specified by the last call to open(...)
   *        and adds the elements to the provided map. Input data source
   *        will likely be closed after this call
   * @param map
   */
  virtual void read(OsmMapPtr map);

  /**
   * @brief loadFromString - Builds a map from the JSON string. Throws a
   *        HootException with error and line number if JSON parsing fails
   * @param jsonStr - input string
   * @return Smart pointer to the OSM map
   */
  OsmMapPtr loadFromString(QString jsonStr);

  /**
   * @brief loadFromFile - Reads the whole file as a string, passes it
   *        to loadFromString()
   * @param path - Path to file
   * @return Smart pointer to the OSM map
   */
  OsmMapPtr loadFromFile(QString path);

private:

  /**
   * @brief parseOverpassJson Traverses our property tree and adds
   *        elements to the map
   */
  void _parseGeoJson();

  /**
   * @brief _parseGeoJsonNode Reads node info out of the property tree and
   *        builds a Node object. Adds the node to the map.
   * @param id Element ID string
   * @param properties Property tree (including tags)
   * @param geometry Geometry Tree of Point, LineString, Polygon, or GeometryCollection
   */
  void _parseGeoJsonNode(const std::string& id,
                         const boost::property_tree::ptree& properties,
                         const boost::property_tree::ptree& geometry);

  /**
   * @brief _parseGeoJsonWay Reads way info out of the property tree and
   *        builds a Way object. Adds the way to the map.
   * @param id Element ID string
   * @param properties Property tree (including tags)
   * @param geometry Geometry Tree of Point, LineString, Polygon, or GeometryCollection
   */
  void _parseGeoJsonWay(const std::string& id,
                        const boost::property_tree::ptree& properties,
                        const boost::property_tree::ptree& geometry);

  /**
   * @brief _parseGeoJsonRelation Reads relation info out of the property tree
   *        and builds a Relation object. Adds relation to the map.
   * @param id Element ID string
   * @param properties Property tree (including tags)
   * @param geometry Geometry Tree of Point, LineString, Polygon, or GeometryCollection
   */
  void _parseGeoJsonRelation(const std::string& id,
                             const boost::property_tree::ptree& properties,
                             const boost::property_tree::ptree& geometry);

  /**
   * @brief _parseGeometry Parse the simple geometry object into a vector of coordinates
   * @param geometry Tree of simple geometry in JSON format
   * @return Vector of coordinates
   */
  JsonCoordinates _parseGeometry(const boost::property_tree::ptree& geometry);

  /**
   * @brief _parseMulti*Geometry Parse the multi geometries into a vector of coordinates
   *  for each geometry that is pushed into a vector
   * @param geometry Tree of multi-geometry in JSON format
   * @param relation OSM relation to represent the multi-geometry that all elements are added to
   */
  void _parseMultiPointGeometry(const boost::property_tree::ptree& geometry, const RelationPtr& relation);
  void _parseMultiLineGeometry(const boost::property_tree::ptree& geometry, const RelationPtr& relation);
  void _parseMultiPolygonGeometry(const boost::property_tree::ptree& geometry, const RelationPtr& relation);

  /**
   * @brief _parseMultiGeometry Parse "Multi" geometry object into a vector of vectors of coordiantes
   * @param geometry Three of "multi" geometry in JSON format
   * @return Vector of vectors of coordinates
   */
  std::vector<JsonCoordinates> _parseMultiGeometry(const boost::property_tree::ptree& geometry);

  /**
   * @brief _parseBbox Parse the bounding box array in JSON format into Geos Envelope
   * @param bbox Tree of bounding box JSON array
   * @return Bounding box as a usable Envelope
   */
  geos::geom::Envelope _parseBbox(const boost::property_tree::ptree& bbox);

  /**
   * @brief _addTags Reads tags or properties from the given ptree, and adds them to the
   *        supplied map element
   * @param item Property Tree (subtree)
   * @param element Element to which we will add the tags
   */
  void _addTags(const boost::property_tree::ptree &item, ElementPtr element);

  /**
   * @brief _roles List of roles for the current relation, saved for recursive relations
   */
  std::queue<std::string> _roles;

};

} // end namespace hoot

#endif // OSM_GEOJSON_READER_H
