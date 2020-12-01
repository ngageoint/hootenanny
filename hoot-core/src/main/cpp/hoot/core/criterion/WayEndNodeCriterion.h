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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAY_END_NODE_CRITERION_H
#define WAY_END_NODE_CRITERION_H

// hoot
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/*
 * TODO: false positive with this: service=driveway in refSinglePointTest, Northern corner
 */
class WayEndNodeCriterion : public WayNodeCriterion
{

public:

  static std::string className() { return "hoot::WayEndNodeCriterion"; }

  WayEndNodeCriterion(const bool allowShared = true);
  WayEndNodeCriterion(ConstOsmMapPtr map, const bool allowShared = true);
  virtual ~WayEndNodeCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new WayEndNodeCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies way end nodes"; }

private:

  // If true, way end nodes shared between multiple ways satisfy this criterion. If false, then
  // shared way end nodes do not satisfy this criterion.
  bool _allowShared;
};

}

#endif // WAY_END_NODE_CRITERION_H
