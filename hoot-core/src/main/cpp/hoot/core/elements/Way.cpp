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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "Way.h"

// Hoot
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>

// Boost
using namespace boost;

// Geos
#include <geos/geom/CoordinateSequenceFactory.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
using namespace geos::geom;

#include <tgs/StreamUtils.h>

namespace hoot
{

Way::Way(Status s, long id, Meters circularError, long changeset, long version,
         unsigned int timestamp, QString user, long uid, bool visible)
: Element(s)
{
  _wayData.reset(new WayData(id, changeset, version, timestamp, user, uid, visible));
  _getElementData().setCircularError(circularError);
}

Way::Way(const Way& from) :
Element(from.getStatus()),
_wayData(from._wayData)
{

}

Way::~Way()
{
}

void Way::addNode(long id)
{
  _preGeometryChange();
  _makeWritable();
  _wayData->addNode(id);
  _postGeometryChange();
}

void Way::addNodes(const vector<long>& ids)
{
  addNodes(ids.begin(), ids.end());
}

void Way::clear()
{
  _preGeometryChange();
  _makeWritable();
  _wayData->clear();
  _postGeometryChange();
}

bool Way::containsNodeId(long nid) const
{
  const std::vector<long>& nids = getNodeIds();
  for (size_t i = 0; i < nids.size(); i++)
  {
    if (nids[i] == nid)
    {
      return true;
    }
  }
  return false;
}

void Way::visitRo(const ElementProvider& map, ElementVisitor& filter) const
{
  filter.visit(map.getWay(getId()));
  const std::vector<long>& nids = getNodeIds();

  for (size_t i = 0; i < nids.size(); i++)
  {
    if (map.containsNode(nids[i]))
    {
      filter.visit(map.getNode(nids[i]));
    }
  }
}

void Way::visitRw(ElementProvider& map, ElementVisitor& filter)
{
  visitRo(map, filter);
}

const Envelope& Way::getApproximateEnvelope(shared_ptr<const ElementProvider> ep) const
{
  bool goodNodes = true;
  if (ep.get())
  {
    const std::vector<long>& ids = _wayData->getNodeIds();
    for (size_t i = 0; i < ids.size(); i++)
    {
      if (ep->containsElement(ElementId::node(ids[i])) == false)
      {
        goodNodes = false;
      }
      else
      {
        shared_ptr<const Element> e = ep->getElement(ElementId::node(ids[i]));
        ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
        assert(n.get());
        _cachedEnvelope.expandToInclude(n->getX(), n->getY());
      }
    }
  }
  else
  {
    goodNodes = false;
  }

  if (goodNodes)
  {
    return getEnvelopeInternal(ep);
  }
  else
  {
    return _cachedEnvelope;
  }
}

const Envelope& Way::getEnvelopeInternal(shared_ptr<const ElementProvider> ep) const
{
  _cachedEnvelope.init();

  const std::vector<long>& ids = _wayData->getNodeIds();
  for (size_t i = 0; i < ids.size(); i++)
  {
    shared_ptr<const Element> e = ep->getElement(ElementId::node(ids[i]));
    ConstNodePtr n = dynamic_pointer_cast<const Node>(e);
    assert(n.get());
    _cachedEnvelope.expandToInclude(n->getX(), n->getY());
  }

  return _cachedEnvelope;
}

void Way::setNodes(const vector<long>& newNodes)
{
  _preGeometryChange();
  _makeWritable();

  _wayData->getNodeIds() = newNodes;

  _postGeometryChange();
}

int Way::getNodeIndex(long nodeId) const
{
  const std::vector<long>& ids = _wayData->getNodeIds();

  for (size_t i = 0; i < ids.size(); i++)
  {
    if (ids[i] == nodeId)
    {
      return i;
    }
  }

  assert(false);
  return -1;
}

bool Way::hasNode(long nodeId) const
{
  const std::vector<long>& ids = _wayData->getNodeIds();

  for (size_t i = 0; i < ids.size(); i++)
  {
    if (ids[i] == nodeId)
    {
      return true;
    }
  }

  return false;
}

bool Way::isOneWay() const
{
  bool result = false;
  QString oneway = getTags()["oneway"].toLower();
  if (getTags().isTrue("oneway") || oneway == "-1" || oneway == "reverse")
  {
    result = true;
  }
  return result;
}

bool Way::isValidPolygon() const
{
  size_t nc = getNodeCount();
  bool result = false;

  // the way must have either 0 or 4 points to be considered a valid polygon by GEOS.
  if (nc == 0 || nc >= 4)
  {
    result = true;
  }
  // if there are only 3 points and the first/last don't match then we'll add one if the user
  // requests a polygon.
  else if (nc == 3 && getNodeId(0) != getLastNodeId())
  {
    result = true;
  }

  return result;
}

void Way::_makeWritable()
{
  // make sure we're the only ones referencing the way data.
  if(_wayData.use_count() > 1)
  {
    _wayData.reset(new WayData(*_wayData));
  }
}

void Way::removeNode(long id)
{
  std::vector<long>& nodes = _wayData->getNodeIds();
  size_t newCount = 0;

  // copy the array in place and remove the unwanted nodes.
  for (size_t i = 0; i < nodes.size(); i++)
  {
    if (nodes[i] != id) {
      nodes[newCount] = nodes[i];
      newCount++;
    }
  }

  nodes.resize(newCount);
}

void Way::replaceNode(long oldId, long newId)
{
  const vector<long>& ids = getNodeIds();

  bool change = false;
  for (size_t i = 0; i < ids.size(); i++)
  {
    if (ids[i] == oldId)
    {
      change = true;
    }
  }

  if (change)
  {
    _preGeometryChange();
    _makeWritable();
    vector<long>& newIds = _wayData->getNodeIds();
    for (size_t i = 0; i < newIds.size(); i++)
    {
      if (newIds[i] == oldId)
      {
        newIds[i] = newId;
      }
    }
    _postGeometryChange();
  }
}

void Way::reverseOrder()
{
  _preGeometryChange();
  _makeWritable();

  std::vector<long> oldOrder = getNodeIds();
  std::vector<long>& nodes = _wayData->getNodeIds();

  for (size_t i = 0; i < oldOrder.size(); i++)
  {
    nodes[i] = oldOrder[oldOrder.size() - i - 1];
  }
  _postGeometryChange();
}

QString Way::toString() const
{
  stringstream ss(stringstream::out);
  ss << "way(" << getId() << ")" << endl;
  ss << "nodes: ";
  Tgs::operator <<(ss, getNodeIds());
  ss << endl;
  ss << "tags: " << getTags().toString().toStdString();
  ss << "cached envelope: " << GeometryUtils::toString(_cachedEnvelope).toStdString() << endl;
  ss << "status: " << getStatusString().toStdString() << endl;
  ss << "version: " << getVersion() << endl;
  ss << "visible: " << getVisible();
  return QString::fromStdString(ss.str());
}

bool Way::isFirstLastNodeIdentical() const
{
  if ( getNodeCount() < 2 )
  {
    return false;
  }

  return ( getFirstNodeId() == getLastNodeId() );
}

}
