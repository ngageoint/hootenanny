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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYREVIEWCLEANEROP_H
#define HIGHWAYREVIEWCLEANEROP_H

// hoot
#include <hoot/core/conflate/review/ReviewMarker.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/info/OperationStatusInfo.h>

namespace hoot
{

class OsmMap;

/**
 * This class will go through all highway reviews and look for reviews that are impossible due to
 * a lack of string match. If the review is impossible it will be deleted. This is handy due to the
 * way multilinestrings are generated during the conflation process.
 */
class HighwayReviewCleanerOp : public OsmMapOperation, public OperationStatusInfo
{
public:

  static std::string className() { return "hoot::HighwayReviewCleanerOp"; }

  HighwayReviewCleanerOp();

  virtual void apply(OsmMapPtr& map);

  virtual QString getDescription() const { return "Identifies invalid road reviews"; }

  virtual QString getInitStatusMessage() const
  { return "Identifying invalid road reviews..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Identified " + QString::number(_numAffected) + " invalid road reviews"; }

private:

  bool _isBadHighwayReview(OsmMapPtr& map, ReviewMarker::ReviewUid review);
};

}

#endif // HIGHWAYREVIEWCLEANEROP_H
