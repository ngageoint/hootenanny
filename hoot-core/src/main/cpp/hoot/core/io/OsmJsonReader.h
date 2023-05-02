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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Maxar (http://www.maxar.com/)
 */

#ifndef OSM_JSON_READER_H
#define OSM_JSON_READER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/io/OsmMapReader.h>
#include <hoot/core/io/ParallelBoundedApiReader.h>
#include <hoot/core/io/StringMemoryInterface.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Boundable.h>

// Qt
#include <QUrl>

// Standard
#include <fstream>
#include <mutex>

// Boost
#include <boost/property_tree/ptree.hpp>

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
 * The input string must be well-formed JSON. StringUtils::scrubQuotes() can convert hand-jammed
 * JSON string into c++ code using single quotes. If you are using single quotes, you must escape
 * apostrophes with a double backslash.
 *
 * Also, be aware that this class can stream large datasets into an OsmMap so there is only one copy
 * of everything in memory. Be careful if you want to use it with large datasets.
 */
class OsmJsonReader : public OsmMapReader, public Boundable, protected ParallelBoundedApiReader, protected StringMemoryInterface
{
public:

  static QString className() { return "OsmJsonReader"; }

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
  bool isSupported(const QString& url) const override;

  /**
   * @brief open Specifies the URL to read from. Can be a file (file://some/path/to/a/file)
   *        or a json provider like:
   *    http://overpass-api.de/api/interpreter?data=[out:json];way(35.2097,-120.6207,35.2241,-120.5955);out;
   * @param url
   */
  void open(const QString& url) override;

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
  void read(const OsmMapPtr& map) override;

  /**
   * @brief setDefaultStatus Sets the default status to use for elements
   *        in the map
   * @param status
   */
  void setDefaultStatus(const Status& status) override { _defaultStatus = status; }

  /**
   * @brief setUseDataSourceIds Sets whether the reader should use the element IDs
   *        from the data being read, or self-generate unique IDs
   * @param useDataSourceIds true to use source IDs
   */
  void setUseDataSourceIds(bool useDataSourceIds) override
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
  virtual OsmMapPtr loadFromPtree(const boost::property_tree::ptree& tree);

  /**
   * @brief loadFromFile - Reads the whole file as a string, passes it
   *        to loadFromString()
   * @param path - Path to file
   * @return Smart pointer to the OSM map
   */
  virtual OsmMapPtr loadFromFile(const QString& path);

  QString supportedFormats() const override { return ".json"; }

  /**
   * Set the configuration for this object.
   */
  void setConfiguration(const Settings& conf) override;

  bool isValidJson(const QString& jsonStr);

  /** See Boundable */
  void setBounds(std::shared_ptr<geos::geom::Geometry> bounds) override;
  void setBounds(const geos::geom::Envelope& bounds) override;

  /**
   * @brief getVersion Overpass API version, if that's where JSON comes from
   * @return version string
   */
  QString getVersion() const { return _version; }
  /**
   * @brief getGenerator Map generator name, if supplied in JSON
   * @return generator name string
   */
  QString getGenerator() const { return _generator; }
  /**
   * @brief getTimestampBase OSM timestamp base, if supplied in JSON
   * @return timestamp base string
   */
  QString getTimestampBase() const { return _timestamp_base; }
  /**
   * @brief getCopyright Copyright statement, if supplied in JSON
   * @return copyright string
   */
  QString getCopyright() const { return _copyright; }

  void setKeepImmediatelyConnectedWaysOutsideBounds(bool keep)
  { _keepImmediatelyConnectedWaysOutsideBounds = keep; }
  void setLogWarningsForMissingElements(bool log) { _logWarningsForMissingElements = log; }

protected:

  // Items to conform to OsmMapReader ifc
  Status _defaultStatus;
  bool _useDataSourceIds;

  // the CE value used if no CE tag is found
  Meters _defaultCircErr;

  // Our property tree that holds JSON
  boost::property_tree::ptree _propTree;

  // Store these items from overpass api
  QString _generator;

  bool _isFile;
  std::ifstream _file;

  OsmMapPtr _map;

  long _numRead;
  long _statusUpdateInterval;

  /**
   * @brief _loadJSON Loads JSON into a boost property tree
   * @param jsonStr String to load
   */
  void _loadJSON(const QString& jsonStr);

  /**
   * @brief _loadJSON Loads JSON into a boost property tree
   * @param in_stream Stream to read the JSON from
   */
  void _loadJSON(std::istream& in_stream);

  /**
   * @brief _addTags Reads tags from the given ptree, and adds them to the
   *        supplied map element
   * @param item Property Tree (subtree)
   * @param pElement Element to which we will add the tags
   */
  void _addTags(const boost::property_tree::ptree& item, ElementPtr pElement);

  /**
   * @brief _readFromHttp Creates HTTP(S) connection and downloads JSON and parses the results,
   *   could also spawn a thread pool to query bounding boxes
   */
  void _readFromHttp();

private:

  // keys for tags containing CE data
  QStringList _circularErrorTagKeys;

  // Store these items from overpass api
  QString _version;
  QString _timestamp_base;
  QString _copyright;

  bool _isWeb;

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

  //adds child refs to elements when they aren't present in the source data
  bool _addChildRefsWhenMissing;
  // determines whether missing elements trigger a warning
  bool _logWarningsForMissingElements;

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
  void _parseOverpassNode(const boost::property_tree::ptree& item);
  /**
   * @brief _parseOverpassWay Reads way info out of the property tree and
   *        builds a Way object. Adds the way to the map.
   * @param item Property Tree (or sub-tree)
   */
  void _parseOverpassWay(const boost::property_tree::ptree& item);
  /**
   * @brief _parseOverpassRelation Reads relation info out of the property tree
   *        and builds a Relation object. Adds relation to the map.
   * @param item Property Tree (likely a subtree)
   */
  void _parseOverpassRelation(const boost::property_tree::ptree& item);

  void _readToMap();

  long _getRelationId(long fileId);

  /*
   * This updates child ref ID's owned by ways/relations where the child element was parsed after
   * the ref ID.
   */
  void _updateChildRefs();
  void _updateWayChildRefs();
  void _updateRelationChildRefs(const ElementType& childElementType) const;

  void _reset();
  void _resetIds();
  /**
   * @brief _loadBounds - Get either the `bounds` parameter value
   *  or the bounds of a file listed in `bounds.file` parameter
   * @return true if the bounds were parsed correctly
   */
  bool _loadBounds();

protected:

  virtual long _getVersion(const boost::property_tree::ptree& item, ElementType::Type type, long id) const;
  virtual long _getChangeset(const boost::property_tree::ptree& item) const;
  virtual unsigned int _getTimestamp(const boost::property_tree::ptree& item) const;
  virtual std::string _getUser(const boost::property_tree::ptree& item) const;
  virtual long _getUid(const boost::property_tree::ptree& item) const;

  long _getVersion(const std::string& field_name, const boost::property_tree::ptree& item, ElementType::Type type, long id) const;
  long _getChangeset(const std::string& field_name, const boost::property_tree::ptree& item) const;
  unsigned int _getTimestamp(const std::string& field_name, const boost::property_tree::ptree& item) const;
  std::string _getUser(const std::string& field_name, const boost::property_tree::ptree& item) const;
  long _getUid(const std::string& field_name, const boost::property_tree::ptree& item) const;

  friend class OsmJsonReaderTest;
};

}

#endif // OSM_JSON_READER_H
