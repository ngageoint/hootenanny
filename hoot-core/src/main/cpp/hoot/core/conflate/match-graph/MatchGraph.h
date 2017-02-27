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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MATCHGRAPH_H
#define MATCHGRAPH_H

// Hoot
#include <hoot/core/conflate/MatchSet.h>

// Standard
#include <memory>
#include <set>
#include <vector>

namespace hoot
{
using namespace std;

class Match;
class MatchGraphInternal;
class MatchThreshold;

typedef vector< MatchSet > MatchSetVector;

/**
 * Represents a graph of matches. At some point I'd like to extend this to include additional
 * relationships (such as two roads are linked by an intersection). This will enable higher fidelity
 * matching. See #2376 for some musings and references.
 *
 * At this point this is useful for identifying many to many matches and one to many matches.
 */
class MatchGraph
{
public:
  MatchGraph();

  /**
   * Search the graph for subgraphs. The threshold value is used to determine links. If the
   * probability of a link is less than threshold then the link is disregarded. This is primarily
   * useful with one to many and many to many matches. Each match is guaranteed to only be in one
   * collection.
   *
   * @return Returns a collection of sets of Matches. The outer level set is the set of subgraphs.
   *    The inner set contains the matches in each subgraph.
   */
  MatchSetVector findSubgraphs(const ConstOsmMapPtr& map) const;

  /**
   * Given an iterator, add all the values that are in use by the iterator. STL style. The
   * iterator should contain pointers to Match. The matches will be treated as consts and
   * MatchGraph will not assume ownership. It is assumed the caller will maintain the Matches
   * through the lifecycle of MatchGraph.
   */
  template <typename InputIterator>
  void addMatches(InputIterator first, InputIterator last);

  /**
   * Defaults to true. If set to false then it will not check to make sure all subgraphs contain
   * no conflicts.
   */
  void setCheckForConflicts(bool c) { _checkForConflicts = c; }

private:
  vector<const Match*> _matches;
  bool _checkForConflicts;
  /**
   * An internal data structure to prevent users of the class from recompiling the boost graph
   * craziness.
   */
  auto_ptr<MatchGraphInternal> _d;

  /**
   * Invalidates the current graph.
   */
  void _resetGraph();
};


template <typename InputIterator>
void MatchGraph::addMatches(InputIterator first, InputIterator last)
{
  _matches.insert(_matches.begin(), first, last);
  _resetGraph();
}

}

#endif // MATCHGRAPH_H
