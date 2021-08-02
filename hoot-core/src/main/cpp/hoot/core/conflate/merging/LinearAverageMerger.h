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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LINEAR_AVERAGE_MERGER_H
#define LINEAR_AVERAGE_MERGER_H

// Hoot
#include <hoot/core/conflate/merging/LinearMergerAbstract.h>

namespace hoot
{

/**
 * Uses averaging to merge linear features
 */
class LinearAverageMerger : public LinearMergerAbstract
{

public:

  static QString className() { return "hoot::LinearAverageMerger"; }

  LinearAverageMerger();
  LinearAverageMerger(const std::set<std::pair<ElementId, ElementId>>& pairs);
  ~LinearAverageMerger() = default;

  QString getDescription() const override
  { return "Merges linear features by averaging tags and geometries"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

protected:

  bool _mergePair(
    ElementId eid1, ElementId eid2,
    std::vector<std::pair<ElementId, ElementId>>& replaced) override;

  /*
   * @see LinearMergerAbstract
   *
   * not implemented
   */
  WaySublineMatchString _matchSubline(ElementPtr /*e1*/, ElementPtr /*e2*/) override
  { return WaySublineMatchString(); }

private:

  double _getMinSplitSize(const ConstWayPtr& way1, const ConstWayPtr& way2) const;

  WayPtr _getMaximalNearestSubline(
    const ConstWayPtr& way1, const ConstWayPtr& way2, const double minSplitSize,
    std::vector<WayPtr>& splits) const;
};

}

#endif // LINEAR_AVERAGE_MERGER_H
