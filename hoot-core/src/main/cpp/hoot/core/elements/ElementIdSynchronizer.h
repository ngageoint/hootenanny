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

#ifndef ELEMENT_ID_SYNCHRONIZER_H
#define ELEMENT_ID_SYNCHRONIZER_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/criterion/WayNodeCriterion.h>

namespace hoot
{

/**
 * This class allows for synchronizing element IDs between to maps that have identical features.
 * This works across multiple maps, so something like IdSwapOp won't work here.
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
   */
  virtual void synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2);

  int getNumNodeIdsSynchronized() const { return _updatedNodeCtr; }
  int getNumWayIdsSynchronized() const { return _updatedWayCtr; }
  int getNumRelationIdsSynchronized() const { return _updatedRelationCtr; }
  int getNumTotalFeatureIdsSynchronized() const
  { return _updatedNodeCtr + _updatedWayCtr + _updatedRelationCtr; }

  void setUseNodeTagsForHash(bool use) { _useNodeTagsForHash = use; }

protected:

  // see ElementHashVisitor
  bool _useNodeTagsForHash;

  OsmMapPtr _map1;
  OsmMapPtr _map2;

  WayNodeCriterion _wayNodeCrit;

  // TODO
  QMap<QString, ElementId> _map1HashesToElementIds;
  QMap<ElementId, QString> _map1ElementIdsToHashes;
  QMap<QString, ElementId> _map2HashesToElementIds;
  QMap<ElementId, QString> _map2ElementIdsToHashes;

  int _updatedNodeCtr;
  int _updatedWayCtr;
  int _updatedRelationCtr;

  /*
   * TODO
   */
  void _calcElementHashes(
    const OsmMapPtr& map, QMap<QString, ElementId>& hashesToElementIds,
    QMap<ElementId, QString>& elementIdsToHashes,
      const int coordinateComparisonSensitivity =
        ConfigOptions().getNodeComparisonCoordinateSensitivity());

  /*
   * TODO
   */
  bool _areWayNodesWithoutAWayInCommon(ElementPtr element1, ElementPtr element2);

  /*
   * TODO
   */
  bool _areWayNodesInWaysOfMismatchedType(ElementPtr element1, ElementPtr element2);
};

}

#endif // ELEMENT_ID_SYNCHRONIZER_H
