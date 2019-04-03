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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "GeometryModifierVisitor.h"

// Hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, GeometryModifierVisitor)

GeometryModifierVisitor::GeometryModifierVisitor()
{
}

void GeometryModifierVisitor::visit(const ElementPtr& pElement)
{
  _numProcessed++;

  // only process Ways
  if( pElement->getElementType() != ElementType::Way ) return;

  Tags& tags = pElement->getTags();

  // for now use a hardocded filter to get a basic framework going
  if( tags.find("aeroway") == tags.end() ) return;

  if( tags["aeroway"] == "runway")
  {
    extrude( boost::dynamic_pointer_cast<Way>(pElement));

    tags["Test"] = "GeometryModifierVisitor";
    _numAffected++;
  }
}

void GeometryModifierVisitor::extrude(const WayPtr& pWay)
{
  int nodeCount = pWay->getNodeCount();

  // too small, nothing to do
  if( nodeCount < 2 ) return;

  // create poly way and add it to map
  WayPtr pPoly( new Way(Status::Unknown1, _pMap->createNextWayId(), -1) );
  pPoly->setTag("name", "GeneratedPoly");
  _pMap->addElement(pPoly);

  // build poly by extruding existing nodes
  const std::vector<long> nodeIds = pWay->getNodeIds();
  for (uint i = 0; i < nodeIds.size(); i++)
  {
    const NodePtr pNode = _pMap->getNode(nodeIds[i]);

    // Coordinate c = pNode->toCoordinate();
    // boost::shared_ptr<geos::geom::Point> p = pNode->toPoint();
    // ...
  }

  LOG_VAR(nodeCount);
}

} // namespace hoot
