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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef COLLECTION_RELATION_CRITERION_H
#define COLLECTION_RELATION_CRITERION_H

// hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>

namespace hoot
{

/**
 * Identifies collection relations
 */
class CollectionRelationCriterion : public ConflatableElementCriterion
{
public:

  static QString className() { return "hoot::CollectionRelationCriterion"; }

  CollectionRelationCriterion() = default;
  virtual ~CollectionRelationCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  // TODO: should this be linear instead?
  virtual GeometryType getGeometryType() const { return GeometryType::Polygon; }

  virtual bool supportsSpecificConflation() const { return true; }

  virtual ElementCriterionPtr clone()
  { return ElementCriterionPtr(new CollectionRelationCriterion()); }

  virtual QString getDescription() const { return "Identifies collection relations"; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }
};

}

#endif // COLLECTION_RELATION_CRITERION_H
