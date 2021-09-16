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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __ADD_HILBERT_REVIEW_SORT_ORDER_H__
#define __ADD_HILBERT_REVIEW_SORT_ORDER_H__

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/StringUtils.h>

namespace hoot
{

class AddHilbertReviewSortOrderOp : public OsmMapOperation
{
public:

  static QString className() { return "AddHilbertReviewSortOrderOp"; }

  static int logWarnCount;

  AddHilbertReviewSortOrderOp() = default;
  ~AddHilbertReviewSortOrderOp() override = default;

  void apply(OsmMapPtr& map) override;

  QString getInitStatusMessage() const override
  { return "Adding geospatial sorting tags to review relations..."; }
  QString getCompletedStatusMessage() const override
  { return "Added " + StringUtils::formatLargeNumber(_numAffected) + " sorting tags"; }

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Adds tags that enable sorting reviewable features geospatially"; }

private:

  std::shared_ptr<geos::geom::Envelope> _mapEnvelope;

  int64_t _calculateHilbertValue(const ConstOsmMapPtr &map, const std::set<ElementId>& eids);
};

}

#endif // __ADD_HILBERT_REVIEW_SORT_ORDER_H__
