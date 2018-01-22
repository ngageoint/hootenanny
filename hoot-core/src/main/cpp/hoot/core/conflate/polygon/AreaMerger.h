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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 * @copyright Copyright (C) 2016, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef AREAMERGER_H
#define AREAMERGER_H


// hoot
#include <hoot/core/OsmMap.h>


namespace hoot
{

class OsmMap;

/**
 * Merges two non-building areas
 *
 * Adapted from PoiPolygonMerger; may need some additional work
 */
class AreaMerger
{

public:

  AreaMerger();

  /**
   * Merges to non-building areas
   *
   * @param map an OSM map containing two non-building areas
   */
  static void merge(OsmMapPtr map);

};

}

#endif // AREAMERGER_H
