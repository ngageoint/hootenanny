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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef NETWORKVERTEX_H
#define NETWORKVERTEX_H

// hoot
#include <hoot/core/elements/Element.h>

// geos


namespace hoot
{

/**
 * Why NetworkVertex? To avoid confusion with the OSM Node.
 *
 * A vertex is not necessarily associated with just a single OSM node. A vertex could represent an
 * entire roudabout instead of each of the intersections that make up a roundabout. Whatever makes
 * the conflation more effective.
 */
class NetworkVertex
{
public:

  NetworkVertex(ConstElementPtr e);

  ConstElementPtr getElement() const { return _e; }

  ElementId getElementId() const { return _e->getElementId(); }

  /**
   * This should only be used during testing to get consistent results.
   */
  static void reset();

  QString toString() const;

  geos::geom::Coordinate getCentroid() const;

private:

  ConstElementPtr _e;
  int _uid;
  static int uidCount;
};

using NetworkVertexPtr = std::shared_ptr<NetworkVertex>;
using ConstNetworkVertexPtr = std::shared_ptr<const NetworkVertex>;

inline bool operator<(const NetworkVertexPtr& v1, const NetworkVertexPtr& v2)
{
  return v1->getElementId() < v2->getElementId();
}

inline bool operator<(const ConstNetworkVertexPtr& v1, const ConstNetworkVertexPtr& v2)
{
  return v1->getElementId() < v2->getElementId();
}

inline uint qHash(const ConstNetworkVertexPtr& v)
{
  return qHash(v->getElementId());
}

inline uint qHash(const NetworkVertexPtr& v)
{
  return qHash(v->getElementId());
}

}

#endif // NETWORKVERTEX_H
