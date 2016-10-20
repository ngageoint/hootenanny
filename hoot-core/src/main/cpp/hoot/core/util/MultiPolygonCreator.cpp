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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiPolygonCreator.h"

// geos
#include <geos/algorithm/CGAlgorithms.h>
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
//#include <hoot/core/OsmMap.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/GeometryUtils.h>

// tgs
#include <tgs/DisjointSet/DisjointSetMap.h>

namespace hoot
{

MultiPolygonCreator::MultiPolygonCreator(const ConstElementProviderPtr& provider,
  const ConstRelationPtr& r) :
  _provider(provider),
  _r(r)
{
}

Geometry* MultiPolygonCreator::_addHoles(vector<LinearRing*> &outers,
  vector<LinearRing*> &inners) const
{
  const GeometryFactory& gf = *GeometryFactory::getDefaultInstance();

  vector<Geometry*>* polygons = new vector<Geometry*>();
  vector<Geometry*>& tmpPolygons = *polygons;
  tmpPolygons.reserve(outers.size());

  vector<double> outerArea;
  // a vector of holes for each of the outer polygons.
  vector< vector<Geometry*>* > holes;
  holes.resize(outers.size());

  outerArea.reserve(outers.size());
  vector<Geometry*> noHoles;
  for (size_t i = 0; i < outers.size(); i++)
  {
    Polygon* p = gf.createPolygon(*outers[i], noHoles);
    tmpPolygons.push_back(p);
    outerArea.push_back(p->getArea());
    holes[i] = new vector<Geometry*>();
  }

  // go through all the holes.
  for (size_t i = 0; i < inners.size(); i++)
  {
    // the index of the containing polygon
    int polygonIndex = -1;
    bool contained = false;

    // look for an outer ring that contains this hole.
    for (size_t j = 0; j < outers.size(); j++)
    {
      // if we've already found a polygon that contains this inner
      if (contained && (outerArea[j] < outerArea[polygonIndex]))
      {
        if (tmpPolygons[j]->contains(inners[i]))
        {
          contained = true;
          polygonIndex = j;
        }
      }
      // if we haven't found an appropriate polygon, or this polygon is smaller than the best
      // containing polygon we've found so far.
      else if (polygonIndex == -1 || (outerArea[j] < outerArea[polygonIndex]))
      {
        auto_ptr<IntersectionMatrix> im(tmpPolygons[j]->relate(inners[i]));
        if (im->isContains())
        {
          contained = true;
          polygonIndex = j;
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
        LOG_WARN("Could not find a polygon that fully contains a hole. inner[" << i << "] " <<
          _r->toString());
      }
    }
    else
    {
      holes[polygonIndex]->push_back(inners[i]);
    }
  }

  // create the polygons from outers & holes
  for (size_t i = 0; i < outers.size(); i++)
  {
    // delete temporary polygons
    delete tmpPolygons[i];
    Polygon* p = gf.createPolygon(outers[i], holes[i]);
    tmpPolygons[i] = p;
  }

  // create the final MultiPolygon
  return gf.createMultiPolygon(polygons);
}

void MultiPolygonCreator::_addWayToSequence(ConstWayPtr w, CoordinateSequence& cs, bool reversed)
  const
{
  size_t start = 0;
  size_t increment = 1;
  if (reversed)
  {
    start = w->getNodeCount() - 1;
    increment = -1;
  }

  for (int i = start; i < (int)w->getNodeCount() && i >= 0; i += increment)
  {
    Coordinate c = _provider->getNode(w->getNodeId(i))->toCoordinate();
    if (cs.getSize() == 0 || c != cs.getAt(cs.getSize() - 1))
    {
      cs.add(c);
    }
  }
}

shared_ptr<Geometry> MultiPolygonCreator::createMultipolygon() const
{
  vector<LinearRing*> outers;
  _createRings("outer", outers);
  _createRings("part", outers);

  vector<LinearRing*> inners;
  _createRings("inner", inners);

  shared_ptr<Geometry> result(_addHoles(outers, inners));

  // recursively add any child relation multipolygons.
  for (size_t i = 0; i < _r->getMembers().size(); i++)
  {
    const RelationData::Entry& e = _r->getMembers()[i];
    if (e.getElementId().getType() == ElementType::Relation &&
        (e.role == "outer" || e.role == "part"))
    {
      shared_ptr<const Relation> r = _provider->getRelation(e.getElementId().getId());
      if (r->isMultiPolygon() ||
        OsmSchema::getInstance().isArea(r->getTags(), ElementType::Relation))
      {
        shared_ptr<Geometry> child(MultiPolygonCreator(_provider, r).createMultipolygon());
        try
        {
          result.reset(result->Union(child.get()));
        }
        catch (geos::util::TopologyException& e)
        {
          child.reset(GeometryUtils::validateGeometry(child.get()));
          result.reset(GeometryUtils::validateGeometry(result.get()));
          result.reset(result->Union(child.get()));
        }
      }
    }
  }

  return result;
}

void MultiPolygonCreator::_createRings(const QString& role, vector<LinearRing *> &rings) const
{
  vector<ConstWayPtr> partials;

  const vector<RelationData::Entry>& elements = _r->getMembers();
  for (size_t i = 0; i < elements.size(); i++)
  {
    const RelationData::Entry& e = elements[i];
    if (e.getElementId().getType() == ElementType::Way && e.role == role)
    {
      const shared_ptr<const Way>& w = _provider->getWay(e.getElementId().getId());

      if (!w)
      {
        continue;
      }

      // if the way forms a simple ring
      if (w->getNodeId(0) == w->getLastNodeId())
      {
        // don't try to add empty ways.
        if (w->getNodeCount() > 0)
        {
          LinearRing* lr = _toLinearRing(w);
          rings.push_back(lr);
        }
      }
      else
      {
//        LOG_WARN("A multipolygon relation contains a way that is not a ring. While likely valid, "
//                 "this is currently unsupported by Hoot. Will attempt to deal with it. " <<
//                 e.getElementId());
        partials.push_back(w);
      }
    }
  }

  if (partials.size() > 0)
  {
    _createRingsFromPartials(partials, rings);
  }
}

void MultiPolygonCreator::_createRingsFromPartials(const vector<ConstWayPtr>& partials,
  vector<LinearRing *> &rings) const
{
  Tgs::DisjointSetMap<ConstWayPtr> ringSets;

  // find all the ways that are part of a single ring
  for (size_t i = 0; i < partials.size(); i++)
  {
    ConstWayPtr wi = partials[i];
    for (size_t j = i; j < partials.size(); j++)
    {
      ConstWayPtr wj = partials[j];

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
  for (Tgs::DisjointSetMap<ConstWayPtr>::AllGroups::const_iterator it = ag.begin(); it != ag.end();
       ++it)
  {
    _createSingleRing(it->second, rings);
  }
}

void MultiPolygonCreator::_createSingleRing(const vector<ConstWayPtr>& partials,
  vector<LinearRing *> &rings) const
{
  deque<ConstWayPtr> orderedWays = _orderWaysForRing(partials);
  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
      create((size_t)0, (size_t)2);

  for (size_t i = 0; i < orderedWays.size(); i++)
  {
    _addWayToSequence(orderedWays[i], *cs, false);
  }

  if (cs->getAt(0) != cs->getAt(cs->getSize() - 1))
  {
    cs->add(cs->getAt(0));
  }

  if (cs->getSize() <= 3)
  {
    LOG_WARN("Unable to create ring -- fewer than 4 points.");
  }
  else
  {
    rings.push_back(GeometryFactory::getDefaultInstance()->createLinearRing(cs));
  }
}

bool MultiPolygonCreator::_isValidInner(LinearRing* innerRing) const
{
  if (innerRing->getNumPoints() > 0 && innerRing->getNumPoints() < 4)
  {
    return false;
  }

  const GeometryFactory& gf = *GeometryFactory::getDefaultInstance();
  vector<Geometry*> noHoles;
  auto_ptr<Polygon> p(gf.createPolygon(*innerRing, noHoles));
  if (p->getArea() <= 0.0)
  {
    return false;
  }

  return true;
}

/**
 * Put the ways into the right order so they can just be added one after another into a ring.
 */
deque<ConstWayPtr> MultiPolygonCreator::_orderWaysForRing(const vector<ConstWayPtr>& partials) const
{
  deque<ConstWayPtr> result;

  // we have to start somewhere
  result.push_back(partials[0]);

  long firstId = partials[0]->getNodeId(0);
  long lastId = partials[0]->getLastNodeId();

  for (size_t i = 1; i < partials.size(); i++)
  {
    ConstWayPtr w = partials[i];
    // if the ways are start to start or end to end
    if (w->getNodeId(0) == firstId ||
        w->getLastNodeId() == lastId)
    {
      // this way needs to be reversed, but clone it first so we don't change any source data
      WayPtr cloned = WayPtr(new Way(*partials[i]));
      cloned->reverseOrder();
      w = cloned;
    }

    if (w->getNodeId(0) == lastId)
    {
      result.push_back(w);
    }
    else if (w->getLastNodeId() == firstId)
    {
      result.push_front(w);
    }
    else
    {
      LOG_WARN("Unable to connect all ways in an outer ring. This may give unexpected results. " <<
               partials[i]->getElementId());
    }
  }

  return result;
}

LinearRing* MultiPolygonCreator::_toLinearRing(const ConstWayPtr& w) const
{
  const std::vector<long>& ids = w->getNodeIds();
  int size = ids.size();
  if (size == 1)
  {
    size = 2;
  }
  else if (ids[0] != ids[ids.size() - 1])
  {
    size++;
  }

  if (size <= 3)
  {
    return GeometryFactory::getDefaultInstance()->createLinearRing();
  }

  CoordinateSequence* cs = GeometryFactory::getDefaultInstance()->getCoordinateSequenceFactory()->
                           create(size, 2);

  size_t i = 0;
  for (; i < ids.size(); i++)
  {
    shared_ptr<const Node> n = _provider->getNode(ids[i]);
    cs->setAt(n->toCoordinate(), i);
  }

  // a linestring cannot contain 1 point. Do this to keep it valid.
  if (ids.size() == 1)
  {
    shared_ptr<const Node> n = _provider->getNode(ids[0]);
    cs->setAt(n->toCoordinate(), i++);
  }
  else if (ids[0] != ids[ids.size() - 1])
  {
    shared_ptr<const Node> n = _provider->getNode(ids[0]);
    cs->setAt(n->toCoordinate(), i++);
  }

  return GeometryFactory::getDefaultInstance()->createLinearRing(cs);
}


}
