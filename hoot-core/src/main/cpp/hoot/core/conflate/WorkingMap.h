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
#ifndef WORKINGMAP_H
#define WORKINGMAP_H

// Boost
#include <boost/shared_ptr.hpp>

// Hoot
#include <hoot/core/util/Units.h>
namespace hoot
{
  class OsmMap;
}

// Standard
#include <deque>

namespace hoot
{

class Manipulation;
class Status;

using namespace std;
using namespace boost;

/**
 * The WorkingMap represents one possible solution to the conflation problem. This should only be
 * modified by the initial creator of the WorkingMap.
 */
class WorkingMap
{
public:

  WorkingMap(const WorkingMap& map);

  WorkingMap(boost::shared_ptr<OsmMap> map);

  WorkingMap(boost::shared_ptr<const WorkingMap> map, boost::shared_ptr<const Manipulation> manipulation);

  WorkingMap();

  double calculatePotential() const;

  boost::shared_ptr<const OsmMap> getMap() const;

  boost::shared_ptr<OsmMap> getMap() { return _map; }

  boost::shared_ptr<OsmMap> takeMap() const;

  double getScore() const;

  void setScore(double s) { _score = s; }

protected:

  mutable double _score;

  mutable boost::shared_ptr<OsmMap> _map;
  boost::shared_ptr<const WorkingMap> _baseWorking;
  boost::shared_ptr<const Manipulation> _manipulation;

  static deque< pair< const WorkingMap*, boost::shared_ptr<OsmMap> > > _mapCache;

  Meters _sumWayLengths(Status status) const;

  int _countIntersections(Status status) const;
};

}

#endif // WORKINGMAP_H
