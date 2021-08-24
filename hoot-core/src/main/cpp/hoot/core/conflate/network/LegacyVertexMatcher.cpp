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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#include "LegacyVertexMatcher.h"

// hoot
#include <hoot/core/algorithms/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/conflate/matching/NodeMatcher.h>
#include <hoot/core/conflate/network/SearchRadiusProvider.h>

// tgs
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

using namespace Tgs;

LegacyVertexMatcher::LegacyVertexMatcher(ConstOsmMapPtr map) :
  _map(map)
{
  _confidentThreshold = 0.7;
}

void LegacyVertexMatcher::_balanceVertexScores()
{
  foreach (ConstNetworkVertexPtr v1, _scores1.keys())
  {
    QList<TiePointScorePtr> ties1 = _scores1.value(v1);
    for (int i = 0; i < ties1.size(); ++i)
    {
      _finalScores[v1][ties1[i]->v2] = ties1[i]->rawScore / _denominatorForTie(ties1[i]);
    }
  }


  foreach (ConstNetworkVertexPtr v1, _scores1.keys())
  {
    QList<TiePointScorePtr> ties1 = _scores1.value(v1);
    for (int i = 0; i < ties1.size(); ++i)
    {
      // if we're confident then clear the scores on all neighboring ties.
      if (isConfidentTiePoint(v1, ties1[i]->v2))
      {
        double score = scoreMatch(v1, ties1[i]->v2);
        _hasConfidentTie.insert(v1);
        _hasConfidentTie.insert(ties1[i]->v2);
        _finalScores[v1].clear();
        _finalScores[v1][ties1[i]->v2] = score;
      }
    }
  }

  foreach (ConstNetworkVertexPtr v1, _finalScores.keys())
  {
    QMap<ConstNetworkVertexPtr, double> m = _finalScores[v1];
    foreach (ConstNetworkVertexPtr v2, m.keys())
    {
      if (m[v2] > 0.0)
      {
        _finalCandidates[v2].append(v1);
        _finalCandidates[v1].append(v2);
      }
    }
  }
}

IntersectionIterator LegacyVertexMatcher::_createIterator(const Envelope& env)  const
{
  vector<double> min(2), max(2);
  min[0] = env.getMinX();
  min[1] = env.getMinY();
  max[0] = env.getMaxX();
  max[1] = env.getMaxY();
  IntersectionIterator it(_vertex2Index.get(), min, max);

  return it;
}

void LegacyVertexMatcher::_createVertexIndex(const OsmNetwork::VertexMap& vm,
  const SearchRadiusProvider& srp)
{
  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children = 368 bytes
  _vertex2Index = std::make_shared<HilbertRTree>(std::make_shared<MemoryPageStore>(728), 2);

  std::vector<Box> boxes;
  std::vector<int> fids;

  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    // expand the indexed envelop by the search radius to make search radius intersections easy.
    Box b(2);
    Meters searchRadius = srp.getSearchRadius(it.value());
    Envelope env(srp.getEnvelope(it.value()));
    env.expandBy(searchRadius);

    b.setBounds(0, env.getMinX(), env.getMaxX());
    b.setBounds(1, env.getMinY(), env.getMaxY());

    boxes.push_back(b);

    fids.push_back(_vertex2Fid.size());
    _vertex2Fid.append(it.value());
  }

  _vertex2Index->bulkInsert(boxes, fids);
}

double LegacyVertexMatcher::_denominatorForTie(TiePointScorePtr tie) const
{
  QSet<TiePointScorePtr> ties;

  QList<TiePointScorePtr> tie1 = _scores1.value(tie->v1);
  for (int i = 0; i < tie1.size(); ++i)
  {
    ties.insert(tie1[i]);
  }

  QList<TiePointScorePtr> tie2 = _scores2.value(tie->v2);
  for (int i = 0; i < tie2.size(); ++i)
  {
    ties.insert(tie2[i]);
  }

  double sum = 0.0;
  foreach (TiePointScorePtr t, ties)
  {
    sum += t->rawScore;
  }

  return sum;
}

QList<ConstNetworkVertexPtr> LegacyVertexMatcher::getCandidateMatches(ConstNetworkVertexPtr v)
  const
{
  return _finalCandidates[v];
}

NodeMatcherPtr LegacyVertexMatcher::_getNodeMatcher()
{
  if (!_nodeMatcher)
  {
    _nodeMatcher = std::make_shared<NodeMatcher>();
    _nodeMatcher->setMap(_map);
  }

  return _nodeMatcher;
}

void LegacyVertexMatcher::identifyVertexMatches(
  ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2, const SearchRadiusProvider& srp)
{
  LOG_DEBUG("Identifying vertex matches...");

  _createVertexIndex(n2->getVertexMap(), srp);

  // go through all the vertices in n1
  const OsmNetwork::VertexMap& vm1 = n1->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm1.begin(); it != vm1.end(); ++it)
  {
    ConstNetworkVertexPtr v1 = it.value();
    // find the neighboring vertices in n2
    Envelope env = srp.getEnvelope(v1);
    env.expandBy(srp.getSearchRadius(v1));
    IntersectionIterator iit = _createIterator(env);

    while (iit.next())
    {
      ConstNetworkVertexPtr v2 = _vertex2Fid[iit.getId()];

      double score = _scoreSinglePair(v1, v2);
      if (score > 0)
      {
        TiePointScorePtr tps = std::make_shared<TiePointScore>(v1, v2, score);
        // calculate the vertex score and store it
        _scores1[v1].append(tps);
        _scores2[v2].append(tps);
      }
    }
  }

  // Balance the vertex scores by considering neighboring scores.
  _balanceVertexScores();
}

bool LegacyVertexMatcher::isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
  const SearchRadiusProvider& srp)
{
  bool result = false;
  double score = scoreMatch(v1, v2);

  if (score > 0)
  {
    result = true;
  }
  // If this isn't a tie point, the intersections aren't part of any confident tie points, and
  else if (score == 0.0 &&
           _hasConfidentTie.contains(v1) == false && _hasConfidentTie.contains(v2) == false &&
           // these aren't technically intersections, then they might be tie points.
           (_getNodeMatcher()->getDegree(v1->getElementId()) <= 2 ||
            _getNodeMatcher()->getDegree(v2->getElementId()) <= 2))
  {
    Meters sr = srp.getSearchRadius(v1, v2);
    double d = EuclideanDistanceExtractor().distance(*_map, v1->getElement(), v2->getElement());
    result = d <= sr;
  }

  return result;
}

bool LegacyVertexMatcher::isConfidentTiePoint(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  return scoreMatch(v1, v2) > _confidentThreshold;
}

double LegacyVertexMatcher::_scoreSinglePair(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  double score = _getNodeMatcher()->scorePair(v1->getElementId().getId(),
    v2->getElementId().getId());
  return score;
}

double LegacyVertexMatcher::scoreMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  return _finalScores[v1][v2];
}

}
