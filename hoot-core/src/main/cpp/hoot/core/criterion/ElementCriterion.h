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
#ifndef ELEMENTCRITERION_H
#define ELEMENTCRITERION_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/info/ApiEntityInfo.h>

namespace hoot
{
class Element;

/**
 * Does the element satisfy some crierion? If so, it'll return true.
 *
 * The criterion could be just about anything. E.g.
 * - Distance to a point
 * - line length
 * - building type
 * - etc.
 *
 * How this criterion is used is based on the context. E.g. you could use it to delete all elements
 * that satisfy the criteria, or add tags to all those that meet the criteria. In general, you
 * should only use the criteria in a positive sense. (e.g. apply something to the elements that
 * meet the criteria, rather than apply it to those that don't meet the criteria). Look at the
 * NotCriterion for an example to negate criterion.
 */
class ElementCriterion : public ApiEntityInfo
{
public:

  static QString className() { return "hoot::ElementCriterion"; }

  ElementCriterion() = default;
  virtual ~ElementCriterion() = default;

  /**
   * Returns true if the element satisfies the criterion.
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const = 0;

  /**
   * Use the clone pattern for all classes based on the ElementCriterion class
   */
  virtual std::shared_ptr<ElementCriterion> clone() = 0;

  virtual QString toString() const override { return ""; }
};

typedef std::shared_ptr<ElementCriterion> ElementCriterionPtr;

}

#endif // ELEMENTCRITERION_H
