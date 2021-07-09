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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef NOTCRITERION_H
#define NOTCRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>

namespace hoot
{

class NotCriterion : public ElementCriterion, public ElementCriterionConsumer, public Configurable,
  public ConstOsmMapConsumer
{
public:

  static QString className() { return "hoot::NotCriterion"; }

  NotCriterion() = default;
  // TODO: Think we can probably get rid of this version that takes in ElementCriterion*.
  NotCriterion(ElementCriterion* c) : _child(c) { }
  NotCriterion(ElementCriterionPtr c) : _child(c) { }
  ~NotCriterion() = default;

  void addCriterion(const ElementCriterionPtr& e) override;

  /**
   * Returns true if the element satisfies the criterion.
   */
  bool isSatisfied(const ConstElementPtr& e) const override;

  ElementCriterionPtr clone() override
  { return ElementCriterionPtr(new NotCriterion(_child->clone())); }

  QString getDescription() const override { return "Negates a criterion"; }
  QString toString() const override;
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  void setConfiguration(const Settings& conf) override;

  void setOsmMap(const OsmMap* map) override;

private:

  ElementCriterionPtr _child;
};

using NotCriterionPtr = std::shared_ptr<NotCriterion>;

}

#endif // NOTCRITERION_H
