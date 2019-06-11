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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "CollapsePolyGeoModifierAction.h"

// geos
#include <geos/algorithm/MinimumDiameter.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateSequence.h>

// Hoot
#include <hoot/core/elements/ElementConverter.h>
#include <hoot/core/ops/RemoveNodeByEid.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/CoordinateExt.h>

using namespace geos::geom;
using namespace geos::algorithm;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, CollapsePolyGeoModifierAction)

const QString CollapsePolyGeoModifierAction::MAX_AREA_PARAM = "max_area_in_m";
const QString CollapsePolyGeoModifierAction::MAX_LENGTH_PARAM = "max_length_in_m";

bool CollapsePolyGeoModifierAction::processElement(const ElementPtr& pElement, OsmMap* pMap)
{
  // only process closed area ways
  if (pElement->getElementType() != ElementType::Way) return false;
  const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pElement);
  if (!pWay->isClosedArea()) return false;

  OsmMapPtr mapPtr = pMap->shared_from_this();
  ElementConverter elementConverter(mapPtr);

  bool checkLength = _length != 0;
  bool checkArea = _area != 0;

  if (checkArea || checkLength)
  {
    std::shared_ptr<Polygon> pPoly = elementConverter.convertToPolygon(pWay);

    // calculate poly area only if we need it
    double polyArea = checkArea ? pPoly->getArea() : 0;

    // calculate polygon length only if we need it
    double polyLength = 0;

    if (checkLength)
    {
      // calculate minimum rectangle/aligned bounding box
      Geometry* pMinRect = MinimumDiameter::getMinimumRectangle(pPoly.get());
      CoordinateSequence* pMinRectCoords = pMinRect->getCoordinates();

      /* Debug polygon
      WayPtr pDebugWay(new Way(Status::Unknown1, pMap->createNextWayId()));
      pMap->addElement(pDebugWay);

      for (size_t i = 0; i < pMinRectCoords->getSize(); i++)
      {
        Coordinate pos = pMinRectCoords->getAt(i);
        NodePtr pNode(new Node(Status::Unknown1, pMap->createNextNodeId(), pos));
        pDebugWay->addNode(pNode->getId());
        pMap->addElement(pNode);
      }
      */

      if (pMinRectCoords->getSize() > 2)
      {
        double len1 = (CoordinateExt(pMinRectCoords->getAt(0)) - CoordinateExt(pMinRectCoords->getAt(1))).length();
        double len2 = (CoordinateExt(pMinRectCoords->getAt(1)) - CoordinateExt(pMinRectCoords->getAt(2))).length();
        polyLength = std::max(len1,len2);
      }
    }

    if ((checkArea && (polyArea < _area)) ||
        (checkLength && (polyLength < _length)))
    {
      Coordinate centroid;
      if (pPoly->getCentroid(centroid) == false)
      {
        // throwing a HootException might be too harsh
        LOG_ERROR("Collapse polygon modifier could not calculate centroid for element id " + pElement->getId());
        return false;
      }

      NodePtr pNode(new Node(Status::Unknown1, pMap->createNextNodeId(), centroid));

      // copy tags from original way to node
      pNode->setTags(pWay->getTags());

      std::vector<long> nodeIds = pWay->getNodeIds();

      // replace original way with node
      pMap->replace(pWay, pNode);

      // remove unused nodes of previous way
      for (long nodeId : nodeIds)
      {
        RemoveNodeByEid removeOp(nodeId, true, false, true);
        removeOp.apply(mapPtr);
      }

      return true;
    }
  }

  return false;
}

void CollapsePolyGeoModifierAction::parseArguments(const QHash<QString, QString>& arguments)
{
  _area = DEFAULT_AREA;
  _length = DEFAULT_LENGTH;

  if (arguments.keys().contains(MAX_AREA_PARAM))
  {
    _area = arguments[MAX_AREA_PARAM].toDouble();
  }

  if (arguments.keys().contains(MAX_LENGTH_PARAM))
  {
    _length = arguments[MAX_LENGTH_PARAM].toDouble();
  }
}

}
