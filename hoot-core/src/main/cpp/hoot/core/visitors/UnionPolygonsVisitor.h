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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef UNIONPOLYGONSVISITOR_H
#define UNIONPOLYGONSVISITOR_H

#include "ElementConstOsmMapVisitor.h"

// geos
#include <geos/geom/Geometry.h>

namespace hoot
{

/**
 * Unions all "area" elements that are visited to create a single geometry.
 *
 * @optimize This could be made more efficient by sacrificing a bit of RAM. See
 * AlphaShape::toGeometry() for an example implementation.
 */
class UnionPolygonsVisitor : public ElementConstOsmMapVisitor
{
public:

  static std::string className() { return "hoot::UnionPolygonsVisitor"; }

  UnionPolygonsVisitor();

  const boost::shared_ptr<geos::geom::Geometry>& getUnion() const { return _result; }

  virtual void visit(const boost::shared_ptr<const Element>& e);

private:

  boost::shared_ptr<geos::geom::Geometry> _result;
};

}

#endif // UNIONPOLYGONSVISITOR_H
