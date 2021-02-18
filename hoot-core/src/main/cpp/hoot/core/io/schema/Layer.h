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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LAYER_H
#define LAYER_H

// geos
#include <geos/geom/Geometry.h>

// Qt
#include <QString>

// Standard
#include <vector>

namespace hoot
{

class FeatureDefinition;

class Layer
{
public:
  Layer();

  const std::shared_ptr<const FeatureDefinition>& getFeatureDefinition() const { return _definition; }

  geos::geom::GeometryTypeId getGeometryType() const { return _geometryType; }

  QString getName() const { return _name; }

  void setFeatureDefinition(const std::shared_ptr<FeatureDefinition>& fd) { _definition = fd; }

  void setGeometryType(geos::geom::GeometryTypeId geometryType) { _geometryType = geometryType; }

  void setName(const QString& name) { _name = name; }

  QString getFdName() const { return _fdname; }

  void setFdName(const QString& name) { _fdname = name; }

private:
  std::shared_ptr<const FeatureDefinition> _definition;
  geos::geom::GeometryTypeId _geometryType;
  QString _name;
  QString _fdname;
};

}

#endif // LAYER_H
