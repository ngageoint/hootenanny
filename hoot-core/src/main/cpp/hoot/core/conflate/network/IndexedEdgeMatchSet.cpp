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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "IndexedEdgeMatchSet.h"

#include <hoot/core/conflate/network/IndexedEdgeLinks.h>

namespace hoot
{

int IndexedEdgeMatchSet::logWarnCount = 0;

void IndexedEdgeMatchSet::addEdgeMatch(const ConstEdgeMatchPtr& em, double score)
{
  if (!contains(em))
  {
    LOG_TRACE("Adding edge match...");

    _matches.insert(em, score);

    // index it so we can quickly determine which match an edge is part of.
    _addEdgeToMatchMapping(em->getString1(), em);
    _addEdgeToMatchMapping(em->getString2(), em);
    _addVertexToMatchMapping(em->getString1(), em);
    _addVertexToMatchMapping(em->getString2(), em);
  }
}

void IndexedEdgeMatchSet::_addEdgeToMatchMapping(ConstEdgeStringPtr str,
                                                 const ConstEdgeMatchPtr& em)
{
  QList<EdgeString::EdgeEntry> e = str->getAllEdges();
  for (const auto& ee : qAsConst(e))
    _edgeToMatch[ee.getEdge()].insert(em);
}

void IndexedEdgeMatchSet::_addVertexToMatchMapping(ConstEdgeStringPtr str,
                                                   const ConstEdgeMatchPtr& em)
{
  QList<EdgeString::EdgeEntry> e = str->getAllEdges();
  for (const auto& ee : qAsConst(e))
  {
    if (ee.getSubline()->getStart()->isExtreme())
      _vertexToMatch[ee.getSubline()->getStart()->getVertex()].insert(em);
    if (ee.getSubline()->getEnd()->isExtreme())
      _vertexToMatch[ee.getSubline()->getEnd()->getVertex()].insert(em);
  }
}

void IndexedEdgeMatchSet::removeEdgeMatch(const ConstEdgeMatchPtr& em)
{
  if (contains(em))
  {
    _matches.remove(em);

    _removeEdgeToMatchMapping(em->getString1(), em);
    _removeEdgeToMatchMapping(em->getString2(), em);
    _removeVertexToMatchMapping(em->getString1(), em);
    _removeVertexToMatchMapping(em->getString2(), em);
  }
  else
  {
    if (logWarnCount < Log::getWarnMessageLimit())
    {
      LOG_WARN("Attempted to remove edge match that doesn't exist: " << em);
    }
    else if (logWarnCount == Log::getWarnMessageLimit())
    {
      LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
    }
    logWarnCount++;
  }
}

void IndexedEdgeMatchSet::_removeEdgeToMatchMapping(ConstEdgeStringPtr str,
                                                    const ConstEdgeMatchPtr& em)
{
  QList<EdgeString::EdgeEntry> e = str->getAllEdges();
  for (const auto& ee : qAsConst(e))
    _edgeToMatch[ee.getEdge()].remove(em);
}

void IndexedEdgeMatchSet::_removeVertexToMatchMapping(ConstEdgeStringPtr str,
                                                      const ConstEdgeMatchPtr& em)
{
  QList<EdgeString::EdgeEntry> e = str->getAllEdges();
  for (const auto& ee : qAsConst(e))
  {
    if (ee.getSubline()->getStart()->isExtreme())
      _vertexToMatch[ee.getSubline()->getStart()->getVertex()].remove(em);
    if (ee.getSubline()->getEnd()->isExtreme())
      _vertexToMatch[ee.getSubline()->getEnd()->getVertex()].remove(em);
  }
}

std::shared_ptr<IndexedEdgeLinks> IndexedEdgeMatchSet::calculateEdgeLinks()
{
  std::shared_ptr<IndexedEdgeLinks> result = std::make_shared<IndexedEdgeLinks>();
  for (auto it = getAllMatches().begin(); it != getAllMatches().end(); ++it)
  {
    ConstEdgeMatchPtr em = it.key();
    ConstNetworkVertexPtr from1, from2, to1, to2;
    LOG_VART(em);
    from1 = em->getString1()->getFromVertex();
    to1 = em->getString1()->getToVertex();
    from2 = em->getString2()->getFromVertex();
    to2 = em->getString2()->getToVertex();

    // get all the edges that connect to from1/from2 and are also a proper pair.
    QSet<ConstEdgeMatchPtr> fromLinks = getMatchesWithTermination(from1, from2);

    // get all the edges that connect to to1/to2 and are also a proper pair.
    QSet<ConstEdgeMatchPtr> toLinks = getMatchesWithTermination(to1, to2);

    QSet<ConstEdgeMatchPtr> links = fromLinks | toLinks;

    for (const auto& other : qAsConst(links))
    {
      // if the other edge isn't part of this edge.
      if (other->overlaps(em) == false)
        result->insertMulti(em, other);
    }
  }

  return result;
}

std::shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSet::clone() const
{
  IndexedEdgeMatchSetPtr result = std::make_shared<IndexedEdgeMatchSet>();
  result->_matches = _matches;
  return result;
}

bool IndexedEdgeMatchSet::contains(const ConstEdgeMatchPtr& em) const
{
  bool result = false;
  if (_matches.contains(em))
    result = true;
  else
  {
    EdgeMatchPtr r = em->clone();
    r->reverse();
    // we allow reversed stubs, but no others.
    if (_matches.contains(r))
      result = true;
  }
  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatContain(ConstNetworkEdgePtr e) const
{
  return _edgeToMatch[e];
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatTerminateAt(ConstNetworkVertexPtr v) const
{
  QSet<ConstEdgeMatchPtr> result;
  for (const auto& em : _vertexToMatch[v])
  {
    if (em->getString1()->isAtExtreme(v) || em->getString2()->isAtExtreme(v))
      result.insert(em);
  }
  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatOverlap(ConstEdgeMatchPtr e) const
{
  QSet<ConstEdgeMatchPtr> result;
  QList<ConstEdgeMatchPtr> list;
  QSet<ConstEdgeMatchPtr> candidates;

  for (const auto& ee : e->getString1()->getAllEdges())
  {
    for (const auto& em : _edgeToMatch[ee.getEdge()])
      list.append(em);
  }

  for (const auto& ee : e->getString2()->getAllEdges())
  {
    for (const auto& em : _edgeToMatch[ee.getEdge()])
      list.append(em);
  }

  candidates = list.toSet();

  for (const auto& em : qAsConst(candidates))
  {
    if (em->getString1()->overlaps(e->getString1()) || em->getString2()->overlaps(e->getString2()))
      result.insert(em);
  }

  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesWithInteriorVertex(ConstNetworkVertexPtr v) const
{
  QSet<ConstEdgeMatchPtr> result;
  for (const auto& em : _vertexToMatch[v])
  {
    if (em->getString1()->containsInteriorVertex(v) || em->getString2()->containsInteriorVertex(v))
      result.insert(em);
  }
  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesWithTermination(ConstNetworkVertexPtr v1,
                                                                       ConstNetworkVertexPtr v2) const
{
  QSet<ConstEdgeMatchPtr> result;

  // go through all the matches that contain v1 and v2
  auto vertex_set = _vertexToMatch[v1] & _vertexToMatch[v2];
  for (const auto& em : qAsConst(vertex_set))
  {
        //  FromOnVertex,  string1 -> v1, string2 ->v2
    if ((em->getString1()->isFromOnVertex() &&
         em->getString1()->getFromVertex() == v1 &&
         em->getString2()->isFromOnVertex() &&
         em->getString2()->getFromVertex() == v2) ||
        //  ToOnVertex, string1 -> v1, string2 ->v2
        (em->getString1()->isToOnVertex() &&
         em->getString1()->getToVertex() == v1 &&
         em->getString2()->isToOnVertex() &&
         em->getString2()->getToVertex() == v2) ||
        //  FromOnVertex, string1 -> v2, string2 -> v1
        (em->getString1()->isFromOnVertex() &&
         em->getString1()->getFromVertex() == v2 &&
         em->getString2()->isFromOnVertex() &&
         em->getString2()->getFromVertex() == v1) ||
        //  ToOnVertex, string1 -> v2, string2 -> v1
        (em->getString1()->isToOnVertex() &&
         em->getString1()->getToVertex() == v2 &&
         em->getString2()->isToOnVertex() &&
         em->getString2()->getToVertex() == v1))
    {
      result.insert(em);
    }
  }

  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getConnectingStubs(ConstEdgeMatchPtr a,
                                                                ConstEdgeMatchPtr b) const
{
  QSet<ConstEdgeMatchPtr> result;

  result += getConnectingStubs(a->getString1()->getFrom(), a->getString2()->getFrom(),
                               b->getString1()->getFrom(), b->getString2()->getFrom());
  result += getConnectingStubs(a->getString1()->getFrom(), a->getString2()->getFrom(),
                               b->getString1()->getTo(), b->getString2()->getTo());
  result += getConnectingStubs(a->getString1()->getTo(), a->getString2()->getTo(),
                               b->getString1()->getTo(), b->getString2()->getTo());
  result += getConnectingStubs(a->getString1()->getTo(), a->getString2()->getTo(),
                               b->getString1()->getFrom(), b->getString2()->getFrom());

  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getConnectingStubs(ConstEdgeLocationPtr ela1,
                                                                ConstEdgeLocationPtr ela2,
                                                                ConstEdgeLocationPtr elb1,
                                                                ConstEdgeLocationPtr elb2) const
{
  QSet<ConstEdgeMatchPtr> result;

  if (ela1->isExtreme() == false || ela2->isExtreme() == false || elb1->isExtreme() == false || elb2->isExtreme() == false)
    return result;

  ConstNetworkVertexPtr va1 = ela1->getVertex();
  ConstNetworkVertexPtr va2 = ela2->getVertex();
  ConstNetworkVertexPtr vb1 = elb1->getVertex();
  ConstNetworkVertexPtr vb2 = elb2->getVertex();

  // if they're directly connected or they aren't connected by at least one vertex
  if ((va1 == vb1 && va2 == vb2) || (va1 != vb1 && va2 != vb2))
  {
    // pass
  }
  else if (va1 == vb1)  // if the vertices in string 1 match
  {
    auto vertex_set = _vertexToMatch[va1] & _vertexToMatch[va2] & _vertexToMatch[vb2];
    for (const auto& em : qAsConst(vertex_set))
    {
      if (em->containsStub() && em->contains(va1) && em->contains(va2) && em->contains(vb2))
        result.insert(em);
    }
  }
  else if (va2 == vb2)    // if the vertices in string 2 match
  {
    auto vertex_set = _vertexToMatch[va1] & _vertexToMatch[va2] & _vertexToMatch[vb1];
    for (const auto& em : qAsConst(vertex_set))
    {
      if (em->containsStub() && em->contains(va1) && em->contains(va2) && em->contains(vb1))
        result.insert(em);
    }
  }

  return result;
}


QString IndexedEdgeMatchSet::toString() const
{
  return hoot::toString(_matches);
}

}
