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

#include "OsmJsonReader.h"

// hoot
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/HootNetworkRequest.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/FileUtils.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/RemoveMissingElementsVisitor.h>

// Boost
#include <boost/property_tree/json_parser.hpp>

// Qt
#include <QTextCodec>
#include <QTextStream>
#include <QUrlQuery>

// Standard
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <unistd.h>

//  Geos
#include <geos/geom/GeometryFactory.h>

namespace pt = boost::property_tree;
using namespace std;

namespace hoot
{

int OsmJsonReader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapReader, OsmJsonReader)

OsmJsonReader::OsmJsonReader()
  : ParallelBoundedApiReader(true, false),
    _defaultStatus(Status::Invalid),
    _useDataSourceIds(true),
    _defaultCircErr(ConfigOptions().getCircularErrorDefaultValue()),
    _isFile(false),
    _numRead(0),
    _statusUpdateInterval(ConfigOptions().getTaskStatusUpdateInterval() * 10),
    _circularErrorTagKeys(ConfigOptions().getCircularErrorTagKeys()),
    _isWeb(false),
    _keepImmediatelyConnectedWaysOutsideBounds(ConfigOptions().getBoundsKeepImmediatelyConnectedWaysOutsideBounds()),
    _addChildRefsWhenMissing(ConfigOptions().getMapReaderAddChildRefsWhenMissing()),
    _logWarningsForMissingElements(ConfigOptions().getLogWarningsForMissingElements())
{
  setConfiguration(conf());
}

OsmJsonReader::~OsmJsonReader()
{
  close();
}

bool OsmJsonReader::isSupported(const QString& url) const
{
  QUrl myUrl(url);

  const bool isRelativeUrl = myUrl.isRelative();
  const bool isLocalFile =  myUrl.isLocalFile();

  // Is it a file?
  if ((isRelativeUrl || isLocalFile) && url.endsWith(".json", Qt::CaseInsensitive) && !url.startsWith("http", Qt::CaseInsensitive))
    return true;

  // Is it a JSON Overpass web address?
  if (isOverpassJson(url))
    return true;

  // Default to not supported
  return false;
}

void OsmJsonReader::open(const QString& url)
{
  LOG_DEBUG("Opening: " << url << "...");

  OsmMapReader::open(url);
  try
  {
    _isFile = false;
    _isWeb = false;

    // Bail out if unsupported
    if (!isSupported(url))
      return;

    // Handle files or URLs
    _sourceUrl = QUrl(url);

    bool isRelativeUrl = _sourceUrl.isRelative();
    bool isLocalFile =  _sourceUrl.isLocalFile();

    if (isRelativeUrl || isLocalFile)
    {
      QString filename = isRelativeUrl ? _sourceUrl.toString() : _sourceUrl.toLocalFile();
      _isFile = true;
      _file.open(filename.toStdString().c_str());
      if (!_file.is_open())
        throw std::runtime_error("Unable to open JSON file");
    }
    else
    {
      setOverpassUrl(url);
      _isWeb = true;
    }
    LOG_VARD(_isFile);
    LOG_VARD(_isWeb);
  }
  catch (const std::exception& ex)
  {
    throw HootException(QString("Exception opening URL (%1): %2").arg(url, ex.what()));
  }
}

void OsmJsonReader::_reset()
{
  _propTree.clear();
  _resetIds();
}

void OsmJsonReader::_resetIds()
{
  _nodeIdMap.clear();
  _relationIdMap.clear();
  _wayIdMap.clear();

  _wayIdsToWayNodeIdsNotPresent.clear();
  _relationIdsToNodeMemberIdsNotPresent.clear();
  _relationIdsToWayMemberIdsNotPresent.clear();
  _relationIdsToRelationMemberIdsNotPresent.clear();
}

void OsmJsonReader::close()
{
  if (_isFile)
    _file.close();

  _reset();
}

void OsmJsonReader::read(const OsmMapPtr& map)
{
  LOG_DEBUG("Reading map...");

  //  Set the bounds once we begin the read if setBounds() hasn't already been called
  if (_bounds == nullptr)
    _loadBounds();

  if (_bounds == nullptr && !_isFile)
    throw IllegalArgumentException("OsmJsonReader requires rectangular bounds");

  _map = map;
  _map->appendSource(_url);
  if (_isFile)
  {
    _loadJSON(_file);
    _parseOverpassJson();
  }
  else
    _readFromHttp();

  LOG_VARD(_map->getElementCount());

  //  Bounded network queries are already cropped
  if (_bounds.get() && !_isWeb)
  {
    // See related note in OsmXmlReader::read.
    IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
    LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
  }
}

void OsmJsonReader::_readToMap()
{
  _parseOverpassJson();
  LOG_VARD(_map->getElementCount());

  //  Bounded network queries are already cropped
  if (_bounds.get() && !_isWeb)
  {
    IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
    LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
  }
}

void OsmJsonReader::_loadJSON(std::istream &in_stream)
{
  if (!in_stream.good())
    throw HootException("Error reading from JSON stream.\n");

  try
  {
    pt::read_json(in_stream, _propTree);
  }
  catch (const pt::json_parser::json_parser_error& e)
  {
    QString reason = QString::fromStdString(e.message());
    QString line = QString::number(e.line());

    throw HootException(QString("Error parsing JSON: %1 (line %2)").arg(reason, line));
  }
  catch (const std::exception& e)
  {
    QString reason = e.what();
    throw HootException("Error parsing JSON " + reason);
  }
}

void OsmJsonReader::_loadJSON(const QString& jsonStr)
{
  // Clear out anything that might be hanging around
  _propTree.clear();

  // Convert string to stringstream
  stringstream ss(jsonStr.toUtf8().constData(), ios::in);

  if (!ss.good())
    throw HootException(QString("Error reading from JSON string:\n%1").arg(jsonStr));

  _loadJSON(ss);
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

void OsmJsonReader::loadFromString(const QString& jsonStr, const OsmMapPtr& map)
{
  _isFile = false;
  _isWeb = false;
  _map = map;
  _loadJSON(jsonStr);
  _readToMap();
  _map.reset();
}

OsmMapPtr OsmJsonReader::loadFromPtree(const boost::property_tree::ptree& tree)
{
  _isFile = false;
  _isWeb = false;
  _propTree = tree;
  _map = std::make_shared<OsmMap>();
  _readToMap();
  return _map;
}

OsmMapPtr OsmJsonReader::loadFromFile(const QString& path)
{
  ifstream infile(path.toStdString());
  if (!infile.is_open())
    throw HootException("Unable to read JSON file: " + path);

  _isFile = true;
  _isWeb = false;
  _loadJSON(infile);
  _map = std::make_shared<OsmMap>();
  _readToMap();
  return _map;
}

void OsmJsonReader::setConfiguration(const Settings& conf)
{
  OsmMapReader::setConfiguration(conf);
  ConfigOptions opts(conf);
  _coordGridSize = opts.getReaderHttpBboxMaxSize();
  _threadCount = opts.getReaderHttpBboxThreadCount();
  setBounds(Boundable::loadBounds(opts));
  setWarnOnVersionZeroElement(opts.getReaderWarnOnZeroVersionElement());
}

void OsmJsonReader::setBounds(std::shared_ptr<geos::geom::Geometry> bounds)
{
  //  Check if the bounds are rectangular
  _isPolygon = bounds ? !bounds->isRectangle() : false;
  _boundingPoly = bounds;
  Boundable::setBounds(bounds);
}

void OsmJsonReader::setBounds(const geos::geom::Envelope& bounds)
{
  //  An envelope isn't a poly bounds
  _isPolygon = false;
  _boundingPoly.reset(geos::geom::GeometryFactory::getDefaultInstance()->toGeometry(&bounds).release());
  Boundable::setBounds(bounds);
}

bool OsmJsonReader::_loadBounds()
{
  setBounds(Boundable::loadBounds());
  //  Return true if the bounds object was created
  return _bounds != nullptr;
}

void OsmJsonReader::_parseOverpassJson()
{
  LOG_VARD(_useDataSourceIds);
  LOG_VARD(_ignoreDuplicates);

  // clear node id maps in case the reader is used for multiple files
  _resetIds();

  // Overpass has 4 top level items: version, generator, osm3s, elements
  _version = QString::fromStdString(_propTree.get("version", string("")));
  _generator = QString::fromStdString(_propTree.get("generator", string("")));
  _timestamp_base = QString::fromStdString(_propTree.get("osm3s.timestamp_osm_base", string("")));
  _copyright = QString::fromStdString(_propTree.get("osm3s.copyright", string("")));

  // Make a map, and iterate through all of our elements, adding them
  pt::ptree elements = _propTree.get_child("elements");
  for (auto elementIt = elements.begin(); elementIt != elements.end(); ++elementIt)
  {
    // Type can be node, way, or relation
    string typeStr = elementIt->second.get("type", string("--"));
    if ("node" == typeStr)
      _parseOverpassNode(elementIt->second);
    else if ("way" == typeStr)
      _parseOverpassWay(elementIt->second);
    else if ("relation" == typeStr)
      _parseOverpassRelation(elementIt->second);
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

  // Go back through all child refs and update their IDs with what we remapped them to in case we
  // parsed their parents before the children the refs point to. If we loaded the original element
  // IDs, then this step isn't needed and gets skipped.
  if (!_useDataSourceIds)
    _updateChildRefs();

  if (!_addChildRefsWhenMissing)
  {
    // Now that we've corrected all child refs (or didn't need to), if we find any remaining missing
    // child refs we're going to remove them from their parents and log a warning, as they never
    // actually existed in the input.
    Log::WarningLevel logLevel;
    if (_logWarningsForMissingElements)
      logLevel = Log::Warn;
    else
      logLevel = Log::Debug;
    RemoveMissingElementsVisitor visitor(logLevel);
    LOG_INFO("\t" << visitor.getInitStatusMessage());
    _map->visitRw(visitor);
    LOG_DEBUG("\t" << visitor.getCompletedStatusMessage());
  }

  // The previous two final load steps correspond to effectively the same behavior that the XML
  // reader uses, except with that reader we simply don't add a missing ref in the first place b/c
  // we're guaranteed element type ordering (unless map.reader.add.child.refs.when.missing=true).
  // Because of that, we know at the time a parent element is loaded any missing child refs mean
  // the child itself is definitely absent from the input.
}

void OsmJsonReader::_updateRelationChildRefs(const ElementType& childElementType) const
{
  QList<long> relationIdsWithChildrenNotPresent;
  QHash<long, long> originalIdMap;
  if (childElementType == ElementType::Node)
  {
    relationIdsWithChildrenNotPresent = _relationIdsToNodeMemberIdsNotPresent.keys();
    originalIdMap = _nodeIdMap;
  }
  else if (childElementType == ElementType::Way)
  {
    relationIdsWithChildrenNotPresent = _relationIdsToWayMemberIdsNotPresent.keys();
    originalIdMap = _wayIdMap;
  }
  else if (childElementType == ElementType::Relation)
  {
    relationIdsWithChildrenNotPresent = _relationIdsToRelationMemberIdsNotPresent.keys();
    originalIdMap = _relationIdMap;
  }

  for (auto relationId : qAsConst(relationIdsWithChildrenNotPresent))
  {
    LOG_VART(relationId);
    RelationPtr relation = _map->getRelation(relationId);
    LOG_VART(relation.get());
    if (relation)   // not completely sure why the relation wouldn't exist at this point
    {
      QList<long> childIdsNotPresentAtLoad;
      if (childElementType == ElementType::Node)
        childIdsNotPresentAtLoad = _relationIdsToNodeMemberIdsNotPresent.values(relationId);
      else if (childElementType == ElementType::Way)
        childIdsNotPresentAtLoad = _relationIdsToWayMemberIdsNotPresent.values(relationId);
      else if (childElementType == ElementType::Relation)
        childIdsNotPresentAtLoad = _relationIdsToRelationMemberIdsNotPresent.values(relationId);

      for (auto memberId : qAsConst(childIdsNotPresentAtLoad))
      {
        LOG_VART(memberId);
        ElementId idToReplace = ElementId(childElementType, memberId);
        LOG_VART(idToReplace);
        LOG_VART(relation->getElementId() != idToReplace);
        LOG_VART(relation->contains(idToReplace));
        if (relation->getElementId() != idToReplace && relation->contains(idToReplace))
        {
          auto remappedMemberIdItr = originalIdMap.find(memberId);
          LOG_VART(remappedMemberIdItr.value());
          if (remappedMemberIdItr != originalIdMap.end() && remappedMemberIdItr.value() != memberId)
          {
            ElementId idToReplaceWith = ElementId(childElementType, remappedMemberIdItr.value());
            LOG_TRACE(
              "Replacing element ID: " << idToReplace << " with: " << idToReplaceWith << "...");
            relation->replaceElement(idToReplace, idToReplaceWith);
          }
        }
      }
    }
  }
}

void OsmJsonReader::_updateWayChildRefs()
{
  const QList<long> wayIdsWithWayNodesNotPresent = _wayIdsToWayNodeIdsNotPresent.keys();
  for (auto wayId : qAsConst(wayIdsWithWayNodesNotPresent))
  {
    LOG_VART(wayId);
    WayPtr way = _map->getWay(wayId);
    // haven't seen a null one yet but adding this to stay consistent with the behavior for
    // relations
    if (way)
    {
      const QList<long> wayNodeIdsNotPresentAtLoad = _wayIdsToWayNodeIdsNotPresent.values(wayId);
      for (auto wayNodeId : qAsConst(wayNodeIdsNotPresentAtLoad))
      {
        LOG_VART(wayNodeId);
        if (way->containsNodeId(wayNodeId))
        {
          auto remappedNodeIdItr = _nodeIdMap.find(wayNodeId);
          LOG_VART(remappedNodeIdItr.value());
          if (remappedNodeIdItr != _nodeIdMap.end() && remappedNodeIdItr.value() != wayNodeId)
          {
            LOG_TRACE(
              "Replacing way ID: " << wayNodeId << " with: " << remappedNodeIdItr.value() << "...");
            way->replaceNode(wayNodeId, remappedNodeIdItr.value());
          }
        }
      }
    }
  }
}

void OsmJsonReader::_updateChildRefs()
{
  LOG_DEBUG("Updating child element ID references...");

  // For any ways in which added way nodes that weren't validated upon load, let's update those way
  // node id refs now with what we actually loaded.
  _updateWayChildRefs();

  // Do the same as above but for relation member refs
  _updateRelationChildRefs(ElementType::Node);
  _updateRelationChildRefs(ElementType::Way);
  _updateRelationChildRefs(ElementType::Relation);
}

void OsmJsonReader::_parseOverpassNode(const pt::ptree& item)
{
  // Get info we need to construct our node
  long id = 0;
  id = item.get("id", id);
  if (id == 0)
  {
    LOG_TRACE("Node parsing error, no ID found, ignoring node.");
    return;
  }

  if (_nodeIdMap.contains(id))
  {
    if (_ignoreDuplicates)
    {
      LOG_TRACE("Ignoring node id " << id << " already exists");
      return;
    }
    else
      throw HootException(QString("Duplicate node id %1 in map %2 encountered.").arg(id).arg(_url));
  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
    LOG_TRACE("Using source node id: " << newId << "...");
  }
  else
  {
    newId = _map->createNextNodeId();
    LOG_TRACE("Created new node id: " << newId << "...");
  }
  LOG_VART(id);
  LOG_VART(newId);
  _nodeIdMap.insert(id, newId);

  const QString msg = "Reading " + ElementId(ElementType::Node, newId).toString() + "...";
  LOG_VART(msg);

  double lat = item.get("lat", 0.0);
  double lon = item.get("lon", 0.0);

  long version = _getVersion(item, ElementType::Node, newId);
  long changeset = _getChangeset(item);
  unsigned int timestamp = _getTimestamp(item);
  std::string user = _getUser(item);
  long uid = _getUid(item);

  // Construct node
  NodePtr pNode(Node::newSp(_defaultStatus, newId, lon, lat, _defaultCircErr, changeset, version, timestamp, QString::fromStdString(user), uid));

  // Add tags
  _addTags(item, pNode);

  // Add node to map
  _map->addNode(pNode);

  LOG_TRACE("Loaded node: " << pNode->getElementId());
  //LOG_TRACE("Loaded node: " << pNode);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmJsonReader::_parseOverpassWay(const pt::ptree& item)
{
  // Get info we need to construct our way
  long id = 0;
  id = item.get("id", id);
  if (id == 0)
  {
    LOG_TRACE("Way parsing error, no ID found, ignoring way.");
    return;
  }

  if (_wayIdMap.contains(id))
  {
    if (_ignoreDuplicates)
    {
      LOG_TRACE("Ignoring way id " << id << " already exists");
      return;
    }
    else
      throw HootException(QString("Duplicate way id %1 in map %2 encountered.").arg(id).arg(_url));
  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
    LOG_TRACE("Using source way id: " << newId << "...");
  }
  else
  {
    newId = _map->createNextWayId();
    LOG_TRACE("Created new way id: " << newId << "...");
  }
  _wayIdMap.insert(id, newId);

  long version = _getVersion(item, ElementType::Way, newId);
  long changeset = _getChangeset(item);
  unsigned int timestamp = _getTimestamp(item);
  std::string user = _getUser(item);
  long uid = _getUid(item);

  // Construct Way
  WayPtr pWay = std::make_shared<Way>(_defaultStatus, newId, _defaultCircErr, changeset, version, timestamp, QString::fromStdString(user), uid);

  // Add nodes
  if (item.not_found() != item.find("nodes"))
  {
    pt::ptree nodes = item.get_child("nodes");
    pt::ptree::const_iterator nodeIt = nodes.begin();
    while (nodeIt != nodes.end())
    {
      const long wayNodeId = nodeIt->second.get_value<long>();
      long wayNodeIdToUse = wayNodeId;

      // Since the actual nodes referenced by these way node IDs could be found later in the
      // input file (JSON doesn't require element type ordering like XML does), we will add the way
      // node to the way whether it has been parsed yet or not. At the very end of the file load,
      // we'll check for missing child refs.

      if (!_useDataSourceIds)
      {
        // If we're not using source element IDs, check to see if we've already mapped this node ID
        // to a new ID. If so, we'll use that ID instead.
        auto wayNodeIdItr = _nodeIdMap.find(wayNodeId);
        if (wayNodeIdItr != _nodeIdMap.end())
        {
          wayNodeIdToUse = wayNodeIdItr.value();
          LOG_TRACE("Retrieved mapped way node ID: " << wayNodeIdToUse << " for ID: " << wayNodeId);
        }
        else
          _wayIdsToWayNodeIdsNotPresent.insertMulti(newId, wayNodeIdToUse);
      }

      LOG_TRACE("Adding way node: " << wayNodeIdToUse << "...");
      pWay->addNode(wayNodeIdToUse);

      ++nodeIt;
    }
  }

  // Add tags
  _addTags(item, pWay);

  // Add way to map
  _map->addWay(pWay);

  LOG_TRACE("Loaded way: " << pWay->getElementId());
  //LOG_TRACE("Loaded way: " << pWay);

  _numRead++;
  if (_numRead % _statusUpdateInterval == 0)
  {
    PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
  }
}

void OsmJsonReader::_parseOverpassRelation(const pt::ptree& item)
{
  // Get info we need to construct our relation
  long id = 0;
  id = item.get("id", id);
  if (id == 0)
  {
    LOG_TRACE("Relatioon parsing error, no ID found, ignoring relation.");
    return;
  }

  if (_relationIdMap.contains(id) && _ignoreDuplicates)
  {
    LOG_TRACE("Ignoring relation id " << id << " already exists");
    return;
  }
  // See related note in OsmXmlReader::_createRelation.
//  if (_relationIdMap.contains(id))
//  {
//    throw HootException(QString("Duplicate realtion id %1 in map %2 encountered.").arg(id).arg(_path));
//  }

  long newId;
  if (_useDataSourceIds)
  {
    newId = id;
    LOG_TRACE("Using source relation id: " << newId << "...");
  }
  else
  {
    newId = _map->createNextRelationId();
    LOG_TRACE("Created new relation id: " << newId << "...");
  }
  _relationIdMap.insert(id, newId);

  long version = _getVersion(item, ElementType::Relation, newId);
  long changeset = _getChangeset(item);
  unsigned int timestamp = _getTimestamp(item);
  std::string user = _getUser(item);
  long uid = _getUid(item);

  // Construct Relation
  RelationPtr pRelation = std::make_shared<Relation>(_defaultStatus, newId, _defaultCircErr, "", changeset, version, timestamp, QString::fromStdString(user), uid);

  // Add members
  if (item.not_found() != item.find("members"))
  {
    pt::ptree members = item.get_child("members");
    for (auto memberIt = members.begin(); memberIt != members.end(); ++memberIt)
    {
      string typeStr = memberIt->second.get("type", string(""));
      const long refId = memberIt->second.get("ref", -1l); // default -1 ?
      long refIdToUse = refId;
      string role = memberIt->second.get("role", string(""));

      // See related note in _parseOverpassWay about loading child refs regardless of whether they
      // have yet been parsed or not.
      bool okToAdd = true;
      if (typeStr == "node")
      {
        if (!_useDataSourceIds)
        {
          auto nodeMemberIdItr = _nodeIdMap.find(refId);
          if (nodeMemberIdItr != _nodeIdMap.end())
          {
            refIdToUse = nodeMemberIdItr.value();
            LOG_TRACE("Retrieved mapped node member ID: " << refIdToUse << " for ID: " << refId);
          }
          else
            _relationIdsToNodeMemberIdsNotPresent.insertMulti(newId, refIdToUse);
        }
      }
      else if (typeStr == "way")
      {
        if (!_useDataSourceIds)
        {
          auto wayMemberIdItr = _wayIdMap.find(refId);
          if (wayMemberIdItr != _wayIdMap.end())
          {
            refIdToUse = wayMemberIdItr.value();
            LOG_TRACE("Retrieved mapped way member ID: " << refIdToUse << " for ID: " << refId);
          }
          else
            _relationIdsToWayMemberIdsNotPresent.insertMulti(newId, refIdToUse);
        }
      }
      else if (typeStr == "relation")
        refIdToUse = _getRelationId(refId);
      else
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN(
            "Found a relation member with unexpected type: " << typeStr << " in relation (" <<
            id << ")");
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
        LOG_TRACE("Adding relation relation member: " << refIdToUse << "...");
        pRelation->addElement(QString::fromStdString(role),
                              ElementType::fromString(QString::fromStdString(typeStr)), refIdToUse);
      }
    }
  }

  // Add tags
  _addTags(item, pRelation);

  // Add relation to map
  _map->addRelation(pRelation);

  LOG_TRACE("Loaded relation: " << pRelation->getElementId());
  //LOG_TRACE("Loaded relation: " << pRelation);

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
      _relationIdsToRelationMemberIdsNotPresent.insertMulti(fileId, newId);
    }
    else
      newId = _relationIdMap[fileId];
  }
  return newId;
}

void OsmJsonReader::_addTags(const boost::property_tree::ptree& item, ElementPtr pElement) const
{
  // Find tags and add them
  if (item.not_found() != item.find("tags"))
  {
    pt::ptree tags = item.get_child("tags");
    for (auto tagIt = tags.begin(); tagIt != tags.end(); ++tagIt)
    {
      const QString key = QString::fromStdString(tagIt->first).trimmed();
      const QString value = QString::fromStdString(tagIt->second.get_value<string>()).trimmed();

      // If we are a CE key, need to set it on the element object rather than add it as a tag.
      // Arbitrarily pick the first error tag found. If the element has both, the last one parsed
      // will be used. We're not expecting elements to have more than one CE tag.
      if (_circularErrorTagKeys.contains(key))
        pElement->setCircularError(Meters(value.toInt()));
      else if (!value.isEmpty())
        pElement->setTag(key, value);
    }
  }
}

void OsmJsonReader::_readFromHttp()
{
  if (!_sourceUrl.isValid())
    throw HootException("Invalid URL: " + _sourceUrl.toString(QUrl::RemoveUserInfo));
  if (!_isOverpass)
    throw HootException("Unable to read non-Overpass JSON sources");
  //  When reading in from the Overpass there won't be duplicates unless we are
  //  dividing up the bounds into smaller quadrants that fit below the 0.25 degrees
  //  squared limits, when we do it is safe to ignore duplicate elements
  setIgnoreDuplicates(true);
  //  Update the `srsname` parameter to use EPSG:4326
  QUrlQuery urlQuery(_sourceUrl);
  if (urlQuery.hasQueryItem("srsname"))
    urlQuery.removeQueryItem("srsname");
  urlQuery.addQueryItem("srsname", "EPSG:4326");
  geos::geom::Envelope env;

  if (!_bounds) // If no bounds was set, the read method still expects an empty one.
    env = geos::geom::Envelope();
  else          // Use the envelope of the boundsto throw away any non-retangular bounds that may have been passed.
    env = *(_bounds->getEnvelopeInternal());

  //  Spin up the threads
  beginRead(_sourceUrl, env);
  //  Iterate all of the XML results
  while (hasMoreResults())
  {
    QString jsonResult;
    //  Get one JSON string at a time
    if (getSingleResult(jsonResult))
    {
      _loadJSON(jsonResult);
      _parseOverpassJson();
    }
    else
      _sleep();
  }
}

long OsmJsonReader::_getVersion(const boost::property_tree::ptree& item, ElementType::Type type, long id) const
{
  return _getVersion("version", item, type, id);
}

long OsmJsonReader::_getVersion(const std::string& field_name, const boost::property_tree::ptree& item, ElementType::Type type, long id) const
{
  long version = ElementData::VERSION_EMPTY;
  version = item.get(field_name, version);
  LOG_VART(version);
  if (_warnOnVersionZeroElement && version == 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Element with version = 0: " << ElementId(type, id));
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  return version;
}

long OsmJsonReader::_getChangeset(const boost::property_tree::ptree& item) const
{
  return _getChangeset("changeset", item);
}

long OsmJsonReader::_getChangeset(const std::string& field_name, const boost::property_tree::ptree& item) const
{
  long changeset = ElementData::CHANGESET_EMPTY;
  changeset = item.get(field_name, changeset);
  return changeset;
}

unsigned int OsmJsonReader::_getTimestamp(const boost::property_tree::ptree& item) const
{
  return _getTimestamp("timestamp", item);
}

unsigned int OsmJsonReader::_getTimestamp(const std::string& field_name, const boost::property_tree::ptree& item) const
{
  unsigned int timestamp = static_cast<unsigned int>(ElementData::TIMESTAMP_EMPTY);
  timestamp = item.get(field_name, timestamp);
  return timestamp;
}

std::string OsmJsonReader::_getUser(const boost::property_tree::ptree& item) const
{
  return _getUser("user", item);
}

std::string OsmJsonReader::_getUser(const std::string& field_name, const boost::property_tree::ptree& item) const
{
  std::string user = ElementData::USER_EMPTY.toStdString();
  user = item.get(field_name, user);
  return user;
}

long OsmJsonReader::_getUid(const boost::property_tree::ptree& item) const
{
  return _getUid("uid", item);
}

long OsmJsonReader::_getUid(const std::string& field_name, const boost::property_tree::ptree& item) const
{
  long uid = ElementData::UID_EMPTY;
  uid = item.get(field_name, uid);
  return uid;
}

}
