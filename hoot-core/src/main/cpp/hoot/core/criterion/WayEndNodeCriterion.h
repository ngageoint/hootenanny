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
#ifndef WAY_END_NODE_CRITERION_H
#define WAY_END_NODE_CRITERION_H

// hoot
#include <hoot/core/criterion/WayNodeCriterion.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * Identifies way end nodes, which are end way nodes of non-closed poly ways that may or may not
 * be shared among more than one way.
 *
 * TODO: fix false positive with this: service=driveway in refSinglePointTest, Northern corner
 */
class WayEndNodeCriterion : public WayNodeCriterion
{

public:

  static QString className() { return "hoot::WayEndNodeCriterion"; }

  WayEndNodeCriterion(const bool allowShared = true);
  WayEndNodeCriterion(ConstOsmMapPtr map, const bool allowShared = true);
  ~WayEndNodeCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<WayEndNodeCriterion>(_map); }

  QString getDescription() const override { return "Identifies way end nodes"; }
  QString toString() const override { return className(); }

private:

  // If true, way end nodes shared between multiple ways satisfy this criterion. If false, then
  // shared way end nodes do not satisfy this criterion.
  bool _allowShared;
};

}

#endif // WAY_END_NODE_CRITERION_H
