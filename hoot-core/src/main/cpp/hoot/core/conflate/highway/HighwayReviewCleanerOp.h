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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYREVIEWCLEANEROP_H
#define HIGHWAYREVIEWCLEANEROP_H

// hoot
#include <hoot/core/conflate/ReviewMarker.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

class OsmMap;

/**
 * This class will go through all highway reviews and look for reviews that are impossible due to
 * a lack of string match. If the review is impossible it will be deleted. This is handy due to the
 * way multilinestrings are generated during the conflation process.
 */
class HighwayReviewCleanerOp : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::HighwayReviewCleanerOp"; }

  HighwayReviewCleanerOp();

  virtual void apply(OsmMapPtr& map);

  virtual QString toString() const { return QString::fromStdString(className()); }

  virtual QString getDescription() const
  { return "Finds highway reviews that are impossible due to a lack of string match"; }

private:

  bool _isBadHighwayReview(OsmMapPtr& map, ReviewMarker::ReviewUid review);
};

}

#endif // HIGHWAYREVIEWCLEANEROP_H
