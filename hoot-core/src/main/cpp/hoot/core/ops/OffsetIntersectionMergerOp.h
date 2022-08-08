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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef OFFSET_INTERSECTION_MERGER_OP_H
#define OFFSET_INTERSECTION_MERGER_OP_H

// Hoot
#include <hoot/core/ops/OsmMapOperation.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class OsmMap;

/**
 * Locates intersections of a very specific type where each endpoint of a small way
 * is an intersection of two other ways each that should really be just one
 * intersection. See below:
 *    \ /
 *     |
 *    / \
 * This type of intersection should have the way deleted and the two nodes merged.
 */
class OffsetIntersectionMergerOp : public OsmMapOperation
{
public:

  static QString className() { return "OffsetIntersectionMergerOp"; }

  OffsetIntersectionMergerOp();
  ~OffsetIntersectionMergerOp() override = default;

  void apply(std::shared_ptr<OsmMap>& map) override;

  static void mergeOffsetIntersections(std::shared_ptr<OsmMap> map);

  QString getInitStatusMessage() const override
  { return "Merging offset intersections..."; }
  QString getCompletedStatusMessage() const override
  { return "Merged " + QString::number(_numAffected) + " offset intersections"; }

  QString getDescription() const override
  { return "Merges road intersections that are offset into one intersection"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  double _offsetMax;

  void _mergeOffsetIntersection(long node1, long node2);
};

}


#endif // OFFSET_INTERSECTION_MERGER_OP_H
