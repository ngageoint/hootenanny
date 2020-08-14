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

namespace hoot
{

/**
 * This class allows for synchronizing element IDs between to maps that have identical features.
 */
class ElementIdSynchronizer
{

public:

  ElementIdSynchronizer();

  /**
   * For all identical elements between two maps, this updates the identical elements in the second
   * map with the element IDs from the first map.
   *
   * @param map1 reference map to use as an element ID source and is not modified; Note that
   * ConstOsmMapPtr can't be used here due to the use of ElementHashVisitor, even though the map
   * is not modified.
   * @param map2 map to update element IDs on
   */
  void synchronize(const OsmMapPtr& map1, const OsmMapPtr& map2);

  int getNumNodeIdsSynchronized() const { return _updatedNodeCtr; }
  int getNumWayIdsSynchronized() const { return _updatedWayCtr; }
  int getNumRelationIdsSynchronized() const { return _updatedRelationCtr; }
  int getNumTotalFeatureIdsSynchronized() const
  { return _updatedNodeCtr + _updatedWayCtr + _updatedRelationCtr; }

private:

  int _updatedNodeCtr;
  int _updatedWayCtr;
  int _updatedRelationCtr;

  QMap<QString, ElementId> _calcElementHashes(const OsmMapPtr& map);
};

}

#endif // ELEMENT_ID_SYNCHRONIZER_H
