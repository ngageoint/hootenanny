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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RelationToMultiPolygonConverter.h"

// geos
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Polygon.h>
#include <geos/util/TopologyException.h>

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/geometry/GeometryUtils.h>

// tgs
#include <tgs/DisjointSet/DisjointSetMap.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int RelationToMultiPolygonConverter::logWarnCount = 0;

RelationToMultiPolygonConverter::RelationToMultiPolygonConverter(const ConstElementProviderPtr& provider,
                                                                 const ConstRelationPtr& r)
  : _provider(provider),
    _r(r)
{
  LOG_VART(_r.get());
}

Geometry* RelationToMultiPolygonConverter::_addHoles(vector<LinearRing*>& outers,
                                                     vector<LinearRing*>& inners) const
{
  LOG_TRACE("Adding holes...");

  const GeometryFactory& gf = *GeometryFactory::getDefaultInstance();

  vector<Geometry*>* polygons = new vector<Geometry*>();
  vector<Geometry*>& tmpPolygons = *polygons;
  tmpPolygons.reserve(outers.size());
  vector<std::shared_ptr<Geometry>> recover;

  vector<double> outerArea;
  outerArea.reserve(outers.size());
  // a vector of holes for each of the outer polygons.
  vector<vector<LinearRing*>*> holes;
  holes.reserve(outers.size());

  vector<LinearRing*> noHoles;
  for (const auto& outer : outers)
  {
    Polygon* p = gf.createPolygon(*outer, noHoles);
    tmpPolygons.push_back(p);
    outerArea.push_back(p->getArea());
    holes.push_back(new vector<LinearRing*>());
  }

  // go through all the holes.
  for (size_t i = 0; i < inners.size(); ++i)
  {
    // the index of the containing polygon
    int polygonIndex = -1;
    bool contained = false;

    // look for an outer ring that contains this hole.
    for (size_t j = 0; j < outers.size(); ++j)
    {
      // if we've already found a polygon that contains this inner
      if (contained && (outerArea[j] < outerArea[polygonIndex]))
      {
        if (tmpPolygons[j]->contains(inners[i]))
        {
          contained = true;
          polygonIndex = static_cast<int>(j);
        }
      }
      // if we haven't found an appropriate polygon, or this polygon is smaller than the best
      // containing polygon we've found so far.
      else if (polygonIndex == -1 || (outerArea[j] < outerArea[polygonIndex]))
      {
        std::shared_ptr<IntersectionMatrix> im(tmpPolygons[j]->relate(inners[i]));
        if (im->isContains())
        {
          contained = true;
          polygonIndex = static_cast<int>(j);
        }
        else if (im->isIntersects())
        {
          contained = false;
          polygonIndex = j;
        }
      }
    }

    if (polygonIndex == -1)
    {
      // if it isn't a valid inner ring then who cares.
      if (_isValidInner(inners[i]))
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Could not find a polygon that fully contains a hole.");
          LOG_TRACE("inner[" << i << "] " <<_r->toString());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
      //  Because ownership of this inner won't be given to GeometryFactory::createPolygon below
      //  add the object to the memory recovery vector
      recover.emplace_back(inners[i]);
      inners[i] = nullptr;
    }
    else
      holes[polygonIndex]->push_back(inners[i]);
  }

  // create the polygons from outers & holes
  for (size_t i = 0; i < outers.size(); ++i)
  {
    //  Move the temp polygon to the memory recovery vector to not leak the polygons
    recover.emplace_back(tmpPolygons[i]);
    //  Create the new polygons
    tmpPolygons[i] = gf.createPolygon(outers[i], holes[i]);
  }

  // create the final MultiPolygon; GeometryFactory takes ownership of these input parameters.
  return gf.createMultiPolygon(polygons);
}

void RelationToMultiPolygonConverter::_addWayToSequence(ConstWayPtr w, CoordinateSequence& cs, bool reversed) const
{
  size_t start = 0;
  size_t increment = 1;
  if (reversed)
  {
    start = w->getNodeCount() - 1;
    increment = -1;
  }

  std::vector<Coordinate> points;
  cs.toVector(points);
  for (int i = static_cast<int>(start); i < static_cast<int>(w->getNodeCount()) && i >= 0; i += increment)
  {
    Coordinate c = _provider->getNode(w->getNodeId(i))->toCoordinate();
    if (points.empty() || c != points.operator [](points.size() - 1))
      points.push_back(c);
  }
  cs.setPoints(points);
}

std::shared_ptr<Geometry> RelationToMultiPolygonConverter::createMultipolygon() const
{
  LOG_TRACE("Creating multipolygon...");

  vector<LinearRing*> outers;
  _createRings(MetadataTags::RoleOuter(), outers);
  _createRings(MetadataTags::RolePart(), outers);

  vector<LinearRing*> inners;
  _createRings(MetadataTags::RoleInner(), inners);

  // Now go looking for rings that are not classified (empty role)
  vector<LinearRing*> noRole;
  _createRings("", noRole);
  // Make sure that all rings are either an inner or an outer
  _classifyRings(noRole, inners, outers);
  LOG_TRACE("After classify: Outers: " << outers.size() << "  Inners: " << inners.size());
  //  Cleanup the memory for any dangling LinearRings still left in noRole
  vector<std::shared_ptr<LinearRing>> cleanup;
  for (auto ring : noRole)
    cleanup.push_back(std::shared_ptr<LinearRing>(ring));

  std::shared_ptr<Geometry> result(_addHoles(outers, inners));

  // recursively add any child relation multipolygons.
  for (const auto& e : _r->getMembers())
  {
    LOG_VART(e.getElementId());
    if (e.getElementId().getType() == ElementType::Relation &&
       (e.getRole() == MetadataTags::RoleOuter() || e.getRole() == MetadataTags::RolePart()))
    {
      ConstRelationPtr r = _provider->getRelation(e.getElementId().getId());
      if (r && (r->isMultiPolygon() || AreaCriterion().isSatisfied(r)))
      {
        LOG_VART(r->getElementId());
        std::shared_ptr<Geometry> child(
          RelationToMultiPolygonConverter(_provider, r).createMultipolygon());
        try
        {
          result = result->Union(child.get());
        }
        catch (const geos::util::TopologyException&)
        {
          child.reset(GeometryUtils::validateGeometry(child.get()));
          result.reset(GeometryUtils::validateGeometry(result.get()));
          result = result->Union(child.get());
        }
      }
    }
  }

  return result;
}

RelationToMultiPolygonConverter::MultiPolygonRelationRole RelationToMultiPolygonConverter::_findRelationship(const LinearRing* ring1,
                                                                                                             const LinearRing* ring2) const
{
  MultiPolygonRelationRole result = MultiPolygonRelationRole::NoRole;

  const GeometryFactory& gf = *GeometryFactory::getDefaultInstance();
  vector<LinearRing*> noHoles;
  std::shared_ptr<Polygon> p1(gf.createPolygon(*ring1, noHoles));

  // It would be nice to do this in one call but "isWithin" doesn't seem to return anything.
  std::shared_ptr<IntersectionMatrix> im(p1->relate(ring2));
  if (im->isContains())
    result = MultiPolygonRelationRole::Outer;
  else
  {
    std::shared_ptr<Polygon> p2(gf.createPolygon(*ring2, noHoles));
    std::shared_ptr<IntersectionMatrix> im2(p2->relate(ring1));
    if (im2->isContains())
      result = MultiPolygonRelationRole::Inner;
  }

  return result;
}

void RelationToMultiPolygonConverter::_classifyRings(std::vector<LinearRing*>& noRole, std::vector<LinearRing*>& inners,
                                                     std::vector<LinearRing*>& outers) const
{
  // Empty == nothing else to do
  if (noRole.empty())
    return;

  LOG_TRACE("Rings without a role: " << noRole.size() << "  Outers: " << outers.size() << "  Inners: " <<
            inners.size());

  // One polygon, no inners or outers
  if (noRole.size() == 1 && inners.empty() && outers.empty())
  {
    outers.push_back(noRole.front());
    //  Must clear because there are no more noRole objects
    noRole.clear();
    return;
  }

  // A list of things we haven't found yet.
  deque<LinearRing*> notFound;

  for (const auto& member : noRole)
  {
    MultiPolygonRelationRole status = MultiPolygonRelationRole::NoRole;

    // Check if we have an inner polygon.
    for (const auto& outer : outers)
    {
      status = _findRelationship(member, outer);

      if (status == MultiPolygonRelationRole::Inner)
      {
        inners.push_back(member);
        break;
      }
      else if (status == MultiPolygonRelationRole::Outer)
      {
        // This is not a good combination: Outside of an Outer
        outers.push_back(member);
        break;
      }
    }

    // No outers or we didn't find an inner polygon
    if (status == MultiPolygonRelationRole::NoRole)
    {
      // Look for an outer polygon
      for (const auto& inner : inners)
      {
        status =_findRelationship(member, inner);

        if (status == MultiPolygonRelationRole::Inner)
        {
          // This is not a good combination: Inside of an inner
          inners.push_back(member);
          break;
        }
        else if (status == MultiPolygonRelationRole::Outer)
        {
          outers.push_back(member);
          break;
        }
      }

      // No inners or we still didn't find anything so push it onto the list of things to check
      // later.
      if (status == MultiPolygonRelationRole::NoRole)
        notFound.push_back(member);
    }
  }

  // Now go through the things we didn't find.
  while (!notFound.empty())
  {
    //  Grab the first member off of the deque
    LinearRing* member = notFound.front();
    notFound.pop_front();
    if (notFound.empty())
    {
      // We only have one polygon left so it must be an outer
      outers.push_back(member);
    }
    else
    {
      // We have 2 or more polygons. Grab the first one to compare with and remove it from the list
      MultiPolygonRelationRole status = MultiPolygonRelationRole::NoRole;

      // Loop through the rest of the list looking for a match
      for (auto it = notFound.begin(); it != notFound.end(); ++it)
      {
        status =_findRelationship(member, *it);

        // If we find inner or outer, push both elements to the right list and then delete the
        // one from the loop. We break out of the loop since deleteing the element screws up the
        // iterator and we only care about one match for each pair of polygons.
        if (status == MultiPolygonRelationRole::Inner)
        {
          inners.push_back(member);
          outers.push_back(*it);
          notFound.erase(it);
          break;
        }
        else if (status == MultiPolygonRelationRole::Outer)
        {
          inners.push_back(*it);
          outers.push_back(member);
          notFound.erase(it);
          break;
        }
      }

      // If we didn't find a match, this must be an outer.
      // If we did find a match then we have already added it to a list so move on through the list
      if (status == MultiPolygonRelationRole::NoRole)
        outers.push_back(member);
    }
  }
  //  Empty out the list of elements with no role as all have now been assigned
  noRole.clear();
}

void RelationToMultiPolygonConverter::_createRings(const QString& role, vector<LinearRing*>& rings) const
{
  LOG_TRACE("Creating rings for role: " << role << "...");

  vector<ConstWayPtr> partials;

  LOG_VART(_r.get());
  const vector<RelationData::Entry>& elements = _r->getMembers();
  LOG_VART(elements.size());
  for (const auto& e : elements)
  {
    if (e.getElementId().getType() == ElementType::Way && e.getRole() == role)
    {
      const ConstWayPtr& w = _provider->getWay(e.getElementId().getId());
      if (!w || w->getNodeCount() == 0)
        continue;
      LOG_VART(w->getElementId());

      // if the way forms a simple ring
      if (w->getNodeId(0) == w->getLastNodeId())
      {
        // don't try to add empty ways
        LOG_VART(w->getNodeCount());
        if (w->getNodeCount() > 0)
          rings.push_back(_toLinearRing(w));
      }
      else
      {
        // Leaving this one at trace level, since it is happening so much in poi/poly conflation.
        // See #2287.
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_TRACE(
            "A multipolygon relation contains a way that is not a ring. While likely a valid " <<
            "feature, fixing it is currently unsupported by Hoot and it will be dealt with as " <<
            "is: " << e.getElementId());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_TRACE(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;

        partials.push_back(w);
      }
    }
  }
  LOG_VART(partials.size());

  if (!partials.empty())
    _createRingsFromPartials(partials, rings);
}

void RelationToMultiPolygonConverter::_createRingsFromPartials(const vector<ConstWayPtr>& partials, vector<LinearRing*>& rings) const
{
  LOG_TRACE("Creating rings from partials...");

  Tgs::DisjointSetMap<ConstWayPtr> ringSets;

  // find all the ways that are part of a single ring
  for (size_t i = 0; i < partials.size(); i++)
  {
    ConstWayPtr wi = partials[i];
    if (!wi)
      continue;
    for (size_t j = i; j < partials.size(); j++)
    {
      ConstWayPtr wj = partials[j];
      if (!wj)
        continue;

      if (wi->getNodeId(0) == wj->getNodeId(0) ||
          wi->getNodeId(0) == wj->getLastNodeId() ||
          wi->getLastNodeId() == wj->getNodeId(0) ||
          wi->getLastNodeId() == wj->getLastNodeId())
      {
        ringSets.joinT(wi, wj);
      }
    }
  }

  Tgs::DisjointSetMap<ConstWayPtr>::AllGroups ag = ringSets.getAllGroups();
  for (auto it = ag.begin(); it != ag.end(); ++it)
    _createSingleRing(it->second, rings);
}

void RelationToMultiPolygonConverter::_createSingleRing(const vector<ConstWayPtr>& partials, vector<LinearRing*>& rings) const
{
  LOG_TRACE("Creating single ring...");

  deque<ConstWayPtr> orderedWays = _orderWaysForRing(partials);
  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create((size_t)0, (size_t)2).release();

  for (const auto& way : orderedWays)
    _addWayToSequence(way, *cs, false);

  if (cs->getAt(0) != cs->getAt(cs->getSize() - 1))
  {
    vector<Coordinate> points;
    cs->toVector(points);
    points.push_back(cs->getAt(0));
    cs->setPoints(points);
  }

  if (cs->getSize() <= 3)
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Unable to create ring -- fewer than 4 points.");
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
  else
    rings.push_back(GeometryFactory::getDefaultInstance()->createLinearRing(cs));
}

bool RelationToMultiPolygonConverter::_isValidInner(const LinearRing* innerRing) const
{
  if (innerRing->getNumPoints() > 0 && innerRing->getNumPoints() < 4)
    return false;

  const GeometryFactory& gf = *GeometryFactory::getDefaultInstance();
  vector<LinearRing*> noHoles;
  std::shared_ptr<Polygon> p(gf.createPolygon(*innerRing, noHoles));

  if (p->getArea() <= 0.0)
    return false;

  return true;
}

deque<ConstWayPtr> RelationToMultiPolygonConverter::_orderWaysForRing(const vector<ConstWayPtr>& partials) const
{
  LOG_TRACE("Ordering ways for ring...");

  deque<ConstWayPtr> result;

  deque<ConstWayPtr> extras;

  // we have to start somewhere
  result.push_back(partials[0]);

  long firstId = partials[0]->getNodeId(0);
  long lastId = partials[0]->getLastNodeId();

  for (size_t i = 1; i < partials.size(); i++)
  {
    ConstWayPtr w = partials[i];
    if (!w)
      continue;

    // if the ways are start to start or end to end
    if (w->getNodeId(0) == firstId || w->getLastNodeId() == lastId)
    {
      // This way needs to be reversed, but clone it first so we don't change any source data.
      WayPtr cloned = std::make_shared<Way>(*partials[i]);
      cloned->reverseOrder();
      w = cloned;
    }

    if (w->getNodeId(0) == lastId)
    {
      result.push_back(w);
      //  Update the last id with the new last id
      lastId = w->getLastNodeId();
    }
    else if (w->getLastNodeId() == firstId)
    {
      result.push_front(w);
      //  Update the first id with the new first id
      firstId = w->getNodeId(0);
    }
    else
    {
      // If this way doesn't currently match the way we are building, add it to a list to be
      // processed later.
      // NOTE: If partials wasn't a const, we could probably do this in one loop.
      extras.push_back(w);
    }
  }

  // Now go through the "extra" items and add them to the result.
  // This makes some assumptions and I'm sure that we will eventually get an edge case that
  // makes it fail. Also note that this part of the algorithm is of n-factorial complexity.
  // If it becomes a pain point, we can probably improve it's performance. Also note that
  // there is probably simlary logic in the WayJoiner class - in the future we might abstract
  // this algorithm to one place.
  // Assumptions:
  // 1) All of the elements in the partials vector are part of the same ring.
  // 2) Since they are part of the same ring, they should all join
  // 3) If they don't all join, the misfists will just go away...

  size_t misfitCount = 0; // The number of ways that don't fit in our ring
  while (extras.size() > misfitCount)
  {
    ConstWayPtr wayPtr = extras.front();
    extras.pop_front();
    if (wayPtr->getNodeId(0) == lastId)
    {
      result.push_back(wayPtr);
      //  Update the last id with the new last id
      lastId = wayPtr->getLastNodeId();
      misfitCount = 0; // Since we've added a way, need to re-check all the misfits
    }
    else if (wayPtr->getLastNodeId() == firstId)
    {
      result.push_front(wayPtr);
      //  Update the first id with the new first id
      firstId = wayPtr->getNodeId(0);
      misfitCount = 0;
    }
    else
    {
      // We didn't find a match. Add the way to the end of the list so that it gets
      // checked again later. Increment our misfitCount
      extras.push_back(wayPtr);
      misfitCount++;
    }
  }

  return result;
}

LinearRing* RelationToMultiPolygonConverter::_toLinearRing(const ConstWayPtr& w) const
{
  LOG_TRACE("Converting " << w->getElementId() << " to linear ring...");

  const std::vector<long>& ids = w->getNodeIds();
  size_t size = ids.size();
  LOG_VART(size);
  if (size == 1)
    size = 2;
  else if (ids[0] != ids[size - 1])
    size++;

  if (size <= 3)
  {
    LOG_TRACE("Returning default linear ring...");
    return GeometryFactory::getDefaultInstance()->createLinearRing().release();
  }

  CoordinateSequence* cs =
    GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->create(size, 2).release();
  LOG_VART(cs->size());

  size_t i = 0;
  for (; i < size; i++)
  {
    ConstNodePtr n = _provider->getNode(ids[i]);
    if (n)
      cs->setAt(n->toCoordinate(), i);
  }
  LOG_VART(cs->size());
  // A linestring cannot contain 1 point. Do this to keep it valid.
  if (size == 1)
  {
    ConstNodePtr n = _provider->getNode(ids[0]);
    if (n)
      cs->setAt(n->toCoordinate(), i++);
  }
  else if (ids[0] != ids[size - 1])
  {
    ConstNodePtr n = _provider->getNode(ids[0]);
    if (n)
      cs->setAt(n->toCoordinate(), i++);
  }
  LOG_VART(cs->size());

  LOG_TRACE("Returning default linear ring...");
  return GeometryFactory::getDefaultInstance()->createLinearRing(cs);
}

}
