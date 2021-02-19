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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef POI_POLYGON_POLY_CRITERION_H
#define POI_POLYGON_POLY_CRITERION_H

// hoot
#include <hoot/core/criterion/ConflatableElementCriterion.h>

// Qt
#include <QStringList>

namespace hoot
{

/**
 * Identifies polygons for use with POI/Polygon conflation
 */
class PoiPolygonPolyCriterion : public ConflatableElementCriterion
{
public:

  static QString className() { return "hoot::PoiPolygonPolyCriterion"; }

  PoiPolygonPolyCriterion();
  virtual ~PoiPolygonPolyCriterion() = default;

  virtual bool isSatisfied(const ConstElementPtr& e) const override;

  virtual GeometryType getGeometryType() const
  { return GeometryType::Polygon; }

  virtual ElementCriterionPtr clone() { return ElementCriterionPtr(new PoiPolygonPolyCriterion()); }

  virtual QString getDescription() const { return ""; }

  virtual QString getName() const override { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual bool supportsSpecificConflation() const { return true; }

private:

  QStringList _tagIgnoreList;
};

}

#endif // POI_POLYGON_POLY_CRITERION_H
