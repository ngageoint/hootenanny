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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef __MULTI_LINE_STRING_LOCATION_H__
#define __MULTI_LINE_STRING_LOCATION_H__

// Hoot
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/algorithms/linearreference/WayLocation.h>

namespace hoot
{

class Relation;
class OsmMap;

/**
 * Represents a location on a multi-line feature
 */
class MultiLineStringLocation
{
  public:

    //I don't like having a default constructor here, but its needed by PertyWaySplitVisitor
    //for now.
    MultiLineStringLocation(){}

    /**
     * Creates a location along the multi-line string
     *
     * @param map map containing the multi-line string data
     * @param relation a multi-line string relation containing only ways; will throw if relation
     *        contains feature types other than ways
     * @param wayIndex the index of the way in the collection of ways in @a relation
     * @param wayLocation the location along the way at @a wayIndex where the multi-line string
     *        location resides
     */
    MultiLineStringLocation(boost::shared_ptr<const OsmMap> map, boost::shared_ptr<const Relation> relation,
                            const int wayIndex, const WayLocation& wayLocation);

    /**
     * Returns the collection of way sublines up to and including _wayLocation
     */
    WaySublineCollection getWaySublineString() const { return _waySublineString; }

    /**
     * Returns the location along the selected way in the multi-line string
     */
    WayLocation getWayLocation() const { return _wayLocation; }

    /**
     * Determines whether the location is valid
     */
    bool isValid() const { return _wayLocation.getSegmentIndex() != -1; }

  private:

    WayLocation _wayLocation;
    WaySublineCollection _waySublineString;
};

}

#endif
