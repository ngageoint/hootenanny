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
 * @copyright Copyright (C) 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef DUAL_HIGHWAY_CRITERION_H
#define DUAL_HIGHWAY_CRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

namespace hoot
{

/**
 * TODO
 *
 * This creation of the needed map index for this is an expensive operation, so instances of this
 * class should be cached for repeated operation on the same map.
 */
class DualHighwayCriterion : public ElementCriterion
{
public:

  static QString className() { return "hoot::DualHighwayCriterion"; }

  DualHighwayCriterion() = default;
  DualHighwayCriterion(ConstOsmMapPtr map);
  virtual ~DualHighwayCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new DualHighwayCriterion(_map)); }

  virtual QString getDescription() const { return "TODO"; }

  virtual void setOsmMap(const OsmMap* map);

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  ConstOsmMapPtr _map;

  // spatial indexes
  std::shared_ptr<Tgs::HilbertRTree> _index;
  std::deque<ElementId> _indexToEid;

  void _createIndex();
  bool _isMatchCandidate(ConstElementPtr element) const;
  Meters _getSearchRadius(const ConstElementPtr& e) const;
};

}
#endif // DUAL_HIGHWAY_CRITERION_H
