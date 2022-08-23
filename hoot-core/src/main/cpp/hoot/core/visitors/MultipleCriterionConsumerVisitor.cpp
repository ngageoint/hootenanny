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
 * @copyright Copyright (C) 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultipleCriterionConsumerVisitor.h"

#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>


namespace hoot
{

MultipleCriterionConsumerVisitor::MultipleCriterionConsumerVisitor()
  : _negateCriteria(false),
    _chainCriteria(false),
    _configureChildren(true)
{
}

void MultipleCriterionConsumerVisitor::addCriterion(const ElementCriterionPtr& crit)
{
  LOG_VART(_negateCriteria);
  LOG_VART(crit.get());
  if (_negateCriteria)
    _criteria.push_back(std::make_shared<NotCriterion>(crit));
  else
    _criteria.push_back(ElementCriterionPtr(crit));
  LOG_VART(_criteria.size());
}

void MultipleCriterionConsumerVisitor::_addCriteria(const QStringList& criteriaClassNames)
{
  if (!criteriaClassNames.empty())
  {
    _criteria.clear();
    for (const auto& critName : qAsConst(criteriaClassNames))
    {
      if (!critName.trimmed().isEmpty())
      {
        LOG_VART(critName);
        addCriterion(Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed()));
      }
    }
  }
}

bool MultipleCriterionConsumerVisitor::_criteriaSatisfied(const ConstElementPtr& e) const
{
  bool criteriaSatisfied;
  if (!_chainCriteria)
  {
    criteriaSatisfied = false;
    for (const auto& crit : _criteria)
    {
      LOG_VART(crit->toString());
      if (crit->isSatisfied(e))
      {
        criteriaSatisfied = true;
        LOG_TRACE(
          "One OR'd criterion satisfied in: " << toString() << ". Filter satisfied for: " <<
          e->getElementId());
        break;
      }
    }
    if (!criteriaSatisfied)
    {
      LOG_TRACE(
        "No OR'd criterion satisfied in: " <<  toString() << ". Filter not satisfied for: " <<
        e->getElementId());
    }
  }
  else
  {
    criteriaSatisfied = true;
    for (const auto& crit : _criteria)
    {
      LOG_VART(crit->toString());
      if (!crit->isSatisfied(e))
      {
        criteriaSatisfied = false;
        LOG_TRACE(
          "One chained criterion not satisfied in: " << toString() << ". Filter not satisfied " <<
          "for: " << e->getElementId());
        break;
      }
    }
    if (criteriaSatisfied)
    {
      LOG_TRACE(
        "One chained criteria satisfied in: " << toString() << ". Filter satisfied for: " <<
        e->getElementId());
    }
  }
  return criteriaSatisfied;
}

QString MultipleCriterionConsumerVisitor::toString() const
{
  QStringList txt;
  for (const auto& crit : _criteria)
    txt.append(crit->toString());
  return txt.join(";");
}

}
