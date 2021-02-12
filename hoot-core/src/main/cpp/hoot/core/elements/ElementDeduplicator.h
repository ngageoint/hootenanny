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
 * @copyright Copyright (C) 2020 Maxar (http://www.maxar.com/)
 */

#ifndef ELEMENT_DEDUPLICATOR_H
#define ELEMENT_DEDUPLICATOR_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/ElementCriterion.h>

namespace hoot
{

/**
 * This class de-duplicates features within a map or across multiple maps. It ignores element IDs,
 * versions and other OSM metadata.
 *
 * This has a more strict definition of way duplicates than DuplicateWayRemover and will only remove
 * exact duplicates.
 *
 * @todo This doesn't handle way nodes as well as it should. See
 * ChangesetReplacementElementIdSynchronizer.
 * @todo its convoluted to have both a boolean and a criterion for each element type; removal should
 * be able to all be done with a single criterion and all elements, regardless of type, can be
 * processed in one pass...will just take a decent amount of refactoring
 * @todo Is there some overlap here with ElementComparer?
 * @todo Is there any way this could replace DuplicateNodeRemover? I don't think so...
 */
class ElementDeduplicator
{

public:

  ElementDeduplicator();

  /**
   * Removes intra-map duplicate elements from a map. Ignores element ID, version, changeset,
   * and metadata tags
   *
   * @param map the map to de-duplicate
   */
  void dedupe(OsmMapPtr map);

  /**
   * Removes intra-map and inter-map duplicate elements from two maps. Ignores element ID, version,
   * changeset, and metadata tags. The first map's elements are kept when inter-map duplicate
   * elements are found.
   *
   * @param map1 the first map to de-duplicate
   * @param map2 the second map to de-duplicate
   */
  void dedupe(OsmMapPtr map1, OsmMapPtr map2);

  /**
   * Uses ElementHashVisitor to assign unique hashes to elements and also retrieves the element
   * IDs of any duplicates found within a single map.
   *
   * @param map the map owning the elements to assign hashes to
   * @param hashes a collection of hashes to update
   * @param duplicates a collection of duplicate elements found
   */
  static void calculateDuplicateElements(
    OsmMapPtr map, QMap<QString, ElementId>& hashes,
    QSet<std::pair<ElementId, ElementId>>& duplicates,
    const int coordinateComparisonSensitivity =
      ConfigOptions().getNodeComparisonCoordinateSensitivity());

  int getMap1DuplicateNodesRemoved() const { return _map1DuplicateNodesRemoved; }
  int getMap1DuplicateWaysRemoved() const { return _map1DuplicateWaysRemoved; }
  int getMap1DuplicateRelationsRemoved() const { return _map1DuplicateRelationsRemoved; }
  int getMap1DuplicateTotalFeaturesRemoved() const
  { return _map1DuplicateNodesRemoved + _map1DuplicateWaysRemoved + _map1DuplicateRelationsRemoved; }
  int getMap2DuplicateNodesRemoved() const { return _map2DuplicateNodesRemoved; }
  int getMap2DuplicateWaysRemoved() const { return _map1DuplicateWaysRemoved; }
  int getMap2DuplicateRelationsRemoved() const { return _map1DuplicateRelationsRemoved; }
  int getMap2DuplicateTotalFeaturesRemoved() const
  { return _map2DuplicateNodesRemoved + _map2DuplicateWaysRemoved + _map2DuplicateRelationsRemoved; }

  void setDedupeIntraMap(bool dedupe) { _dedupeIntraMap = dedupe; }
  void setDedupeNodes(bool dedupe) { _dedupeNodes = dedupe; }
  void setDedupeWays(bool dedupe) { _dedupeWays = dedupe; }
  void setDedupeRelations(bool dedupe) { _dedupeRelations = dedupe; }
  void setNodeCriterion(ElementCriterionPtr crit) { _nodeCrit = crit; }
  void setWayCriterion(ElementCriterionPtr crit) { _wayCrit = crit; }
  void setRelationCriterion(ElementCriterionPtr crit) { _relationCrit = crit; }
  void setFavorMoreConnectedWays(bool favor) { _favorMoreConnectedWays = favor; }

private:

  // If true, and two maps are being de-duped duplicates within each single map are also removed.
  // This setting is ignored when de-deuping one map as intra-map de-deduplication is always done
  // in that case.
  bool _dedupeIntraMap;

  // allows for controlling the element types being de-duped
  bool _dedupeNodes;
  bool _dedupeWays;
  bool _dedupeRelations;

  // criterion for each element type to further restrict what gets replaced
  ElementCriterionPtr _nodeCrit;
  ElementCriterionPtr _wayCrit;
  ElementCriterionPtr _relationCrit;

  // If true when ways are deduped, the way sharing more nodes with other ways is kept over the one
  // with less shared nodes.
  bool _favorMoreConnectedWays;

  int _map1DuplicateNodesRemoved;
  int _map1DuplicateWaysRemoved;
  int _map1DuplicateRelationsRemoved;
  int _map2DuplicateNodesRemoved;
  int _map2DuplicateWaysRemoved;
  int _map2DuplicateRelationsRemoved;

  void _validateInputs();

  /*
   * Converts pairs of duplicated features' element IDs to a collection of element IDs sorted by
   * element type for removal purposes; the second element is arbitarily selected for removal
   */
  QMap<ElementType::Type, QSet<ElementId>> _dupesToElementIds(
    const QSet<std::pair<ElementId, ElementId>>& duplicates);

  /*
   * Similar to _dupesToElementIds except for ways if _favorMoreConnectedWays is enabled, this
   * will use way connectedness as a factor in determining which element of the pair to mark for
   * removal
   */
  void _dupesToElementIdsCheckMap(
    const QSet<std::pair<ElementId, ElementId>>& duplicates, OsmMapPtr map1, OsmMapPtr map2,
    QMap<ElementType::Type, QSet<ElementId>>& elementsToRemove,
    QMap<ElementId, QString>& elementIdsToRemoveFromMap);

  /*
   * Converts element hashes of duplicated features' element IDs to a collection of element IDs
   * sorted by element type for removal purposes; for ways if _favorMoreConnectedWays is enabled,
   * this will use way connectedness as a factor in determining which element of the pair to mark
   * for removal
   */
  void _dupeHashesToElementIdsCheckMap(
    const QSet<QString>& sharedHashes, OsmMapPtr map1, OsmMapPtr map2,
    const QMap<QString, ElementId>& map1Hashes, const QMap<QString, ElementId>& map2Hashes,
    QMap<ElementType::Type, QSet<ElementId>>& elementsToRemove,
    QMap<ElementId, QString>& elementIdsToRemoveFromMap);

  void _removeElements(const QSet<ElementId>& elementsToRemove, OsmMapPtr map);

  /*
   * Similar to _removeElements except for ways if _favorMoreConnectedWays is enabled, this
   * will use way connectedness as a factor in determining which element of the pair to remove
   */
  void _removeWaysCheckMap(
    const QSet<ElementId>& waysToRemove, OsmMapPtr map1, OsmMapPtr map2,
    const QMap<ElementId, QString>& elementIdsToRemoveFromMap);

  /*
   * Determines if two elements belong to ways with different types.
   */
  static bool _areWayNodesInWaysOfMismatchedType(
    ElementPtr element1, ElementPtr element2, OsmMapPtr map);

  static QSet<std::pair<ElementId, ElementId>>_filterOutNonDupeWayNodes(
    const QSet<std::pair<ElementId, ElementId>>& dupes, OsmMapPtr map);
};

}

#endif // ELEMENT_DEDUPLICATOR_H
