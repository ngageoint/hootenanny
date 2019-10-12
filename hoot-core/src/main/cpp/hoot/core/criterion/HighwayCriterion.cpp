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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "HighwayCriterion.h"

// hoot
#include <hoot/core/criterion/AreaCriterion.h>
#include <hoot/core/criterion/BuildingCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, HighwayCriterion)

bool HighwayCriterion::isSatisfied(const ConstElementPtr& element) const
{
  LOG_VART(element->getElementId());
  //LOG_VART(element);
  bool result = false;
  const Tags& tags = element->getTags();
  const ElementType type = element->getElementType();
  Tags::const_iterator it = tags.find("highway");

  // Is it a legit highway?
  if ((type == ElementType::Way || type == ElementType::Relation) &&
      it != tags.end() && it.value() != "")
  {
    result = true;
    LOG_VART(result);
  }
  //  Make sure it isn't a building or a part of a building
  else if (_map && (BuildingCriterion(_map).isSatisfied(element) ||
                    BuildingCriterion(_map).isParentABuilding(element->getElementId())))
  {
    result = false;
    LOG_VART(result);
  }
  else
  {
    /*
     * I don't think this is a good idea. A date isn't enough to identify a road. You could end up
     * with false positive matches. The runtime performance could also be hurt by checking for
     * highway matches against non-roads (although I haven't seen that in practice yet). Road tags
     * should be added to the feature before conflation, if necessary.
     *
     * Tried removing this and three case tests fail. One is just an element ID reordering, so ok.
     * The other two are legit failures. Interestingly enough if I turn off hash.seed.zero, then
     * those tests pass. Unfortunately, doing that isn't really an option for the tests. This else
     * statement can probably be removed and have the tests passing as well but will just take a
     * ton of debugging to figure out what's going on with the hash ordering.
     */

    // Maybe it's a way with nothing but a time tag...
    it = tags.find(MetadataTags::SourceDateTime());
    if (type == ElementType::Way && tags.keys().size() < 2 && it != tags.end())
    {
      // We can treat it like a highway
      result = true;
      LOG_VART(result);
    }
  }
  // Make sure this isn't an area highway section!
  if (result)
  {
    result = !AreaCriterion().isSatisfied(element);
    LOG_VART(result);
  }

  LOG_VART(result);
  return result;
}

}

