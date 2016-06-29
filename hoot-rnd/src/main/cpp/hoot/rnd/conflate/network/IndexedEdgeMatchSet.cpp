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
#include "IndexedEdgeMatchSet.h"

#include <hoot/core/util/Log.h>

namespace hoot
{

IndexedEdgeMatchSet::IndexedEdgeMatchSet()
{
}

void IndexedEdgeMatchSet::addEdgeMatch(const ConstEdgeMatchPtr &em, double score)
{
  if (!contains(em))
  {
    _matches.insert(em, score);

    // index it so we can quickly determine which match an edge is part of.
    _addEdgeToMatchMapping(em->getString1(), em);
    _addEdgeToMatchMapping(em->getString2(), em);
  }
}

void IndexedEdgeMatchSet::_addEdgeToMatchMapping(ConstEdgeStringPtr str,
  const ConstEdgeMatchPtr &em)
{
  QList<EdgeString::EdgeEntry> e = str->getAllEdges();

  foreach (const EdgeString::EdgeEntry& ee, e)
  {
    _edgeToMatch[ee.e].insert(em);
  }
}

shared_ptr<IndexedEdgeLinks> IndexedEdgeMatchSet::calculateEdgeLinks()
{
  shared_ptr<IndexedEdgeLinks> result(new IndexedEdgeLinks());

  for (QHash<ConstEdgeMatchPtr, double>::const_iterator it = getAllMatches().begin();
    it != getAllMatches().end(); ++it)
  {
    ConstEdgeMatchPtr em = it.key();
    ConstNetworkVertexPtr from1, from2, to1, to2;
    from1 = em->getString1()->getFrom();
    to1 = em->getString1()->getTo();
    from2 = em->getString2()->getFrom();
    to2 = em->getString2()->getTo();

    // get all the edges that connect to from1/from2 and are also a proper pair.
    QSet<ConstEdgeMatchPtr> fromLinks = getMatchesWithTermination(from1, from2);

    // get all the edges that connect to to1/to2 and are also a proper pair.
    QSet<ConstEdgeMatchPtr> toLinks = getMatchesWithTermination(to1, to2);

    QSet<ConstEdgeMatchPtr> links = fromLinks | toLinks;

    foreach(ConstEdgeMatchPtr other, links)
    {
      // if the other edge isn't part of this edge.
      if (other->overlaps(em) == false)
      {
        result->insertMulti(em, other);
      }
    }
  }

  return result;
}

shared_ptr<IndexedEdgeMatchSet> IndexedEdgeMatchSet::clone() const
{
  IndexedEdgeMatchSetPtr result(new IndexedEdgeMatchSet());
  result->_matches = _matches;
  return result;
}

bool IndexedEdgeMatchSet::contains(const ConstEdgeMatchPtr &em) const
{
  bool result = false;

  if (_matches.contains(em))
  {
    result = true;
  }
  else
  {
    EdgeMatchPtr r = em->clone();
    r->reverse();
    if (_matches.contains(r))
    {
      result = true;
    }
  }

  return result;
}

ConstEdgeMatchPtr IndexedEdgeMatchSet::getMatch(const ConstEdgeMatchPtr &em) const
{
  MatchHash::const_iterator it = _matches.find(em);

  if (it == _matches.end())
  {
    throw HootException("The specified match was not found: " + em->toString());
  }

  return it.key();
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatContain(ConstNetworkEdgePtr e) const
{
  return _edgeToMatch[e];
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatOverlap(ConstEdgeStringPtr str) const
{
  QSet<ConstEdgeMatchPtr> result;

  foreach (const EdgeString::EdgeEntry& ee, str->getAllEdges())
  {
    result.unite(getMatchesThatContain(ee.e));
  }

  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesThatOverlap(ConstEdgeMatchPtr e) const
{
  QSet<ConstEdgeMatchPtr> result;

  QSet<ConstEdgeMatchPtr> candidates = getMatchesThatOverlap(e->getString1());
  candidates.unite(getMatchesThatOverlap(e->getString2()));

  foreach (const ConstEdgeMatchPtr& em, candidates)
  {
    if (em->getString1()->overlaps(e->getString1()) ||
      em->getString2()->overlaps(e->getString2()))
    {
      result.insert(em);
    }
  }

  return result;
}

QSet<ConstEdgeMatchPtr> IndexedEdgeMatchSet::getMatchesWithTermination(ConstNetworkVertexPtr v1,
  ConstNetworkVertexPtr v2) const
{
  QSet<ConstEdgeMatchPtr> result;

  /// @todo replace this with an index. This will get quite slow w/ 100k edges.

  foreach (const ConstEdgeMatchPtr& em, _matches.keys())
  {
    if (em->getString1()->getFrom() == v1 &&
      em->getString2()->getFrom() == v2)
    {
      result.insert(em);
    }
    else if (em->getString1()->getTo() == v1 &&
      em->getString2()->getTo() == v2)
    {
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
