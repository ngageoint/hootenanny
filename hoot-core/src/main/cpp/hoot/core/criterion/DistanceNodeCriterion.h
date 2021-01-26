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

#ifndef DISTANCENODECRITERION_H
#define DISTANCENODECRITERION_H

// GEOS
#include <geos/geom/Coordinate.h>

// Hoot
#include <hoot/core/util/Units.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

class Element;

/**
 * isSatisfied returns true if an element is within the specified distance of the given center
 */
class DistanceNodeCriterion : public ElementCriterion, public Configurable
{
public:

  static QString className() { return "hoot::DistanceNodeCriterion"; }

  DistanceNodeCriterion() = default;
  DistanceNodeCriterion(geos::geom::Coordinate center, Meters distance);
  virtual ~DistanceNodeCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const;

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new DistanceNodeCriterion(_center, _distance)); }

  virtual QString getDescription() const
  { return "Determines if an element is within the specified distance of a point"; }

  virtual void setConfiguration(const Settings& s);

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

private:

  geos::geom::Coordinate _center;
  Meters _distance;
};

}

#endif // DISTANCENODECRITERION_H
