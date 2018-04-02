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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef MAPCOMPARATOR_H
#define MAPCOMPARATOR_H

// Boost
#include <boost/shared_ptr.hpp>

namespace hoot
{

class OsmMap;

/**
 * Compares two maps.
 */
class MapComparator
{
public:

  MapComparator();

  /**
   * Returns true if the maps are essentially the same. Minor differences in node locations are
   * ignored.
   */
  bool isMatch(boost::shared_ptr<OsmMap> ref, boost::shared_ptr<OsmMap> test);

  void setIgnoreUUID() { _ignoreUUID = true; }

  void setUseDateTime() { _useDateTime = true; }

private:

  bool _ignoreUUID;
  bool _useDateTime;
};

}

#endif // MAPCOMPARATOR_H
