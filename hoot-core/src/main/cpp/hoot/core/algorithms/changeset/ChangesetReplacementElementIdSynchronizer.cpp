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
#include <hoot/core/algorithms/extractors/EuclideanDistanceExtractor.h>
#include <hoot/core/util/StringUtils.h>
#include <hoot/core/io/OsmMapWriterFactory.h>

namespace hoot
{

void ChangesetReplacementElementIdSynchronizer::synchronize(
  const OsmMapPtr& map1, const OsmMapPtr& map2, const ElementType& /*elementType*/)
{
  // This is convoluted, but we're going to run the ID synchronization in multiple stages. Its
  // possible eventually that it could be run in a single stage if improvements are made to
  // ElementHashVisitor (also see ElementHashOp). Since we don't allow any single element ID to
  // synchronize more than once, this allows for syncing the IDs at different levels of node
  // coordinate precision granularity.

  // First run at a higher precision than what we're configured for with by default for C&R. This
  // will prevent some bad syncs, like for way nodes in completely different unconnected ways.
  _useNodeTagsForHash = true;
  _coordinateComparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity() + 2;
  ElementIdSynchronizer::synchronize(map1, map2, ElementType::Node);

  // Now, let's run at the C&R configured coord sensitivity. Its been lowered primarily to allow the
  // syncing of road nodes which are slightly offset from each other between the input datasets.
  _useNodeTagsForHash = true;
  _coordinateComparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity();
  ElementIdSynchronizer::synchronize(map1, map2);

  QString msg = "Synchronizing IDs for nearly identical way nodes";
  if (!_map1->getName().trimmed().isEmpty() && !_map2->getName().trimmed().isEmpty())
  {
    msg += " between " + _map1->getName() + " and " + _map2->getName();
  }
  msg += "...";
  LOG_INFO(msg);
  const int numTotalIdsSyncedBefore = getNumTotalFeatureIdsSynchronized();

  // Finally, we'll loosen the matching tags requirement and just look at way nodes
  // (_useNodeTagsForHash=false). We're looking for any way nodes between the two maps that belong
  // to the same way, assuming matching way IDs across the maps that happened as a result of the
  // previous ID synchronization. In those instances, we'll copy the ID from the first map element
  // over to the second map element as was done in the previous synchronization, but we'll make sure
  // the second element's tags are used.

  // Don't really understand yet why the comparison sensitivity needs to be reduced so much for
  // this. It may have to do with decimal rounding issues when ElementHashVisitor writes node JSON.
  // If we don't drop this sensitivity by a decimal place compared to the C&R default, we miss
  // synchronizing some way nodes that are very close together...in all cases checked so far, all
  // are much less than a meter apart. As noted in ChangesetReplacementCreator::_setGlobalOpts, a
  // coord sensitivity of 5 is a max diff of 1.1m, and going to 4 here takes us to a max of 11.11m.
  // Very strange that it needs to be done when the nodes in question aren't anywhere near that far
  // apart. There is an additional distance check explained in the loop below to prevent utter chaos
  // caused by this change.

  // Calc element hashes associated with element IDs.
  _useNodeTagsForHash = false;
  _coordinateComparisonSensitivity = ConfigOptions().getNodeComparisonCoordinateSensitivity() - 1;
  _calcElementHashes(_map1, _map1HashesToElementIds, _map1ElementIdsToHashes);
  LOG_VARD(_map1HashesToElementIds.size());
  QSet<QString> map1HashesSet = _map1HashesToElementIds.keys().toSet();
  _calcElementHashes(_map2, _map2HashesToElementIds, _map2ElementIdsToHashes);
  LOG_VARD(_map2HashesToElementIds.size());
  QSet<QString> map2HashesSet = _map2HashesToElementIds.keys().toSet();

  // Obtain the hashes for the elements that are identical between the two maps.
  const QSet<QString> identicalHashes = map1HashesSet.intersect(map2HashesSet);
  LOG_VARD(identicalHashes.size());

  // Overwrite map2 IDs with the IDs from map1 for the features that are identical.
  _syncElementIds(identicalHashes);

  LOG_DEBUG(
    "Updated " <<
    StringUtils::formatLargeNumber(getNumTotalFeatureIdsSynchronized() - numTotalIdsSyncedBefore) <<
    " nearly identical way nodes in second map.");
}

void ChangesetReplacementElementIdSynchronizer::_syncElementIds(
  const QSet<QString>& identicalHashes)
{
  for (QSet<QString>::const_iterator itr = identicalHashes.begin(); itr != identicalHashes.end();
       ++itr)
  {
    const QString identicalHash = *itr;
    LOG_VART(identicalHash);

    // Get the element with matching hash from the ref map.
    ElementPtr map1IdenticalElement = _map1->getElement(_map1HashesToElementIds[identicalHash]);
    _wayNodeCrit.setOsmMap(_map1.get());
    // If its a way node and we haven't already synced this ID, keep going...
    if (map1IdenticalElement && !_syncedElementIds.contains(map1IdenticalElement->getElementId()) &&
        _wayNodeCrit.isSatisfied(map1IdenticalElement))
    {
      LOG_VART(map1IdenticalElement->getElementId());

      // Get the element with matching hash from the sec map.
      ElementPtr map2IdenticalElement = _map2->getElement(_map2HashesToElementIds[identicalHash]);
      _wayNodeCrit.setOsmMap(_map2.get());
      // if its a way node, keep going
      if (map2IdenticalElement && _wayNodeCrit.isSatisfied(map2IdenticalElement))
      {
        // find all ways each node belong to
        QSet<long> containingWayIds1 =
          CollectionUtils::stdSetToQSet(
            WayUtils::getContainingWayIdsByNodeId(map1IdenticalElement->getId(), _map1));
        QSet<long> containingWayIds2 =
          CollectionUtils::stdSetToQSet(
            WayUtils::getContainingWayIdsByNodeId(map2IdenticalElement->getId(), _map2));
        // If any of them match, we'll proceed to copy the element ID of the first map over to the
        // second map element and be sure to keep the second map element's tags (nodes matched only
        // on coordinate, so that will be the same between the two).
        if (containingWayIds1.intersect(containingWayIds2).size() > 0 &&
            !_map2->containsElement(map1IdenticalElement->getElementId()))
        {
          // Due to our lapsing of the coord comparison sensitivity, we'll actually do a distance
          // check that only allows distances between nodes being synchronized no greater than what
          // they would be if we hadn't lowered the sensitiviy. Moving this as far down in the loop
          // as possible due to the geometry conversion expense.
          const double distance =
            EuclideanDistanceExtractor().distance(
              *_map1, *_map2, map1IdenticalElement, map2IdenticalElement);
          LOG_VART(distance);
          if (distance > 1.11)
          {
            // too far apart, don't sync
            continue;
          }

          // Copy it to be safe.
          ElementPtr map2IdenticalElementCopy(map2IdenticalElement->clone());
          map2IdenticalElementCopy->setId(map1IdenticalElement->getId());
          // need to use the ref map version
          map2IdenticalElementCopy->setVersion(map1IdenticalElement->getVersion());
          LOG_VART(map2IdenticalElementCopy->getElementId());

          LOG_TRACE(
            "Updating map 2 element: " << map2IdenticalElement->getElementId() << " to " <<
            map2IdenticalElementCopy->getElementId() << "...");

          // Add a custom metadata tag for debugging purposes.
          map2IdenticalElementCopy->getTags().set(MetadataTags::HootIdSynchronized(), "yes");
          // Add the element from the ref map.
          _map2->addElement(map2IdenticalElementCopy);
          // Replace the element from the sec map with the newly added element, which removes the
          // old element.
          _map2->replace(map2IdenticalElement, map2IdenticalElementCopy);
          _syncedElementIds.insert(map2IdenticalElementCopy->getElementId());
          _updatedNodeCtr++;

          // expensive; leave disabled by default
//          OsmMapWriterFactory::writeDebugMap(
//            _map2,
//            "after-id-sync-" + map2IdenticalElement->getElementId().toString() + "-to-" +
//            map2IdenticalElementCopy->getElementId().toString());
        }
      }
    }
  }
}

}
