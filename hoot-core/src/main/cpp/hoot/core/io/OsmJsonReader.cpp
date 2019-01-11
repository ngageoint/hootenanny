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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "OsmJsonReader.h"

// hoot
#include <hoot/core/Hoot.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/schema/MetadataTags.h>

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
namespace pt = boost::property_tree;

// Qt
#include <QTextStream>
#include <QTextCodec>

// Standard
#include <fstream>
#include <iostream>
#include <thread>
#include <sstream>
#include <unistd.h>
using namespace std;

namespace hoot
{

unsigned int OsmJsonReader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapReader, OsmJsonReader)

// Default constructor
OsmJsonReader::OsmJsonReader()
  : _defaultStatus(Status::Invalid),
    _useDataSourceIds(true),
    _defaultCircErr(ElementData::CIRCULAR_ERROR_EMPTY),
    _propTree(),
    _version(""),
    _generator(""),
    _timestamp_base(""),
    _copyright(""),
    _url(""),
    _isFile(false),
    _isWeb(false),
    _bboxContinue(true),
    _runParallel(ConfigOptions().getJsonReaderHttpBboxParallel()),
    _coordGridSize(ConfigOptions().getJsonReaderHttpBboxMaxSize()),
    _threadCount(ConfigOptions().getJsonReaderHttpBboxThreadCount())
{
  // Do nothing special
}

OsmJsonReader::~OsmJsonReader()
{
  close();
}

bool OsmJsonReader::isSupported(QString url)
{
  QUrl myUrl(url);

  // Is it a file?
  if (myUrl.isLocalFile())
  {
    QString filename = myUrl.toLocalFile();

    if (QFile::exists(filename) && url.endsWith(".json", Qt::CaseInsensitive))
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
 * Opens the specified URL for reading.
 */
void OsmJsonReader::open(QString url)
{
  try
  {
    _isFile = false;
    _isWeb = false;

    // Bail out if unsupported
    if (!isSupported(url))
      return;

    // Handle files or URLs
    _url = QUrl(url);
    if (_url.isRelative() || _url.isLocalFile())
    {
      _isFile = true;
      _file.setFileName(_url.toLocalFile());
      _file.open(QFile::ReadOnly | QFile::Text);
    }
    else
    {
      _isWeb = true;
    }
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

/**
 * Reads the specified map. When this method is complete
 * the input will likely be closed.
 */
void OsmJsonReader::read(OsmMapPtr map)
{
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
}

// Throws HootException on error
void OsmJsonReader::_loadJSON(QString jsonStr)
{
  // Clear out anything that might be hanging around
  _propTree.clear();

  // Handle single or double quotes
  scrubQuotes(jsonStr);

  // Handle IDs
  scrubBigInts(jsonStr);

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
  catch (pt::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());

    LOG_DEBUG(jsonStr);
    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason).arg(line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
}

OsmMapPtr OsmJsonReader::loadFromString(QString jsonStr)
{
  _loadJSON(jsonStr);
  _map.reset(new OsmMap());
  _parseOverpassJson();
  return _map;
}

OsmMapPtr OsmJsonReader::loadFromPtree(const boost::property_tree::ptree &tree)
{
  _propTree = tree;
  _map.reset(new OsmMap());
  _parseOverpassJson();
  return _map;
}

OsmMapPtr OsmJsonReader::loadFromFile(QString path)
{
  QFile infile(path);
  if (!infile.open(QFile::ReadOnly | QFile::Text))
  {
    throw HootException("Unable to read JSON file: " + path);
  }

  QTextStream instream(&infile);
  QString jsonStr = instream.readAll();
  _loadJSON(jsonStr);
  _map.reset(new OsmMap());
  _parseOverpassJson();
  return _map;
}

void OsmJsonReader::setConfiguration(const Settings& conf)
{
  _runParallel = ConfigOptions(conf).getJsonReaderHttpBboxParallel();
  _coordGridSize = ConfigOptions(conf).getJsonReaderHttpBboxMaxSize();
  _threadCount = ConfigOptions(conf).getJsonReaderHttpBboxThreadCount();
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
  for (pt::ptree::const_iterator elementIt = elements.begin(); elementIt != elements.end(); ++elementIt)
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

void OsmJsonReader::_parseOverpassNode(const pt::ptree &item)
{
  // Get info we need to construct our node
  long id = -1;
  if (_useDataSourceIds)
    id = item.get("id", id);
  else
    id = _map->createNextNodeId();

  double lat = item.get("lat", 0.0);
  double lon = item.get("lon", 0.0);

  // Construct node
  NodePtr pNode(new Node(_defaultStatus, id, lon, lat, _defaultCircErr));

  // Add tags
  _addTags(item, pNode);

  // Add node to map
  _map->addNode(pNode);
}

void OsmJsonReader::_parseOverpassWay(const pt::ptree &item)
{
  // Get info we need to construct our way
  long id = -1;
  if (_useDataSourceIds)
    id = item.get("id", id);
  else
    id = _map->createNextWayId();

  // Construct Way
  WayPtr pWay(new Way(_defaultStatus, id, _defaultCircErr));

  // Add nodes
  if (item.not_found() != item.find("nodes"))
  {
    pt::ptree nodes = item.get_child("nodes");
    pt::ptree::const_iterator nodeIt = nodes.begin();
    while (nodeIt != nodes.end())
    {
      long v = nodeIt->second.get_value<long>();
      pWay->addNode(v);
      ++nodeIt;
    }
  }

  // Add tags
  _addTags(item, pWay);

  // Add way to map
  _map->addWay(pWay);
}

void OsmJsonReader::_parseOverpassRelation(const pt::ptree &item)
{
  // Get info we need to construct our relation
  long id = -1;
  if (_useDataSourceIds)
    id = item.get("id", id);
  else
    id = _map->createNextRelationId();

  // Construct Relation
  RelationPtr pRelation(new Relation(_defaultStatus, id, _defaultCircErr));

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
  _addTags(item, pRelation);

  // Add relation to map
  _map->addRelation(pRelation);
}

void OsmJsonReader::_addTags(const boost::property_tree::ptree &item, hoot::ElementPtr pElement)
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

void OsmJsonReader::scrubQuotes(QString &jsonStr)
{
  // We allow the use of single quotes, for ease of coding
  // test strings into c++. Single quotes within string literals
  // should be escaped as \'
  // Detect if they are using single quotes or doubles
  if (jsonStr.indexOf("\"features\"", Qt::CaseInsensitive) > -1)
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

void OsmJsonReader::scrubBigInts(QString &jsonStr)
{
  // Boost 1.41 property tree json parser has trouble with
  // integers bigger than 2^31. So we put quotes around big
  // numbers, and that makes it all better
  QRegExp rx1("(\"[^\"]+\"\\s*:\\s*)(-?\\d{8,})");
  jsonStr.replace(rx1, "\\1\"\\2\"");
  QRegExp rx2("([\\[:,\\s]\\s*)(-?\\d{8,})([,\\}\\]\\n])");
  jsonStr.replace(rx2, "\\1\"\\2\"\\3");
}

void OsmJsonReader::_readFromHttp()
{
  if (!_url.isValid())
    throw HootException("Invalid URL: " + _url.toString());
  //  Update the `srsname` parameter to use EPSG:4326
  if (_url.hasQueryItem("srsname"))
  {
    _url.removeQueryItem("srsname");
    _url.addQueryItem("srsname", "EPSG:4326");
  }
  bool split = false;
  int numSplits = 1;
  vector<thread> threads;
  //  Check if there is a bounding box
  if (_url.hasQueryItem("bbox") && _runParallel)
  {
    QStringList bbox = _url.allQueryItemValues("bbox");
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
    _results.append(QString::fromUtf8(request.getResponseContent().data()));
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
      url.removeQueryItem("bbox");
      url.addQueryItem("bbox", GeometryUtils::toString(envelope) + ",EPSG:4326");
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

