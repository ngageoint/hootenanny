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

#ifndef ELEMENT_ID_SYNCHRONIZER_H
#define ELEMENT_ID_SYNCHRONIZER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

/**
 * This class allows for synchronizing element IDs between two maps that have identical features. It
 * could handle way nodes better (see ChangesetReplacementElementIdSynchronizer).
 */
class ElementIdSynchronizer
{

public:

  ElementIdSynchronizer();
  virtual ~ElementIdSynchronizer() = default;

  /**
   * For all identical elements between two maps, this updates the identical elements in the second
   * map with the element IDs from the first map.
   *
   * @param map1 reference map to use as an element ID source and is not modified; Note that
   * ConstOsmMapPtr can't be used here due to the use of ElementHashVisitor, even though the map
   * is not modified.
   * @param map2 map to update element IDs on
   * @param elementType the type of element to synchronize the IDs of; all element types will have
   * IDs synchronized if no type is specified
   */
  virtual void synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2,
                           const ElementType& elementType = ElementType::Unknown);

  int getNumNodeIdsSynchronized() const { return _updatedNodeCtr; }
  int getNumWayIdsSynchronized() const { return _updatedWayCtr; }
  int getNumRelationIdsSynchronized() const { return _updatedRelationCtr; }
  int getNumTotalFeatureIdsSynchronized() const
  { return _updatedNodeCtr + _updatedWayCtr + _updatedRelationCtr; }

  void setUseNodeTagsForHash(bool use) { _useNodeTagsForHash = use; }

protected:

  // see ElementHashVisitor
  bool _useNodeTagsForHash;
  int _coordinateComparisonSensitivity;

  OsmMapPtr _map1;
  OsmMapPtr _map2;

  WayNodeCriterion _wayNodeCrit;

  // see ElementHashVisitor
  QMap<QString, ElementId> _map1HashesToElementIds;
  QMap<ElementId, QString> _map1ElementIdsToHashes;
  QSet<std::pair<ElementId, ElementId>> _map1Dupes;
  QMap<QString, ElementId> _map2HashesToElementIds;
  QMap<ElementId, QString> _map2ElementIdsToHashes;
  QSet<std::pair<ElementId, ElementId>> _map2Dupes;

  QSet<ElementId> _syncedElementIds;

  int _updatedNodeCtr;
  int _updatedWayCtr;
  int _updatedRelationCtr;

  /*
   * Calculates the element unique hashes used for comparison
   */
  void _calcElementHashes(const OsmMapPtr& map, QMap<QString, ElementId>& hashesToElementIds,
                          QMap<ElementId, QString>& elementIdsToHashes) const;

  /*
   * Determines if two elements (one from each input map) are way nodes which don't have a way
   * parent in common.
   */
  bool _areWayNodesWithoutAWayInCommon(ElementPtr element1, ElementPtr element2);

  /*
   * Determines if two elements (one from each input map) belong to ways with types different enough
   * to prevent ID synchronization from occurring..
   */
  bool _areWayNodesInWaysOfMismatchedType(ElementPtr element1, ElementPtr element2);

  QSet<QString> _getHashesByElementType(const QMap<ElementId, QString>& hashesByElementId,
                                        const ElementType& elementType) const;

private:

  void _syncElementIds(const QSet<QString>& identicalHashes);
};

}

#endif // ELEMENT_ID_SYNCHRONIZER_H
