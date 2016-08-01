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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "AddHilbertReviewSortOrderOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/visitors/CalculateBoundsVisitor.h>

// Tgs
#include <tgs/RStarTree/HilbertCurve.h>

namespace hoot
{

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

AddHilbertReviewSortOrderOp::AddHilbertReviewSortOrderOp()
{
}

void AddHilbertReviewSortOrderOp::apply(shared_ptr<OsmMap>& map)
{
  _mapEnvelope.reset();
  MapProjector::projectToPlanar(map);

  const RelationMap& relations = map->getRelationMap();

  Envelope e = CalculateBoundsVisitor::getGeosBounds(map);


  vector< pair<ElementId, int64_t> > reviewOrder;
  // reserves at least as much as we need.
  reviewOrder.reserve(relations.size());

  for (RelationMap::const_iterator it = relations.begin(); it != relations.end(); ++it)
  {
    RelationPtr r = it->second;
    if (ReviewMarker::isReviewUid(map, r->getElementId()))
    {
      const set<ElementId> eids = ReviewMarker::getReviewElements(map, r->getElementId());
      //LOG_VARD(eids.size());
      if (eids.size() > 0)
      {
        int64_t hv = _calculateHilbertValue(map, eids);

        pair<ElementId, int64_t> p(r->getElementId(), hv);
        reviewOrder.push_back(p);
      }
      else
      {
        throw HootException(
          "No review elements returned for relation with ID: " + r->getElementId().toString());
      }
    }
  }

  sort(reviewOrder.begin(), reviewOrder.end(), reviewLess);

  for (size_t i = 0; i < reviewOrder.size(); ++i)
  {
    RelationPtr r = map->getRelation(reviewOrder[i].first.getId());

    r->getTags().set(ReviewMarker::reviewSortOrderKey, (long)i);
  }
}

int64_t AddHilbertReviewSortOrderOp::_calculateHilbertValue(const ConstOsmMapPtr &map,
  const set<ElementId> eids)
{
  auto_ptr<Envelope> env;
  for (set<ElementId>::const_iterator it = eids.begin(); it != eids.end(); ++it)
  {
    Envelope::AutoPtr te(map->getElement(*it)->getEnvelope(map));
    if (env.get() == 0)
    {
      env.reset(new Envelope(*te));
    }
    else
    {
      env->expandToInclude(te.get());
    }
  }
  //LOG_VARD(env->toString());

  if (_mapEnvelope.get() == 0)
  {
    _mapEnvelope.reset(new Envelope(CalculateBoundsVisitor::getGeosBounds(map)));
  }

  Coordinate center;
  env->centre(center);

  Meters cellSize = 10.0;
  int xorder = max(1.0, ceil(log(_mapEnvelope->getWidth() / cellSize) / log(2.0)));
  int yorder = max(1.0, ceil(log(_mapEnvelope->getHeight() / cellSize) / log(2.0)));

  // 31 bits is the most supported for 2 dimensions.
  int order = min(31, max(xorder, yorder));
  // always 2 dimensions.
  Tgs::HilbertCurve c(2, order);
  int64_t maxRange = 1 << order;
  int point[2];

  point[0] = max<int64_t>(0, min<int64_t>(maxRange - 1,
    round((center.x - _mapEnvelope->getMinX()) / cellSize)));
  point[1] = max<int64_t>(0, min<int64_t>(maxRange - 1,
    round((center.y - _mapEnvelope->getMinY()) / cellSize)));

  // pad with zeros to make sorting a little easier.
  return c.encode(point);
}


}
