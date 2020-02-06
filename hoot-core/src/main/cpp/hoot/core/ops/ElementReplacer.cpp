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
 * @copyright Copyright (C) 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ElementReplacer.h"

// hoot
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, ElementReplacer)

ElementReplacer::ElementReplacer()
{
}

ElementReplacer::ElementReplacer(OsmMapPtr mapToReplaceFrom) :
_mapToReplaceFrom(mapToReplaceFrom)
{
}

void ElementReplacer::apply(std::shared_ptr<OsmMap>& map)
{
  if (!_mapToReplaceFrom)
  {
    throw IllegalArgumentException("No replace from map specified.");
  }
  LOG_VART(_mapToReplaceFrom->size());

  while (_mapToReplaceFrom->hasNext())
  {
    const ElementPtr elementToReplaceWith = _mapToReplaceFrom->next();
    LOG_VART(elementToReplaceWith->getElementId());
    LOG_VART(map->containsElement(elementToReplaceWith->getElementId()));
    if (map->containsElement(elementToReplaceWith->getElementId()))
    {
      LOG_TRACE(
        "Updating " << map->getElement(elementToReplaceWith->getElementId())->getElementId() <<
        " with " << elementToReplaceWith->getElementId() << "...");

      ElementPtr elementToReplace = map->getElement(elementToReplaceWith->getElementId());

      LOG_VART(elementToReplaceWith);
      LOG_VART(elementToReplace);
      assert(elementToReplace->getElementId() == elementToReplaceWith->getElementId());

      // map replace doesn't seem to work when the elements have the same id but different content,
      // so cloning the element and re-adding it to do the replacement
      elementToReplace.reset(elementToReplaceWith->clone());
      map->addElement(elementToReplace);

      LOG_VART(elementToReplace);

      _numAffected++;
    }
  }
}

}
