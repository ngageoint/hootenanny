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
 * @copyright Copyright (C) 2015-2023 Maxar (http://www.maxar.com/)
 */

#include "OsmXmlReader.h"

// Hoot
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Tags.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/GeometryUtils.h>
#include <hoot/core/io/IoUtils.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/DateTimeUtils.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/visitors/ConstElementVisitor.h>
#include <hoot/core/visitors/ReportMissingElementsVisitor.h>

// Qt
#include <QBuffer>
#include <QDateTime>

// Standard
#include <cstdlib> // std::system()
#include <cstdio> // for std::remove()

namespace hoot
{

int OsmXmlReader::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapReader, OsmXmlReader)

OsmXmlReader::OsmXmlReader()
  : _status(Status::Invalid),
    _keepStatusTag(false),
    _useFileStatus(false),
    _useDataSourceId(false),
    _numRead(0),
    _osmFound(false),
    _missingNodeCount(0),
    _missingWayCount(0),
    _badAccuracyCount(0),
    _addSourceDateTime(true),
    _wayId(0),
    _relationId(0),
    _inputCompressed(false),
    _addChildRefsWhenMissing(false),
    _logWarningsForMissingElements(true),
    _statusUpdateInterval(1000),
    _keepImmediatelyConnectedWaysOutsideBounds(false)
{
  setConfiguration(conf());
}

OsmXmlReader::~OsmXmlReader()
{
  close();
}

void OsmXmlReader::setConfiguration(const Settings& conf)
{
  PartialOsmMapReader::setConfiguration(conf);

  ConfigOptions configOptions(conf);
  setDefaultAccuracy(configOptions.getCircularErrorDefaultValue());
  setKeepStatusTag(configOptions.getReaderKeepStatusTag());
  setUseFileStatus(configOptions.getReaderUseFileStatus());
  setAddSourceDateTime(configOptions.getReaderAddSourceDatetime());
  setPreserveAllTags(configOptions.getReaderPreserveAllTags());
  setStatusUpdateInterval(configOptions.getTaskStatusUpdateInterval() * 10);
  setBounds(Boundable::loadBounds(configOptions));
  // If a bounds was set and we don't want to remove missing elements as a result of cropping, we
  // need to modify the reader to allow reading in the missing refs.
  if (_bounds.get() && !configOptions.getBoundsRemoveMissingElements())
    setAddChildRefsWhenMissing(true);
  else
    setAddChildRefsWhenMissing(configOptions.getMapReaderAddChildRefsWhenMissing());
  setKeepImmediatelyConnectedWaysOutsideBounds(configOptions.getBoundsKeepImmediatelyConnectedWaysOutsideBounds());
  setWarnOnVersionZeroElement(configOptions.getReaderWarnOnZeroVersionElement());
  setLogWarningsForMissingElements(configOptions.getLogWarningsForMissingElements());
  setCircularErrorTagKeys(configOptions.getCircularErrorTagKeys());
}

void OsmXmlReader::_parseTimeStamp(const QXmlAttributes& attributes) const
{
  if ((attributes.value("timestamp") != "") &&
      (attributes.value("timestamp") != "1970-01-01T00:00:00Z") &&
      (_addSourceDateTime == true))
  {
    _element->setTag(MetadataTags::SourceDateTime(), attributes.value("timestamp"));
  }
}

void OsmXmlReader::_createNode(const QXmlAttributes& attributes)
{
  _element.reset();

  long id = _parseLong(attributes.value("id"));
  LOG_VART(id);

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
  if (_useDataSourceId)
    newId = id;
  else
    newId = _map->createNextNodeId();
  LOG_VART(newId);
  _nodeIdMap.insert(id, newId);

  double x = _parseDouble(attributes.value("lon"));
  double y = _parseDouble(attributes.value("lat"));

  // Check the next 3 attributes to see if a value exists, if not, assign a default since these
  // are not officially required by the DTD.
  long version = _getVersion(attributes);
  long changeset = _getChangeset(attributes);
  unsigned int timestamp = _getTimestamp(attributes);
  QString user = _getUser(attributes);
  long uid = _getUid(attributes);

  //LOG_VART(version);
  if (_warnOnVersionZeroElement && version == 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Element with version = 0: " << ElementId(ElementType::Node, newId));
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  _element = Node::newSp(_status, newId, x, y, _defaultCircularError, changeset, version, timestamp, user, uid);

  if (_element->getTags().getInformationCount() > 0)
    _parseTimeStamp(attributes);
}

void OsmXmlReader::_createWay(const QXmlAttributes& attributes)
{
  _element.reset();

  long id = _parseLong(attributes.value("id"));

  if (_wayIdMap.contains(id))
  {
    if (_ignoreDuplicates)
    {
      LOG_TRACE("Ignoring way id " << id << " already exists");
      return;
    }
    else
      throw HootException(QString("Duplicate way id %1 in map %2 encountered.").arg(_wayId).arg(_url));
  }

  _wayId = id;

  long newId;
  if (_useDataSourceId)
    newId = _wayId;
  else
    newId = _map->createNextWayId();

  _wayIdMap.insert(_wayId, newId);

  // check the next 3 attributes to see if a value exists, if not, assign a default since
  // these are not officially required by the DTD
  long version = _getVersion(attributes);
  long changeset = _getChangeset(attributes);
  unsigned int timestamp = _getTimestamp(attributes);
  QString user = _getUser(attributes);
  long uid = _getUid(attributes);

  LOG_VART(version);
  if (_warnOnVersionZeroElement && version == 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Element with version = 0: " << ElementId(ElementType::Way, newId));
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  _element = std::make_shared<Way>(_status, newId, _defaultCircularError, changeset, version, timestamp, user, uid);

  _parseTimeStamp(attributes);
}

void OsmXmlReader::_createRelation(const QXmlAttributes& attributes)
{
  _element.reset();

  long id = _parseLong(attributes.value("id"));

  if (_ignoreDuplicates && _relationIdMap.contains(id))
  {
    LOG_TRACE("Ignoring relation id " << id << " already exists");
    return;
  }

  _relationId = id;

  long newId = _getRelationId(_relationId);

  // Check the next 3 attributes to see if a value exists, if not, assign a default since these are
  // not officially required by the DTD.
  long version = _getVersion(attributes);
  long changeset = _getChangeset(attributes);
  unsigned int timestamp = _getTimestamp(attributes);
  QString user = _getUser(attributes);
  long uid = _getUid(attributes);

  LOG_VART(version);
  if (_warnOnVersionZeroElement && version == 0)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Element with version = 0: " << ElementId(ElementType::Relation, newId));
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }

  _element = std::make_shared<Relation>(_status, newId, _defaultCircularError, "", changeset, version, timestamp, user, uid);

  _parseTimeStamp(attributes);
}

bool OsmXmlReader::fatalError(const QXmlParseException& exception)
{
  _errorString =
    QObject::tr("OsmXmlReader: Parse error at line %1, column %2:\n%3")
      .arg(exception.lineNumber())
      .arg(exception.columnNumber())
      .arg(exception.message());
  return false;
}

bool OsmXmlReader::seekAndReset(uint64_t pos)
{
  _nodeIdMap.clear();
  _wayIdMap.clear();
  _relationIdMap.clear();
  _inputFile.seek(pos);
  _streamReader.setDevice(&_inputFile);
  return true;
}

uint64_t OsmXmlReader::pos() const
{
  return _inputFile.pos();
}

bool OsmXmlReader::isSupported(const QString& url) const
{
  QStringList validExtensions = supportedFormats().split(";");
  const QString checkString(url.toLower());
  // support compressed osm files
  for (const auto& ext : qAsConst(validExtensions))
  {
    if (checkString.endsWith(ext))
      return true;
  }
  // If we fall out of loop, no dice
  return false;
}

double OsmXmlReader::_parseDouble(const QString& s) const
{
  bool ok;
  double result = s.toDouble(&ok);

  if (ok == false)
    throw HootException("Error parsing double: " + s);

  return result;
}

long OsmXmlReader::_parseLong(const QString& s) const
{
  bool ok;
  long result = s.toLong(&ok);

  if (ok == false)
    throw HootException("Error parsing long: " + s);

  return result;
}

void OsmXmlReader::read(const OsmMapPtr& map)
{
  LOG_VART(_status);
  LOG_VART(_useDataSourceId);
  LOG_VART(_useFileStatus);
  LOG_VART(_keepStatusTag);
  LOG_VART(_preserveAllTags);
  LOG_VART(_url);

  // Reusing the reader for multiple files has two options, the first is the default where the
  // reader is reset and duplicates error out. The second is where duplicates are ignored in the
  // same file and across files so the ID maps aren't reset.
  if (!_ignoreDuplicates)
  {
    _nodeIdMap.clear();
    _relationIdMap.clear();
    _wayIdMap.clear();

    _numRead = 0;
    finalizePartial();
  }
  // Note that if the incoming map is already partially populated from a previous load, like how
  // ConflateExecutor works via multiple calls to IoUtils::loadMap, that we're not recording its IDs
  // in order to check for duplicates when loading this subsequent input (#5009). If we're not
  // ignoring duplicates we won't get the expected behavior of an error when loading duplicate IDs
  // and instead the feature with the duplicate ID from the subsequent input will overwrite that
  // from the previous input.
  _map = map;
  _map->appendSource(_url);

  if (_url.endsWith(".osm.bz2") || _url.endsWith(".osm.gz"))
  {
    _inputCompressed = true;
    _uncompressInput();
  }

  // do xml parsing
  QXmlSimpleReader reader;
  reader.setContentHandler(this);
  reader.setErrorHandler(this);

  QFile file(_url);
  if (!file.open(QFile::ReadOnly | QFile::Text))
    throw HootException(QObject::tr("Error opening OSM file for parsing: %1").arg(_url));
  LOG_DEBUG("File " << _url << " opened for read");

  QXmlInputSource xmlInputSource(&file);
  if (reader.parse(xmlInputSource) == false)
    throw HootException(_errorString);
  file.close();
  LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));

  // This is meant for taking a larger input down to a smaller size. Clearly, if the input data's
  // bounds is already smaller than _bounds, this will have no effect. Also, We don't support
  // cropping during streaming, and there is a check in IoUtils::isStreamableIo to make
  // sure nothing tries to stream with this reader when a bounds has been set.
  if (_bounds.get() && getCropOnReadIfBounded())
  {
    LOG_VARD(_bounds->toString());
    IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
    LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
  }

  // If cropping was run with missing element being removed, then this shouldn't be necessary.
  ReportMissingElementsVisitor visitor;
  _map->visitRw(visitor);
  if (visitor.getMissingCount() > 0)
  {
    LOG_INFO("\t" << visitor.getInitStatusMessage());
    LOG_DEBUG("\t" << visitor.getCompletedStatusMessage());
  }

  _map.reset();
}

OsmMapPtr OsmXmlReader::fromXml(const QString& xml, const bool useDataSourceId, const bool useDataSourceStatus,
                                const bool keepStatusTag, const bool addChildRefsWhenMissing)
{
  if (xml.isEmpty())
    return OsmMapPtr();

  LOG_DEBUG("Reading map from xml...");
  OsmMapPtr map = std::make_shared<OsmMap>();
  OsmXmlReader reader;
  reader.setUseDataSourceIds(useDataSourceId);
  reader.setUseFileStatus(useDataSourceStatus);
  reader.setKeepStatusTag(keepStatusTag);
  reader.setAddChildRefsWhenMissing(addChildRefsWhenMissing);
  reader.readFromString(xml, map);
  return map;
}

void OsmXmlReader::readFromString(const QString& xml, const OsmMapPtr& map)
{
  _numRead = 0;
  finalizePartial();
  _map = map;

  LOG_DEBUG("Parsing map from xml...");

  if (!xml.isEmpty())
  {
    // do xml parsing
    QXmlSimpleReader reader;
    reader.setContentHandler(this);
    reader.setErrorHandler(this);

    QBuffer buffer;
    buffer.setData(xml.toUtf8());

    QXmlInputSource xmlInputSource(&buffer);
    if (reader.parse(xmlInputSource) == false)
    {
      LOG_ERROR(_errorString);
    }
    else
    {
      LOG_DEBUG("Parsed map from xml.");

      LOG_VARD(_bounds.get());
      if (_bounds.get())
      {
        IoUtils::cropToBounds(_map, _bounds, _keepImmediatelyConnectedWaysOutsideBounds);
        LOG_VARD(StringUtils::formatLargeNumber(_map->getElementCount()));
      }

      ReportMissingElementsVisitor visitor;
      LOG_INFO("\t" << visitor.getInitStatusMessage());
      _map->visitRw(visitor);
      LOG_INFO("\t" << visitor.getCompletedStatusMessage());
    }
  }
  _map.reset();
}

void OsmXmlReader::read(const QString& path, const OsmMapPtr& map)
{
  open(path);
  read(map);
}

const QString& OsmXmlReader::_saveMemory(const QString& s)
{
  if (!_strings.contains(s))
    _strings[s] = s;
  return _strings[s];
}

bool OsmXmlReader::startElement(const QString& /*namespaceURI*/, const QString& /*localName*/,
                                const QString& qName, const QXmlAttributes& attributes)
{
  try
  {
    if (!_osmFound)
    {
      if (qName != "osm")
        throw HootException("The file is not an OSM file.");
      else
      {
        _osmFound = true;
        if (attributes.value("version") != "0.6")
          throw HootException("Only version 0.6 OSM files are supported.");
      }
    }

    if (qName == QLatin1String("node"))
    {
      if (attributes.value("action") != QLatin1String("delete"))
        _createNode(attributes);
      else
      {
        LOG_TRACE("Node " << attributes.value("id") << " set to delete.");
        _element.reset();
      }
    }
    else if (qName == QLatin1String("way"))
    {
      if (attributes.value("action") != QLatin1String("delete"))
        _createWay(attributes);
      else
      {
        LOG_TRACE("Way " << attributes.value("id") << " set to delete.");
        _element.reset();
      }
    }
    else if (qName == QLatin1String("relation"))
    {
      if (attributes.value("action") != QLatin1String("delete"))
        _createRelation(attributes);
      else
      {
        LOG_TRACE("Relation " << attributes.value("id") << " set to delete.");
        _element.reset();
      }
    }
    else if (qName == QLatin1String("nd") && _element)
    {
      long ref = _parseLong(attributes.value("ref"));
      const bool nodePresent = _nodeIdMap.contains(ref);

      if (!nodePresent)
      {
        if (_addChildRefsWhenMissing)
        {
          WayPtr w = std::dynamic_pointer_cast<Way, Element>(_element);
          w->addNode(ref);
        }
        else
        {
          _missingNodeCount++;
          if (_logWarningsForMissingElements)
          {
            if (logWarnCount < Log::getWarnMessageLimit())
            {
              LOG_WARN(
                "Skipping missing " << ElementId(ElementType::Node, ref) << " in " <<
                ElementId(ElementType::Way, _wayId) << "...");
            }
            else if (logWarnCount == Log::getWarnMessageLimit())
            {
              LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
            }
            logWarnCount++;
          }
        }
      }
      else
      {
        long newRef = _nodeIdMap.value(ref);
        LOG_TRACE("Adding way node: " << newRef << "...");
        WayPtr w = std::dynamic_pointer_cast<Way, Element>(_element);
        w->addNode(newRef);
      }
    }
    else if (qName == QLatin1String("member") && _element)
    {
      long ref = _parseLong(attributes.value("ref"));
      QString type = attributes.value("type");
      QString role = attributes.value("role");

      RelationPtr r = std::dynamic_pointer_cast<Relation, Element>(_element);

      if (type == QLatin1String("node"))
      {
        const bool nodePresent = _nodeIdMap.contains(ref);
        if (!nodePresent)
        {
          if (_addChildRefsWhenMissing)
            r->addElement(role, ElementType::Node, ref);
          else
          {
            _missingNodeCount++;
            if (_logWarningsForMissingElements)
            {
              if (logWarnCount < Log::getWarnMessageLimit())
              {
                LOG_WARN(
                  "Skipping missing " << ElementId(ElementType::Node, ref) << " in " <<
                  ElementId(ElementType::Relation, _relationId) << "...");
              }
              else if (logWarnCount == Log::getWarnMessageLimit())
              {
                LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
              }
              logWarnCount++;
            }
          }
        }
        else
        {
          long newRef = _nodeIdMap.value(ref);
          LOG_TRACE("Adding relation node member: " << newRef << "...");
          r->addElement(role, ElementType::Node, newRef);
        }
      }
      else if (type == QLatin1String("way"))
      {
        const bool wayPresent = _wayIdMap.contains(ref);

        if (!wayPresent)
        {
          if (_addChildRefsWhenMissing)
            r->addElement(role, ElementType::Way, ref);
          else
          {
            _missingWayCount++;
            if (_logWarningsForMissingElements)
            {
              if (logWarnCount < Log::getWarnMessageLimit())
              {
                LOG_WARN(
                  "Skipping missing " << ElementId(ElementType::Way, ref) << " in " <<
                  ElementId(ElementType::Relation, _relationId) << "...");
              }
              else if (logWarnCount == Log::getWarnMessageLimit())
              {
                LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
              }
              logWarnCount++;
            }
          }
        }
        else
        {
          long newRef = _wayIdMap.value(ref);
          LOG_TRACE("Adding relation way member: " << newRef << "...");
          r->addElement(role, ElementType::Way, newRef);
        }
      }
      else if (type == QLatin1String("relation"))
      {
        // relations may be out of order so we don't check for consistency at this stage.
        long newRef = _getRelationId(ref);
        LOG_TRACE("Adding relation relation member: " << newRef << "...");
        r->addElement(role, ElementType::Relation, newRef);
      }
      else
      {
        if (_logWarningsForMissingElements)
        {
          if (logWarnCount < Log::getWarnMessageLimit())
          {
            LOG_WARN("Found a relation member with unexpected type: " << type << " in relation ("
                       << _relationId << ")");
          }
          else if (logWarnCount == Log::getWarnMessageLimit())
          {
            LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
          }
          logWarnCount++;
        }
      }
    }
    else if (qName == QLatin1String("tag") && _element)
    {
      const QString& key = _saveMemory(attributes.value("k").trimmed());
      const QString& value = _saveMemory(attributes.value("v").trimmed());
      LOG_VART(key);
      LOG_VART(value);
      if (!key.isEmpty() && !value.isEmpty())
      {
        if (_useFileStatus && key == MetadataTags::HootStatus())
        {
          _element->setStatus(Status::fromString(value));
          LOG_VART(_element->getStatus());

          if (_keepStatusTag)
            _element->setTag(key, value);
        }
        else if (key == MetadataTags::RelationType() &&
                 _element->getElementType() == ElementType::Relation)
        {
          RelationPtr r = std::dynamic_pointer_cast<Relation, Element>(_element);
          r->setType(value);

          if (_preserveAllTags)
            _element->setTag(key, value);
        }
        // Arbitrarily pick the first error tag found. If the element has both, the last one parsed
        // will be used. We're not expecting elements to have more than one CE tag.
        else if (_circularErrorTagKeys.contains(key))
        {
          bool ok;
          Meters circularError = value.toDouble(&ok);

          if (circularError > 0 && ok)
            _element->setCircularError(circularError);
          else
          {
            Tags t1;
            t1.set(key, value);

            if (!_setElementCircularError(t1, key))
            {
              _badAccuracyCount++;
              if (logWarnCount < Log::getWarnMessageLimit())
              {
                LOG_WARN("Bad circular error value: " << value.toStdString());
              }
              else if (logWarnCount == Log::getWarnMessageLimit())
              {
                LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
              }
              logWarnCount++;
            }
          }
          if (_preserveAllTags)
          {
            LOG_TRACE("setting tag with key: " << key << " and value: " << value);
            _element->setTag(key, value);
          }
        }
        else if (key == MetadataTags::HootWayNodeCount())
        {
          bool ok;
          size_t node_count = value.toULong(&ok);
          if (!ok)
          {
            LOG_TRACE("Unable to parse '" << MetadataTags::HootWayNodeCount() << "' tag.");
          }
          if (_element->getElementType() == ElementType::Way)
          {
            WayPtr w = std::dynamic_pointer_cast<Way, Element>(_element);
            if (w->getNodeCount() != node_count)
            {
              LOG_TRACE("Way node count tag does not equal actual way node count.");
            }
          }
          else
          {
            LOG_TRACE(_element->getElementType().toString() << " elements shouldn't contain way node count tag.");
          }
          //  In the end, this tag isn't loaded because it is calculated
        }
        else
        {
          if (key != MetadataTags::HootId() && !value.isEmpty())
          {
            LOG_TRACE("setting tag with key: " << key << " and value: " << value);
            _element->setTag(key, value);
          }
        }
      }
    }
  }
  catch (const HootException& e)
  {
    _errorString = e.what();
    return false;
  }

  return true;
}

bool OsmXmlReader::_setElementCircularError(const Tags& tags, const QString& key) const
{
  bool result = false;
  try
  {
    Meters circularError = tags.getLength(key).value();
    if (circularError > 0)
    {
      _element->setCircularError(circularError);
      result = true;
    }
  }
  catch (const HootException& e)
  {
    LOG_TRACE("Set circular error failed: " + e.getWhat());
  }
  return result;
}

bool OsmXmlReader::endElement(const QString& /* namespaceURI */,
                              const QString& /* localName */,
                              const QString& qName)
{
  if (_element)
  {
    if (qName == QLatin1String("node"))
    {
      NodePtr n = std::dynamic_pointer_cast<Node, Element>(_element);
      _map->addNode(n);
      _element.reset();
      LOG_TRACE("Added: " << n->getElementId());
      _numRead++;
    }
    else if (qName == QLatin1String("way"))
    {
      WayPtr w = std::dynamic_pointer_cast<Way, Element>(_element);
      _map->addWay(w);
      _element.reset();
      LOG_TRACE("Added: " << w->getElementId());
      _numRead++;
    }
    else if (qName == QLatin1String("relation"))
    {
      RelationPtr r = std::dynamic_pointer_cast<Relation, Element>(_element);
      _map->addRelation(r);
      _element.reset();
      LOG_TRACE("Added: " << r->getElementId());
      _numRead++;
    }

    if (_numRead % _statusUpdateInterval == 0)
    {
      PROGRESS_INFO("Read " << StringUtils::formatLargeNumber(_numRead) << " elements from input.");
    }
  }
  return true;
}

long OsmXmlReader::_getRelationId(long fileId)
{
  long newId;
  if (_useDataSourceId)
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
      newId = _relationIdMap[fileId];
  }
  return newId;
}

std::shared_ptr<OGRSpatialReference> OsmXmlReader::getProjection() const
{
  if (!_wgs84)
    _wgs84 = MapProjector::createWgs84Projection();
  return _wgs84;
}

void OsmXmlReader::_uncompressInput()
{
  // uncompress .osm.bz2 or .osm.gz files before processing
  QString originalFile;
  if (_url.endsWith(".osm.bz2") == true)
  {
    originalFile = _url;
    _url.chop(std::strlen(".bz2"));

    // "man bunzip2" confirms success return code is zero
    // -f option decompresses file even if decompressed file is already there
    // -k option is "keep", meaning don't delete input .osm.bz2
    const std::string cmd(std::string("bunzip2 -fk ") + originalFile.toStdString());
    LOG_DEBUG("Running uncompress command: " << cmd);

    int retVal;
    if ((retVal = std::system(cmd.c_str())) != 0)
    {
      QString error = QString("Error %1 returned from uncompress command: %2").arg(retVal).arg(QString::fromStdString(cmd));
      throw HootException(error);
    }

    LOG_DEBUG("Uncompress succeeded!");
  }
  else if (_url.endsWith(".osm.gz") == true)
  {
    originalFile = _url;
    _url.chop(std::strlen(".gz"));

    // "man gzip" confirms success return code is zero
    //  -d option is "decompress"
    //  -c option is "write on standard output, keep original files unchanged" meaning won't delete input .osm.gz
    const std::string cmd(std::string("gzip -dc ")
                          + originalFile.toStdString()
                          + " > " + _url.toStdString());
    LOG_DEBUG("Running uncompress command: " << cmd);

    int retVal;
    if ((retVal = std::system(cmd.c_str())) != 0)
    {
      QString error = QString("Error %1 returned from uncompress command: %2").arg(retVal).arg(QString::fromStdString(cmd));
      throw HootException(error);
    }

    LOG_DEBUG("Uncompress succeeded!");
  }
}

QXmlAttributes OsmXmlReader::_streamAttributesToAttributes(const QXmlStreamAttributes& streamAttributes) const
{
  QXmlAttributes attributes;
  for (const auto& streamAttribute : streamAttributes)
    attributes.append(streamAttribute.qualifiedName().toString(), "", "", streamAttribute.value().toString());
  return attributes;
}

bool OsmXmlReader::hasMoreElements()
{
  if (!_inputFile.isOpen())
  {
    finalizePartial();
    //  map needed for assigning new element ids only (not actually putting any of the elements that
    //  are read into this map, since this is the partial reading logic)
    _map = std::make_shared<OsmMap>();

    if (_url.endsWith(".osm.bz2") || _url.endsWith(".osm.gz"))
    {
      _inputCompressed = true;
      _uncompressInput();
    }

    _inputFile.setFileName(_url);
    if (!_inputFile.open(QFile::ReadOnly | QFile::Text))
      throw HootException(QObject::tr("Error opening OSM file for parsing: %1").arg(_url));
    _streamReader.setDevice(&_inputFile);

    //  check for a valid osm header as soon as the file is opened
    while (!_foundOsmHeaderXmlStartElement() && !_streamReader.atEnd())
      _streamReader.readNext();

    if (!_osmFound)
      throw HootException(_url + " is not an OSM file.");
  }

  //  chew up tokens until we find a node/way/relation start element or read to the end of the file
  while (!_foundOsmElementXmlStartElement() && !_streamReader.atEnd())
    _streamReader.readNext();

  if ((_streamReader.isEndElement() && _streamReader.name().toString() == "osm") || _streamReader.atEnd())
    return false;

  return true;
}

bool OsmXmlReader::_foundOsmHeaderXmlStartElement()
{
  //  this is a little redundant with the logic at the beginning startElement

  if (_streamReader.isStartElement() && _streamReader.name().toString() == "osm")
    _osmFound = true;

  if (_osmFound && _streamAttributesToAttributes(_streamReader.attributes()).value("version") != "0.6")
    throw HootException("Only version 0.6 OSM files are supported.");

  return _osmFound;
}

bool OsmXmlReader::_foundOsmElementXmlStartElement() const
{
  const QString xmlElementName = _streamReader.name().toString();
  return _streamReader.isStartElement() &&
         (xmlElementName == QLatin1String("node") || xmlElementName == QLatin1String("way") ||
          xmlElementName == QLatin1String("relation"));
}

bool OsmXmlReader::_foundOsmElementXmlEndElement() const
{
  const QString xmlElementName = _streamReader.name().toString();
  return _streamReader.isEndElement() &&
         (xmlElementName == QLatin1String("node") || xmlElementName == QLatin1String("way") ||
          xmlElementName == QLatin1String("relation"));
}

ElementPtr OsmXmlReader::readNextElement()
{
  //hasMoreElements should have always put us at a node/way/relation start xml element by this point
  if (!_foundOsmElementXmlStartElement())
    throw HootException("Call hasMoreElements before calling readNextElement.");

  //chew up tokens until we find the end of the node/way/relation or read to the end of the file;
  //if the osm file is valid, we should never read to the end of the file within this method, but
  //the check is put in as a precaution to avoid an endless loop if the file is invalid
  while (!_foundOsmElementXmlEndElement() && !_streamReader.atEnd())
  {
    //parse the start xml element; startElement is only interested in node/way/relation, tag,
    //way nodes, or relation members...ignores the rest
    //  this attribute conversion isn't the best for performance...but will leave as is for now
    if (_streamReader.isStartElement())
      startElement("", "", _streamReader.qualifiedName().toString(), _streamAttributesToAttributes(_streamReader.attributes()));

    _streamReader.readNext();
  }

  //this should never happen here
  if (_streamReader.atEnd())
    throw HootException("Error reading XML file: readNextElement reached end of file.");

  assert(_foundOsmElementXmlEndElement());

  //we're parsed the entire node/way/relation, so return it
  if (_element.get())
  {
    LOG_TRACE("Read " << _element->getElementId());
  }
  else
  {
    // must be a deleted element
    LOG_TRACE("Read empty element. Does your input data unnecessarily contain 'action=delete'?");
  }
  return _element;
}

void OsmXmlReader::close()
{
  finalizePartial();
  _inputFile.close();

  if (_inputCompressed)
  {
    // Delete the temp file
    std::remove(_url.toStdString().c_str());
    LOG_DEBUG("Removed decompressed file " << _url);
  }

  _map.reset();
}

void OsmXmlReader::finalizePartial()
{
  _osmFound = false;
  _missingNodeCount = 0;
  _missingWayCount = 0;
  _badAccuracyCount = 0;
}

long OsmXmlReader::_getVersion(const QXmlAttributes& attributes) const
{
  if (attributes.value("version") != "")
    return _parseLong(attributes.value("version"));
  return ElementData::VERSION_EMPTY;
}

long OsmXmlReader::_getChangeset(const QXmlAttributes& attributes) const
{
  if (attributes.value("changeset") != "")
    return _parseLong(attributes.value("changeset"));
  return ElementData::CHANGESET_EMPTY;
}

unsigned int OsmXmlReader::_getTimestamp(const QXmlAttributes& attributes) const
{
  if (attributes.value("timestamp") != "")
    return static_cast<unsigned int>(DateTimeUtils::fromTimeString(attributes.value("timestamp")));
  return static_cast<unsigned int>(ElementData::TIMESTAMP_EMPTY);
}

QString OsmXmlReader::_getUser(const QXmlAttributes& attributes) const
{
  if (attributes.value("user") != "")
    return attributes.value("user");
  return ElementData::USER_EMPTY;
}

long OsmXmlReader::_getUid(const QXmlAttributes& attributes) const
{
  if (attributes.value("uid") != "")
    return _parseLong(attributes.value("uid"));
  return ElementData::UID_EMPTY;
}

}
