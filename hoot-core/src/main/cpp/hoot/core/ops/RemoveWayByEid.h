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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef REMOVEWAYBYEID_H
#define REMOVEWAYBYEID_H

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/ops/OsmMapOperation.h>

namespace hoot
{

/**
 * Removes ways from an map
 */
class RemoveWayByEid : public OsmMapOperation
{
public:

  static QString className() { return "RemoveWayByEid"; }

  /**
   * Constructor
   *
   * @param removeFully if true, way is removed from all relations in the map, then removed from the
   * map's way collection.
   */
  RemoveWayByEid(bool removeFully = false);
  /**
   * Constructor
   *
   * @param wId ID of the way to remove
   * @param removeFully if true, way is removed from all relations in the map, then removed from the
   * map's way collection.
   */
  RemoveWayByEid(long wId, bool removeFully = false);
  ~RemoveWayByEid() override = default;

  /**
   * see OsmMapOperation
   */
  void apply(OsmMapPtr& map) override;

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

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override { return "Removes a single way by element ID"; }

  void setWayId(long wId) { _wayIdToRemove = wId; }

private:

  long _wayIdToRemove;
  bool _removeFully;

  void _removeWay(const OsmMapPtr& map, long wId) const;
  void _removeWayFully(const OsmMapPtr& map, long wId) const;
};

}

#endif // REMOVEWAYBYEID_H
