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
 * @copyright Copyright (C) 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NonConflatableCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

QMap<QString, ElementCriterionPtr> NonConflatableCriterion::_conflatableCriteria;

HOOT_FACTORY_REGISTER(ElementCriterion, NonConflatableCriterion)

NonConflatableCriterion::NonConflatableCriterion()
{
  if (_conflatableCriteria.isEmpty())
  {
    _createConflatableCriteria();
  }
}

void NonConflatableCriterion::_createConflatableCriteria()
{
  const std::vector<std::string> criterionClassNames =
    Factory::getInstance().getObjectNamesByBase(ElementCriterion::className());
  for (std::vector<std::string>::const_iterator itr = criterionClassNames.begin();
       itr != criterionClassNames.end(); ++itr)
  {
    ElementCriterionPtr crit(Factory::getInstance().constructObject<ElementCriterion>(*itr));
    if (std::dynamic_pointer_cast<ConflatableElementCriterion>(crit) != 0)
    {
      _conflatableCriteria[QString::fromStdString(*itr)] = crit;
    }
  }
  LOG_VART(_conflatableCriteria.size());
}

bool NonConflatableCriterion::isSatisfied(const ConstElementPtr& e) const
{
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = _conflatableCriteria.begin();
       itr != _conflatableCriteria.end(); ++itr)
  {
    if (itr.value()->isSatisfied(e))
    {
      // It is something we can conflate.
      return false;
    }
  }
  // It is not something we can conflate
  return true;
}

QStringList NonConflatableCriterion::conflatableCriteria(const ConstElementPtr& e)
{
  QStringList conflatableCriteriaForElement;
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = _conflatableCriteria.begin();
       itr != _conflatableCriteria.end(); ++itr)
  {
    if (itr.value()->isSatisfied(e))
    {
      // It is something we can conflate.
      conflatableCriteriaForElement.append(itr.key());
    }
  }
  return conflatableCriteriaForElement;
}

}
