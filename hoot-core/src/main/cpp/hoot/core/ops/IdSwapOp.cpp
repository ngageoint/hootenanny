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
#include "IdSwapOp.h"

//  Hoot
#include <hoot/core/conflate/IdSwap.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, IdSwapOp)

IdSwapOp::IdSwapOp()
{
}

void IdSwapOp::apply(std::shared_ptr<hoot::OsmMap>& map)
{
  //  Validate the swap information
  IdSwapPtr swap = map->getIdSwap();
  if (!swap)
    return;
  //  Iterate all of the entries to swap
  for (IdSwap::iterator it = swap->begin(); it != swap->end(); ++it)
  {
    ElementPtr element1 = map->getElement(it->first);
    ElementPtr element2 = map->getElement(it->second);
    //  All matches are processed but not all are affected
    _numProcessed++;
    //  Can't swap non-existant elements
    if (!element1 || !element2)
      continue;
    //  Don't swap elements that aren't the same type
    if (element1->getElementType() != element2->getElementType())
      continue;
    //  Create a copy of element elements
    ElementPtr element1a(element1->clone());
    ElementPtr element2a(element2->clone());
    //  Replace element 2 with an empty ID temporarily
    element2a->setId(0);
    map->replace(element2, element2a);
    //  Replace element 1 with element 2's ID
    element1a->setId(element2->getId());
    map->replace(element1, element1a);
    //  Replace element 2a with element 1's ID
    element2->setId(element1->getId());
    map->replace(element2a, element2);
    //  Swap the PIDs also
    if (element1->getElementType() == ElementType::Way &&
        element2->getElementType() == ElementType::Way)
    {
      //  This is tricky because element1a has replaced element1 as the actual object
      WayPtr way1 = std::dynamic_pointer_cast<Way>(element1a);
      WayPtr way2 = std::dynamic_pointer_cast<Way>(element2);
      long temp = way1->getPid();
      way1->setPid(way2->getPid());
      way2->setPid(temp);
    }
    //  Update the number affected
    _numAffected++;
  }
}

}
