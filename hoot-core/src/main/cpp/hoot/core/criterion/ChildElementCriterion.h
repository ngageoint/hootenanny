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
#ifndef CHILD_ELEMENT_CRITERION_H
#define CHILD_ELEMENT_CRITERION_H

// hoot
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Identifies way nodes and relation members
 */
class ChildElementCriterion : public ElementCriterion, public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::ChildElementCriterion"; }

  ChildElementCriterion() = default;
  ChildElementCriterion(ConstOsmMapPtr map) : _map(map) { }
  virtual ~ChildElementCriterion() = default;

  /**
   * @see ElementVisitor
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new ChildElementCriterion(_map)); }

  virtual QString getDescription() const { return "Identifies way nodes and relation members"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual void setOsmMap(const OsmMap* map) { _map = map->shared_from_this(); }

private:

  ConstOsmMapPtr _map;
};

}

#endif // AREACRITERION_H
