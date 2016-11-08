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

#include "MapCropper.h"

// GEOS
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/LineString.h>
#include <geos/geom/MultiLineString.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/io/WKBReader.h>
#include <geos/io/WKBWriter.h>
#include <geos/util/GEOSException.h>

// Hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/conflate/NodeToWayMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RemoveWayOp.h>
#include <hoot/core/ops/RemoveNodeOp.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/FindNodesInWayFactory.h>
#include <hoot/core/util/ElementConverter.h>
#include <hoot/core/util/GeometryUtils.h>
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Validate.h>
#include <hoot/core/visitors/RemoveEmptyRelationsVisitor.h>

// Standard
#include <limits>

// TGS
#include <tgs/StreamUtils.h>
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, MapCropper)

MapCropper::MapCropper()
{
  _invert = false;
  setConfiguration(conf());
}

MapCropper::MapCropper(const Envelope& envelope)
{
  _envelope = envelope;
  _invert = false;
  _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
}

MapCropper::MapCropper(const shared_ptr<const Geometry> &g, bool invert)
{
  _envelopeG = g;
  _invert = invert;
}

void MapCropper::setConfiguration(const Settings& conf)
{
  const QString boundsStr = ConfigOptions(conf).getCropBounds();
  if (!boundsStr.isEmpty())
  {
    const QString errorMsg =
      "Invalid bounds passed to map cropper: " + boundsStr + ".  Must be of the form: minx,miny,maxx,maxy";
    const QRegExp boundsRegEx("(-*\\d+\\.*\\d*,){3}-*\\d+\\.*\\d*");
    if (!boundsRegEx.exactMatch(boundsStr))
    {
      throw HootException(errorMsg);
    }
    const QStringList boundsParts = boundsStr.split(",");
    assert(boundsParts.size() == 4);
    if ((boundsParts.at(2).toDouble() <= boundsParts.at(0).toDouble()) ||
         boundsParts.at(3).toDouble() <= boundsParts.at(1).toDouble())
    {
      throw HootException(errorMsg);
    }

    _envelope =
      Envelope(boundsParts.at(0).toDouble(), boundsParts.at(2).toDouble(),
        boundsParts.at(1).toDouble(), boundsParts.at(3).toDouble());
    _invert = false;
    _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
  }
}

void MapCropper::apply(shared_ptr<OsmMap>& map)
{
  LOG_INFO("Cropping map.");
  shared_ptr<OsmMap> result = map;

  if (MapProjector::isGeographic(map) == false && _nodeBounds.isNull() == false)
  {
    throw HootException("If the node bounds is set the projection must be geographic.");
  }

  /// @todo visit the elements from the most senior (e.g. relation that has no parents) to the
  /// most junior (nodes).

  // go through all the ways
  const WayMap ways = result->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); it++)
  {
    const shared_ptr<Way>& w = it->second;
    shared_ptr<LineString> ls = ElementConverter(map).convertToLineString(w);
    const Envelope& e = *(ls->getEnvelopeInternal());

    // if the way is completely outside the region we're keeping
    if (_isWhollyOutside(e))
    {
      // remove the way
      RemoveWayOp::removeWayFully(result, w->getId());
    }
    else if (_isWhollyInside(e))
    {
      // keep the way
    }
    else
    {
      // do an expensive operation to decide how much to keep, if any.
      _cropWay(result, w->getId());
    }
  }

  shared_ptr<NodeToWayMap> n2wp = result->getIndex().getNodeToWayMap();
  NodeToWayMap& n2w = *n2wp;

  LOG_INFO("  Removing nodes...");

  // go through all the nodes
  long nodesRemoved = 0;
  const NodeMap nodes = result->getNodeMap();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); it++)
  {
    const Coordinate& c = it->second->toCoordinate();

    bool nodeInside = false;

    if (_envelope.isNull() == false)
    {
      if (_invert == false)
      {
        nodeInside = _envelope.covers(c);
      }
      else
      {
        nodeInside = !_envelope.covers(c);
      }
    }
    else
    {
      auto_ptr<Point> p(GeometryFactory::getDefaultInstance()->createPoint(c));

      if (_invert == false)
      {
        nodeInside = _envelopeG->intersects(p.get());
      }
      else
      {
        nodeInside = !_envelopeG->intersects(p.get());
      }
    }

    // if the node is outside
    if (!nodeInside)
    {
      // if the node is within the limiting bounds.
      if (_nodeBounds.isNull() == true || _nodeBounds.contains(c))
      {
        // if the node is not part of a way
        if (n2w.find(it->first) == n2w.end())
        {
          // remove the node
          LOG_TRACE(
            "Removing node with coords: " << it->second->getX() << " : " << it->second->getY());
          RemoveNodeOp::removeNodeNoCheck(result, it->second->getId());
          nodesRemoved++;
        }
      }
    }
  }
  LOG_DEBUG("Nodes removed: " + QString::number(nodesRemoved));

  RemoveEmptyRelationsVisitor v;
  map->visitRw(v);
}

void MapCropper::crop(shared_ptr<OsmMap> map, const Envelope& envelope)
{
  MapCropper mc(envelope);
  mc.apply(map);
}

void MapCropper::crop(shared_ptr<OsmMap> map, const shared_ptr<const Geometry>& g, bool invert)
{
  MapCropper mc(g, invert);
  mc.apply(map);
}

void MapCropper::_cropWay(shared_ptr<OsmMap> map, long wid)
{
  shared_ptr<Way> way = map->getWay(wid);

  shared_ptr<Geometry> fg = ElementConverter(map).convertToGeometry(way);

  // perform the intersection with the geometry
  shared_ptr<Geometry> g;
  if (_invert == false)
  {
    try
    {
      g.reset(fg->intersection(_envelopeG.get()));
    }
    catch (geos::util::GEOSException& e)
    {
      // try cleaning up the geometry and try again.
      fg.reset(GeometryUtils::validateGeometry(fg.get()));
      g.reset(fg->intersection(_envelopeG.get()));
    }
  }
  else
  {
    try
    {
      g.reset(fg->difference(_envelopeG.get()));
    }
    catch (geos::util::GEOSException& e)
    {
      // try cleaning up the geometry and try again.
      fg.reset(GeometryUtils::validateGeometry(fg.get()));
      g.reset(fg->difference(_envelopeG.get()));
    }
  }

  shared_ptr<FindNodesInWayFactory> nodeFactory(new FindNodesInWayFactory(way));
  GeometryConverter gc(map);
  gc.setNodeFactory(nodeFactory);
  shared_ptr<Element> e = gc.convertGeometryToElement(g.get(), way->getStatus(),
    way->getCircularError());

  if (e == 0)
  {
    RemoveWayOp::removeWayFully(map, way->getId());
  }
  else
  {
    e->setTags(way->getTags());
    map->replace(way, e);
  }
}

long MapCropper::_findNodeId(shared_ptr<const OsmMap> map, shared_ptr<const Way> w,
  const Coordinate& c)
{
  long result = std::numeric_limits<long>::max();
  const std::vector<long>& nodeIds = w->getNodeIds();

  for (size_t i = 0; i < nodeIds.size(); i++)
  {
    shared_ptr<const Node> n = map->getNode(nodeIds[i]);
    if (n->toCoordinate() == c)
    {
      // if there are multiple corresponding nodes, throw an exception.
      if (result != std::numeric_limits<long>::max() && result != nodeIds[i])
      {
//        throw InternalErrorException(QString("Internal Error: Two nodes were found with the same "
//                                     "coordinate. way: %1").arg(w->getId()));
        LOG_ERROR("" << "Internal Error: Two nodes were found with the same coordinate. way: " <<
            w->getId());
      }
      result = nodeIds[i];
    }
  }

  return result;
}

bool MapCropper::_isWhollyInside(const Envelope& e)
{
  bool result = false;
  if (_envelope.isNull() == false)
  {
    if (_invert)
    {
      result = !_envelope.intersects(e);
    }
    else
    {
      result = _envelope.covers(e);
    }
  }
  else
  {
    if (_invert)
    {
      result = !_envelopeG->getEnvelopeInternal()->intersects(e);
    }
    else
    {
      // if it isn't inverted we'd need to do a complex and expensive check so just return false.
    }
  }
  return result;
}

bool MapCropper::_isWhollyOutside(const Envelope& e)
{
  bool result = false;
  if (_envelope.isNull() == false)
  {
    if (_invert)
    {
      result = _envelope.covers(e);
    }
    else
    {
      result = !_envelope.intersects(e);
    }
  }
  else
  {
    if (_invert == false)
    {
      result = !_envelopeG->getEnvelopeInternal()->intersects(e);
    }
  }

  return result;
}

void MapCropper::readObject(QDataStream& is)
{
  is >> _invert;
  uint egSize;
  is >> egSize;
  _envelopeG.reset();
  // if this is a polygon for cropping
  if (egSize > 0)
  {
    string s;
    s.resize(egSize, '*');
    char* data = (char*)s.data();
    is.readRawData(data, egSize);
    stringstream ss(s, ios_base::in);
    _envelopeG.reset(geos::io::WKBReader().read(ss));
    _envelope.setToNull();
  }
  // if this is a simple envelope
  else
  {
    double minx, miny, maxx, maxy;
    is >> minx >> miny >> maxx >> maxy;
    _envelope = Envelope(minx, maxx, miny, maxy);
    _envelopeG.reset(GeometryFactory::getDefaultInstance()->toGeometry(&_envelope));
  }
}

shared_ptr<Way> MapCropper::_reintroduceWay(shared_ptr<OsmMap> map, shared_ptr<const Way> w,
  const LineString* ls)
{
  // create a new way
  shared_ptr<Way> newWay(new Way(w->getStatus(), map->createNextWayId(),
    w->getRawCircularError()));
  newWay->setTags(w->getTags());

  // for each point on the linestring
  for (size_t i = 0; i < ls->getNumPoints(); i++)
  {
    const Coordinate& c = ls->getCoordinateN(i);

    // find the corresponding node in the way
    long nid = _findNodeId(map, w, c);

    // if there isn't a corresponding node in the way
    if (nid == std::numeric_limits<long>::max())
    {
      // if this isn't the first or last point
      if (i != 0 && i != ls->getNumPoints() - 1)
      {
        // this shouldn't be possible, something went wrong.
        throw InternalErrorException("Internal Error: An unexpected coordinate was found.");
      }
      // create a new node
      shared_ptr<Node> node(new Node(w->getStatus(), map->createNextNodeId(), c,
        w->getCircularError()));
      map->addNode(node);
      nid = node->getId();
    }

    // add the node to the way
    newWay->addNode(nid);
  }

  return newWay;
}

void MapCropper::writeObject(QDataStream& os) const
{
  os << _invert;
  // if this is a polygon for cropping
  if (_envelope.isNull())
  {
    stringstream ss;
    // use WKB to avoid small differences in the geometries before/after serialization.
    geos::io::WKBWriter().write(*_envelopeG, ss);
    const string& s = ss.str();
    os << (uint)s.size();
    os.writeRawData(s.data(), s.size());
  }
  // if this is a simple envelope
  else
  {
    os << 0;
    os << _envelope.getMinX();
    os << _envelope.getMinY();
    os << _envelope.getMaxX();
    os << _envelope.getMaxY();
  }
}


}
