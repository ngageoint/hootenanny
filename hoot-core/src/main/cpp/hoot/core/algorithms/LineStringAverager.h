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

#ifndef LINESTRINGAVERAGER_H
#define LINESTRINGAVERAGER_H

// GEOS
#include <geos/geom/LineString.h>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{

class LineStringAverager
{
public:
  LineStringAverager(boost::shared_ptr<geos::geom::LineString> l1,
                     boost::shared_ptr<geos::geom::LineString> l2);

  boost::shared_ptr<geos::geom::LineString> average();

  static boost::shared_ptr<geos::geom::LineString> average(boost::shared_ptr<geos::geom::LineString> l1,
                                                           boost::shared_ptr<geos::geom::LineString> l2);

protected:
  boost::shared_ptr<geos::geom::LineString> _l1;
  boost::shared_ptr<geos::geom::LineString> _l2;

  geos::geom::Coordinate _merge(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2);

  geos::geom::Coordinate _moveToLine(const geos::geom::Coordinate& c1, const geos::geom::LineString* ls);
};

}

#endif // LINESTRINGAVERAGER_H
