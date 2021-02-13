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
 * @copyright Copyright (C) 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef REMOVEREF2VISITORMULTIPLECRITERION_H
#define REMOVEREF2VISITORMULTIPLECRITERION_H

// Hoot
#include <hoot/core/visitors/RemoveRef2Visitor.h>

namespace hoot
{

/**
 * If one specified criterion meets for REF1 and another also for REF2 elements, then the REF2 tag is
 * removed.
 *
 * This class is re-entrant, but not thread safe.
 */
class RemoveRef2VisitorMultipleCriterion : public RemoveRef2Visitor
{

public:

  static QString className() { return "hoot::RemoveRef2VisitorMultipleCriterion"; }

  RemoveRef2VisitorMultipleCriterion() = default;
  virtual ~RemoveRef2VisitorMultipleCriterion() = default;

  virtual void addCriterion(const ElementCriterionPtr& e) override;

  virtual void visit(const ElementPtr& e) override;

  virtual bool ref1CriterionSatisfied(const ConstElementPtr& e) const override;
  virtual bool ref2CriterionSatisfied(const ConstElementPtr& e) const override;

  virtual QString getDescription() const override
  { return "Removes REF2 tags when multiple criteria are met for both REF1 and REF2 elements"; }

private:

  ElementCriterionPtr _ref1Criterion;
  ElementCriterionPtr _ref2Criterion;

};

}

#endif // REMOVEREF2VISITORMULTIPLECRITERION_H
