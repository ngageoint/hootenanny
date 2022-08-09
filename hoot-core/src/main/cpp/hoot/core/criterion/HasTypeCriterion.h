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
#ifndef HAS_TYPE_CRITERION_H
#define HAS_TYPE_CRITERION_H

// Hoot
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * Determines if an element has a type recognized by the Hootenanny schema
 */
class HasTypeCriterion : public ElementCriterion
{
public:

  static QString className() { return "HasTypeCriterion"; }

  HasTypeCriterion() = default;
  ~HasTypeCriterion() override = default;

  bool isSatisfied(const ConstElementPtr& e) const override;
  ElementCriterionPtr clone() override { return std::make_shared<HasTypeCriterion>(); }

  QString getDescription() const override
  { return "Determines if an element has a recognizable type."; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString toString() const override { return className(); }
};

}

#endif // HAS_TYPE_CRITERION_H
