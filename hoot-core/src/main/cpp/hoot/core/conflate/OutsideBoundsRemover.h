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

#ifndef OUTSIDEBOUNDSREMOVER_H
#define OUTSIDEBOUNDSREMOVER_H

// GEOS
#include <geos/geom/Envelope.h>

// Standard
#include <set>

// TGS
#include <tgs/SharedPtr.h>

namespace hoot
{
  using namespace geos::geom;
  using namespace std;

  class OsmMap;

/**
 * Remove all ways outside the given bounds.
 */
class OutsideBoundsRemover
{
public:
  OutsideBoundsRemover(boost::shared_ptr<OsmMap> map, const Envelope& e, bool inverse = false);

  /**
   * Removes ways completely outside the given envelope.
   */
  static void removeWays(boost::shared_ptr<OsmMap> map, const Envelope& e, bool inverse = false);

  void removeWays();

protected:

  boost::shared_ptr<OsmMap> _inputMap;
  Envelope _envelope;
  bool _inverse;
};

}

#endif // OUTSIDEBOUNDSREMOVER_H
