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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OSM_JSON_READER_H
#define OSM_JSON_READER_H

// Qt
#include <QUrl>
#include <QFile>
#include <QString>

// Standard
#include <mutex>
#include <string>

// Boost
#include <boost/property_tree/ptree.hpp>

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/ParallelBoundedApiReader.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Boundable.h>

namespace hoot
{

/**
 * This class is intended to create an OsmMap from a given json string. JSON output from the
 * overpass-api was used as the model for development
 * (http://overpass-api.de/output_formats.html#json).
 *
 * Element type ordering (element children before parents) is not guaranteed with JSON, as it is
 * with XML.
 *
 * The input string must be well-formed JSON, with the exception that it can be coded using single
 * quotes, rather than double quotes... which makes things a lot cleaner if you are hand-jamming the
 * JSON string into c++ code. If you are using single quotes, you may escape apostrophes with a
 * backslash.
 *
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
 *   "     'name': 'Mike\\'s Street'        \n"
 *   "   }                                   \n"
 *   " }                                     \n"
 *   "]                                      \n"
 *   "}                                      \n";
 *
 * It's all-or-nothing, though for the quotes - don't mix and match singles and doubles!
 *
 * Also, be aware that this class doesn't do anything clever to handle large datasets - it simply
 * keeps everything in memory. Be careful if you want to use it with large datasets.
 */
class OsmJsonReader : public OsmMapReader, public Boundable, private ParallelBoundedApiReader
{
public:

  static std::string className() { return "hoot::OsmJsonReader"; }

  static int logWarnCount;

  OsmJsonReader();
  virtual ~OsmJsonReader();

  /**
   * @brief isSupported returns true if the URL is likely supported. This isn't
   *        an exhaustive check - it just looks to see if the URL appears to be
   *        a file, or web address
   * @param url
   * @return
   */
  virtual bool isSupported(const QString& url) override;

  /**
   * @brief open Specifies the URL to read from. Can be a file (file://some/path/to/a/file)
   *        or a json provider like:
   *    http://overpass-api.de/api/interpreter?data=[out:json];way(35.2097,-120.6207,35.2241,-120.5955);out;
   * @param url
   */
  virtual void open(const QString& url) override;

  /**
   * @brief close close internal filehandle, if needed
   */
  void close();

  /**
   * @brief read Reads the data specified by the last call to open(...)
   *        and adds the elements to the provided map. Input data source
   *        will likely be closed after this call
   * @param map
   */
  virtual void read(const OsmMapPtr& map) override;

  /**
   * @brief setDefaultStatus Sets the default status to use for elements
   *        in the map
   * @param status
   */
  virtual void setDefaultStatus(Status status) override { _defaultStatus = status; }

  /**
   * @brief setUseDataSourceIds Sets whether the reader should use the element IDs
   *        from the data being read, or self-generate unique IDs
   * @param useDataSourceIds true to use source IDs
   */
  virtual void setUseDataSourceIds(bool useDataSourceIds) override
  { _useDataSourceIds = useDataSourceIds; }

  /**
   * @brief loadFromString - Builds a map from the JSON string. Throws a
   *        HootException with error and line number if JSON parsing fails
   * @param jsonStr - input string, map - The OSM map to load it into
   */
  virtual void loadFromString(const QString& jsonStr, const OsmMapPtr& map);

  /**
   * @brief loadFromPtree - Builds a map from the supplied boost property tree
   * @param ptree - input boost property tree
   * @return Smart pointer to the OSM map
   */
  virtual OsmMapPtr loadFromPtree(const boost::property_tree::ptree &tree);

  /**
   * @brief loadFromFile - Reads the whole file as a string, passes it
   *        to loadFromString()
   * @param path - Path to file
   * @return Smart pointer to the OSM map
   */
  virtual OsmMapPtr loadFromFile(const QString& path);

  /**
   * @brief getVersion Overpass API version, if that's where JSON comes from
   * @return version string
   */
  QString getVersion() { return _version; }

  /**
   * @brief getGenerator Map generator name, if supplied in JSON
   * @return generator name string
   */
  QString getGenerator() { return _generator; }

  /**
   * @brief getTimestampBase OSM timestamp base, if supplied in JSON
   * @return timestamp base string
   */
  QString getTimestampBase() { return _timestamp_base; }

  /**
   * @brief getCopyright Copyright statement, if supplied in JSON
   * @return copyright string
   */
  QString getCopyright() { return _copyright; }

  /**
   * @brief scrubQuotes Converts single quotes to double quotes, and escaped
   *         apostrophes to regular apostrophes
   * @param jsonStr proper JSON string
   */
  static void scrubQuotes(QString &jsonStr);

  /**
   * @brief scrubBigInts Ensures that we have quotes around big integers.
   *        Numbers > 2^31 seem to cause trouble with the boost property_tree
   *        json parser in boost 1.41
   * @param jsonStr string upon which we operate
   */
  static void scrubBigInts(QString &jsonStr);

  virtual QString supportedFormats() override { return ".json"; }

  /**
   * Set the configuration for this object.
   */
  virtual void setConfiguration(const Settings& conf) override;

  virtual void setBounds(const std::shared_ptr<geos::geom::Geometry>& bounds) { _bounds = bounds; }

  void setKeepImmediatelyConnectedWaysOutsideBounds(bool keep)
  { _keepImmediatelyConnectedWaysOutsideBounds = keep; }
  void setLogWarningsForMissingElements(bool log) { _logWarningsForMissingElements = log; }

  bool isValidJson(const QString& jsonStr);

protected:

  // Items to conform to OsmMapReader ifc
  Status _defaultStatus;
  bool _useDataSourceIds;

  // the CE value used if no CE tag is found
  Meters _defaultCircErr;
  // keys for tags containing CE data
  QStringList _circularErrorTagKeys;

  // Our property tree that holds JSON
  boost::property_tree::ptree _propTree;

  // Store these items from overpass api
  QString _version;
  QString _generator;
  QString _timestamp_base;
  QString _copyright;

  bool _isFile;
  bool _isWeb;
  QFile _file;

  OsmMapPtr _map;

  long _numRead;
  long _statusUpdateInterval;

  /** List of JSON strings, one for each HTTP response */
  QStringList _results;

  std::shared_ptr<geos::geom::Geometry> _bounds;
  // only valid is _bounds is not null
  bool _keepImmediatelyConnectedWaysOutsideBounds;

  /// Maps from old node ids to new node ids.
  QHash<long, long> _nodeIdMap;
  QHash<long, long> _relationIdMap;
  QHash<long, long> _wayIdMap;

  // If we aren't using element source IDs and a child element hasn't been parsed yet, map the
  // parent element to the missing child element's ID so that we may later update the child ID
  // with the newer remapped ID.
  QMultiHash<long, long> _wayIdsToWayNodeIdsNotPresent;
  QMultiHash<long, long> _relationIdsToNodeMemberIdsNotPresent;
  QMultiHash<long, long> _relationIdsToWayMemberIdsNotPresent;
  QMultiHash<long, long> _relationIdsToRelationMemberIdsNotPresent;

  int _missingNodeCount;
  int _missingWayCount;

  //adds child refs to elements when they aren't present in the source data
  bool _addChildRefsWhenMissing;
  // determines whether missing elements trigger a warning
  bool _logWarningsForMissingElements;

  /**
   * @brief _loadJSON Loads JSON into a boost property tree
   * @param jsonStr String to load
   */
  void _loadJSON(const QString& jsonStr);

  /**
   * @brief parseOverpassJson Traverses our property tree and adds elements to the map. Removes
   * child elements ref'd by parents that don't actually exist
   */
  void _parseOverpassJson();

  /**
   * @brief _parseOverpassNode Reads node info out of the property tree and
   *        builds a Node object. Adds the node to the map.
   * @param item Property Tree (likely a sub-tree)
   */
  void _parseOverpassNode(const boost::property_tree::ptree &item);

  /**
   * @brief _parseOverpassWay Reads way info out of the property tree and
   *        builds a Way object. Adds the way to the map.
   * @param item Property Tree (or sub-tree)
   */
  void _parseOverpassWay(const boost::property_tree::ptree &item);

  /**
   * @brief _parseOverpassRelation Reads relation info out of the property tree
   *        and builds a Relation object. Adds relation to the map.
   * @param item Property Tree (likely a subtree)
   */
  void _parseOverpassRelation(const boost::property_tree::ptree &item);

  /**
   * @brief _addTags Reads tags from the given ptree, and adds them to the
   *        supplied map element
   * @param item Property Tree (subtree)
   * @param pElement Element to which we will add the tags
   */
  void _addTags(const boost::property_tree::ptree &item,
                hoot::ElementPtr pElement);

  /**
   * @brief _readFromHttp Creates HTTP(S) connection and downloads JSON to the _results list or
   *   spawns a thread pool to query bounding boxes
   */
  void _readFromHttp();

  void _readToMap();

  long _getRelationId(long fileId);

  /*
   * This updates child ref ID's owned by ways/relations where the child element was parsed after
   * the ref ID.
   */
  void _updateChildRefs();
  void _updateWayChildRefs();
  void _updateRelationChildRefs(const ElementType& childElementType);

  void _reset();
  void _resetIds();
};

}

#endif // OSM_JSON_READER_H
