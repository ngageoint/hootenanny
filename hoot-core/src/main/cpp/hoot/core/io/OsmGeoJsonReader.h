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
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/io/OsmJsonReader.h>

namespace hoot
{

/**
 * This class is intended to create an OsmMap from a given json string. JSON
 * output from the overpass-api was used as the model for development
 * (http://overpass-api.de/output_formats.html#json).
 *
 * The input string must be well-formed JSON, with the exception that it can
 * be coded using single quotes, rather than double quotes... which makes
 * things a lot cleaner if you are hand-jamming the JSON string into c++ code.
 * If you are using single quotes, you may escape apostrophes with a backslash.
 * Consider this example:
 *
 * QString testJsonStr =
 *   "{                                      \n"
 *   " 'version': 0.6,                       \n"
 *   " 'generator': 'Overpass API',          \n"
 *   " 'osm3s': {                            \n"
 *   "   'timestamp_osm_base': 'date',       \n"
 *   "   'copyright': 'c 1999'               \n"
 *   " },                                    \n"
 *   " 'elements': [                         \n"
 *   " {                                     \n"
 *   "   'type': 'node',                     \n"
 *   "   'id': -1,                           \n"
 *   "   'lat': 2.0,                         \n"
 *   "   'lon': -3.0                         \n"
 *   " },                                    \n"
 *   " {                                     \n"
 *   "   'type': 'node',                     \n"
 *   "   'id': -2,                           \n"
 *   "   'lat': 3.0,                         \n"
 *   "   'lon': -3.0,                        \n"
 *   "   'timestamp': '2010-01-01T00:00:00Z',\n"
 *   "   'version': 4,                       \n"
 *   "   'changeset': 5,                     \n"
 *   "   'user': 'somebody',                 \n"
 *   "   'uid': 6                            \n"
 *   " },                                    \n"
 *   " {                                     \n"
 *   "   'type': 'node',                     \n"
 *   "   'id': -3,                           \n"
 *   "   'lat': 4.0,                         \n"
 *   "   'lon': -3.0,                        \n"
 *   "   'tags': {                           \n"
 *   "     'highway': 'bus_stop',            \n"
 *   "     'name': 'Micah\\'s Street'        \n"
 *   "   }                                   \n"
 *   " }                                     \n"
 *   "]                                      \n"
 *   "}                                      \n";
 *
 * It's all-or-nothing, though for the quotes - don't mix and match
 * singles and doubles! Also, be aware that this class doesn't do anything
 * clever to handle large datasets - it simply keeps everything in memory.
 * Be careful if you want to use it with large datasets.
 */

class OsmGeoJsonReader : public OsmJsonReader
{
public:

  static std::string className() { return "hoot::OsmGeoJsonReader"; }

  OsmGeoJsonReader()          { }   //  Taken care of in OsmJsonReader
  virtual ~OsmGeoJsonReader() { }   //  Taken care of in OsmJsonReader

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
   * @brief _loadJSON Loads JSON into a boost property tree
   * @param jsonStr String to load
   */
  void _loadJSON(QString jsonStr);

  /**
   * @brief parseOverpassJson Traverses our property tree and adds
   *        elements to the map
   * @param pMap Append elements to this map
   */
  void _parseOverpassJson(OsmMapPtr pMap);

  /**
   * @brief _parseOverpassNode Reads node info out of the property tree and
   *        builds a Node object. Adds the node to the map.
   * @param item Property Tree (likely a sub-tree)
   * @param pMap Map to which we add the node
   */
  void _parseOverpassNode(const boost::property_tree::ptree &item,
                          OsmMapPtr pMap);

  /**
   * @brief _parseOverpassWay Reads way info out of the property tree and
   *        builds a Way object. Adds the way to the map.
   * @param item Property Tree (or sub-tree)
   * @param pMap Map to which we add the way
   */
  void _parseOverpassWay(const boost::property_tree::ptree &item,
                         OsmMapPtr pMap);

  /**
   * @brief _parseOverpassRelation Reads relation info out of the property tree
   *        and builds a Relation object. Adds relation to the map.
   * @param item Property Tree (likely a subtree)
   * @param pMap Map to which we add the Relation
   */
  void _parseOverpassRelation(const boost::property_tree::ptree &item,
                              OsmMapPtr pMap);

  /**
   * @brief _addTags Reads tags from the given ptree, and adds them to the
   *        supplied map element
   * @param item Property Tree (subtree)
   * @param pElement Element to which we will add the tags
   */
  void _addTags(const boost::property_tree::ptree &item,
                hoot::ElementPtr pElement);

};

} // end namespace hoot

#endif // OSM_GEOJSON_READER_H
