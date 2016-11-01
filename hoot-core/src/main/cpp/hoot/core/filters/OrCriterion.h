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
#ifndef ORCRITERION_H
#define ORCRITERION_H

#include "ChainCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>

namespace hoot
{

/**
 * Filters an element if any of the child filters return true.
 */
class OrCriterion : public ChainCriterion
{
public:

  static string className() { return "hoot::OrCriterion"; }

  OrCriterion() {}
  OrCriterion(ElementCriterion* child1, ElementCriterion* child2) :
    ChainCriterion(child1, child2)
  {
  }

  virtual bool isSatisfied(const shared_ptr<const Element>& e) const
  {
    for (size_t i = 0; i < _filters.size(); i++)
    {
      if (_filters[i]->isSatisfied(e))
      {
        return true;
      }
    }

    return false;
  }

  virtual ElementCriterion* clone() { return new OrCriterion(_filters[0]->clone(), _filters[1]->clone()); }

};

}

#endif // ORCRITERION_H
