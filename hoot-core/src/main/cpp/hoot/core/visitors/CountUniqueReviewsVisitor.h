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
#ifndef COUNTUNIQUEREVIEWSVISITOR_H
#define COUNTUNIQUEREVIEWSVISITOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/ConstOsmMapConsumer.h>
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/elements/ElementVisitor.h>

#include "SingleStatistic.h"

namespace hoot
{
using namespace std;

/**
 * Sums the length of all the ways. The map projection is used so to get meters the map must be
 * first reprojected into meters.
 */
class CountUniqueReviewsVisitor : public ElementVisitor, public ConstOsmMapConsumer, public SingleStatistic
{
public:

  static string className() { return "hoot::CountUniqueReviewsVisitor"; }

  CountUniqueReviewsVisitor() {}

  virtual ~CountUniqueReviewsVisitor() {}

  double getStat() const { return _reviews.size(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  virtual void visit(const ConstElementPtr& e)
  {
    set<ReviewMarker::ReviewUid> reviews = ReviewMarker().getReviewUids(_map->shared_from_this(),
      e);

    _reviews.insert(reviews.begin(), reviews.end());
  }

private:
  const OsmMap* _map;
  set<ReviewMarker::ReviewUid> _reviews;
};

}
#endif // COUNTUNIQUEREVIEWSVISITOR_H
