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
 * @copyright Copyright (C) 2016, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef PROJECTTOGEOGRAPHICVISITOR_H
#define PROJECTTOGEOGRAPHICVISITOR_H

// GDAL
#include <ogr_geometry.h>
#include <ogr_spatialref.h>

#include "ElementOsmMapVisitor.h"

namespace hoot
{

class ReprojectCoordinateFilter;

class ProjectToGeographicVisitor : public ElementOsmMapVisitor
{
public:
  static std::string className() { return "hoot::ProjectToGeographicVisitor"; }

  ProjectToGeographicVisitor();
  ~ProjectToGeographicVisitor();

  void initialize(boost::shared_ptr<OGRSpatialReference>& projection);

  virtual void visit(const boost::shared_ptr<Element>& e);

private:

  OGRCoordinateTransformation* _transform;
 boost::shared_ptr<ReprojectCoordinateFilter> _rcf;
};

}

#endif // PROJECTTOGEOGRAPHICVISITOR_H
