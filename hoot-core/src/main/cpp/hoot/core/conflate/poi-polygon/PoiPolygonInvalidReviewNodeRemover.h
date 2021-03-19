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
 * @copyright Copyright (C) 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef POI_POLYGON_INVALID_REVIEW_NODE_REMOVER_H
#define POI_POLYGON_INVALID_REVIEW_NODE_REMOVER_H

// Hoot
#include <hoot/core/ops/ConstOsmMapOperation.h>

// Standard
#include <set>

namespace hoot
{

/**
 * This removes any untagged nodes from POI/Polygon review relations. Its a band-aid on a POI/Poly
 * bug that should eventually be dealt with. The bug involves the fact that auto merging or the
 * keep closest matches only options don't work when an an element is involved in a both a match and
 * a review.
 *
 * See related notes in the disabled code for
 * PoiPolygonMergerCreator::convertSharedMatchesToReviews.
 */
class PoiPolygonInvalidReviewNodeRemover : public ConstOsmMapOperation
{
public:

  static QString className() { return "hoot::PoiPolygonInvalidReviewNodeRemover"; }

  PoiPolygonInvalidReviewNodeRemover();
  ~PoiPolygonInvalidReviewNodeRemover() = default;

  void apply(const std::shared_ptr<OsmMap>& map) override;

  QString getName() const override { return className(); }

  QString getClassName() const override { return className(); }

  QString getDescription() const override
  { return "Removes all reviewable POI/Polygon POIs and their reviews that are invalid"; }

  QString getInitStatusMessage() const override { return "Removing invalid POIs from reviews..."; }

  QString getCompletedStatusMessage() const override
  {
    return
      "Removed " + QString::number(_numAffected) + " invalid reviewable POIs and " +
      QString::number(_numRelationsRemoved) + " reviews";
  }

  /**
   * @see FilteredByGeometryTypeCriteria
   */
  QStringList getCriteria() const override;

protected:

  std::set<long> _nodesToRemove;
  std::set<long> _reviewRelationsToRemove;
  int _numRelationsRemoved;

  int _taskStatusUpdateInterval;
};

}

#endif // POI_POLYGON_INVALID_REVIEW_NODE_REMOVER_H
