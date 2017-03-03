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
#ifndef VERTEXHAUSDORFFDISTANCE_H
#define VERTEXHAUSDORFFDISTANCE_H

// geos
namespace geos
{
  namespace geom
  {
    class Geometry;
  }
}

namespace hoot
{

using namespace geos::geom;

/**
 * Implements algorithm for computing a distance metric
 * which can be thought of as the "Vertex Hausdorff Distance".
 * This is the Hausdorff distance restricted to vertices for
 * one of the geometries.
 * Also computes two points of the Geometries which are separated by the computed distance.
 * <p>
 * <b>NOTE: This algorithm does NOT compute the full Hausdorff distance correctly, but
 * an approximation that is correct for a large subset of useful cases.
 * One important part of this subset is Linestrings that are roughly parallel to each other,
 * and roughly equal in length - just what is needed for line matching.
 * </b>
 *
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class VertexHausdorffDistance
{
public:

  VertexHausdorffDistance() { _distance = -1; }
  VertexHausdorffDistance(const Geometry& g1, const Geometry& g2);

  void compute(const Geometry& g1, const Geometry& g2);

  double getDistance() const { return _distance; }

private:

  double _distance;
};

}

#endif // VERTEXHAUSDORFFDISTANCE_H
