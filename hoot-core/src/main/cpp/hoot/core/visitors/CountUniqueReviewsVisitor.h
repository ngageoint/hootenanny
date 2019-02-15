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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COUNTUNIQUEREVIEWSVISITOR_H
#define COUNTUNIQUEREVIEWSVISITOR_H

// hoot
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/visitors/SingleStatistic.h>

namespace hoot
{

class OsmMap;

/**
 * Sums the length of all the ways. The map projection is used so to get meters the map must be
 * first reprojected into meters.
 */
class CountUniqueReviewsVisitor : public ElementConstOsmMapVisitor, public SingleStatistic
{

public:

  static std::string className() { return "hoot::CountUniqueReviewsVisitor"; }

  CountUniqueReviewsVisitor() {}

  double getStat() const { return _reviews.size(); }

  virtual void visit(const ConstElementPtr& e) override;

  virtual QString getDescription() const { return "Counts the number of unique feature reviews"; }

private:

  std::set<ReviewMarker::ReviewUid> _reviews;
  ReviewMarker _reviewMarker;
};

}
#endif // COUNTUNIQUEREVIEWSVISITOR_H
