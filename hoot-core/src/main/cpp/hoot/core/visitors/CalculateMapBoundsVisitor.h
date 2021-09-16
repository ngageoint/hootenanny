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
 * This will properly maintain the copyright information. Maxar
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef CALCULATEMAPBOUNDSVISITOR_H
#define CALCULATEMAPBOUNDSVISITOR_H

// hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/visitors/ConstElementVisitor.h>

namespace hoot
{

/**
 * @brief The CalculateMapBoundsVisitor class is a visitor for finding the bounds of a map, based on
 * the old OsmMap::calculateBounds method.
 *
 * As with the original function, this uses map nodes to generate the envelope.
 */
class CalculateMapBoundsVisitor : public ConstElementVisitor
{
public:

  static QString className() { return "CalculateMapBoundsVisitor"; }

  CalculateMapBoundsVisitor() = default;
  ~CalculateMapBoundsVisitor() override = default;

  OGREnvelope getBounds() const { return _envelope; }

  // Note: should only visit nodes when calculating bounds
  void visit(const std::shared_ptr<const Element>& e) override;

  // Convenient way to get bounds
  static OGREnvelope getBounds(const OsmMapPtr& map);
  static OGREnvelope getBounds(const ConstOsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const OsmMapPtr& map);
  static geos::geom::Envelope getGeosBounds(const ConstOsmMapPtr& map);

  QString getDescription() const override { return "Calculates the extent of a map"; }
  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }

private:

  OGREnvelope _envelope;
};

}

#endif // CALCULATEMAPBOUNDSVISITOR_H
