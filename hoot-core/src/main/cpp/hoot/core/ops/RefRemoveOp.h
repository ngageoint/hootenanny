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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REFREMOVEOP_H
#define REFREMOVEOP_H

// Hoot
#include <hoot/core/criterion/ElementCriterionConsumer.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Removes all elements that meet a specified criterion. When removing these elements the REF1
 * values are tracked. After all the elements have been removed any REF2 references to REF1 are
 * updated appropriately. See [1] for an overview of the REF1/REF2 tags.
 *
 * 1. https://github.com/ngageoint/hootenanny/blob/master/docs/developer/HootenannyManualConflation.asciidoc
 */
class RefRemoveOp : public OsmMapOperation, public ElementCriterionConsumer
{
public:

  static QString className() { return "RefRemoveOp"; }

  RefRemoveOp() = default;
  ~RefRemoveOp() override = default;

  void addCriterion(const ElementCriterionPtr &e) override { assert(!_criterion); _criterion = e; }

  /**
   * Remove all the ops that meet the given criterion.
   */
  void apply(std::shared_ptr<OsmMap>& map) override;

  QString getDescription() const override
  { return "Removes elements meeting a criterion and updates REF tags"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  ElementCriterionPtr _criterion;
};

}

#endif // REFREMOVEOP_H
