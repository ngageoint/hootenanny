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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef WAYCLEANER_H
#define WAYCLEANER_H

#include <hoot/core/OsmMap.h>

namespace hoot
{

/**
 * Cleans ways
 */
class WayCleaner
{
  public:

  static std::string className() { return "hoot::WayCleaner"; }

  static unsigned int logWarnCount;

  /**
     * Determines whether a way has more than one node with the same coordinate (nodes with
     * different ID's and having the same exact coordinate).
     *
     * @param way the way to be examined for duplicate coordinates
     * @param map the map owning the way being examined
     * @param logDetails if true, details about the duplicate coordinates are logged
     * @return true if the way has coordinates; false otherwise
     */
    static bool hasDuplicateCoords(ConstWayPtr way, const OsmMap& map,
                                   const bool logDetails = false);

    /**
     * Determines whether a way has more than one node with the same coordinate (nodes with
     * different ID's and having the same exact coordinate).
     *
     * @param way the way to be examined for duplicate nodes
     * @param logDetails if true, details about the duplicate nodes are logged
     * @return true if the way has duplicate nodes; false otherwise
     */
    static bool hasDuplicateNodes(ConstWayPtr way, const bool logDetails = false);

    /**
     * Determines whether a way is of zero length
     *
     * Only returns true for ways of size two.
     *
     * @param way way to be examined
     * @param map the map owning the way to be examined
     * @return true if the way is of zero length; false otherwise
     */
    static bool isZeroLengthWay(ConstWayPtr way, const ConstOsmMapPtr& map);

    /**
     * Removes duplicate nodes and coordinates from a way
     *
     * @param way the way to be cleaned
     * @param map the map owning the way to be cleaned
     */
    static void cleanWay(WayPtr way, const ConstOsmMapPtr& map);

    /**
     * Cleans an unmodifiable way by removing duplicate nodes and coordinate from a copy of it
     * and returning the copy.
     *
     * @param way the way to be cleaned; will be copied
     * @pdddaram map the map owning the way to be cleaned
     * @return an unmodifiable cleaned way
     * @see cleanWay
     */
    static ConstWayPtr cleanWay(ConstWayPtr way, const ConstOsmMapPtr& map);

    /**
     * Cleans unmodifiable ways by removing duplicate nodes and coordinate from copies of them and
     * returning the copies
     *
     * Zero length ways are removed.  It copies the input way and returns unmodifiable way to allow
     * the method to be used inside conflation methods expecting const ways.
     *
     * @param ways the ways to be cleaned; will be copied
     * @param map the map owning the ways to be cleaned
     * @return a collection of unmodifiable cleaned ways
     */
    static vector<ConstWayPtr> cleanWays(const vector<ConstWayPtr>& ways, const ConstOsmMapPtr& map);
};

}

#endif // WAYCLEANER_H
