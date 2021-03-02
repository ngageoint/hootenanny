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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef RAILWAYCRITERION_H
#define RAILWAYCRITERION_H

// hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * A criterion that will match linear railways.
 *
 * @todo rename to LinearRailwayCriterion?
 */
class RailwayCriterion : public ConflatableElementCriterion
{
public:

  static QString className() { return "hoot::RailwayCriterion"; }

  RailwayCriterion() = default;
  virtual ~RailwayCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual GeometryType getGeometryType() const { return GeometryType::Line; }

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new RailwayCriterion()); }

  virtual QString getDescription() const { return "Identifies linear railways"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual bool supportsSpecificConflation() const { return true; }

  virtual QStringList getChildCriteria() const;
};

}

#endif // RAILWAYCRITERION_H
