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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef IMPLICIT_POI_POLYGON_TYPE_TAGGER_H
#define IMPLICIT_POI_POLYGON_TYPE_TAGGER_H

// hoot
#include <hoot/rnd/visitors/ImplicitPoiTypeTagger.h>
#include <hoot/core/criterion/poi-polygon/PoiPolygonPolyCriterion.h>

namespace hoot
{

/**
 * Adds tags implicitly derived from feature names to POIs and polygons
 */
class ImplicitPoiPolygonTypeTagger : public ImplicitPoiTypeTagger
{
public:

  static QString className() { return "hoot::ImplicitPoiPolygonTypeTagger"; }

  ImplicitPoiPolygonTypeTagger() = default;
  ImplicitPoiPolygonTypeTagger(const QString& databasePath);
  virtual ~ImplicitPoiPolygonTypeTagger() = default;

  virtual QString getDescription() const
  { return "Adds tags to POIs and polygons implicitly derived from their names"; }

  virtual QString getInitStatusMessage() const { return "Adding tags to POIs and polygons..."; }

  virtual QString getCompletedStatusMessage() const
  { return "Added " + QString::number(_numAffected) + " tags to POIs and polygons"; }

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

protected:

  virtual bool _visitElement(const ElementPtr& e);

private:

  PoiPolygonPolyCriterion _polyCrit;
  bool _inABuildingOrPoiCategory;

  bool _elementIsATaggablePolygon(const ElementPtr& e);
};

}

#endif // IMPLICIT_POI_POLYGON_TYPE_TAGGER_H
