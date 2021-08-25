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
#ifndef CHAINCRITERION_H
#define CHAINCRITERION_H

#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/elements/ConstOsmMapConsumer.h>

namespace hoot
{

/**
 * Is satisfied if all of its children criteria are satisfied.
 */
class ChainCriterion : public ElementCriterion, public ElementCriterionConsumer,
  public Configurable, public ConstOsmMapConsumer
{
public:

  static QString className() { return "ChainCriterion"; }

  ChainCriterion() = default;
  ChainCriterion(const std::vector<ElementCriterionPtr>& criteria);
  ChainCriterion(const ElementCriterionPtr& child1, const ElementCriterionPtr& child2);
  ChainCriterion(ElementCriterion* child1, ElementCriterion* child2);
  virtual ~ChainCriterion() = default;

  void addCriterion(const ElementCriterionPtr& e) override;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<ChainCriterion>(_criteria); }

  void setOsmMap(const OsmMap* map) override;

  void setConfiguration(const Settings& conf) override;

  QString getDescription() const override
  { return "Allows for chaining criteria together (logical AND)"; }
  QString toString() const override;
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

  int criteriaSize() const { return _criteria.size(); }

protected:

  std::vector<std::shared_ptr<ElementCriterion>> _criteria;

private:

  friend class RemoveRef2VisitorMultipleCriterion;
};

using ChainCriterionPtr = std::shared_ptr<ChainCriterion>;

}
#endif // CHAINCRITERION_H
