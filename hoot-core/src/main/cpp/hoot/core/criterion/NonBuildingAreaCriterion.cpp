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
 * @copyright Copyright (C) 2016, 2017, 2018, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NonBuildingAreaCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/AreaCriterion.h>

#include <QElapsedTimer>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NonBuildingAreaCriterion)

bool NonBuildingAreaCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const bool isArea = AreaCriterion(_map).isSatisfied(e);
  if (!isArea)
  {
    return false;
  }

  const bool isBuilding = BuildingCriterion(_map).isSatisfied(e);
  if (isBuilding)
  {
    return false;
  }

  LOG_TRACE("is non-building area: " << e);

  return true;
}

}
