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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef REMOVEWAYBYEID_H
#define REMOVEWAYBYEID_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * @brief The RemoveWayByEid class is used to remove ways from an OsmMap
 */
class RemoveWayByEid : public OsmMapOperation
{
public:

  /**
   * @brief className gets the name of the class as a string
   * @return class name
   */
  static QString className() { return "hoot::RemoveWayByEid"; }
  virtual QString getName() const { return className(); }
  virtual QString getClassName() const override { return className(); }

  /**
   * @brief RemoveWayByEid is used to remove ways from maps
   * @param removeFully if true, way is removed from all relations in the map, then
   *                    removed from the map's way collection.
   */
  RemoveWayByEid(bool removeFully = false);

  /**
   * @brief RemoveWayByEid is used to remove ways from maps
   * @param wId ID of the way to remove
   * @param removeFully if true, way is removed from all relations in the map, then
   *                    removed from the map's way collection.
   */
  RemoveWayByEid(long wId, bool removeFully = false);

  virtual ~RemoveWayByEid() = default;

  /**
   * @brief apply Apply the operation to the given map
   * @param map the map to operate on
   */
  void apply(OsmMapPtr& map);

  /**
   * @brief setWayId set the ID of the way to remove on the apply() call
   * @param wId ID of the way to remove
   */
  void setWayId(long wId) { _wayIdToRemove = wId; }

  /**
   * @brief removeWay Remove the given way from the given map
   * @param map Map to operate on
   * @param wId ID of way to remove
   */
  static void removeWay(OsmMapPtr map, long wId);
  /**
   * @brief removeWayFully Removes the way from all relations and
   *                       then removes the way from the map.
   * @param map Map to operate on
   * @param wId ID of way to remove
   */
  static void removeWayFully(OsmMapPtr map, long wId);

  virtual QString getDescription() const { return "Removes a single way by element ID"; }

private:

  long _wayIdToRemove;
  bool _removeFully;

  void _removeWay(OsmMapPtr& map, long wId);
  void _removeWayFully(OsmMapPtr& map, long wId);
};

}

#endif // REMOVEWAYBYEID_H
