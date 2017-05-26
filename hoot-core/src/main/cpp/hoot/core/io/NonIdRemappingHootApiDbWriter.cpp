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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NonIdRemappingHootApiDbWriter.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapWriter, NonIdRemappingHootApiDbWriter)

NonIdRemappingHootApiDbWriter::NonIdRemappingHootApiDbWriter() :
HootApiDbWriter()
{
}

long NonIdRemappingHootApiDbWriter::_getRemappedElementId(const ElementId& eid)
{
  switch(eid.getType().getEnum())
  {
  case ElementType::Node:
    _nodeRemap[eid.getId()] = eid.getId();
    break;

  case ElementType::Way:
    _wayRemap[eid.getId()] = eid.getId();
    break;

  case ElementType::Relation:
    _relationRemap[eid.getId()] = eid.getId();
    break;

  default:
    LOG_ERROR("Tried to create or remap ID for invalid type");
    throw NotImplementedException();
  }

  LOG_TRACE("No ID remap performed.  Returning original ID " << eid);
  return eid.getId();
}

}
