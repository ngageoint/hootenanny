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

#ifndef LINESTRINGAVERAGER_H
#define LINESTRINGAVERAGER_H

// GEOS
#include <geos/geom/LineString.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{
  using namespace geos::geom;

class LineStringAverager
{
public:
  LineStringAverager(boost::shared_ptr<LineString> l1, boost::shared_ptr<LineString> l2);

  boost::shared_ptr<LineString> average();

  static boost::shared_ptr<LineString> average(boost::shared_ptr<LineString> l1, boost::shared_ptr<LineString> l2);

protected:
  boost::shared_ptr<LineString> _l1;
  boost::shared_ptr<LineString> _l2;

  Coordinate _merge(const Coordinate& c1, const Coordinate& c2);

  Coordinate _moveToLine(const Coordinate& c1, const LineString* ls);
};

}

#endif // LINESTRINGAVERAGER_H
