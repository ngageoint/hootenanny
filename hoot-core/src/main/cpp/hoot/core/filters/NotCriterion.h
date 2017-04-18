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
#ifndef NOTCRITERION_H
#define NOTCRITERION_H

#include "ElementCriterion.h"
#include "ElementCriterionConsumer.h"

namespace hoot
{

class NotCriterion : public ElementCriterion, public ElementCriterionConsumer
{
public:

  static string className() { return "hoot::NotCriterion"; }

  NotCriterion() {}
  NotCriterion(ElementCriterion* c) : _child(c) {}
  NotCriterion(ElementCriterionPtr& c) : _child(c) {}

  virtual ~NotCriterion() {}

  virtual void addCriterion(const ElementCriterionPtr& e);

  /**
   * Returns true if the element satisfies the criterion.
   */
  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual ElementCriterion* clone() { return new NotCriterion(_child->clone()); }

private:
  ElementCriterionPtr _child;
};

}

#endif // NOTCRITERION_H
