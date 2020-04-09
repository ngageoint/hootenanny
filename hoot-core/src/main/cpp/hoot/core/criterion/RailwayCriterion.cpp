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

#include "RailwayCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, RailwayCriterion)

RailwayCriterion::RailwayCriterion()
{
}

bool RailwayCriterion::isSatisfied(const ConstElementPtr& e) const
{
  // TODO: Should we throw LinearCriterion in here as a requirement, like we did for
  // LinearWaterwayCriterion, since railway conflation really only handles linear railways anyway?
  if (e->getElementType() == ElementType::Way || e->getElementType() == ElementType::Relation)
  {
    const Tags& tags = e->getTags();
    for (Tags::const_iterator it = tags.constBegin(); it != tags.constEnd(); ++it)
    {
      // I think we may want to narrow down to a more specific set of railway values here
      // at some point.
      if (it.key() == "railway" || OsmSchema::getInstance().isAncestor(it.key(), "railway"))
      {
        return true;
      }
    }
  }
  return false;
}

}
