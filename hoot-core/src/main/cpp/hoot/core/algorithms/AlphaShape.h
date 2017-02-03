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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __ALPHASHAPE_H__
#define __ALPHASHAPE_H__

// Boost
#include <boost/shared_ptr.hpp>

// Geos
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/Geometry.h>

// Qt
#include <QString>

// Standard
#include <map>
#include <memory>
#include <set>
#include <vector>

namespace Tgs
{
class DelaunayTriangulation;
class DisjointSet;
class Edge;
class Face;
}

namespace hoot
{
using namespace boost;
using namespace geos;
using namespace std;

class FaceGroup;
class OsmMap;
class Way;

/**
 * Technically a Alpha complex, not an Alpha Shape, but the literature seems to alternate between
 * the terms.
 *
 */
class AlphaShape
{
public:
  AlphaShape(double alpha);

  shared_ptr<geos::geom::Geometry> toGeometry();

  void insert(const std::vector< std::pair<double, double> >& points);

  QString toString();

  shared_ptr<OsmMap> toOsmMap();

private:

  double _alpha;

  std::auto_ptr<Tgs::DelaunayTriangulation> _dt;
  set< pair<double, double> > _outsidePoint;

  shared_ptr<hoot::Way> _addFaceAsWay(const Tgs::Face *face, shared_ptr<OsmMap> map);

  shared_ptr<geos::geom::Polygon> _convertFaceToPolygon(const Tgs::Face& face) const;

  // The root group represents empty space
  // first level children represent filled space
  // second level children are empty
  // third filled
  // etc, alternating at each level between filled and empty
  shared_ptr<OsmMap> _groupFaces();

  bool _isBoundary(const Tgs::Edge& e) const;

  /**
   * Returns true if the face is inside an alpha shape.
   */
  bool _isInside(const Tgs::Face &face) const;

  /**
   * Returns true if this edge is part of the artificial outer bounds of the triangulation.
   */
  bool _isOutsideEdge(const Tgs::Edge& e) const;

  bool _isTooLong(const Tgs::Edge& e) const;

  shared_ptr<geos::geom::Geometry> _validateGeometry(const shared_ptr<geos::geom::Geometry>& g);
};

}

#endif // __ALPHASHAPE_H__
