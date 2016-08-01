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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CALCULATEBOUNDSVISITOR_H
#define CALCULATEBOUNDSVISITOR_H

#include <hoot/core/OsmMap.h>
#include <hoot/core/visitors/ElementConstOsmMapVisitor.h>
#include <hoot/core/visitors/SingleStatistic.h>

/**
 * A visitor for finding the bounds of a map, based on the old OsmMap::calculateBounds
 * method. As with the original function, this uses map nodes to generate
 * the envelope.
 */
namespace hoot
{

class CalculateBoundsVisitor : public ElementConstOsmMapVisitor
{
public:
  static string className() { return "hoot::CalculateBoundsVisitor"; }

  CalculateBoundsVisitor();

  OGREnvelope getBounds() { return _envelope; }

  virtual void setOsmMap(const OsmMap* map) { _map = map; }

  // Note: should only visit nodes when calculating bounds
  virtual void visit(const shared_ptr<const Element>& e);

  // Convenient way to get bounds
  static OGREnvelope getBounds(const OsmMapPtr& map);
  static OGREnvelope getBounds(const ConstOsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const OsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const ConstOsmMapPtr& map);

private:

  const OsmMap* _map;
  OGREnvelope _envelope;
};

} // end namespace hoot

#endif // CALCULATEBOUNDSVISITOR_H
