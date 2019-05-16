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
#include <hoot/core/elements/Tags.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/elements/Element.h>
#include <hoot/core/criterion/HighwayCriterion.h>
#include <hoot/core/criterion/LinearWaterwayCriterion.h>
#include <hoot/core/criterion/PoiCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/PowerLineCriterion.h>
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPoiCriterion.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>
#include <hoot/core/criterion/BuildingPartCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NonConflatableCriterion)

bool NonConflatableCriterion::isSatisfied(const ConstElementPtr& e) const
{
  // This could possibly be made cleaner by adding a "conflatable" property to criterion (there
  // may be a better way). Its also unnecessarily maintenance-prone as new types get added. - #2941

  if (HighwayCriterion().isSatisfied(e)
      || LinearWaterwayCriterion().isSatisfied(e)
      || PoiCriterion().isSatisfied(e)
      || BuildingCriterion().isSatisfied(e)
      || RailwayCriterion().isSatisfied(e)
      || PowerLineCriterion().isSatisfied(e)
      || AreaCriterion().isSatisfied(e)
      || PoiPolygonPoiCriterion().isSatisfied(e)
      || PoiPolygonPolyCriterion().isSatisfied(e)
     )
    return false;

  // It is not something we can conflate
  return true;
}

}
