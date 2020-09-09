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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultipleCriterionConsumerVisitor.h"

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

MultipleCriterionConsumerVisitor::MultipleCriterionConsumerVisitor() :
_negateCriteria(false),
_chainCriteria(false),
_configureChildren(true)
{
}

void MultipleCriterionConsumerVisitor::addCriterion(const ElementCriterionPtr& crit)
{
  LOG_VART(_negateCriteria);
  LOG_VART(crit.get());
  if (_negateCriteria)
  {
    _criteria.push_back(ElementCriterionPtr(new NotCriterion(crit)));
  }
  else
  {
    _criteria.push_back(ElementCriterionPtr(crit));
  }
  LOG_VART(_criteria.size());
}

void MultipleCriterionConsumerVisitor::_addCriteria(
  const QStringList& criteriaClassNames)
{
  if (criteriaClassNames.size() > 0)
  {
    _criteria.clear();
    for (int i = 0; i < criteriaClassNames.size(); i++)
    {
      const QString critName = criteriaClassNames.at(i);
      if (!critName.trimmed().isEmpty())
      {
        LOG_VART(critName);
        ElementCriterionPtr crit =
          std::shared_ptr<ElementCriterion>(
            Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed()));
        addCriterion(crit);
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
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
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
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
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
  QString txt;
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    txt += _criteria.at(i)->toString() + ";";
  }
  txt.chop(1);
  return txt;
}

}

