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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "OrCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, OrCriterion)

OrCriterion::OrCriterion(ElementCriterion* child1, ElementCriterion* child2) :
ChainCriterion(child1, child2)
{
}

OrCriterion::OrCriterion(ElementCriterionPtr child1, ElementCriterionPtr child2) :
ChainCriterion(child1, child2)
{
}

ElementCriterionPtr OrCriterion::clone()
{
  return ElementCriterionPtr(new OrCriterion(_criteria[0]->clone(), _criteria[1]->clone()));
}

bool OrCriterion::isSatisfied(const ConstElementPtr& e) const
{
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    if (_criteria[i]->isSatisfied(e))
    {
      LOG_TRACE(
        "One OR'd criterion satisfied in: " << toString() << ". Filter satisfied for: " << e);
      return true;
    }
  }
  LOG_TRACE(
    "No OR'd criterion satisfied in: " << toString() << ". Filter not satisfied for: " << e);
  return false;
}

QString OrCriterion::toString() const
{
  return
    ChainCriterion::toString().replace(
      QString::fromStdString(ChainCriterion::className()).replace("hoot::", ""),
      QString::fromStdString(className()).replace("hoot::", ""));
}

}
