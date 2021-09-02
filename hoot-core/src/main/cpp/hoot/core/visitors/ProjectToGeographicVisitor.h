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
#ifndef PROJECTTOGEOGRAPHICVISITOR_H
#define PROJECTTOGEOGRAPHICVISITOR_H

// GDAL
#include <ogr_geometry.h>
#include <ogr_spatialref.h>

#include <hoot/core/visitors/ElementVisitor.h>

namespace hoot
{

class ReprojectCoordinateFilter;

class ProjectToGeographicVisitor : public ElementVisitor
{
public:

  static QString className() { return "ProjectToGeographicVisitor"; }

  ProjectToGeographicVisitor();
  ~ProjectToGeographicVisitor();

  void initialize(const std::shared_ptr<OGRSpatialReference>& projection);

  void visit(const std::shared_ptr<Element>& e) override;

  QString getDescription() const override
  { return "Projects features to a geographic coordinate system"; }
  QString getName() const  override{ return className(); }
  QString getClassName() const override { return className(); }

private:

  OGRCoordinateTransformation* _transform;
  std::shared_ptr<ReprojectCoordinateFilter> _rcf;
};

}

#endif // PROJECTTOGEOGRAPHICVISITOR_H
