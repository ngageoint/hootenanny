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
 * @copyright Copyright (C) 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "WayToPolyGeoModifierAction.h"

// Hoot
#include <hoot/core/geometry/CoordinateExt.h>
#include <hoot/core/util/Factory.h>

// Standard
#include <math.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(GeometryModifierAction, WayToPolyGeoModifierAction)

const QString WayToPolyGeoModifierAction::WIDTH_TAG_PARAM = "width_tag_m";
const QString WayToPolyGeoModifierAction::DEFAULT_WIDTH_PARAM = "default_width_m";

bool WayToPolyGeoModifierAction::processElement(const ElementPtr& pElement, OsmMap* pMap)
{
  // only process Ways
  if (pElement->getElementType() != ElementType::Way) return false;

  // process the way as requested
  const WayPtr& pWay = std::dynamic_pointer_cast<Way>(pElement);
  long nodeCount = pWay->getNodeCount();
  bool isLoop = pWay->isSimpleLoop();

  // too small, nothing to do
  if (nodeCount < 2) return false;

  // find out what width to use
  Tags tags = pElement->getTags();
  double currWidth = _width;

  // if WIDTH_TAG_PARAM has a valid string, and a tag with the same name is found, use the width value in the tag
  if (!_widthTag.isEmpty() && tags.find(_widthTag) != tags.end())
  {
    double readWidth = tags[_widthTag].toDouble();
    if (readWidth > 0)
    {
      currWidth = readWidth;
    }
  }

  // build poly by 'extruding' existing nodes
  const vector<long> nodeIds = pWay->getNodeIds();
  assert(nodeCount == (long)nodeIds.size());

  // ignore duplicate last node for loops to properly calculate merged ends
  if (isLoop) nodeCount--;

  // create coordinate arrays
  Coordinate polyPositions[2][nodeCount];
  double polyLen[2] = {0};

  for (long i = 0; i < nodeCount; i++)
  {
    long currId = nodeIds[i];
    long prevId = (i > 0) ? nodeIds[i-1] : (isLoop ? nodeIds[nodeCount-1] : currId);
    long nextId = (i < nodeCount-1) ? nodeIds[i+1] : (isLoop ? nodeIds[0] : currId);

    const NodePtr pCurrNode = pMap->getNode(currId);
    const NodePtr pPrevNode = pMap->getNode(prevId);
    const NodePtr pNextNode = pMap->getNode(nextId);

    CoordinateExt currCoor(pCurrNode->toCoordinate());
    CoordinateExt prevCoor(pPrevNode->toCoordinate());
    CoordinateExt nextCoor(pNextNode->toCoordinate());

    // find perpendicular vector to vector between previous and next point
    CoordinateExt c1 = currCoor - prevCoor;
    CoordinateExt c2 = nextCoor - currCoor;
    if (c1.length() == 0) c1 = c2;             // correction for first way point
    c1.normalize();
    c2.normalize();
    CoordinateExt vector = c1 + c2;
    CoordinateExt perp(vector.y, -vector.x);  // vector perpendicular to sum of both segments
    perp.normalize();

    // fix for collapsing areas in corners with angles greater than 45 degrees,
    // smaller angles will push the points out too far
    double angle = fabs(atan2(perp.y,perp.x) - atan2(c1.y,c1.x));
    if (angle > M_PI_2) angle = fabs(angle-M_PI);
    if (angle < M_PI_4) angle = M_PI_4;

    // apply requested width
    double width = currWidth / sin(angle);
    perp.x *= width;
    perp.y *= width;

    // store positions for both sides
    for (int p = 0; p < 2; p++)
    {
      CoordinateExt pos = (p == 0) ? (currCoor + perp) : (currCoor - perp);
      polyPositions[p][i] = pos;

      // calculate length to determing inner vs outer polygon for loop
      if (isLoop && i > 0)
      {
        CoordinateExt diff = pos - polyPositions[p][i-1];
        polyLen[0] += diff.length();
      }
    }
  }

  if (isLoop)
  {
    // closed loop, creating a multipolygon
    WayPtr pPoly0 = std::make_shared<Way>(Status::Unknown1, pMap->createNextWayId(), -1);
    WayPtr pPoly1 = std::make_shared<Way>(Status::Unknown1, pMap->createNextWayId(), -1);
    for (long i = 0; i < nodeCount; i++)
    {
      addNodeToPoly(polyPositions[0][i], pMap, pPoly0);
      addNodeToPoly(polyPositions[1][i], pMap, pPoly1);
    }

    // duplicate first id to close poly
    pPoly0->addNode(pPoly0->getNodeId(0));
    pPoly1->addNode(pPoly1->getNodeId(0));

    // copy tags from original way to poly ways
    pPoly0->setTags(tags);
    pPoly1->setTags(tags);

    // replace original way with poly0 and add poly1
    pMap->replace(pWay,pPoly0);
    pMap->addElement(pPoly1);

    // add relation for multipolygon
    RelationPtr pRelation =
      std::make_shared<Relation>(
        Status::Unknown1, pMap->createNextRelationId(), ElementData::CIRCULAR_ERROR_EMPTY,
        MetadataTags::RelationMultiPolygon());
    bool poly0isOuter = polyLen[0] > polyLen[1];
    RelationData::Entry entry0(
      poly0isOuter ? MetadataTags::RelationOuter() : MetadataTags::RelationInner(),
      pPoly0->getElementId());
    RelationData::Entry entry1(
      poly0isOuter ? MetadataTags::RelationInner() : MetadataTags::RelationOuter(),
      pPoly1->getElementId());
    vector<RelationData::Entry> members;
    members.push_back(entry0);
    members.push_back(entry1);
    pRelation->setMembers(members);
    pMap->addElement(pRelation);
  }
  else
  {
    // create poly way and add it to map
    WayPtr pPoly = std::make_shared<Way>(Status::Unknown1, pMap->createNextWayId(), -1);
    for (long i = 0; i < nodeCount; i++) addNodeToPoly(polyPositions[0][i], pMap, pPoly);
    for (long i = nodeCount-1; i >= 0; i--) addNodeToPoly(polyPositions[1][i], pMap, pPoly);

    // duplicate first id to close poly
    pPoly->addNode(pPoly->getNodeId(0));

    // copy tags from original way to poly way
    pPoly->setTags(tags);

    // replace original way with poly
    pMap->replace(pWay,pPoly);
  }

  return true;
}

void WayToPolyGeoModifierAction::addNodeToPoly(
  const CoordinateExt& pos, OsmMap* pMap, WayPtr pPoly) const
{
  long nodeId = pMap->createNextNodeId();
  pMap->addElement(std::make_shared<Node>(Status::Unknown1, nodeId, pos));
  pPoly->addNode(nodeId);
}

void WayToPolyGeoModifierAction::parseArguments(const QHash<QString, QString>& arguments)
{
  _width = DEFAULT_WIDTH;
  _widthTag = QString();

  // read width tag if specified
  if (arguments.keys().contains(WIDTH_TAG_PARAM))
  {
    _widthTag = arguments[WIDTH_TAG_PARAM];
  }

  // read default width if specified
  if (arguments.keys().contains(DEFAULT_WIDTH_PARAM))
  {
    double width = arguments[DEFAULT_WIDTH_PARAM].toDouble();
    if (width > 0)
    {
      _width = width;
    }
  }
}

}
