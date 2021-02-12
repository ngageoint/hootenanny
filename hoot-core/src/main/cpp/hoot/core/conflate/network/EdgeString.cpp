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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#include "EdgeString.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

int EdgeString::logWarnCount = 0;

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

QString EdgeString::EdgeEntry::toString() const
{
  return hoot::toString(_subline);
}

EdgeString::EdgeString()
{
}

void EdgeString::addFirstEdge(const ConstNetworkEdgePtr& e)
{
  assert(_edges.size() == 0);
  _edges.append(EdgeEntry(EdgeSubline::createFullSubline(e)));
  assert(validate());
}

void EdgeString::addFirstEdge(const ConstEdgeSublinePtr& subline)
{
  assert(_edges.size() == 0);
  _edges.append(EdgeEntry(subline));
  assert(validate());
}

void EdgeString::appendEdge(const ConstNetworkEdgePtr& e)
{
  if (_edges.size() == 0)
  {
    addFirstEdge(e);
  }
  else
  {
    assert(isStub() == false);

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
  assert(validate());
}

void EdgeString::appendEdge(const ConstEdgeSublinePtr& subline)
{
  if (_edges.size() == 0)
  {
    addFirstEdge(subline);
  }
  else
  {
    assert(isStub() == false);

    ConstEdgeSublinePtr newEntry;
    if (subline->getStart()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      getTo()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      subline->getStart()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getToVertex())
    {
      newEntry = subline;
    }
    else if (subline->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      getTo()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
      subline->getEnd()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getToVertex())
    {
      EdgeSublinePtr copy = subline->clone();
      copy->reverse();
      newEntry = copy;
    }
    else if (subline->intersects(_edges.back().getSubline()))
    {
      LOG_VART(_edges);
      // replace the last entry with the union of the last and the new subline.
      newEntry = subline->unionSubline(_edges.back().getSubline());
      _edges.removeLast();
    }
    else
    {
      LOG_VARW(subline);
      LOG_VARW(_edges.back().getSubline());
      throw HootException("Error attempting to append a subline that isn't connected.");
    }

    _edges.append(EdgeEntry(newEntry));
  }
  assert(validate());
}

Meters EdgeString::calculateLength(const ConstElementProviderPtr& provider) const
{
  Meters result = 0.0;

  foreach (const EdgeEntry& ee, _edges)
  {
    result += ee.getSubline()->calculateLength(provider);
  }

  return result;
}

std::shared_ptr<EdgeString> EdgeString::clone() const
{
  EdgeStringPtr result(new EdgeString());
  result->_edges = _edges;
  return result;
}

bool EdgeString::contains(const std::shared_ptr<const EdgeString>& other) const
{
  foreach (const EdgeEntry& ee, other->_edges)
  {
    if (contains(ee.getSubline()) == false)
    {
      return false;
    }
  }
  return true;
}

bool EdgeString::contains(const ConstNetworkEdgePtr& e) const
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

bool EdgeString::contains(const ConstEdgeSublinePtr& e) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getSubline()->contains(e))
    {
      return true;
    }
  }
  return false;
}

bool EdgeString::contains(const ConstNetworkVertexPtr& v) const
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

bool EdgeString::contains(const ConstEdgeLocationPtr& el) const
{
  foreach (const EdgeEntry& ee, _edges)
  {
    if (ee.getSubline()->intersects(el))
    {
      return true;
    }
  }
  return false;
}

bool EdgeString::containsInteriorVertex(const ConstNetworkVertexPtr& v) const
{
  return contains(v) && isAtExtreme(v) == false;
}

ConstNetworkEdgePtr EdgeString::getEdgeAtOffset(const ConstOsmMapPtr& map, Meters offset) const
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

ConstEdgeLocationPtr EdgeString::getLocationAtOffset(const ConstElementProviderPtr& map,
                                                     Meters offset) const
{
  Meters d = 0.0;

  if (offset <= 0)
  {
    return getFrom();
  }

  LOG_VART(offset);
  foreach (EdgeEntry ee, _edges)
  {
    Meters length = ee.getSubline()->calculateLength(map);
    LOG_VART(length);
    LOG_VART(ee.getSubline()->getStart());

    if (d + length >= offset)
    {
      if (ee.getSubline()->isBackwards())
      {
        return ee.getSubline()->getStart()->move(map, -(offset - d));
      }
      else
      {
        return ee.getSubline()->getStart()->move(map, offset - d);
      }
    }

    d += length;
  }
  LOG_VART(d);

  assert(offset > calculateLength(map));

  return getTo();
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

bool EdgeString::isAtExtreme(const ConstNetworkVertexPtr& v) const
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

bool EdgeString::overlaps(const std::shared_ptr<const EdgeString>& other) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (other->overlaps(_edges[i].getSubline()))
    {
      LOG_TRACE("Overlaps; this edge: " << _edges[i].getSubline() << " other edge: " << other);
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
      LOG_TRACE("Overlaps; this subline: " << _edges[i].getSubline() << " other subline: " << es);
      return true;
    }
  }
  return false;
}

void EdgeString::prependEdge(const ConstEdgeSublinePtr& subline)
{
  if (_edges.size() == 0)
  {
    addFirstEdge(subline);
  }
  else
  {
    assert(isStub() == false);

    ConstEdgeSublinePtr newEntry;
    if (subline->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
        getFrom()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
        subline->getEnd()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getFromVertex())
    {
      newEntry = subline;
    }
    else if (subline->getStart()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
             getFrom()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
             subline->getStart()->getVertex(EdgeLocation::SLOPPY_EPSILON) == getFromVertex())
    {
      EdgeSublinePtr copy = subline->clone();
      copy->reverse();
      newEntry = copy;
    }
    else if (subline->intersects(_edges.front().getSubline()))
    {
      LOG_VART(_edges);
      // replace the last entry with the union of the last and the new subline.
      newEntry = subline->unionSubline(_edges.front().getSubline());
      _edges.removeFirst();
    }
    else
    {
      LOG_VART(subline->intersects(_edges.front().getSubline()));
      LOG_VART(subline);
      LOG_VART(_edges.front().getSubline());
      throw HootException("Error attempting to prepend an edge that isn't connected.");
    }

    _edges.prepend(EdgeEntry(newEntry));
  }
  assert(validate());
}

void EdgeString::reverse()
{
  std::reverse(_edges.begin(), _edges.end());
  for (int i = 0; i < _edges.size(); ++i)
  {
    _edges[i].reverse();
  }
  assert(validate());
}

void EdgeString::snapExtremes(double epsilon)
{
  if (getFrom()->isExtreme(epsilon))
  {
    ConstEdgeSublinePtr es = _edges[0].getSubline();
    double p;
    if (es->getStart()->getPortion() < 0.5)
    {
      p = 0.0;
    }
    else
    {
      p = 1.0;
    }
    EdgeSublinePtr newEs(new EdgeSubline(es->getEdge(), p, es->getEnd()->getPortion()));
    _edges[0].setSubline(newEs);
  }
  if (getTo()->isExtreme(epsilon))
  {
    ConstEdgeSublinePtr es = _edges.back().getSubline();
    double p;
    if (es->getEnd()->getPortion() < 0.5)
    {
      p = 0.0;
    }
    else
    {
      p = 1.0;
    }
    EdgeSublinePtr newEs(new EdgeSubline(es->getEdge(), es->getStart()->getPortion(), p));
    _edges.back().setSubline(newEs);
  }
}

QString EdgeString::toString() const
{
  return hoot::toString(_edges);
}

bool EdgeString::touches(const ConstEdgeSublinePtr& es) const
{
  if (getFrom()->isExtreme())
  {
    if (es->getStart()->isExtreme() && getFrom()->getVertex() == es->getStart()->getVertex())
    {
      return true;
    }
    if (es->getEnd()->isExtreme() && getFrom()->getVertex() == es->getEnd()->getVertex())
    {
      return true;
    }
  }

  if (getTo()->isExtreme())
  {
    if (es->getStart()->isExtreme() && getTo()->getVertex() == es->getStart()->getVertex())
    {
      return true;
    }
    if (es->getEnd()->isExtreme() && getTo()->getVertex() == es->getEnd()->getVertex())
    {
      return true;
    }
  }

  for (int i = 0; i < _edges.size(); ++i)
  {
    if (_edges[i].getSubline()->intersects(es))
    {
      return true;
    }
  }

  return false;
}

bool EdgeString::touches(const std::shared_ptr<const EdgeString>& es) const
{
  for (int i = 0; i < _edges.size(); ++i)
  {
    if (es->touches(_edges[i].getSubline()))
    {
      return true;
    }
  }
  return false;
}

void EdgeString::trim(const ConstElementProviderPtr& provider, Meters newStartOffset,
                      Meters newEndOffset)
{
  assert(newStartOffset < newEndOffset);

  Meters offset = 0.0;
  // find the new start and end locations
  ConstEdgeLocationPtr newStart = getLocationAtOffset(provider, newStartOffset);
  ConstEdgeLocationPtr newEnd = getLocationAtOffset(provider, newEndOffset);

  QList<EdgeEntry> newEdges;

  // handle this edge case early which simplifies the loop
  if (newStart->getEdge() == newEnd->getEdge())
  {
    newEdges.append(EdgeEntry(ConstEdgeSublinePtr(new EdgeSubline(newStart, newEnd))));
  }
  else
  {
    foreach (const EdgeEntry& ee, _edges)
    {
      ConstEdgeSublinePtr s = ee.getSubline();
      Meters l = s->calculateLength(provider);

      if (s->getEdge() == newStart->getEdge() && newStart != s->getEnd())
      {
        assert(newEdges.size() == 0);
        newEdges.append(EdgeEntry(ConstEdgeSublinePtr(new EdgeSubline(newStart, s->getEnd()))));
      }
      else if (s->getEdge() == newEnd->getEdge() && s->getStart() != newEnd)
      {
        newEdges.append(EdgeEntry(ConstEdgeSublinePtr(new EdgeSubline(s->getStart(), newEnd))));
        break;
      }
      else if (offset >= newStartOffset && offset + l <= newEndOffset)
      {
        newEdges.append(EdgeEntry(s));
      }

      offset += l;
    }
  }

  _edges = newEdges;
}

bool EdgeString::validate() const
{
  bool result = true;

  for (int i = 0; i < _edges.size(); ++i)
  {
    ConstEdgeSublinePtr esi = _edges[i].getSubline();
    if (esi->isZeroLength())
    {
      if (logWarnCount < Log::getWarnMessageLimit())
      {
        LOG_WARN("EdgeString contains a zero length subline: " << esi);
      }
      else if (logWarnCount == Log::getWarnMessageLimit())
      {
        LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
      }
      logWarnCount++;
      result = false;
    }
    for (int j = i + 1; j < _edges.size(); ++j)
    {
      ConstEdgeSublinePtr esj = _edges[j].getSubline();
      if (esi->overlaps(esj))
      {
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("Two edges overlap that shouldn't: " << esi << " and " << esj);
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
        result = false;
      }
    }
  }
  return result;
}

bool EdgeString::isValid() const
{
  if (_edges.size() == 0)
  {
    return false;
  }

  for (int i = 0; i < _edges.size(); ++i)
  {
    ConstEdgeSublinePtr esi = _edges[i].getSubline();
    if (esi->isZeroLength())
    {
      return false;
    }
  }

  return true;
}

}
