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
 * @copyright Copyright (C) 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ADMINISTRATIVE_BOUNDARY_CRITERION_H
#define ADMINISTRATIVE_BOUNDARY_CRITERION_H

// hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * Identifies administrative boundary relations
 */
class AdministrativeBoundaryCriterion : public ConflatableElementCriterion
{
public:

  static std::string className() { return "hoot::AdministrativeBoundaryCriterion"; }

  AdministrativeBoundaryCriterion();

  /**
   * @see ElementCriterion
   */
  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual GeometryType getGeometryType() const { return GeometryType::Polygon; }

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new AdministrativeBoundaryCriterion()); }

  virtual QString getDescription() const { return "Identifies administrative boundaries"; }

  virtual QString toString() const override
  { return QString::fromStdString(className()).remove("hoot::"); }

  virtual bool supportsSpecificConflation() const { return true; }

private:


};

}

#endif // ADMINISTRATIVE_BOUNDARY_CRITERION_H
