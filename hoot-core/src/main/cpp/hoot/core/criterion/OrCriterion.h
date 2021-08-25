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
#ifndef ORCRITERION_H
#define ORCRITERION_H

#include <hoot/core/criterion/ChainCriterion.h>

namespace hoot
{

/**
 * Is satisfied if any of its child criteria are satisfied.
 */
class OrCriterion : public ChainCriterion
{
public:

  static QString className() { return "OrCriterion"; }

  OrCriterion() = default;
  OrCriterion(ElementCriterion* child1, ElementCriterion* child2);
  OrCriterion(ElementCriterionPtr child1, ElementCriterionPtr child2);
  ~OrCriterion() = default;

  bool isSatisfied(const ConstElementPtr& e) const override;

  ElementCriterionPtr clone() override;

  QString getDescription() const override { return "Allows for combining criteria (logical OR)"; }
  QString toString() const override;
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
};

using OrCriterionPtr = std::shared_ptr<OrCriterion>;

}

#endif // ORCRITERION_H
