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
#include "EdgeSublineMatch.h"

namespace hoot
{

EdgeMatchSetFinder::EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
    ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2) :
  _bidirectionalStubs(true),
  _details(details),
  _includePartialMatches(false),
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
  em->getString1()->addFirstEdge(e1);
  if (reversed)
  {
    em->getString2()->addFirstEdge(ConstEdgeSublinePtr(new EdgeSubline(e2, 1.0, 0.0)));
  }
  else
  {
    em->getString2()->addFirstEdge(EdgeSubline::createFullSubline(e2));
  }
  LOG_VART(e1);
  LOG_VART(e2);

  _steps = 0;
  _addEdgeMatches(em);
}

bool EdgeMatchSetFinder::_addEdgeMatches(ConstEdgeMatchPtr em)
{
  _steps++;
  // if both the start and end have a valid matched vertex pair then add the match
  ConstNetworkVertexPtr from1 = em->getString1()->getFromVertex();
  ConstNetworkVertexPtr from2 = em->getString2()->getFromVertex();
  ConstNetworkVertexPtr to1 = em->getString1()->getToVertex();
  ConstNetworkVertexPtr to2 = em->getString2()->getToVertex();
  bool fromMatch = _details->isCandidateMatch(from1, from2);
  bool toMatch = _details->isCandidateMatch(to1, to2);
  bool foundSolution = false;

  if (_steps > 20)
  {
    return false;
  }

  LOG_VART(em);
  LOG_VART(fromMatch);
  LOG_VART(toMatch);

  /// @todo Possibly continue to evaluate matches even if we find an end point. This may make
  /// the search space very large, but would avoid missing matches.
  if (fromMatch && toMatch)
  {
    double score = _scoreMatch(em);
    if (score > 0)
    {
      // if exactly one string is a stub
      if (em->getString1()->isStub() != em->getString2()->isStub())
      {
        _matchSet->addEdgeMatch(em, score);
        if (_bidirectionalStubs)
        {
          // add it in both directions. In some matchers we don't know which is better.
          EdgeStringPtr rev1 = em->getString1()->clone();
          rev1->reverse();
          EdgeMatchPtr em2(new EdgeMatch(rev1, em->getString2()));
          _matchSet->addEdgeMatch(em2, score);
        }
      }
      else
      {
        _matchSet->addEdgeMatch(em, score);
      }
      foundSolution = true;
    }
  }
  else //if (em->getString1()->isStub() == false && em->getString2()->isStub() == false)
  {
    // if the end of the match isn't terminated.
    if (!toMatch)
    {
      // if either of the vertices doesn't have a tie point then we need to keep searching.
      if (_details->hasConfidentTiePoint(to1) == false ||
        _details->hasConfidentTiePoint(to2) == false)
      {
        // get all the neighboring edges to 1 and 2
        QSet<ConstNetworkEdgePtr> neighbors1 = _n1->getEdgesFromVertex(to1).toSet();
        QSet<ConstNetworkEdgePtr> neighbors2 = _n2->getEdgesFromVertex(to2).toSet();

        // subtract all the edges that are already in the solution.
        neighbors1.subtract(em->getString1()->getEdgeSet());
        neighbors2.subtract(em->getString2()->getEdgeSet());

        LOG_VART(neighbors1);
        LOG_VART(neighbors2);

        if (neighbors1.size() > 0 || neighbors2.size() > 0)
        {
          foundSolution = _addEdgeNeighborsToEnd(em, neighbors1, neighbors2) || foundSolution;
        }
      }
    }
    // if the beginning of the match isn't terminated
    if (!fromMatch && !foundSolution)
    {
      // if either of the vertices doesn't have a tie point then we need to keep searching.
      if (_details->hasConfidentTiePoint(from1) == false ||
        _details->hasConfidentTiePoint(from2) == false)
      {
        // get all the neighboring edges to 1 and 2
        QList<ConstNetworkEdgePtr> neighbors1 = _n1->getEdgesFromVertex(from1);
        QList<ConstNetworkEdgePtr> neighbors2 = _n2->getEdgesFromVertex(from2);

        LOG_VART(neighbors1);
        LOG_VART(neighbors2);

        foundSolution = _addEdgeNeighborsToStart(em, neighbors1, neighbors2) || foundSolution;
      }
    }
  }

  LOG_VART(foundSolution);
  // if we couldn't find a whole string solution and we're supposed to include partial matches
  if (foundSolution == false && _includePartialMatches)
  {
    // keep the best partial matches at each end and add it to the edge match
    foundSolution = _addPartialMatch(em);
  }

  return foundSolution;
}

bool EdgeMatchSetFinder::_addEdgeNeighborsToEnd(ConstEdgeMatchPtr em,
  QSet<ConstNetworkEdgePtr> neighbors1Set, QSet<ConstNetworkEdgePtr> neighbors2Set)
{
  bool foundSolution = false;
  // score all the e1 neighbors.
  foreach (ConstNetworkEdgePtr neighbor1, neighbors1Set)
  {
    LOG_VART(neighbor1);
    LOG_VART(em->contains(neighbor1));
    LOG_VART(_details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getLastEdge()));
    // if the neighbor pair score is non-zero
    if (em->contains(neighbor1) == false &&
      /*neighbors1[i]->isStub() == false &&*/
      _details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getLastEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString1()->appendEdge(neighbor1);
      LOG_VART(next);
      foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  // score all the e2 neighbors.
  foreach (ConstNetworkEdgePtr neighbor2, neighbors2Set)
  {
    LOG_VART(neighbor2);
    LOG_VART(em->contains(neighbor2));
    LOG_VART(_details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getLastEdge()));
    // if the neighbor pair score is non-zero
    if (em->contains(neighbor2) == false &&
      /*neighbors2[i]->isStub() == false &&*/
      _details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getLastEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString2()->appendEdge(neighbor2);
      foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  return foundSolution;
}

bool EdgeMatchSetFinder::_addEdgeNeighborsToStart(ConstEdgeMatchPtr em,
  QList<ConstNetworkEdgePtr> neighbors1, QList<ConstNetworkEdgePtr> neighbors2)
{
  bool foundSolution = false;
  // score all the e1 neighbors.
  for (int i = 0; i < neighbors1.size(); ++i)
  {
    LOG_VART(neighbors1[i]);
    LOG_VART(em->getString2()->getFirstEdge());
    LOG_VART(_details->getPartialEdgeMatchScore(neighbors1[i], em->getString2()->getFirstEdge()));
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors1[i]) == false &&
    /// @todo comment me and retest
      neighbors1[i]->isStub() == false &&
      _details->getPartialEdgeMatchScore(neighbors1[i], em->getString2()->getFirstEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString1()->prependEdge(neighbors1[i]);
      foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  // score all the e2 neighbors.
  for (int i = 0; i < neighbors2.size(); ++i)
  {
    LOG_VART(neighbors2[i]);
    LOG_VART(em->getString1()->getFirstEdge());
    LOG_VART(_details->getPartialEdgeMatchScore(neighbors2[i], em->getString1()->getFirstEdge()));
    // if the neighbor pair score is non-zero
    if (em->contains(neighbors2[i]) == false &&
    /// @todo comment me and retest
      neighbors2[i]->isStub() == false &&
      _details->getPartialEdgeMatchScore(neighbors2[i], em->getString1()->getFirstEdge()) > 0)
    {
      // create and evaluate a new match
      EdgeMatchPtr next = em->clone();
      next->getString2()->prependEdge(neighbors2[i]);
      foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  return foundSolution;
}

bool EdgeMatchSetFinder::_addPartialMatch(ConstEdgeMatchPtr em)
{
  ConstNetworkVertexPtr from1 = em->getString1()->getFromVertex();
  ConstNetworkVertexPtr from2 = em->getString2()->getFromVertex();
  ConstNetworkVertexPtr to1 = em->getString1()->getToVertex();
  ConstNetworkVertexPtr to2 = em->getString2()->getToVertex();
  bool fromMatch = _details->isCandidateMatch(from1, from2);
  bool toMatch = _details->isCandidateMatch(to1, to2);

  LOG_VART(em);

  /// @todo There is a horribly unlikely edge case that could pop up here.
  ///
  ///       ,-----,
  /// +-----'     `-------+-------------+
  ///     +--------------------+----+
  ///     ^^^     ^^^^^^^^^^^^^^^^^^^
  ///
  /// In the above case there should be two matches created, but there will likely only be the
  /// second match. Fixing this should just be a matter of adding a bunch more if statements.

  bool result = false;
  EdgeMatchPtr newEm;

  // if this is a partial match in the middle of a line.
  if (fromMatch == false && toMatch == false
    && em->getString1()->getMembers().size() == 1
    && em->getString2()->getMembers().size() == 1)
  {
    // find all partial matches
    QList<EdgeSublineMatchPtr> matches = _details->calculateMatchingSublines(
      em->getString1()->getEdge(0), em->getString2()->getEdge(0));

    // create a new edge match for each of the partial matches
    foreach (const EdgeSublineMatchPtr& m, matches)
    {

      // we won't even try to make partial matches smaller than the search radius. It just creates
      // too much noise.
      Meters searchRadius = _details->getSearchRadius(m->getSubline1()->getEdge(),
        m->getSubline2()->getEdge());
      if (_details->calculateLength(m->getSubline1()) >= searchRadius &&
        _details->calculateLength(m->getSubline2()) >= searchRadius)
      {
        // create a partial match entry here
        EdgeStringPtr s1(new EdgeString());
        s1->addFirstEdge(m->getSubline1());
        EdgeStringPtr s2(new EdgeString());
        s2->addFirstEdge(m->getSubline2());

        newEm.reset(new EdgeMatch(s1, s2));
      }
    }
  }
  else
  {
    newEm = em->clone();
    // trim the ends off the match so you get a partial match
    if (fromMatch == false)
    {
      newEm = _trimFromEdge(newEm);
      if (!newEm)
      {
        return false;
      }
    }

    if (toMatch == false)
    {
      newEm = _trimToEdge(newEm);
      if (!newEm)
      {
        return false;
      }
    }
  }

  if (newEm)
  {
    double score = _scoreMatch(newEm);
    LOG_VART(newEm);
    LOG_VART(score);
    if (score > 0)
    {
      LOG_VART(newEm);
      LOG_VART(score);
      _matchSet->addEdgeMatch(newEm, score);
      result = true;
    }
  }

  return result;
}

double EdgeMatchSetFinder::_scoreMatch(ConstEdgeMatchPtr em) const
{
  return _details->getEdgeStringMatchScore(em->getString1(), em->getString2());
}

EdgeMatchPtr EdgeMatchSetFinder::_trimFromEdge(ConstEdgeMatchPtr em)
{
  EdgeMatchPtr result;

  // trim the beginning of the edge string as appropriate.
  QList<EdgeSublineMatchPtr> matches = _details->calculateMatchingSublines(
    em->getString1()->getEdge(0), em->getString2()->getEdge(0));

  if (matches.size() == 0)
  {
    return result;
  }

  LOG_VART(matches);

  // only the last match is relevant because it matches to the second to last edge in the match.
  ConstEdgeSublinePtr s1 = matches.back()->getSubline1();
  ConstEdgeSublinePtr s2 = matches.back()->getSubline2();

  LOG_VART(s1);
  LOG_VART(s2);

  ConstEdgeSublinePtr origS1 = em->getString1()->getAllEdges().front().getSubline();
  ConstEdgeLocationPtr startS1 = s1->getStart();
  if (origS1->isBackwards() != s1->isBackwards())
  {
    startS1 = s1->getEnd();
  }

  ConstEdgeSublinePtr origS2 = em->getString2()->getAllEdges().front().getSubline();
  ConstEdgeLocationPtr startS2 = s2->getStart();
  if (origS2->isBackwards() != s2->isBackwards())
  {
    startS2 = s2->getEnd();
  }

  LOG_VART(origS1);
  LOG_VART(origS2);

  // we don't want to trim from the back of the edges
  s1 = ConstEdgeSublinePtr(new EdgeSubline(startS1, origS1->getEnd()));
  s2 = ConstEdgeSublinePtr(new EdgeSubline(startS2, origS2->getEnd()));

  LOG_VART(s1);
  LOG_VART(s2);

  if (s1->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
    s2->getEnd()->isExtreme(EdgeLocation::SLOPPY_EPSILON))
  {
    EdgeStringPtr str1(new EdgeString());
    EdgeStringPtr str2(new EdgeString());

    str1->addFirstEdge(s1);
    foreach (const EdgeString::EdgeEntry& ee, em->getString1()->getAllEdges().mid(1))
    {
      str1->appendEdge(ee.getSubline());
    }

    str2->addFirstEdge(s2);
    foreach (const EdgeString::EdgeEntry& ee, em->getString2()->getAllEdges().mid(1))
    {
      str2->appendEdge(ee.getSubline());
    }

    result.reset(new EdgeMatch(str1, str2));
  }

  return result;
}

EdgeMatchPtr EdgeMatchSetFinder::_trimToEdge(ConstEdgeMatchPtr em)
{
  EdgeMatchPtr result;

  LOG_VART(em);
  // trim the beginning of the edge string as appropriate.
  QList<EdgeSublineMatchPtr> matches = _details->calculateMatchingSublines(
    em->getString1()->getLastEdge(), em->getString2()->getLastEdge());

  if (matches.size() == 0)
  {
    return result;
  }

  // only the first match is relevant because it matches to the second to last edge in the match.
  ConstEdgeSublinePtr s1 = matches.front()->getSubline1();
  ConstEdgeSublinePtr s2 = matches.front()->getSubline2();

  LOG_VART(s1);
  LOG_VART(s2);

  ConstEdgeSublinePtr origS1 = em->getString1()->getAllEdges().back().getSubline();
  ConstEdgeLocationPtr endS1 = s1->getEnd();
  if (origS1->isBackwards() != s1->isBackwards())
  {
    endS1 = s1->getStart();
  }

  ConstEdgeSublinePtr origS2 = em->getString2()->getAllEdges().back().getSubline();
  ConstEdgeLocationPtr endS2 = s2->getEnd();
  if (origS2->isBackwards() != s2->isBackwards())
  {
    endS2 = s2->getStart();
  }

  // we don't want to trim from the front of the edges
  s1 = ConstEdgeSublinePtr(new EdgeSubline(origS1->getStart(), endS1));
  s2 = ConstEdgeSublinePtr(new EdgeSubline(origS2->getStart(), endS2));

  LOG_VART(s1);
  LOG_VART(s2);

  // at least one of the strings should start at a vertex.
  if (s1->getStart()->isExtreme(EdgeLocation::SLOPPY_EPSILON) &&
    s2->getStart()->isExtreme(EdgeLocation::SLOPPY_EPSILON))
  {
    EdgeStringPtr str1(new EdgeString());
    EdgeStringPtr str2(new EdgeString());

    if (em->getString1()->getCount() == 1)
    {
      str1->addFirstEdge(s1);
    }
    else
    {
      str1->addFirstEdge(em->getString1()->getAllEdges().first().getSubline());
      // add all but the last edge
      foreach (const EdgeString::EdgeEntry& ee,
        em->getString1()->getAllEdges().mid(1, em->getString1()->getCount() - 2))
      {
        str1->appendEdge(ee.getSubline());
      }
      // add our new trimmed last edge.
      str1->appendEdge(s1);
    }

    if (em->getString2()->getCount() == 1)
    {
      str2->addFirstEdge(s2);
    }
    else
    {
      str2->addFirstEdge(em->getString2()->getAllEdges().first().getSubline());
      // add all but the last edge
      foreach (const EdgeString::EdgeEntry& ee,
        em->getString2()->getAllEdges().mid(1, em->getString2()->getCount() - 2))
      {
        str2->appendEdge(ee.getSubline());
      }
      // add our new trimmed last edge.
      str2->appendEdge(s2);
    }

    result.reset(new EdgeMatch(str1, str2));
  }

  return result;
}

}
