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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef CRITERIONCOUNTVISITOR_H
#define CRITERIONCOUNTVISITOR_H

// hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * Counts the number of elements matching the criterion
 */
class CriterionCountVisitor : public ConstElementVisitor, public ElementCriterionConsumer
{
public:

  static QString className() { return "CriterionCountVisitor"; }

  CriterionCountVisitor();
  CriterionCountVisitor(const ElementCriterionPtr& pCrit);
  ~CriterionCountVisitor() override = default;

  void addCriterion(const ElementCriterionPtr& pCrit) override;

  /**
   * @see ElementVisitor
   */
  void visit(const ConstElementPtr& e) override;

  uint64_t getCount() const { return _count; }
  uint64_t getTotal() const { return _visited; }

  QString getDescription() const override
  { return "Counts the number of elements matching a criterion"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  uint64_t _count;
  uint64_t _visited;
  std::shared_ptr<ElementCriterion> _pCrit;
};

}

#endif // CRITERIONCOUNTVISITOR_H
