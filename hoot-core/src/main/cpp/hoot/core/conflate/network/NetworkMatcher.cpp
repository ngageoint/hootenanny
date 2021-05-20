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
#include "NetworkMatcher.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/core/util/StringUtils.h>

// Tgs
#include <tgs/RStarTree/HilbertRTree.h>
#include <tgs/RStarTree/MemoryPageStore.h>

using namespace geos::geom;
using namespace std;
using namespace Tgs;

namespace hoot
{

const double NetworkMatcher::EPSILON = 1e-6;

void NetworkMatcher::_createEdge2Index()
{
  LOG_DEBUG("Creating edge index...");

  // No tuning was done, I just copied these settings from OsmMapIndex. 10 children = 368 bytes -
  // see #3054
  std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _edge2Index.reset(new HilbertRTree(mps, 2));

  std::vector<Box> boxes;
  std::vector<int> fids;

  const OsmNetwork::EdgeMap& em = _n2->getEdgeMap();
  int ctr = 0;
  for (OsmNetwork::EdgeMap::const_iterator it = em.begin(); it != em.end(); ++it)
  {
    fids.push_back((int)_index2Edge.size());
    _index2Edge.push_back(it.value());

    Box b(2);
    Meters searchRadius = _details->getSearchRadius(it.value());
    Envelope env(_details->getEnvelope(it.value()));
    env.expandBy(searchRadius);

    b.setBounds(0, env.getMinX(), env.getMaxX());
    b.setBounds(1, env.getMinY(), env.getMaxY());

    boxes.push_back(b);

    ctr++;
    if (ctr % 10 == 0)
    {
      PROGRESS_INFO(
        "Created " << StringUtils::formatLargeNumber(ctr) << " of " <<
        StringUtils::formatLargeNumber(em.size()) << " edge mappings.");
    }
  }

  _edge2Index->bulkInsert(boxes, fids);
}

IntersectionIterator NetworkMatcher::_createIterator(const Envelope& env, HilbertRTreePtr tree) const
{
  vector<double> min(2), max(2);
  min[0] = env.getMinX();
  min[1] = env.getMinY();
  max[0] = env.getMaxX();
  max[1] = env.getMaxY();
  IntersectionIterator it(tree.get(), min, max);

  return it;
}

void NetworkMatcher::_createVertex2Index()
{
  // No tuning was done, I just copied these settings from OsmMapIndex.
  // 10 children = 368 bytes - see #3054
  std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore(728));
  _vertex2Index.reset(new HilbertRTree(mps, 2));

  std::vector<Box> boxes;
  std::vector<int> fids;

  const OsmNetwork::VertexMap& vm = _n2->getVertexMap();
  for (OsmNetwork::VertexMap::const_iterator it = vm.begin(); it != vm.end(); ++it)
  {
    fids.push_back((int)_index2Vertex.size());
    _index2Vertex.push_back(it.value());

    Box b(2);
    Meters searchRadius = _details->getSearchRadius(it.value());
    Envelope env(_details->getEnvelope(it.value()));
    env.expandBy(searchRadius);

    b.setBounds(0, env.getMinX(), env.getMaxX());
    b.setBounds(1, env.getMinY(), env.getMaxY());

    boxes.push_back(b);
  }

  _vertex2Index->bulkInsert(boxes, fids);
}

}
