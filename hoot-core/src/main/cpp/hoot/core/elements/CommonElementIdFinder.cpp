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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "CommonElementIdFinder.h"

// Hoot
#include <hoot/core/visitors/UniqueElementIdVisitor.h>
#include <hoot/core/util/CollectionUtils.h>

namespace hoot
{

QSet<ElementId> CommonElementIdFinder::findCommonElementIds(
  const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  QSet<ElementId> idsInCommon;
  while (map1->hasNext())
  {
    ElementPtr element = map1->next();
    if (element && map2->containsElement(element->getElementId()))
    {
      idsInCommon.insert(element->getElementId());
    }
  }
  return idsInCommon;
}

QSet<ElementId> CommonElementIdFinder::findElementIdsInFirstAndNotSecond(
  const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  return _findElementIdDiff(map1, map2, true);
}

QSet<ElementId> CommonElementIdFinder::findElementIdsInSecondAndNotFirst(
  const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  return _findElementIdDiff(map1, map2, false);
}

QSet<ElementId> CommonElementIdFinder::_findElementIdDiff(
  const OsmMapPtr& map1, const OsmMapPtr& map2, const bool keepIdsFromMap1)
{
  UniqueElementIdVisitor idVis1;
  map1->visitRo(idVis1);
  QSet<ElementId> ids1 = CollectionUtils::stdSetToQSet(idVis1.getElementSet());

  UniqueElementIdVisitor idVis2;
  map2->visitRo(idVis2);
  QSet<ElementId> ids2 = CollectionUtils::stdSetToQSet(idVis2.getElementSet());

  if (keepIdsFromMap1)
  {
    return ids2.subtract(ids1);
  }
  else
  {
    return ids1.subtract(ids2);
  }
}

}
