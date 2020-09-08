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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __ALPHASHAPE_H__
#define __ALPHASHAPE_H__

// GEOS
#include <geos/geom/Geometry.h>
#include <geos/geom/MultiPolygon.h>
#include <geos/geom/Polygon.h>

// GDAL
#include <ogr_core.h>
class OGRSpatialReference;

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

class FaceGroup;
class OsmMap;
class Way;

typedef std::shared_ptr<geos::geom::Geometry> GeometryPtr;

/**
 * Representation of an Alpha Shape. Technically an Alpha complex, not an Alpha Shape, but the
 * literature seems to alternate between the terms.
 *
 * https://github.com/ngageoint/hootenanny/files/595246/Hootenanny.-.Alpha.Shape.2013-03-07.pptx
 * https://github.com/ngageoint/hootenanny/blob/master/docs/algorithms/AlphaShape.asciidoc
 * https://github.com/ngageoint/hootenanny/wiki/files/2010-B-01-AlphaShapes.pdf
 */
class AlphaShape
{
public:

  static std::string className() { return "hoot::AlphaShape"; }

  static int logWarnCount;

  /**
   * Constructor
   *
   * @param alpha tuning parameter which determines the makeup of the output shape
   */
  AlphaShape(double alpha = -1.0);

  /**
   * Converts this shape to a GEOS geometry
   *
   * @return a GEOS geometry
   */
  GeometryPtr toGeometry();

  /**
   * Inserts points which are used to build the shape
   *
   * @param points a collection of points
   */
  void insert(const std::vector<std::pair<double, double>>& points);

  /**
   * Returns a string representation of the shape
   *
   * @return a string
   */
  QString toString();

  /**
   * Returns the length of the longest face edge used to create the shape
   *
   * @return a length
   */
  double getLongestFaceEdge() const { return _longestFaceEdge; }

private:

  friend class AlphaShapeTest;

  // tuning parameter which determines the makeup of the output shape
  double _alpha;

  // size of the longest face edge
  mutable double _longestFaceEdge;

  //  Size of the problem set in meters in the x, y directions
  double _sizeX;
  double _sizeY;

  // main data structures used to calculate the shape
  std::shared_ptr<Tgs::DelaunayTriangulation> _pDelauneyTriangles;
  std::set<std::pair<double, double>> _outsidePoint;

  GeometryPtr _convertFaceToPolygon(const Tgs::Face& face) const;

  /*
   * Returns true if the face is on the boundary of the triangulation
   */
  bool _isBoundary(const double alpha, const Tgs::Edge& e) const;

  /*
   * Returns true if the face is inside an alpha shape.
   */
  bool _isInside(const double alpha, const Tgs::Face& face) const;

  bool _isTooLong(const double alpha, const Tgs::Edge& e) const;

  /**
   * @brief _collectValidFaces Iterate all Delaunay Triangulation faces with the specified alpha value
   * @param alpha Alpha value used to collect faces
   * @param faces Vector of Delaunay Triangulation faces that conform to the specified alpha value
   * @param e Envelope containing all faces
   * @return Sum of the area of all faces
   */
  double _collectValidFaces(const double alpha, std::vector<GeometryPtr>& faces, geos::geom::Envelope& e);

  /**
   * @brief _searchAlpha Run a binary search to find the alpha value that create a "complete" alpha shape
   * @param alpha Populates the return alpha value
   * @param faces Populates the vector of faces used to create the alpha shape
   * @param e Populates the envelope of the faces used in the alpha shape
   * @param area Populates the area of the alpha shape
   * @param face_count Number of total faces in the Delaunay Triangulation
   * @param alpha_options List of all Delaunay Triangulation face edge lengths (deduplicated) as candidate alpha values
   * @param min_index Minimum search index in the alpha_options vector, for binary search; starts at 0
   * @param max_index Maximum search index in the alpha_options vector, for binary search; starts at alpha_options.size() - 1
   * @return True when alpha value is found
   */
  bool _searchAlpha(double& alpha, std::vector<GeometryPtr>& faces, geos::geom::Envelope& e, double& area,
                    size_t face_count, const std::vector<double>& alpha_options, size_t min_index, size_t max_index);

  GeometryPtr _validateGeometry(const GeometryPtr& g);

  std::shared_ptr<OsmMap> _toOsmMap();
};

}

#endif // __ALPHASHAPE_H__
