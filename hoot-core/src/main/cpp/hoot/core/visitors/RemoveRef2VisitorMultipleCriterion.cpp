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
 * @copyright Copyright (C) 2016, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "RemoveRef2VisitorMultipleCriterion.h"

// hoot
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveRef2VisitorMultipleCriterion)

void RemoveRef2VisitorMultipleCriterion::addCriterion(const ElementCriterionPtr& e)
{
  //hacking use of ChainCriterion here
  std::shared_ptr<const ChainCriterion> chainCrit = std::dynamic_pointer_cast<const ChainCriterion>(e);
  LOG_VARD(chainCrit->_criteria.size());
  if (!chainCrit.get() && chainCrit->_criteria.size() != 2)
  {
    throw HootException(
      QString("RemoveRef2VisitorDifferentElementTypes expects a chain criterion passed to it "
              "the first criterion as the REF1 criterion and the second as the REF2 criterion."));
  }
  _ref1Criterion = chainCrit->_criteria[0];
  _ref2Criterion = chainCrit->_criteria[1];
}

void RemoveRef2VisitorMultipleCriterion::visit(const ElementPtr& e)
{
  if (!_ref1Criterion || !_ref2Criterion)
  {
    throw IllegalArgumentException("You must specify a criterion before calling "
                                   "RemoveRef2VisitorMultipleCriterion.");
  }
  _criterion = _ref1Criterion; //to keep RemoveRef2Visitor happy; hackish
  RemoveRef2Visitor::visit(e);
}

bool RemoveRef2VisitorMultipleCriterion::ref1CriterionSatisfied(const ConstElementPtr& e) const
{
  return _ref1Criterion->isSatisfied(e);
}

bool RemoveRef2VisitorMultipleCriterion::ref2CriterionSatisfied(const ConstElementPtr& e) const
{
  return _ref2Criterion->isSatisfied(e);
}


}
