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
#include "AddHilbertReviewSortOrderOp.h"

// hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/CalculateMapBoundsVisitor.h>

// Tgs
#include <tgs/RStarTree/HilbertCurve.h>

using namespace geos::geom;
using namespace std;

namespace hoot
{

int AddHilbertReviewSortOrderOp::logWarnCount = 0;

HOOT_FACTORY_REGISTER(OsmMapOperation, AddHilbertReviewSortOrderOp)

bool reviewLess(const pair<ElementId, int64_t>& p1, const pair<ElementId, int64_t>& p2)
{
  if (p1.second < p2.second)
  {
    return true;
  }
  else if (p1.second == p2.second)
  {
    return p1.first < p2.first;
  }
  else
  {
    return false;
  }
}

void AddHilbertReviewSortOrderOp::apply(OsmMapPtr& map)
{
  if (!ConfigOptions().getWriterIncludeConflateReviewDetailTags())
  {
    LOG_DEBUG(
      "AddHilbertReviewSortOrderOp disabled due to " <<
      ConfigOptions::getWriterIncludeConflateReviewDetailTagsKey() << "=false.");
    return;
  }

  _numAffected = 0;
  _mapEnvelope.reset();
  MapProjector::projectToPlanar(map);

  const RelationMap& relations = map->getRelations();

  vector<pair<ElementId, int64_t>> reviewOrder;
  // reserves at least as much as we need
  reviewOrder.reserve(relations.size());

  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr r = it->second;
    LOG_VART(r->getElementId());
    if (ReviewMarker::isReviewUid(map, r->getElementId()))
    {
      const set<ElementId> eids = ReviewMarker::getReviewElements(map, r->getElementId());
      LOG_VART(eids.size());
      LOG_VART(eids);
      if (!eids.empty())
      {
        int64_t hv = _calculateHilbertValue(map, eids);
        if (hv != -1)
        {
          pair<ElementId, int64_t> p(r->getElementId(), hv);
          reviewOrder.push_back(p);
        }
      }
      else
      {
        // don't think this really needs to be an exceptional situation
//        throw HootException(
//          "No review elements returned for relation with ID: " + r->getElementId().toString());
        LOG_WARN("No review elements returned for relation with ID: " << r->getElementId());
        if (logWarnCount < Log::getWarnMessageLimit())
        {
          LOG_WARN("No review elements returned for relation with ID: " << r->getElementId());
        }
        else if (logWarnCount == Log::getWarnMessageLimit())
        {
          LOG_WARN(className() << ": " << Log::LOG_WARN_LIMIT_REACHED_MESSAGE);
        }
        logWarnCount++;
      }
    }
  }

  sort(reviewOrder.begin(), reviewOrder.end(), reviewLess);

  for (size_t i = 0; i < reviewOrder.size(); ++i)
  {
    RelationPtr r = map->getRelation(reviewOrder[i].first.getId());

    r->getTags().set(MetadataTags::HootReviewSortOrder(), (long)i);
    _numAffected++;
  }
}

int64_t AddHilbertReviewSortOrderOp::_calculateHilbertValue(
  const ConstOsmMapPtr& map, const set<ElementId> eids)
{
  std::shared_ptr<Envelope> env;
  for (set<ElementId>::const_iterator it = eids.begin(); it != eids.end(); ++it)
  {
    ConstElementPtr element = map->getElement(*it);
    if (element)
    {
      std::unique_ptr<Envelope> te(element->getEnvelope(map));
      LOG_VART(env.get());
      if (env.get() == 0)
      {
        env.reset(new Envelope(*te));
      }
      else
      {
        env->expandToInclude(te.get());
      }
    }
  }
  if (!env)
  {
    return -1;
  }
  LOG_VART(env->toString());

  if (_mapEnvelope.get() == 0)
  {
    _mapEnvelope.reset(new Envelope(CalculateMapBoundsVisitor::getGeosBounds(map)));
  }

  Coordinate center;
  env->centre(center);

  Meters cellSize = 10.0;
  int xorder = max(1.0, ceil(log(_mapEnvelope->getWidth() / cellSize) / log(2.0)));
  int yorder = max(1.0, ceil(log(_mapEnvelope->getHeight() / cellSize) / log(2.0)));

  // 31 bits is the most supported for 2 dimensions.
  int order = min(31, max(xorder, yorder));
  // always 2 dimensions
  Tgs::HilbertCurve c(2, order);
  int64_t maxRange = 1 << order;
  int point[2];

  point[0] = max<int64_t>(0, min<int64_t>(maxRange - 1,
    round((center.x - _mapEnvelope->getMinX()) / cellSize)));
  point[1] = max<int64_t>(0, min<int64_t>(maxRange - 1,
    round((center.y - _mapEnvelope->getMinY()) / cellSize)));

  // Pad with zeros to make sorting a little easier.
  return c.encode(point);
}

}
