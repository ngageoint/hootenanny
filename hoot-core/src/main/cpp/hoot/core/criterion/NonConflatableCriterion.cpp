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

HOOT_FACTORY_REGISTER(ElementCriterion, NonConflatableCriterion)

NonConflatableCriterion::NonConflatableCriterion()
{
}

NonConflatableCriterion::NonConflatableCriterion(ConstOsmMapPtr map) :
_map(map)
{
}

bool NonConflatableCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const QMap<QString, ElementCriterionPtr> conflatableCriteria =
    ConflatableElementCriterion::getConflatableCriteria();
  for (QMap<QString, ElementCriterionPtr>::const_iterator itr = conflatableCriteria.begin();
       itr != conflatableCriteria.end(); ++itr)
  {
    ElementCriterionPtr crit = itr.value();

    if (_map)
    {
      ConstOsmMapConsumer* mapConsumer = dynamic_cast<ConstOsmMapConsumer*>(crit.get());
      if (mapConsumer != 0)
      {
        mapConsumer->setOsmMap(_map.get());
      }
    }

    if (crit->isSatisfied(e))
    {
      // It is something we can conflate.
      LOG_TRACE("Element: " << e->getElementId() << " is conflatable with: " << itr.key());
      return false;
    }
  }
  // It is not something we can conflate
  LOG_TRACE("Element: " << e->getElementId() << " is not conflatable.");
  return true;
}

}
