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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ELEMENTFILTER_H
#define ELEMENTFILTER_H

#include <hoot/core/util/NotImplementedException.h>

#include "Filter.h"

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
 * How thise criterion is used is based on the context. E.g. you could use it to delete all elements
 * that satisfy the criteria, or add tags to all those that meet the criteria. In general, you
 * should only use the criteria in a positive sense. (e.g. apply something to the elements that
 * meet the criteria, rather than apply it to those that don't meet the criteria). Look at the
 * NotCriterion for an example to negate criterion.
 */
class ElementCriterion
{
public:

  static string className() { return "hoot::ElementCriterion"; }

  virtual ~ElementCriterion() {}

  /**
   * Provided for backwards compatibility with the old filter. This method should be avoided.
   */
  virtual bool isNotSatisfied(const boost::shared_ptr<const Element>& e) const { return !isSatisfied(e); }

  /**
   * Returns true if the element satisfies the criterion.
   */
  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const = 0;

  /**
   * Use the clone pattern for all classes based on the ElementCriterion class
   */
  virtual ElementCriterion* clone() = 0;
};

typedef boost::shared_ptr<ElementCriterion> ElementCriterionPtr;

/**
 * Much of the old (pre 6/24/2014) code is based around the concept of a filter as opposed to a
 * criterion. The criterion is more intuitive and consistent to me, so I've started transitioning
 * the code to the criterion. I haven't moved much of it, but the newer Criterion interface should
 * be favored over the older ElementFilter interface.
 */
class BaseElementFilter : public ElementCriterion, public Filter
{
public:

  static string className() { return "hoot::BaseElementFilter"; }

  virtual ~BaseElementFilter() {}

  /**
   * Returns true if the element should be filtered (removed). Removed depends on the context. E.g.
   * It may just remove the element from a list of returned elements.
   */
  virtual bool isFiltered(const boost::shared_ptr<const Element>& e) const { return isFiltered(*e); }

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const { return !isFiltered(e); }

protected:
  virtual bool isFiltered(const Element& /*e*/) const { throw NotImplementedException(); }
};

}

#endif // ELEMENTFILTER_H
