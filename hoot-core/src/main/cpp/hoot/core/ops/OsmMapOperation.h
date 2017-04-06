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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef OsmMapOperation_H
#define OsmMapOperation_H

// Standard
#include <string>

// Boost
#include <boost/any.hpp>

// Tgs
#include <tgs/SharedPtr.h>

//Qt
#include <QString>

namespace hoot
{
using namespace std;
using namespace boost;

class OsmMap;

/**
 * Modifies an OsmMap in some way.
 */
class OsmMapOperation
{
public:

  static string className() { return "hoot::OsmMapOperation"; }

  virtual ~OsmMapOperation() {}

  /**
   * The operation may make changes to any aspect of the map including creating a new map and
   * assigning the new map to the shared pointer. The only guarantee is that the "map" parameter
   * will point to a valid OsmMap upon completion.
   */
  virtual void apply(boost::shared_ptr<OsmMap>& map) = 0;

  /**
   * Allows for returning the result of some calculation done as a result of the map operation.
   *
   * @return boost any pointer to the result data
   */
  virtual any getResult() { any ptr; return ptr; }

  /**
    Returns a string representation of the operation
    */
  virtual QString toString() { return ""; }

};

}

#endif // OsmMapOperation_H
