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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OsmApiDbAwareHootApiDbReader.h"

// hoot
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapReader, OsmApiDbAwareHootApiDbReader)

OsmApiDbAwareHootApiDbReader::OsmApiDbAwareHootApiDbReader() :
HootApiDbReader()
{
}

OsmApiDbAwareHootApiDbReader::~OsmApiDbAwareHootApiDbReader()
{
  _osmApiDb.close();
}

void OsmApiDbAwareHootApiDbReader::open(QString urlStr)
{
  if (urlStr.startsWith("osmapidb"))
  {
    //something bad has happened if this occurs.  the hack to keep it from happening is in
    //OsmMapReaderFactory::createReader
    throw HootException("The OsmApiDbAwareHootApiDbReader does not read from an OSM API database.");
  }

  if (!_osmApiDb.isSupported(ConfigOptions().getOsmapidbIdAwareUrl()))
  {
    throw HootException(
      "The OsmApiDbAwareHootApiDbReader requires a valid OSM API DB URL for element id sequencing.");
  }

  HootApiDbReader::open(urlStr);
  _osmApiDb.open(ConfigOptions().getOsmapidbIdAwareUrl());
}

ElementId OsmApiDbAwareHootApiDbReader::_mapElementId(const OsmMap& /*map*/, ElementId oldId)
{
  ElementId result;

  long id = oldId.getId();
  //LOG_VARD(oldId);
  switch (oldId.getType().getEnum())
  {
    case ElementType::Node:
      if (_nodeIdMap.count(id) > 0)
      {
        result = ElementId::node(_nodeIdMap.at(id));
      }
      else
      {
        long newId = _osmApiDb.getNextId(ElementType::Node);
        _nodeIdMap[id] = newId;
        result = ElementId::node(newId);
      }
      break;
    case ElementType::Way:
      if (_wayIdMap.count(id) > 0)
      {
        result = ElementId::way(_wayIdMap.at(id));
      }
      else
      {
        long newId = _osmApiDb.getNextId(ElementType::Way);
        _wayIdMap[id] = newId;
        result = ElementId::way(newId);
      }
      break;
    case ElementType::Relation:
      if (_relationIdMap.count(id) > 0)
      {
        result = ElementId::relation(_relationIdMap.at(id));
      }
      else
      {
        long newId = _osmApiDb.getNextId(ElementType::Relation);
        _relationIdMap[id] = newId;
        result = ElementId::relation(newId);
      }
      break;
    default:
      throw IllegalArgumentException("Expected a valid element type, but got: " +
                                     QString::number(oldId.getType().getEnum()));
  }
  //LOG_VARD(result);

  return result;
}

}
