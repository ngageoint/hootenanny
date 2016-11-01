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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CHAINCRITERION_H
#define CHAINCRITERION_H

#include "ElementCriterion.h"
#include "ElementCriterionConsumer.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/Factory.h>

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

  ChainCriterion(shared_ptr<ElementCriterion> child1, shared_ptr<ElementCriterion> child2)
  {
    _filters.push_back(child1);
    _filters.push_back(child2);
  }
  ChainCriterion(ElementCriterion* child1, ElementCriterion* child2)
  {
    _filters.push_back(shared_ptr<ElementCriterion>(child1));
    _filters.push_back(shared_ptr<ElementCriterion>(child2));
  }
  ChainCriterion(ElementCriterion* child1, ElementCriterionPtr child2)
  {
    _filters.push_back(shared_ptr<ElementCriterion>(child1));
    _filters.push_back(child2);
  }
  ChainCriterion(ElementCriterion* child1, ElementCriterion* child2, ElementCriterion* child3)
  {
    _filters.push_back(shared_ptr<ElementCriterion>(child1));
    _filters.push_back(shared_ptr<ElementCriterion>(child2));
    _filters.push_back(shared_ptr<ElementCriterion>(child3));
  }

  virtual void addCriterion(const ElementCriterionPtr& e)
  {
    _filters.push_back(e);
  }

  virtual bool isSatisfied(const shared_ptr<const Element>& e) const
  {
    for (size_t i = 0; i < _filters.size(); i++)
    {
      if (!_filters[i]->isSatisfied(e))
      {
        return false;
      }
    }

    return true;
  }

  virtual ElementCriterion* clone() { return new ChainCriterion(_filters); }

protected:
  ChainCriterion(vector< shared_ptr<ElementCriterion> > filters)
  {
    for (size_t i = 0; i < filters.size(); i++)
      _filters.push_back(shared_ptr<ElementCriterion>(filters[i]->clone()));
  }

  vector< shared_ptr<ElementCriterion> > _filters;

};

}
#endif // CHAINCRITERION_H
