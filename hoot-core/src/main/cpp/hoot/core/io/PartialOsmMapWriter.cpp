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
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PartialOsmMapWriter.h"

#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/elements/Way.h>
#include "ElementInputStream.h"

namespace hoot
{

PartialOsmMapWriter::PartialOsmMapWriter()
{
}

void PartialOsmMapWriter::write(boost::shared_ptr<const OsmMap> map)
{
  writePartial(map);
  finalizePartial();
}

void PartialOsmMapWriter::writePartial(const boost::shared_ptr<const OsmMap>& map)
{
  const NodeMap& nm = map->getNodes();
  for (NodeMap::const_iterator it = nm.begin(); it != nm.end(); ++it)
  {
    writePartial(it->second);
  }

  const WayMap& wm = map->getWays();
  for (WayMap::const_iterator it = wm.begin(); it != wm.end(); ++it)
  {
    writePartial(it->second);
  }

  const RelationMap& rm = map->getRelations();
  for (RelationMap::const_iterator it = rm.begin(); it != rm.end(); ++it)
  {
    writePartial(it->second);
  }
}

void PartialOsmMapWriter::writePartial(const boost::shared_ptr<OsmMap>& map)
{
  writePartial((const boost::shared_ptr<const OsmMap>)map);
}

void PartialOsmMapWriter::writePartial(const boost::shared_ptr<const Element>& e)
{
  switch (e->getElementType().getEnum())
  {
  case ElementType::Node:
    writePartial(dynamic_pointer_cast<const Node>(e));
    break;
  case ElementType::Way:
    writePartial(dynamic_pointer_cast<const Way>(e));
    break;
  case ElementType::Relation:
    writePartial(dynamic_pointer_cast<const Relation>(e));
    break;
  default:
    throw HootException("Unexpected element type: " + e->getElementType().toString());
  }
}

void PartialOsmMapWriter::writePartial(const boost::shared_ptr<Relation>& r)
{
  writePartial((const boost::shared_ptr<const Relation>)r);
}

void PartialOsmMapWriter::writeElement(ElementInputStream& in)
{
  ElementPtr ele = in.readNextElement();
  writePartial(ele);
}

void PartialOsmMapWriter::writeElement(ElementPtr &element)
{
  if (element != 0)
  {
    writePartial(element);
  }
}

}
