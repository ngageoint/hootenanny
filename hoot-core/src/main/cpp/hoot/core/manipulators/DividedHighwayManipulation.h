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

#ifndef DIVIDEDHIGHWAYMANIPULATION_H
#define DIVIDEDHIGHWAYMANIPULATION_H

#include "Manipulation.h"

// GEOS
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/util/Units.h>

namespace hoot
{
  class Way;

  using namespace geos::geom;

class DividedHighwayManipulation : public Manipulation
{
public:
  /**
   * Left and right aren't truly the left and right, it is just easer for me to think about than
   * v1 & v2.
   * @param threshold Error threshold of the input data in meters.
   */
  DividedHighwayManipulation(long leftId, long rightId, long midId, boost::shared_ptr<const OsmMap> map,
                             Meters threshold);

  virtual bool isValid(boost::shared_ptr<const OsmMap> map) const;

  virtual void applyManipulation(boost::shared_ptr<OsmMap> wm, set<ElementId>& impactedElements,
                                               set<ElementId>& newElements) const;

  virtual double calculateProbability(boost::shared_ptr<const OsmMap> map) const;

  virtual double calculateScore(boost::shared_ptr<const OsmMap> map) const;

  virtual const set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr& map) const;

  virtual double getProbabilityEstimate() const { return _p; }

  virtual double getScoreEstimate() const { return _estimate; }

  virtual QString toString() const;

private:
  mutable set<ElementId> _impactedElements;
  mutable set<ElementId> _newWays;
  long _left, _right, _mid;
  Meters _vectorError;
  mutable double _p;

  /**
   * Checks to see if a connector is necessary and then add it to the map if appropriate.
   */
  void _addConnector(boost::shared_ptr<OsmMap> map, long nodeId) const;

  /**
   * Calculates the normalized vector between the nearest points on g1 and g2.
   */
  Coordinate _nearestVector(boost::shared_ptr<Geometry> g1, boost::shared_ptr<Geometry> g2) const;

  void _createStub(boost::shared_ptr<OsmMap> map, boost::shared_ptr<Way> oneway, long nodeId) const;

  void _mergeInbound(boost::shared_ptr<OsmMap> map, boost::shared_ptr<Way> inbound, long nodeId) const;

  double _dotProduct(const Coordinate& c1, const Coordinate& c2) const;
};

}

#endif // DIVIDEDHIGHWAYMANIPULATION_H
