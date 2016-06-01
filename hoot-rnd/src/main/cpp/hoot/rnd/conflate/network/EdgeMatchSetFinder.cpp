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
#include "EdgeMatchSetFinder.h"

#include "EdgeMatch.h"

namespace hoot
{

EdgeMatchSetFinder::EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2) :
  _details(details),
  _matchSet(matchSet),
  _n1(n1),
  _n2(n2)
{
}

void EdgeMatchSetFinder::addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  // recursively explore the edges that neighbor e1 and e2. All the discovered matches will be
  // recorded

  EdgeMatchPtr em(new EdgeMatch());

  bool reversed = _details->isReversed(e1, e2);
  em->getString1()->addFirstEdge(e1, false);
  em->getString2()->addFirstEdge(e2, reversed);

  _addEdgeMatches(em);
}

void EdgeMatchSetFinder::_addEdgeMatches(EdgeMatchPtr em)
{
  // if both the start and end have a valid matched vertex pair then add the match
  ConstNetworkVertexPtr from1 = em->getString1()->getFrom();
  ConstNetworkVertexPtr from2 = em->getString2()->getFrom();
  ConstNetworkVertexPtr to1 = em->getString1()->getTo();
  ConstNetworkVertexPtr to2 = em->getString2()->getTo();
  bool fromMatch = _details->isCandidateMatch(from1, from2);
  bool toMatch = _details->isCandidateMatch(to1, to2);

//  LOG_VAR(em);
//  LOG_VAR(fromMatch);
//  LOG_VAR(toMatch);

  /// @todo Possibly continue to evaluate matches even if we find an end point. This may make
  /// the search space too large, but would avoid missing matches.
  if (fromMatch && toMatch)
  {
    _matchSet->addEdgeMatch(em, _scoreMatch(em));
  }
  // if the end of the match isn't terminated.
  else if (!toMatch)
  {
    // get all the neighboring edges to 1 and 2
    QList<ConstNetworkEdgePtr> neighbors1 = _n1->getEdgesFromVertex(to1);
    QList<ConstNetworkEdgePtr> neighbors2 = _n2->getEdgesFromVertex(to2);

    _addEdgeNeighborsToEnd(em, neighbors1, neighbors2);
  }
  // if the beginning of the match isn't terminated
  else if (!fromMatch)
  {
    // get all the neighboring edges to 1 and 2
    QList<ConstNetworkEdgePtr> neighbors1 = _n1->getEdgesFromVertex(from1);
    QList<ConstNetworkEdgePtr> neighbors2 = _n2->getEdgesFromVertex(from2);

    _addEdgeNeighborsToStart(em, neighbors1, neighbors2);
  }
}

void EdgeMatchSetFinder::_addEdgeNeighborsToEnd(EdgeMatchPtr em,
  QList<ConstNetworkEdgePtr> neighbors1, QList<ConstNetworkEdgePtr> neighbors2)
{
  // score all the e1 neighbors.
  for (int i = 0; i < neighbors1.size(); ++i)
  {
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors1[i]) == false &&
      _details->getPartialEdgeMatchScore(neighbors1[i], em->getString2()->getLastEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString1()->appendEdge(neighbors1[i]);
      _addEdgeMatches(next);
    }
  }

  // score all the e2 neighbors.
  for (int i = 0; i < neighbors2.size(); ++i)
  {
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors2[i]) == false &&
      _details->getPartialEdgeMatchScore(neighbors2[i], em->getString1()->getLastEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString2()->appendEdge(neighbors2[i]);
      _addEdgeMatches(next);
    }
  }
}

void EdgeMatchSetFinder::_addEdgeNeighborsToStart(EdgeMatchPtr em,
  QList<ConstNetworkEdgePtr> neighbors1, QList<ConstNetworkEdgePtr> neighbors2)
{
  // score all the e1 neighbors.
  for (int i = 0; i < neighbors1.size(); ++i)
  {
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors1[i]) == false &&
      _details->getPartialEdgeMatchScore(neighbors1[i], em->getString2()->getFirstEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString1()->prependEdge(neighbors1[i]);
      _addEdgeMatches(next);
    }
  }

  // score all the e2 neighbors.
  for (int i = 0; i < neighbors2.size(); ++i)
  {
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors2[i]) == false &&
      _details->getPartialEdgeMatchScore(neighbors2[i], em->getString1()->getFirstEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString2()->prependEdge(neighbors2[i]);
      _addEdgeMatches(next);
    }
  }
}

}
