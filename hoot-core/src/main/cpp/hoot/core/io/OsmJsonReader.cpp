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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmJsonReader.h"

// hoot
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/util/IoUtils.h>

// Boost
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QTextCodec>
#include <QTextStream>
#include <QUrlQuery>

// Standard
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <unistd.h>
using namespace std;

namespace hoot
{

int OsmJsonReader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapReader, OsmJsonReader)

// TODO: implement Configurable to help simplify this
OsmJsonReader::OsmJsonReader()
  : _defaultStatus(Status::Invalid),
    _useDataSourceIds(true),
    _defaultCircErr(ConfigOptions().getCircularErrorDefaultValue()),
    _propTree(),
    _version(""),
    _generator(""),
    _timestamp_base(""),
    _copyright(""),
    _url(""),
    _isFile(false),
    _isWeb(false),
    _numRead(0),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10),
    _bboxContinue(true),
    _runParallel(ConfigOptions().getJsonReaderHttpBboxParallel()),
    _coordGridSize(ConfigOptions().getJsonReaderHttpBboxMaxSize()),
    _threadCount(ConfigOptions().getJsonReaderHttpBboxThreadCount()),
    _bounds(GeometryUtils::envelopeFromConfigString(ConfigOptions().getConvertBoundingBox())),
    _keepImmediatelyConnectedWaysOutsideBounds(
      ConfigOptions().getConvertBoundingBoxKeepImmediatelyConnectedWaysOutsideBounds()),
    _missingNodeCount(0),
    _missingWayCount(0)
{
}

OsmJsonReader::~OsmJsonReader()
{
  close();
}

bool OsmJsonReader::isSupported(const QString& url)
{
  QUrl myUrl(url);

  const bool isRelativeUrl = myUrl.isRelative();
  const bool isLocalFile =  myUrl.isLocalFile();

  // Is it a file?
  if (isRelativeUrl || isLocalFile)
  {
    if (url.endsWith(".json", Qt::CaseInsensitive) && !url.startsWith("http", Qt::CaseInsensitive))
    {
      return true;
    }
  }

  // Is it a web address?
  if (myUrl.host() == ConfigOptions().getOverpassApiHost() && ("http" == myUrl.scheme() ||
      "https" == myUrl.scheme()))
  {
    return true;
  }

  // Default to not supported
  return false;
}

void OsmJsonReader::open(const QString& url)
{
  try
  {
    _isFile = false;
    _isWeb = false;
    _path = url;

    // Bail out if unsupported
    if (!isSupported(url))
      return;

    // Handle files or URLs
    _url = QUrl(url);

    bool isRelativeUrl = _url.isRelative();
    bool isLocalFile =  _url.isLocalFile();

    if (isRelativeUrl || isLocalFile)
    {
      QString filename = isRelativeUrl ? _url.toString() : _url.toLocalFile();
      _isFile = true;
      _file.setFileName(filename);
      _file.open(QFile::ReadOnly | QFile::Text);
    }
    else
    {
      _isWeb = true;
    }
    LOG_VARD(_isFile);
    LOG_VARD(_isWeb);
  }
  catch (const std::exception& ex)
  {
    ostringstream oss;
    oss << "Exception opening URL (" << url << "): " << ex.what();
    throw HootException(QString("Exception opening URL (%1): %2").arg(url).arg(ex.what()));
  }
}

void OsmJsonReader::close()
{
  if (_isFile)
    _file.close();
}

void OsmJsonReader::read(const OsmMapPtr& map)
{
  // clear node id maps in case the reader is used for multiple files
  _nodeIdMap.clear();
  _relationIdMap.clear();
  _wayIdMap.clear();

  _map = map;
  if (_isFile)
  {
    QTextStream instream(&_file);
    _results.append(instream.readAll());
  }
  else
    _readFromHttp();
  //  Process all of the result strings
  for (int i = 0; i < _results.size(); ++i)
  {
    // This will throw a hoot exception if JSON is invalid
    _loadJSON(_results[i]);
    _parseOverpassJson();
  }
  LOG_VARD(_map->getElementCount());

  // See related note in OsmXmlReader::read.
  if (!_bounds.isNull())
  {
    IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
    LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
  }
}

void OsmJsonReader::_readToMap()
{
  // clear node id maps in case the reader is used for multiple files
  _nodeIdMap.clear();
  _relationIdMap.clear();
  _wayIdMap.clear();

  _map.reset(new OsmMap());
  _parseOverpassJson();
  LOG_VARD(_map->getElementCount());

  if (!_bounds.isNull())
  {
    IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
    LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
  }
}

void OsmJsonReader::_loadJSON(const QString& jsonStr)
{
  QString json(jsonStr);
  // Clear out anything that might be hanging around
  _propTree.clear();

  LOG_TRACE("JSON before cleaning: " << jsonStr.left(100));

  // Handle single or double quotes
  scrubQuotes(json);

  // Handle IDs
  scrubBigInts(json);

  LOG_TRACE("JSON after cleaning: " << jsonStr.left(100));

  // Convert string to stringstream
  stringstream ss(json.toUtf8().constData(), ios::in);

  if (!ss.good())
  {
    throw HootException(QString("Error reading from JSON string:\n%1").arg(jsonStr));
  }

  try
  {
    pt::read_json(ss, _propTree);
  }
  catch (pt::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());

    LOG_DEBUG(json);
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
}

bool OsmJsonReader::isValidJson(const QString& jsonStr)
{
  try
  {
    _loadJSON(jsonStr);
  }
  catch (const HootException&)
  {
    return false;
  }
  return true;
}

OsmMapPtr OsmJsonReader::loadFromString(const QString& jsonStr)
{
  _loadJSON(jsonStr);
  _readToMap();
  return _map;
}

OsmMapPtr OsmJsonReader::loadFromPtree(const boost::property_tree::ptree &tree)
{
  _propTree = tree;
  _readToMap();
  return _map;
}

OsmMapPtr OsmJsonReader::loadFromFile(const QString& path)
{
  QFile infile(path);
  if (!infile.open(QFile::ReadOnly | QFile::Text))
  {
    throw HootException("Unable to read JSON file: " + path);
  }

  QTextStream instream(&infile);
  QString jsonStr = instream.readAll();
  _loadJSON(jsonStr);
  _readToMap();
  return _map;
}

void OsmJsonReader::setConfiguration(const Settings& conf)
{
  ConfigOptions opts(conf);
  _runParallel = opts.getJsonReaderHttpBboxParallel();
  _coordGridSize = opts.getJsonReaderHttpBboxMaxSize();
  _threadCount = opts.getJsonReaderHttpBboxThreadCount();
  setBounds(GeometryUtils::envelopeFromConfigString(opts.getConvertBoundingBox()));
}

void OsmJsonReader::_parseOverpassJson()
{
  // Overpass has 4 top level items: version, generator, osm3s, elements
  _version = QString::fromStdString(_propTree.get("version", string("")));
  _generator = QString::fromStdString(_propTree.get("generator", string("")));
  _timestamp_base = QString::fromStdString(_propTree.get("osm3s.timestamp_osm_base", string("")));
  _copyright = QString::fromStdString(_propTree.get("osm3s.copyright", string("")));

  // Make a map, and iterate through all of our elements, adding them
  pt::ptree elements = _propTree.get_child("elements");
  for (pt::ptree::const_iterator elementIt = elements.begin(); elementIt != elements.end();
       ++elementIt)
  {
    // Type can be node, way, or relation
    string typeStr = elementIt->second.get("type", string("--"));

    if ("node" == typeStr)
    {
      _parseOverpassNode(elementIt->second);
    }
    else if ("way" == typeStr)
    {
      _parseOverpassWay(elementIt->second);
    }
    else if ("relation" == typeStr)
    {
      _parseOverpassRelation(elementIt->second);
    }
    else
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("Unknown JSON elment type (" << typeStr << ") when parsing json osm");
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
    }
  }
}

void OsmJsonReader::_parseOverpassNode(const pt::ptree& item)
{
  const long debugId = -25928;

  // Get info we need to construct our node
  long id = item.get("id", id);

  if (_nodeIdMap.contains(id))
  {
    throw HootException(
      QString("Duplicate node id %1 in map %2 encountered.").arg(id).arg(_path));
  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
  }
  else
  {
    newId = _map->createNextNodeId();
  }
  LOG_VART(id);
  LOG_VART(newId);
  _nodeIdMap.insert(id, newId);

  const QString msg = "Reading " + ElementId(ElementType::Node, newId).toString() + "...";
  if (newId == debugId)
  {
    LOG_VARD(msg);
  }
  else
  {
    LOG_VART(msg);
  }

  double lat = item.get("lat", 0.0);
  double lon = item.get("lon", 0.0);

  long version = ElementData::VERSION_EMPTY;
  version = item.get("version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = item.get("changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = item.get("timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = item.get("user", user);
  long uid = ElementData::UID_EMPTY;
  uid = item.get("uid", uid);

  // Construct node
  NodePtr pNode(
    new Node(
      _defaultStatus, newId, lon, lat, _defaultCircErr, changeset, version, timestamp,
      QString::fromStdString(user), uid));

  // Add tags
  _addTags(item, pNode);

  // Add node to map
  _map->addNode(pNode);

  LOG_TRACE("Loaded node: " << pNode);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmJsonReader::_parseOverpassWay(const pt::ptree& item)
{
  const long debugId = -3047;

  // Get info we need to construct our way
  long id = item.get("id", id);

  if (_wayIdMap.contains(id))
  {
    throw HootException(
      QString("Duplicate way id %1 in map %2 encountered.").arg(id).arg(_path));
  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
  }
  else
  {
    newId = _map->createNextWayId();
  }
  _wayIdMap.insert(id, newId);

  const QString msg = "Reading " + ElementId(ElementType::Way, newId).toString() + "...";
  if (newId == debugId)
  {
    LOG_VARD(msg);
  }
  else
  {
    LOG_VART(msg);
  }

  long version = ElementData::VERSION_EMPTY;
  version = item.get("version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = item.get("changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = item.get("timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = item.get("user", user);
  long uid = ElementData::UID_EMPTY;
  uid = item.get("uid", uid);

  // Construct Way
  WayPtr pWay(
    new Way(
      _defaultStatus, newId, _defaultCircErr, changeset, version, timestamp,
      QString::fromStdString(user), uid));

  // Add nodes
  if (item.not_found() != item.find("nodes"))
  {
    pt::ptree nodes = item.get_child("nodes");
    pt::ptree::const_iterator nodeIt = nodes.begin();
    while (nodeIt != nodes.end())
    {
      long v = nodeIt->second.get_value<long>();
      LOG_VART(v);

      const bool nodePresent = _nodeIdMap.contains(v);
      LOG_VART(nodePresent);
      if (!nodePresent)
      {
        _missingNodeCount++;
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN(
            "Missing " << ElementId(ElementType::Node, v) << " in " <<
            ElementId(ElementType::Way, newId) << ".");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      else
      {
        long newRef = _nodeIdMap.value(v);
        LOG_TRACE("Adding way node: " << newRef << "...");
        pWay->addNode(newRef);
      }

      ++nodeIt;
    }
  }

  // Add tags
  _addTags(item, pWay);

  // Add way to map
  _map->addWay(pWay);

  LOG_TRACE("Loaded way: " << pWay);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmJsonReader::_parseOverpassRelation(const pt::ptree& item)
{
  const long debugId = 0;

  // Get info we need to construct our relation
  long id = item.get("id", id);

  // See related note in OsmXmlReader::_createRelation.
//  if (_relationIdMap.contains(id))
//  {
//    throw HootException(
//      QString("Duplicate realtion id %1 in map %2 encountered.").arg(id).arg(_path));
//  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
  }
  else
  {
    newId = _map->createNextRelationId();
  }
  _relationIdMap.insert(id, newId);

  const QString msg = "Reading " + ElementId(ElementType::Relation, newId).toString() + "...";
  if (newId == debugId)
  {
    LOG_VARD(msg);
  }
  else
  {
    LOG_VART(msg);
  }

  long version = ElementData::VERSION_EMPTY;
  version = item.get("version", version);
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = item.get("changeset", changeset);
  unsigned int timestamp = ElementData::TIMESTAMP_EMPTY;
  timestamp = item.get("timestamp", timestamp);
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = item.get("user", user);
  long uid = ElementData::UID_EMPTY;
  uid = item.get("uid", uid);

  // Construct Relation
  RelationPtr pRelation(
    new Relation(
      _defaultStatus, newId, _defaultCircErr, "", changeset, version, timestamp,
      QString::fromStdString(user), uid));

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

      bool okToAdd = false;
      if (typeStr == "node")
      {
        const bool memberPresent = _nodeIdMap.contains(ref);
        if (!memberPresent)
        {
          _missingNodeCount++;
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN(
              "Missing " << ElementId(ElementType::Node, ref) << " in " <<
              ElementId(ElementType::Relation, newId) << ".");
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
        else
        {
          okToAdd = true;
        }
      }
      else if (typeStr == "way")
      {
        const bool memberPresent = _wayIdMap.contains(ref);
        if (!memberPresent)
        {
          _missingWayCount++;
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN(
              "Missing " << ElementId(ElementType::Way, ref) << " in " <<
              ElementId(ElementType::Relation, newId) << ".");
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
        else
        {
          okToAdd = true;
        }
      }
      else if (typeStr == "relation")
      {
        ref = _getRelationId(ref);
        okToAdd = true;
      }
      else
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Found a relation member with unexpected type: " << typeStr << " in relation ("
                     << id << ")");
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        okToAdd = false;
      }

      if (okToAdd)
      {
        LOG_TRACE("Adding relation relation member: " << ref << "...");
        pRelation->addElement(QString::fromStdString(role),
                              ElementType::fromString(QString::fromStdString(typeStr)), ref);
      }

      ++memberIt;
    }
  }

  // Add tags
  _addTags(item, pRelation);

  // Add relation to map
  _map->addRelation(pRelation);

  LOG_TRACE("Loaded relation: " << pRelation);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

long OsmJsonReader::_getRelationId(long fileId)
{
  long newId;
  if (_useDataSourceIds)
  {
    newId = fileId;
    _relationIdMap.insert(fileId, newId);
  }
  else
  {
    if (_relationIdMap.find(fileId) == _relationIdMap.end())
    {
      newId = _map->createNextRelationId();
      _relationIdMap.insert(fileId, newId);
    }
    else
    {
      newId = _relationIdMap[fileId];
    }
  }
  return newId;
}

void OsmJsonReader::_addTags(const boost::property_tree::ptree& item, hoot::ElementPtr pElement)
{
  // Find tags and add them
  if (item.not_found() != item.find("tags"))
  {
    pt::ptree tags = item.get_child("tags");
    for (pt::ptree::const_iterator tagIt = tags.begin(); tagIt != tags.end(); ++tagIt)
    {
      const QString key = QString::fromStdString(tagIt->first).trimmed();
      //LOG_VART(key);
      const QString value = QString::fromStdString(tagIt->second.get_value<string>()).trimmed();
      //LOG_VART(value);

      // If we are "error:circular", need to set it on the element object,
      // rather than add it as a tag
      if (key == MetadataTags::ErrorCircular())
      {
        pElement->setCircularError(Meters(value.toInt()));
      }
      else if (!value.isEmpty())
      {
        pElement->setTag(key, value);
      }
    }
  }
}

void OsmJsonReader::scrubQuotes(QString& jsonStr)
{
  // We allow the use of single quotes, for ease of coding
  // test strings into c++. Single quotes within string literals
  // should be escaped as \'
  // Detect if they are using single quotes or doubles
  //  "features" is GEOJSON while "elements" comes from overpass
  if (jsonStr.indexOf("\"features\"", Qt::CaseInsensitive) > -1 ||
      jsonStr.indexOf("\"elements\"", Qt::CaseInsensitive) > -1)
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

void OsmJsonReader::scrubBigInts(QString& jsonStr)
{
  // Boost 1.41 property tree json parser has trouble with
  // integers bigger than 2^31. So we put quotes around big
  // numbers, and that makes it all better
  QRegExp rx1("(\"[^\"]+\"\\s*:\\s*)(-?\\d{8,})");
  jsonStr.replace(rx1, "\\1\"\\2\"");
  // see related note in OsmJsonReaderTest::scrubBigIntsTest about changes made to this regex
  QRegExp rx2("([\\[,\\s]\\s*)(-?\\d{8,})([,\\}\\]\\n])");
  jsonStr.replace(rx2, "\\1\"\\2\"\\3");
}

void OsmJsonReader::_readFromHttp()
{
  if (!_url.isValid())
    throw HootException("Invalid URL: " + _url.toString(QUrl::RemoveUserInfo));
  //  Update the `srsname` parameter to use EPSG:4326

  QUrlQuery urlQuery(_url);
  if (urlQuery.hasQueryItem("srsname"))
  {
    urlQuery.removeQueryItem("srsname");
    urlQuery.addQueryItem("srsname", "EPSG:4326");
    _url.setQuery(urlQuery);
  }

  bool split = false;
  int numSplits = 1;
  vector<thread> threads;
  //  Check if there is a bounding box
  if (urlQuery.hasQueryItem("bbox") && _runParallel)
  {
    QStringList bbox = urlQuery.allQueryItemValues("bbox");
    //  Parse the bounding box
    geos::geom::Envelope envelope = GeometryUtils::envelopeFromConfigString(bbox.last());
    //  Check if the bounding box needs to be split
    int lon_div = 1;
    int lat_div = 1;
    //  Don't split an envelope if it is just a little bigger than the prescribed max
    if (envelope.getWidth() > _coordGridSize * 1.5)
      lon_div = (int)std::ceil(envelope.getWidth() / _coordGridSize);
    if (envelope.getHeight() > _coordGridSize * 1.5)
      lat_div = (int)std::ceil(envelope.getHeight() / _coordGridSize);
    numSplits = lat_div * lon_div;
    //  Create envelopes for splitting the request
    if (lon_div != 1 || lat_div != 1)
    {
      //  Only spin up enough threads for the work up to the max
      int max_threads = _threadCount;
      if (numSplits < max_threads)
        max_threads = numSplits;
      //  Fire up the worker threads
      _bboxContinue = true;
      for (int i = 0; i < max_threads; ++i)
        threads.push_back(thread(&OsmJsonReader::_doHttpRequestFunc, this));
      split = true;
      //  Setup the envelopes to query in a grid
      for (int i = 0; i < lon_div; ++i)
      {
        double lon = envelope.getMinX() + _coordGridSize * i;
        for (int j = 0; j < lat_div; ++j)
        {
          double lat = envelope.getMaxY() - _coordGridSize * j;
          _bboxMutex.lock();
          //  Start at the upper right corner and create boxes left to right, top to bottom
          _bboxes.append(
              geos::geom::Envelope(
                  lon,
                  std::max(lat, envelope.getMinY()),
                  std::min(lon + _coordGridSize, envelope.getMaxX()),
                  lat + _coordGridSize));
          _bboxMutex.unlock();
        }
      }
    }
  }

  if (split)
  {
    //  Wait on the work to be completed
    _bboxContinue = false;
    for (size_t i = 0; i < threads.size(); ++i)
      threads[i].join();
  }
  else
  {
    //  Do HTTP GET request without splitting
    HootNetworkRequest request;
    request.networkRequest(_url);
    const QString response = QString::fromUtf8(request.getResponseContent().data());
    LOG_VART(response.left(200));
    _results.append(response);
  }
}

void OsmJsonReader::_doHttpRequestFunc()
{
  //  Lock the mutex before checking
  _bboxMutex.lock();
  while (_bboxContinue || _bboxes.size() > 0)
  {
    if (_bboxes.size() > 0)
    {
      //  Get the envelope
      geos::geom::Envelope envelope = _bboxes.first();
      _bboxes.removeFirst();
      _bboxMutex.unlock();
      //  Update the URL
      QUrl url(_url);
      QUrlQuery urlQuery(url);
      urlQuery.removeQueryItem("bbox");
      urlQuery.addQueryItem("bbox", GeometryUtils::toString(envelope) + ",EPSG:4326");
      url.setQuery(urlQuery);
      HootNetworkRequest request;
      LOG_VART(url);
      request.networkRequest(url);
      QString result = QString::fromUtf8(request.getResponseContent().data());
      //  Store the result inside of a locked mutex
      _resultsMutex.lock();
      LOG_VART(result);
      _results.append(result);
      _resultsMutex.unlock();
    }
    else
    {
      //  Sleep for a bit unlocked so things can happen
      _bboxMutex.unlock();
      this_thread::sleep_for(chrono::milliseconds(100));
    }
    //  Lock up before checking the continue flag and the work queue
    _bboxMutex.lock();
  }
  //  Unlock it all and end the thread
  _bboxMutex.unlock();
}

}

