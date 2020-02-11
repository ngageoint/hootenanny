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

#include "NonConflatableElementRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/NonConflatableCriterion.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, NonConflatableElementRemover)

NonConflatableElementRemover::NonConflatableElementRemover()
{
}

void NonConflatableElementRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  RemoveElementsVisitor removeElementsVisitor;
  removeElementsVisitor.setRecursive(true);
  // RemoveElementsVisitor will auto handle setting the config on this crit.
  removeElementsVisitor.addCriterion(
    std::shared_ptr<NonConflatableCriterion>(new NonConflatableCriterion()));
  removeElementsVisitor.setConfiguration(conf());
  _map->visitRw(removeElementsVisitor);
}

}
