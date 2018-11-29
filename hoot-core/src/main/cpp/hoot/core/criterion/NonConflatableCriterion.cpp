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
 * @copyright Copyright (C) 2018 DigitalGlobe (http://www.digitalglobe.com/)
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

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, NonConflatableCriterion)

// Return true if not conflatable
bool NonConflatableCriterion::isSatisfied(const Element& e) const
{
  //See if our type is known conflatable - return false
  //TODO: This could probably be made cleaner by adding a "conflatable" attribute to criterion.
  if (HighwayCriterion().isSatisfied(*e))
      return false;
  else if (LinearWaterwayCriterion().isSatisfied(*e))
    return false;
  else if (PoiCriterion().isSatisfied(*e))
    return false;
  else if (BuildingCriterion().isSatisfied(*e))
    return false;
  else if (RailwayCriterion().isSatisfied(*e))
    return false;
  else if (PowerLineCriterion().isSatisfied(*e))
    return false;
  else if (AreaCriterion().isSatisfied(*e))
    return false;

  // It is not something we can conflate
  return true;
}

}
