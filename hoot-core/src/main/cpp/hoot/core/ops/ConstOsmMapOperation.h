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
#ifndef CONSTOSMMAPOPERATION_H
#define CONSTOSMMAPOPERATION_H

#include "OsmMapOperation.h"

namespace hoot
{

class OsmMap;

/**
 * Modifies an OsmMap in some way, but don't replace it.
 */
class ConstOsmMapOperation : public OsmMapOperation
{
public:

  static std::string className() { return "hoot::ConstOsmMapOperation"; }

  virtual ~ConstOsmMapOperation() {}

  virtual void apply(boost::shared_ptr<OsmMap>& map) { apply((const boost::shared_ptr<OsmMap>&)map); }

  /**
   * This guarantees that we won't actually change the map pointer, although the map itself may
   * still be changed.
   */
  virtual void apply(const boost::shared_ptr<OsmMap>& map) = 0;

};

}

#endif // CONSTOSMMAPOPERATION_H
