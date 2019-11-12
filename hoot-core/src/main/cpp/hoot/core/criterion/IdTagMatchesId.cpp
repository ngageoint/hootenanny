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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "IdTagMatchesId.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, IdTagMatchesId)

IdTagMatchesId::IdTagMatchesId()
{
}

bool IdTagMatchesId::isSatisfied(const ConstElementPtr& e) const
{
  const QString tagVal = e->getTags().get(MetadataTags::HootId()).trimmed();
  if (!tagVal.isEmpty())
  {
    bool ok = false;
    const long tagId = tagVal.toLong(&ok);
    if (ok && tagId != e->getId())
    {
      LOG_TRACE(
        "crit failed for " << e->getElementId() << "; tagId: " << tagId << "; id: " << e->getId());
      return false;
    }
  }
  return true;
}

}
