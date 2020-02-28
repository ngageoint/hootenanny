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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef CALCULATEMAPBOUNDSVISITOR_H
#define CALCULATEMAPBOUNDSVISITOR_H

#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/ConstElementVisitor.h>

/**
 * A visitor for finding the bounds of a map, based on the old OsmMap::calculateBounds
 * method. As with the original function, this uses map nodes to generate
 * the envelope.
 */
namespace hoot
{

class CalculateMapBoundsVisitor : public ConstElementVisitor
{
public:

  static std::string className() { return "hoot::CalculateMapBoundsVisitor"; }

  CalculateMapBoundsVisitor();

  OGREnvelope getBounds() { return _envelope; }

  // Note: should only visit nodes when calculating bounds
  virtual void visit(const std::shared_ptr<const Element>& e);

  // Convenient way to get bounds
  static OGREnvelope getBounds(const OsmMapPtr& map);
  static OGREnvelope getBounds(const ConstOsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const OsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const ConstOsmMapPtr& map);

  virtual QString getDescription() const { return "Calculates the extent of a map"; }

  virtual std::string getClassName() const { return className(); }

private:

  OGREnvelope _envelope;
};

}

#endif // CALCULATEMAPBOUNDSVISITOR_H
