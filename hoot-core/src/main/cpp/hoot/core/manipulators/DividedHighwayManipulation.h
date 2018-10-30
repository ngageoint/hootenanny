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

#ifndef DIVIDEDHIGHWAYMANIPULATION_H
#define DIVIDEDHIGHWAYMANIPULATION_H

// GEOS
#include <geos/geom/Geometry.h>

// Hoot
#include <hoot/core/manipulators/Manipulation.h>
#include <hoot/core/util/Units.h>

namespace hoot
{

class Way;

class DividedHighwayManipulation : public Manipulation
{
public:
  /**
   * Left and right aren't truly the left and right, it is just easer for me to think about than
   * v1 & v2.
   * @param threshold Error threshold of the input data in meters.
   */
  DividedHighwayManipulation(long leftId, long rightId, long midId, ConstOsmMapPtr map,
                             Meters threshold);

  virtual bool isValid(ConstOsmMapPtr map) const;

  virtual void applyManipulation(OsmMapPtr wm, std::set<ElementId>& impactedElements,
                                               std::set<ElementId>& newElements) const;

  virtual double calculateProbability(ConstOsmMapPtr map) const;

  virtual double calculateScore(ConstOsmMapPtr map) const;

  virtual const std::set<ElementId>& getImpactedElementIds(const ConstOsmMapPtr& map) const;

  virtual double getProbabilityEstimate() const { return _p; }

  virtual double getScoreEstimate() const { return _estimate; }

  virtual QString toString() const;

private:
  mutable std::set<ElementId> _impactedElements;
  mutable std::set<ElementId> _newWays;
  long _left, _right, _mid;
  Meters _vectorError;
  mutable double _p;

  /**
   * Checks to see if a connector is necessary and then add it to the map if appropriate.
   */
  void _addConnector(OsmMapPtr map, long nodeId) const;

  /**
   * Calculates the normalized vector between the nearest points on g1 and g2.
   */
  geos::geom::Coordinate _nearestVector(boost::shared_ptr<geos::geom::Geometry> g1,
                                        boost::shared_ptr<geos::geom::Geometry> g2) const;

  void _createStub(OsmMapPtr map, WayPtr oneway, long nodeId) const;

  void _mergeInbound(OsmMapPtr map, WayPtr inbound, long nodeId) const;

  double _dotProduct(const geos::geom::Coordinate& c1, const geos::geom::Coordinate& c2) const;
};

}

#endif // DIVIDEDHIGHWAYMANIPULATION_H
