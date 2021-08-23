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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MatchGraph.h"

#include <hoot/core/HootConfig.h>

// Boost
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#if HOOT_HAVE_BOOST_PROPERTY_MAP_PROPERTY_MAP_HPP
# include <boost/property_map/property_map.hpp>
#else
# error "Boost properties include not found during configure."
#endif
#ifdef BOOST_GRAPH_NO_BUNDLED_PROPERTIES
#error Bundle support is required.
#endif

// hoot
#include <hoot/core/conflate/matching/Match.h>
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/conflate/merging/MergerFactory.h>
#include <hoot/core/util/HootException.h>


// Qt


// Tgs
#include <tgs/DisjointSet/DisjointSetMap.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

class MatchEdge
{
public:

  enum MatchType
  {
    AssociatedWith,
    MatchWith,
    InvalidMatch
  };

  MatchEdge() : match(nullptr), type(InvalidMatch) { }
  MatchEdge(ConstMatchPtr m, MatchType t) : match(m), type(t) { }

  ConstMatchPtr match;
  MatchType type;
};

class MatchVertex
{
public:

  MatchVertex() = default;
  MatchVertex(ElementId e) : eid(e) { }

  ElementId eid;
};

using MatchBoostGraph = boost::adjacency_list<
  // Use listS for storing VertexList -- faster, but not as space efficient (no biggie)
  boost::listS,
  // use vecS for storing OutEdgeList -- faster for traversal, but slower to build (no biggie)
  boost::vecS,
  // Our graph is undirected
  boost::undirectedS,
  MatchVertex,
  MatchEdge>;

using MatchVertexId = boost::graph_traits<MatchBoostGraph>::vertex_descriptor;
using MatchEdgeId = boost::graph_traits<MatchBoostGraph>::edge_descriptor;

class MatchGraphInternal
{
public:

  MatchGraphInternal(const vector<ConstMatchPtr>& matches) : _matches(matches) { }

  /**
   * Only return true for edges that are matches and meet the specified threshold.
   */
  class MatchThresholdFilter
  {
  public:

    MatchThresholdFilter() :
      _graph(nullptr),
      _threshold(-1) { }

    MatchThresholdFilter(MatchBoostGraph& graph, double threshold) :
      _graph(&graph),
      _threshold(threshold) { }

    bool operator()(const MatchEdgeId& edgeId) const
    {
      MatchEdge::MatchType type = (*_graph)[edgeId].type;
      ConstMatchPtr m = (*_graph)[edgeId].match;
      return (type == MatchEdge::MatchWith && m->getProbability() >= _threshold);
    }

  private:

    MatchBoostGraph* _graph;
    double _threshold;
  };

  MatchVertexId createOrGetVertex(const ElementId& eid)
  {
    if (_eid2Vertex.contains(eid))
    {
      return _eid2Vertex[eid];
    }
    else
    {
      MatchVertex v(eid);

      MatchVertexId vid = _addVertex(v);

      return vid;
    }
  }

  MatchSetVector findSubgraphs(const ConstOsmMapPtr& map, bool checkForConflicts) const
  {
    DisjointSetMap<ElementId> dsm;
    MatchSetVector result;

    for (size_t i = 0; i < _matches.size(); i++)
    {
      ConstMatchPtr m = _matches[i];
      MatchType type = m->getType();
      // if this is a solid match, then add it into the group.
      if (type == MatchType::Match)
      {
        set<pair<ElementId, ElementId>> eids = m->getMatchPairs();
        for (set<pair<ElementId, ElementId>>::const_iterator it = eids.begin();
             it != eids.end(); ++it)
        {
          dsm.joinT(it->first, it->second);
        }
      }
      // if this is a match that requires review.
      else if (type == MatchType::Review)
      {
        result.emplace_back();
        MatchSet& matches = result.back();
        matches.insert(m);
      }
    }

    DisjointSetMap<ElementId>::AllGroups ag = dsm.getAllGroups();

    const QHash<QString, ConstMatchPtr> idIndexedMatches = Match::getIdIndexedMatches(_matches);

    result.reserve(ag.size());
    for (DisjointSetMap<ElementId>::AllGroups::const_iterator it = ag.begin(); it != ag.end(); ++it)
    {
      const vector<ElementId>& v = it->second;
      result.emplace_back();
      MatchSet& matches = result.back();

      for (size_t i = 0; i < v.size(); i++)
      {
        // find all the matches that reference this element id and add them to the set.
        _findMatches(v[i], matches);
      }

      // while this is O(n^2) matches should generally be pretty small.
      for (MatchSet::const_iterator m1_it = matches.begin(); m1_it != matches.end(); ++m1_it)
      {
        ConstMatchPtr m1 = *m1_it;
        for (MatchSet::const_iterator m2_it = matches.begin(); m2_it != matches.end(); ++m2_it)
        {
          ConstMatchPtr m2 = *m2_it;
          if (m1 != m2 && checkForConflicts &&
              MergerFactory::getInstance().isConflicting(map, m1, m2, idIndexedMatches))
          {
            LOG_INFO(m1->toString());
            LOG_INFO(m2->toString());
            throw InternalErrorException("Found an unexpected conflicting match.");
          }
        }
      }
    }

    return result;
  }

  bool isValid() const
  {
    return _eid2Vertex.size() == (int)_matches.size();
  }

  void build()
  {
    if (isValid() == false)
    {
      for (size_t i = 0; i < _matches.size(); i++)
      {
        ConstMatchPtr m = _matches[i];
        _addMatchWith(m);
      }
    }
  }

  void reset()
  {
    _eid2Vertex.clear();
    _graph.clear();
  }

private:

  MatchBoostGraph _graph;
  QHash<ElementId, MatchVertexId> _eid2Vertex;
  const vector<ConstMatchPtr>& _matches;

  void _addMatchWith(ConstMatchPtr m)
  {
    MatchEdge matchWith(m, MatchEdge::MatchWith);

    // add an edge for each of the match pairs (typically just one match pair)
    set<pair<ElementId, ElementId>> eids = m->getMatchPairs();
    for (set<pair<ElementId, ElementId>>::const_iterator it = eids.begin(); it != eids.end();
      ++it)
    {
      MatchVertexId vid1 = createOrGetVertex(it->first);
      MatchVertexId vid2 = createOrGetVertex(it->second);
      add_edge(vid1, vid2, matchWith, _graph);
    }
  }

  MatchVertexId _addVertex(const MatchVertex& v)
  {
    MatchVertexId vid = add_vertex(v, _graph);

    _eid2Vertex[v.eid] = vid;

    return vid;
  }

  void _findMatches(ElementId eid, MatchSet& matches) const
  {
    MatchVertexId mvi = _eid2Vertex[eid];

    boost::graph_traits<MatchBoostGraph>::out_edge_iterator ei, eend;
    for (boost::tie(ei, eend) = out_edges(mvi, _graph); ei != eend; ++ei)
    {
      const MatchEdge& edge = _graph[*ei];
      ConstMatchPtr match = edge.match;
      if (match->getType() == MatchType::Match)
      {
        matches.insert(match);
      }
    }
  }

};

MatchGraph::MatchGraph()
{
  _checkForConflicts = true;
  // The two classes share the matches vector.
  _d = std::make_shared<MatchGraphInternal>(_matches);
}

void MatchGraph::_resetGraph() const
{
  _d->reset();
  _d->build();
}

MatchSetVector MatchGraph::findSubgraphs(const ConstOsmMapPtr& map) const
{
  return _d->findSubgraphs(map, _checkForConflicts);
}

}
