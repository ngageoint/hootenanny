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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultipleCriterionConsumerVisitor.h"

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

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
  const QStringList& criteriaClassNames, const Settings& conf)
{
  if (criteriaClassNames.size() > 0)
  {
    _criteria.clear();
    for (int i = 0; i < criteriaClassNames.size(); i++)
    {
      const QString critName = criteriaClassNames.at(i);
      if (!critName.trimmed().isEmpty())
      {
        LOG_VARD(critName);
        ElementCriterionPtr crit =
          std::shared_ptr<ElementCriterion>(
            Factory::getInstance().constructObject<ElementCriterion>(critName.trimmed()));
        addCriterion(crit);
        Configurable* c = dynamic_cast<Configurable*>(crit.get());
        if (c != 0)
        {
          c->setConfiguration(conf);
        }
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
      if (crit->isSatisfied(e))
      {
        criteriaSatisfied = true;
        break;
      }
    }
  }
  else
  {
    criteriaSatisfied = true;
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      if (!crit->isSatisfied(e))
      {
        criteriaSatisfied = false;
        break;
      }
    }
  }
  return criteriaSatisfied;
}

}

