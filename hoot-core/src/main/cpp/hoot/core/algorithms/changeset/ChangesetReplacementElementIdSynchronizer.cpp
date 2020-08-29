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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "ChangesetReplacementElementIdSynchronizer.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/visitors/ElementHashVisitor.h>
#include <hoot/core/elements/WayUtils.h>
#include <hoot/core/util/CollectionUtils.h>

namespace hoot
{

ChangesetReplacementElementIdSynchronizer::ChangesetReplacementElementIdSynchronizer() :
ElementIdSynchronizer()
{
}

void ChangesetReplacementElementIdSynchronizer::synchronize(const OsmMapPtr& map1,
                                                            const OsmMapPtr& map2)
{
  ElementIdSynchronizer::synchronize(map1, map2);

  // TODO: This fixes a lot of issues with turning circles, but causes issues with several others.
  // TODO: also try laxing the node coord comparison sensitivity just for this run

  QString msg = "Synchronizing IDs for identical elements";
  if (!map1->getName().trimmed().isEmpty() && !map2->getName().trimmed().isEmpty())
  {
    msg += " between " + map1->getName() + " and " + map2->getName();
  }
  msg += "...";
  LOG_INFO(msg);

  _useNodeTagsForHash = false;

  // Calc element hashes associated with element IDs.
  const QMap<QString, ElementId> map1Hashes = _calcElementHashes(map1);
  LOG_VARD(map1Hashes.size());
  QSet<QString> map1HashesSet = map1Hashes.keys().toSet();
  const QMap<QString, ElementId> map2Hashes = _calcElementHashes(map2);
  LOG_VARD(map2Hashes.size());
  QSet<QString> map2HashesSet = map2Hashes.keys().toSet();

  // Obtain the hashes for the elements that are identical between the two maps.
  const QSet<QString> identicalHashes = map1HashesSet.intersect(map2HashesSet);
  LOG_VARD(identicalHashes.size());

  for (QSet<QString>::const_iterator itr = identicalHashes.begin(); itr != identicalHashes.end();
       ++itr)
  {
    const QString identicalHash = *itr;
    // TODO: change back to trace
    LOG_VARD(identicalHash);

    // Get the element with matching hash from the ref map
    ConstElementPtr map1IdenticalElement = map1->getElement(map1Hashes[identicalHash]);
    _wayNodeCrit.setOsmMap(map1.get());
    if (map1IdenticalElement && _wayNodeCrit.isSatisfied(map1IdenticalElement))
    {
      LOG_VARD(map1IdenticalElement->getElementId());

      ElementPtr map2IdenticalElement = map2->getElement(map2Hashes[identicalHash]);
      _wayNodeCrit.setOsmMap(map2.get());
      if (map2IdenticalElement && _wayNodeCrit.isSatisfied(map2IdenticalElement))
      {
        QSet<long> containingWayIds1 =
          CollectionUtils::stdSetToQSet(
            WayUtils::getContainingWayIdsByNodeId(map1IdenticalElement->getId(), map1));
        QSet<long> containingWayIds2 =
          CollectionUtils::stdSetToQSet(
            WayUtils::getContainingWayIdsByNodeId(map2IdenticalElement->getId(), map2));
        if (containingWayIds1.intersect(containingWayIds2).size() > 0)
        {
          // Copy it to be safe.
          ElementPtr map1IdenticalElementCopy(map1IdenticalElement->clone());
          LOG_VARD(map1IdenticalElementCopy->getElementId());
          // Make sure the map being updated doesn't already have an element with this ID (this
          // check may not be necessary).
          LOG_DEBUG(
            "Updating map 2 element: " << map2IdenticalElement->getElementId() << " to " <<
            map1IdenticalElement->getElementId() << "...");

          // Add a custom metadata tag for debugging purposes.
          map1IdenticalElementCopy->getTags().set(MetadataTags::HootIdSynchronized(), "yes");
          // Add the element from the ref map.
          map2->addElement(map1IdenticalElementCopy);
          // Replace the element from the sec map with the newly added element, which removes the
          // old element.
          map2->replace(map2IdenticalElement, map1IdenticalElementCopy);
          _updatedNodeCtr++;
        }
      }
    }
  }

  LOG_DEBUG(
    "Updated IDs on " << getNumTotalFeatureIdsSynchronized() <<
    " identical elements in second map.");
}

}
