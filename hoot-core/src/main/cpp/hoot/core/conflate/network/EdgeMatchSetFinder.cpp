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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "EdgeMatchSetFinder.h"

// hoot
#include <hoot/core/conflate/network/EdgeMatch.h>
#include <hoot/core/conflate/network/EdgeSublineMatch.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

const QString EdgeMatchSetFinder::EDGE_MATCH_SIMILAR_KEY = "01-similar";
const QString EdgeMatchSetFinder::EDGE_MATCH_SIMILAR_FIRST_REVERSED_KEY = "03-similar-first-reversed";
const QString EdgeMatchSetFinder::EDGE_MATCH_SIMILAR_SECOND_REVERSED_KEY = "02-similar-second-reversed";

EdgeMatchSetFinder::EdgeMatchSetFinder(NetworkDetailsPtr details, IndexedEdgeMatchSetPtr matchSet,
                                       ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2)
  : _bidirectionalStubs(true),
    _details(details),
    _includePartialMatches(false),
    _matchSet(matchSet),
    _n1(n1),
    _n2(n2),
    _maxSteps(ConfigOptions().getNetworkEdgeMatchSetFinderMaxIterations()),
    _numSimilarEdgeMatches(0)
{
  _resetEdgeMatchSimilarities();
}

void EdgeMatchSetFinder::_resetEdgeMatchSimilarities()
{
  _edgeMatchSimilarities.clear();
  // purposefully mantaining this order (may not end up needing to actually having to keep it, though)
  _edgeMatchSimilarities[EDGE_MATCH_SIMILAR_KEY] = EdgeMatchSimilarity();
  _edgeMatchSimilarities[EDGE_MATCH_SIMILAR_SECOND_REVERSED_KEY] = EdgeMatchSimilarity();
  _edgeMatchSimilarities[EDGE_MATCH_SIMILAR_FIRST_REVERSED_KEY] = EdgeMatchSimilarity();
}

void EdgeMatchSetFinder::addEdgeMatches(ConstNetworkEdgePtr e1, ConstNetworkEdgePtr e2)
{
  LOG_TRACE("Adding edge matches...");

  // recursively explore the edges that neighbor e1 and e2. All the discovered matches will be
  // recorded

  if (e1->isStub() || e2->isStub())
  {
    EdgeMatchPtr em = std::make_shared<EdgeMatch>();
    em->getString1()->addFirstEdge(e1);
    em->getString2()->addFirstEdge(e2);
    _recordMatch(em);
    LOG_VART(em);
  }
  else
  {
    QList<EdgeSublineMatchPtr> sublines = _details->calculateMatchingSublines(e1, e2);
    for (const auto& s : qAsConst(sublines))
    {
      EdgeMatchPtr em = std::make_shared<EdgeMatch>();
      em->getString1()->addFirstEdge(s->getSubline1());
      em->getString2()->addFirstEdge(s->getSubline2());
      _steps = 0;
      _addEdgeMatches(em);
    }
  }
}

bool EdgeMatchSetFinder::_addEdgeMatches(ConstEdgeMatchPtr em)
{
  LOG_TRACE("Adding edge matches...");
  LOG_VART(em);

  _steps++;

  // if both the start and end have a valid matched vertex pair then add the match
  ConstEdgeLocationPtr from1 = em->getString1()->getFrom();
  ConstEdgeLocationPtr from2 = em->getString2()->getFrom();
  ConstEdgeLocationPtr to1 = em->getString1()->getTo();
  ConstEdgeLocationPtr to2 = em->getString2()->getTo();
  bool fromMatch = _isCandidateMatch(from1, from2);
  bool toMatch = _isCandidateMatch(to1, to2);
  bool foundSolution = false;

  if (_steps > _maxSteps)
  {
    LOG_VART(_steps);
    return false;
  }

  LOG_VART(fromMatch);
  LOG_VART(toMatch);

  // Possibly continue to evaluate matches even if we find an end point. This may make
  // the search space very large, but would avoid missing matches. - #2939
  if (fromMatch && toMatch)
    foundSolution = _recordMatch(em);
  else
  {
    // If the end of the match isn't terminated and
    if (!toMatch &&
        // either of the vertices doesn't have a tie point, then we need to keep searching.
        (_hasConfidentTiePoint(to1) == false || _hasConfidentTiePoint(to2) == false))
    {
      // Get all the neighboring edges to 1 and 2.
      QSet<ConstNetworkEdgePtr> neighbors1 = _getEdgesFromLocation(to1);
      QSet<ConstNetworkEdgePtr> neighbors2 = _getEdgesFromLocation(to2);

      // Subtract all the edges that are already in the solution.
      neighbors1.subtract(em->getString1()->getEdgeSet());
      neighbors2.subtract(em->getString2()->getEdgeSet());

      LOG_VART(neighbors1);
      LOG_VART(neighbors2);

      if (!neighbors1.empty() || !neighbors2.empty())
        foundSolution = _addEdgeNeighborsToEnd(em, neighbors1, neighbors2) || foundSolution;
    }
    // If the beginning of the match isn't terminated and
    if (!fromMatch && !foundSolution &&
        // either of the vertices doesn't have a tie point, then we need to keep searching.
        (_hasConfidentTiePoint(from1) == false || _hasConfidentTiePoint(from2) == false))
    {
      // Get all the neighboring edges to 1 and 2.
      QSet<ConstNetworkEdgePtr> neighbors1 = _getEdgesFromLocation(from1);
      QSet<ConstNetworkEdgePtr> neighbors2 = _getEdgesFromLocation(from2);

      neighbors1.subtract(em->getString1()->getEdgeSet());
      neighbors2.subtract(em->getString2()->getEdgeSet());

      LOG_VART(neighbors1);
      LOG_VART(neighbors2);

      foundSolution = _addEdgeNeighborsToStart(em, neighbors1, neighbors2) || foundSolution;
    }
  }
  LOG_VART(foundSolution);

  // if we couldn't find a whole string solution and we're supposed to include partial matches
  if (foundSolution == false && _includePartialMatches)
    foundSolution = _recordMatch(em);

  LOG_VART(foundSolution);
  return foundSolution;
}

bool EdgeMatchSetFinder::_addEdgeNeighborsToEnd(ConstEdgeMatchPtr em,
                                                QSet<ConstNetworkEdgePtr> neighbors1Set,
                                                QSet<ConstNetworkEdgePtr> neighbors2Set)
{
  LOG_TRACE("Adding edge neighbors to end...");

  bool foundSolution = false;

  // score all the e1 neighbors.
  for (const auto& neighbor1 : qAsConst(neighbors1Set))
  {
    LOG_VART(neighbor1);
    LOG_VART(em->contains(neighbor1));

    // Calling this non-const function can alter the state of the map, and lead to different
    // behaviors at different log levels! Beware!
    //LOG_VART(_details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getLastEdge()));

    // if the neighbor pair score is non-zero
    if (em->contains(neighbor1) == false &&
        _details->isStringCandidate(em->getString1()->getLastEdge(), neighbor1) &&
        _details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getLastEdge()) > 0)
    {
      EdgeMatchPtr next = _details->extendEdgeMatch(em, neighbor1, em->getString2()->getLastEdge());
      if (next)
        foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  // score all the e2 neighbors.
  for (const auto& neighbor2 : qAsConst(neighbors2Set))
  {
    LOG_VART(neighbor2);
    LOG_VART(em->contains(neighbor2));
    LOG_VART(_details->isStringCandidate(em->getString2()->getLastEdge(), neighbor2));

    // Calling this non-const function can alter the state of the map, and lead to different
    // behaviors at different log levels! Beware!
    //LOG_VART(_details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getLastEdge()));

    // if the neighbor pair score is non-zero
    if (em->contains(neighbor2) == false &&
        _details->isStringCandidate(em->getString2()->getLastEdge(), neighbor2) &&
        _details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getLastEdge()) > 0)
    {
      EdgeMatchPtr next = _details->extendEdgeMatch(em, em->getString1()->getLastEdge(), neighbor2);
      if (next)
        foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  LOG_VART(foundSolution);
  return foundSolution;
}

bool EdgeMatchSetFinder::_addEdgeNeighborsToStart(ConstEdgeMatchPtr em,
                                                  QSet<ConstNetworkEdgePtr> neighbors1Set,
                                                  QSet<ConstNetworkEdgePtr> neighbors2Set)
{
  LOG_TRACE("Adding edge neighbors to start...");

  bool foundSolution = false;

  // score all the e1 neighbors.
  for (const auto& neighbor1 : qAsConst(neighbors1Set))
  {
    LOG_VART(neighbor1);
    LOG_VART(em->getString2()->getFirstEdge());

    // Calling this non-const function can alter the state of the map, and lead to different
    // behaviors at different log levels! Beware!
    //LOG_VART(_details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getFirstEdge()));

    // if the neighbor pair score is non-zero
    if (em->contains(neighbor1) == false &&
        _details->isStringCandidate(em->getString1()->getFirstEdge(), neighbor1) &&
        _details->getPartialEdgeMatchScore(neighbor1, em->getString2()->getFirstEdge()) > 0)
    {
      EdgeMatchPtr next = _details->extendEdgeMatch(em, neighbor1, em->getString2()->getFirstEdge());
      if (next)
        foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  // score all the e2 neighbors.
  for (const auto& neighbor2 : qAsConst(neighbors2Set))
  {
    LOG_VART(neighbor2);
    LOG_VART(em->getString1()->getFirstEdge());

    // Calling this non-const function can alter the state of the map, and lead to different
    // behaviors at different log levels! Beware!
    //LOG_VART(_details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getFirstEdge()));

    // if the neighbor pair score is non-zero
    if (em->contains(neighbor2) == false &&
        _details->isStringCandidate(em->getString2()->getFirstEdge(), neighbor2) &&
        _details->getPartialEdgeMatchScore(neighbor2, em->getString1()->getFirstEdge()) > 0)
    {
      EdgeMatchPtr next = _details->extendEdgeMatch(em, em->getString1()->getFirstEdge(), neighbor2);
      if (next)
        foundSolution = _addEdgeMatches(next) || foundSolution;
    }
  }

  LOG_VART(foundSolution);
  return foundSolution;
}

QSet<ConstNetworkEdgePtr> EdgeMatchSetFinder::_getEdgesFromLocation(ConstEdgeLocationPtr l) const
{
  QSet<ConstNetworkEdgePtr> result;

  LOG_VART(l);
  LOG_VART(l->isExtreme(EdgeLocation::SLOPPY_EPSILON));
  LOG_VART(QString("%1").arg(l->getPortion(), 0, 'g', 18));

  if (l->isExtreme(EdgeLocation::SLOPPY_EPSILON))
  {
    ConstNetworkVertexPtr v = l->getVertex(EdgeLocation::SLOPPY_EPSILON);
    if (_n1->contains(v))
      result = _n1->getEdgesFromVertex(v).toSet();
    else if (_n2->contains(v))
      result = _n2->getEdgesFromVertex(v).toSet();
    else
      throw IllegalArgumentException("Vertex not contained by a network.");
  }

  LOG_VART(result);
  return result;
}

bool EdgeMatchSetFinder::_hasConfidentTiePoint(ConstEdgeLocationPtr l) const
{
  bool result;
  if (l->isExtreme(EdgeLocation::SLOPPY_EPSILON))
    result = _details->hasConfidentTiePoint(l->getVertex(EdgeLocation::SLOPPY_EPSILON));
  else
    result = false;
  return result;
}

bool EdgeMatchSetFinder::_isCandidateMatch(ConstEdgeLocationPtr l1, ConstEdgeLocationPtr l2) const
{
  bool result;
  if (l1->isExtreme() && l2->isExtreme())
    result = _details->isCandidateMatch(l1->getVertex(), l2->getVertex());
  else
    result = false;
  return result;
}

bool EdgeMatchSetFinder::_recordMatch(ConstEdgeMatchPtr em)
{
  LOG_TRACE("Recording match: " << em);

  const double score = _scoreMatch(em);
  LOG_VART(score);
  if (score > 0)
  {
    // We want to avoid adding matches that are very similar to ones we've already found and have
    // lower score than they do (see EdgeMatch::isVerySimilarTo).  Otherwise we'll have to remove
    // them before calculating match relationships, which can be very expensive.

    QMap<QString, QString> similarityValuesToIterate;
    similarityValuesToIterate[EDGE_MATCH_SIMILAR_KEY] = em->getSimilarityString();
    similarityValuesToIterate[EDGE_MATCH_SIMILAR_SECOND_REVERSED_KEY] = em->getSecondReversedSimilarityString();
    similarityValuesToIterate[EDGE_MATCH_SIMILAR_FIRST_REVERSED_KEY] = em->getFirstReversedSimilarityString();
    EdgeMatchScore existingSimilarMatch;
    // This will iterate over the similarity indexes in the order we want.
    for (auto it = similarityValuesToIterate.cbegin(); it != similarityValuesToIterate.cend(); ++it)
    {
      existingSimilarMatch = _edgeMatchSimilarities[it.key()][it.value()];
      if (existingSimilarMatch.score != -1.0)
        break;
    }

    // An EdgeMatchScore returned with a score == -1.0 means that no similar match was found.
    if (existingSimilarMatch.score != -1.0)
    {
      // If we already have an edge with a higher score that is very similar to this edge, then
      // don't add it.
      if (existingSimilarMatch.score >= score)
      {
        LOG_TRACE(
          "Found edge match: " << em << " similar to existing match: " <<
          existingSimilarMatch.match << " and with lower score: " << score <<
          ".  Skipping match...");
        _numSimilarEdgeMatches++;
        // Returning success here is slightly counterintuitive but is the correct thing to do for
        // the match alg.
        return true;
      }
      // Otherwise, remove the existing match, since the new one has a higher score.
      else
      {
        LOG_TRACE(
          "Removing similar edge match: " << existingSimilarMatch.match << " with lower score: " <<
          score << " than found edge match: " << em << "...");
        _matchSet->removeEdgeMatch(existingSimilarMatch.match);
      }
    }

    // similarity index our new match (overwrites the index of any existing matches)
    EdgeMatchScore newMatch;
    newMatch.match = em;
    newMatch.score = score;
    for (auto it = similarityValuesToIterate.cbegin(); it != similarityValuesToIterate.cend(); ++it)
      _edgeMatchSimilarities[it.key()][it.value()] = newMatch;

    // add our new match

    // if exactly one string is a stub
    if (em->getString1()->isStub() != em->getString2()->isStub())
    {
      _matchSet->addEdgeMatch(em, score);
      // add it in both directions. In some matchers we don't know which is better.
      if (_bidirectionalStubs)
        _addReverseMatch(em, score);
    }
    else
      _matchSet->addEdgeMatch(em, score);

    return true;
  }

  return false;
}

void EdgeMatchSetFinder::_addReverseMatch(ConstEdgeMatchPtr edgeMatch, const double score) const
{
  EdgeStringPtr rev1 = edgeMatch->getString1()->clone();
  rev1->reverse();
  EdgeMatchPtr em2 = std::make_shared<EdgeMatch>(rev1, edgeMatch->getString2());
  _matchSet->addEdgeMatch(em2, score);
}

double EdgeMatchSetFinder::_scoreMatch(ConstEdgeMatchPtr em) const
{
  LOG_TRACE("Scoring match...");
  return _details->getEdgeStringMatchScore(em->getString1(), em->getString2());
}

}
