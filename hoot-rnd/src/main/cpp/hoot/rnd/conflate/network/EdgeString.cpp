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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "EdgeString.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

bool operator==(const ConstEdgeStringPtr& es1, const ConstEdgeStringPtr& es2)
{
  bool result = false;

  if (es1.get() == es2.get())
  {
    result = true;
  }
  else if (es1->getAllEdges().size() != es2->getAllEdges().size())
  {
    result = false;
  }
  else
  {
    result = true;

    for (int i = 0; i < es1->getAllEdges().size(); ++i)
    {
      const EdgeString::EdgeEntry& ee1 = es1->getAllEdges()[i];
      const EdgeString::EdgeEntry& ee2 = es2->getAllEdges()[i];

      if (ee1.getSubline() != ee2.getSubline())
      {
        result = false;
        break;
      }
    }
  }

//  bool strResult = es1->toString() == es2->toString();

//  if (result != strResult)
//  {
//    LOG_VARE(result);
//    LOG_VARE(strResult);
//    LOG_VARE(es1);
//    LOG_VARE(es2);
//    throw HootException();
//  }

  return result;
}

EdgeString::EdgeString()
{
}

void EdgeString::addFirstEdge(ConstNetworkEdgePtr e)
{
  assert(_edges.size() == 0);
  _edges.append(EdgeEntry(EdgeSubline::createFullSubline(e)));
}

void EdgeString::addFirstEdge(ConstEdgeSublinePtr subline)
{
  assert(_edges.size() == 0);
  _edges.append(EdgeEntry(subline));
}

void EdgeString::appendEdge(ConstNetworkEdgePtr e)
{
  if (_edges.size() == 0)
  {
    addFirstEdge(e);
  }
  else
  {
    assert(isStub() == false);

    if (isEdgeClosed() == true)
    {
      throw IllegalArgumentException("Illegal argument, you attempted to add an edge to an edge "
        "string that is already closed. Ends in a partial match?");
    }

    double fromPortion = 0.0;
    double toPortion = 1.0;
    if (e->getFrom() == getToVertex())
    {
      // pass
    }
    else if (e->getTo() == getToVertex())
    {
      fromPortion = 1.0;
      toPortion = 0.0;
    }
    else
    {
      throw HootException("Error attempting to append an edge that isn't connected.");
    }

    _edges.append(ConstEdgeSublinePtr(new EdgeSubline(e, fromPortion, toPortion)));
  }
}

void EdgeString::appendEdge(ConstEdgeSublinePtr subline)
{
  if (_edges.size() == 0)
  {
    addFirstEdge(subline);
  }
  else
  {
    assert(isStub() == false);
    if (isEdgeClosed() == true)
    {
      throw IllegalArgumentException("Illegal argument, you attempted to add an edge to an edge "
        "string that is already closed. Ends in a partial match?");
    }

    ConstEdgeSublinePtr newEntry;
    if (subline->getStart()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      subline->getStart()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getToVertex())
    {
      newEntry = subline;
    }
    else if (subline->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      subline->getEnd()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getToVertex())
    {
      EdgeSublinePtr copy = subline->clone();
      copy->reverse();
      newEntry = copy;
    }
    else
    {
      throw HootException("Error attempting to append an edge that isn't connected.");
    }

    _edges.append(EdgeEntry(newEntry));
  }
}

Meters EdgeString::calculateLength(const ConstElementProviderPtr& provider) const
{
  Meters result = 0.0;

  for (int i = 0; i < _edges.size(); ++i)
  {
    result += _edges[i].getEdge()->calculateLength(provider);
  }

  return result;
}

shared_ptr<EdgeString> EdgeString::clone() const
{
  EdgeStringPtr result(new EdgeString());
  result->_edges = _edges;
  return result;
}

bool EdgeString::contains(const shared_ptr<const EdgeString> other) const
{
  foreach (const EdgeEntry& ee, other->_edges)
  {
    if (contains(ee.getEdge()) == false)
    {
      return false;
    }
  }

  return true;
}

bool EdgeString::contains(ConstNetworkEdgePtr e) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getEdge() == e)
    {
      return true;
    }
  }

  return false;
}

bool EdgeString::contains(ConstNetworkVertexPtr v) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getSubline()->contains(v))
    {
      return true;
    }
  }

  return false;
}

bool EdgeString::containsInteriorVertex(ConstNetworkVertexPtr v) const
{
  return contains(v) && isAtExtreme(v) == false;
}

ConstNetworkEdgePtr EdgeString::getEdgeAtOffset(ConstOsmMapPtr map, Meters offset) const
{
  Meters d = 0.0;
  foreach (EdgeEntry ee, _edges)
  {
    d += ee.getEdge()->calculateLength(map);

    if (d >= offset)
    {
      return ee.getEdge();
    }
  }

  return getLastEdge();
}

QSet<ConstNetworkEdgePtr> EdgeString::getEdgeSet() const
{
  QSet<ConstNetworkEdgePtr> result;
  foreach (const EdgeEntry& ee, _edges)
  {
    result.insert(ee.getEdge());
  }

  return result;
}

ConstEdgeLocationPtr EdgeString::getFrom() const
{
  return _edges.front().getSubline()->getStart();
}

ConstNetworkVertexPtr EdgeString::getFromVertex() const
{
  return getFrom()->getVertex(EdgeLocation::SLOPPY_EPSILON);
}

QList<ConstElementPtr> EdgeString::getMembers() const
{
  QList<ConstElementPtr> result;

  foreach (const EdgeEntry& e, _edges)
  {
    result += e.getEdge()->getMembers();
  }

  return result;
}

ConstEdgeLocationPtr EdgeString::getTo() const
{
  return _edges.back().getSubline()->getEnd();
}

ConstNetworkVertexPtr EdgeString::getToVertex() const
{
  return getTo()->getVertex(EdgeLocation::SLOPPY_EPSILON);
}

bool EdgeString::isEdgeClosed() const
{
  bool result = false;

  // if the end of the subline isn't at a vertex.
  if (_edges.back().getSubline()->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON) == false ||
    _edges.back().getSubline()->isZeroLength())
  {
    result = true;
  }

  return result;
}

bool EdgeString::isAtExtreme(ConstNetworkVertexPtr v) const
{
  bool result = false;
  const ConstEdgeLocationPtr& start = getFrom();
  const ConstEdgeLocationPtr& end = getTo();

  if ((start->isExtreme() && start->getVertex() == v) ||
    (end->isExtreme() && end->getVertex() == v))
  {
    result = true;
  }

  return result;
}

bool EdgeString::overlaps(shared_ptr<const EdgeString> other) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (other->overlaps(_edges[i].getSubline()))
    {
      return true;
    }
  }

  return false;
}

bool EdgeString::overlaps(const ConstNetworkEdgePtr& e) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getEdge() == e)
    {
      return true;
    }
  }

  return false;
}

bool EdgeString::overlaps(const ConstEdgeSublinePtr& es) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getSubline()->overlaps(es))
    {
      return true;
    }
  }

  return false;
}

void EdgeString::prependEdge(ConstNetworkEdgePtr e)
{
  assert(_edges.size() > 0);

  double start, end;
  if (e->getFrom() == getFromVertex())
  {
    start = 1.0;
    end = 0.0;
  }
  else if (e->getTo() == getFromVertex())
  {
    start = 0.0;
    end = 1.0;
  }
  else
  {
    throw HootException("Error attempting to append an edge that isn't connected.");
  }

  _edges.prepend(ConstEdgeSublinePtr(new EdgeSubline(e, start, end)));
}

void EdgeString::reverse()
{
  std::reverse(_edges.begin(), _edges.end());
  for (int i = 0; i < _edges.size(); ++i)
  {
    _edges[i].reverse();
  }
}

QString EdgeString::toString() const
{
  return hoot::toString(_edges);
}

}
