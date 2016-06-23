#include "LegacyVertexMatcher.h"

// hoot
#include <hoot/core/conflate/NodeMatcher.h>
#include <hoot/core/conflate/polygon/extractors/EuclideanDistanceExtractor.h>

// tgs
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/MemoryPageStore.h>

namespace hoot
{

using namespace Tgs;

LegacyVertexMatcher::LegacyVertexMatcher(ConstOsmMapPtr map) :
  _map(map)
{
  _confidentThreshold = 0.5;
}

void LegacyVertexMatcher::_balanceVertexScores()
{
  foreach (ConstNetworkVertexPtr v1, _scores1.keys())
  {
    QList<TiePointScorePtr> ties1 = _scores1.value(v1);
    for (int i = 0; i < ties1.size(); ++i)
    {
      _finalScores[v1][ties1[i]->v2] = ties1[i]->rawScore / _denominatorForTie(ties1[i]);

//      if (v1->getElementId().getId() == -9 && (ties1[i]->v2->getElementId().getId() == -19 ||
//        ties1[i]->v2->getElementId().getId() == -22))
//      {
//        LOG_VAR(ties1[i]->rawScore);
//        LOG_VAR(_denominatorForTie(ties1[i]));
//        LOG_VAR(_finalScores[v1][ties1[i]->v2]);
//      }
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
        LOG_INFO("high conf: " << v1->getElementId().getId() << ", " <<
          ties1[i]->v2->getElementId().getId() << " " << score);
      }
    }
  }
}

IntersectionIterator LegacyVertexMatcher::_createIterator(Envelope env)
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
  SearchRadiusProvider& srp)
{
  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children = 368 bytes
  shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _vertex2Index.reset(new HilbertRTree(mps, 2));

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

double LegacyVertexMatcher::_denominatorForTie(TiePointScorePtr tie)
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

void LegacyVertexMatcher::identifyVertexMatches(ConstOsmNetworkPtr n1, ConstOsmNetworkPtr n2,
  SearchRadiusProvider& srp)
{
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
        TiePointScorePtr tps(new TiePointScore(v1, v2, score));
        // calculate the vertex score and store it
        _scores1[v1].append(tps);
        _scores2[v2].append(tps);
      }
    }
  }

  // balance the vertex scores by considering neighboring scores.
  _balanceVertexScores();
}

bool LegacyVertexMatcher::isCandidateMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2,
  SearchRadiusProvider& srp)
{
  bool result = false;
  double score = scoreMatch(v1, v2);
  if (v1->getElementId().getId() == -9)
  {
    LOG_VAR(v1);
    LOG_VAR(v2);
    LOG_VAR(score);
  }

  if (score > 0)
  {
    result = true;
  }
  // if this isn't a tie point and the intersections aren't part of any confident tie points
  else if (score == 0.0 &&
    _hasConfidentTie.contains(v1) == false &&
    _hasConfidentTie.contains(v2) == false)
  {
    // if these aren't technically intersections they might be tie points.
    if (_nodeMatcher->getDegree(v1->getElementId()) <= 2 ||
      _nodeMatcher->getDegree(v2->getElementId()) <= 2)
    {
      Meters sr = srp.getSearchRadius(v1, v2);

      double d = EuclideanDistanceExtractor().distance(*_map, v1->getElement(), v2->getElement());

      result = d <= sr;
    }
  }

  return result;
}

bool LegacyVertexMatcher::isConfidentTiePoint(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  return scoreMatch(v1, v2) > _confidentThreshold;
}

double LegacyVertexMatcher::_scoreSinglePair(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  if (!_nodeMatcher)
  {
    _nodeMatcher.reset(new NodeMatcher());
    _nodeMatcher->setMap(_map);
  }

  return _nodeMatcher->scorePair(v1->getElementId().getId(), v2->getElementId().getId());
}

double LegacyVertexMatcher::scoreMatch(ConstNetworkVertexPtr v1, ConstNetworkVertexPtr v2)
{
  return _finalScores[v1][v2];
}

}
