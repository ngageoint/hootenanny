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
 * @copyright Copyright (C) 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LinearWaterwayCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/criterion/LinearCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, LinearWaterwayCriterion)

bool LinearWaterwayCriterion::isSatisfied(const ConstElementPtr& e) const
{
  LOG_VART(e->getElementId());
  //LOG_VART(e);

  if (e->getElementType() == ElementType::Node)
  {
    return false;
  }

  bool passedTagFilter = false;

  const Tags& tags = e->getTags();
  if (tags.contains("waterway"))
  {
    passedTagFilter = true;
  }
  else
  {
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
    {
      const QString key = it.key();
      const QString val = it.value();
      if (OsmSchema::getInstance().isAncestor(key, "waterway") ||
          (key == "type" && OsmSchema::getInstance().isAncestor("waterway=" + val, "waterway")))
      {
        LOG_TRACE("passed crit");
        passedTagFilter = true;
        break;
      }
    }
  }

  if (passedTagFilter && !LinearCriterion().isSatisfied(e))
  {
    LOG_TRACE("failed linear crit");
    return false;
  }

  return passedTagFilter;
}

}

