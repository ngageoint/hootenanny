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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHAINCRITERION_H
#define CHAINCRITERION_H

#include "ElementCriterion.h"
#include "ElementCriterionConsumer.h"

namespace hoot
{

/**
 * isSatisfied returns true if all the children are satisfied
 */
class ChainCriterion : public ElementCriterion, public ElementCriterionConsumer
{
public:

  static std::string className() { return "hoot::ChainCriterion"; }

  ChainCriterion() {}
  ChainCriterion(ElementCriterionPtr child1, ElementCriterionPtr child2);
  ChainCriterion(ElementCriterion* child1, ElementCriterion* child2);
  ChainCriterion(ElementCriterion* child1, ElementCriterionPtr child2);
  ChainCriterion(ElementCriterion* child1, ElementCriterion* child2, ElementCriterion* child3);

  virtual void addCriterion(const ElementCriterionPtr& e);

  virtual bool isSatisfied(const boost::shared_ptr<const Element>& e) const;

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new ChainCriterion(_filters)); }

  virtual QString getDescription() const
  { return "Allows for chaining together multiple criterion"; }

protected:

  ChainCriterion(std::vector< boost::shared_ptr<ElementCriterion> > filters);

  std::vector< boost::shared_ptr<ElementCriterion> > _filters;

private:

  friend class RemoveRef2VisitorMultipleCriterion;

};

}
#endif // CHAINCRITERION_H
