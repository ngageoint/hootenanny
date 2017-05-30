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

#ifndef __GEOMETRY_PAINTER_H__
#define __GEOMETRY_PAINTER_H__

//OGR Includes
class OGREnvelope;
class OGRGeometry;
class OGRGeometryCollection;
class OGRLinearRing;
class OGRLineString;
class OGRPoint;
class OGRPolygon;
class OGRMultiPoint;

// Qt Includes
#include <QMatrix>
class QPainter;
class QPolygonF;
class QRect;

namespace hoot
{

class OsmMap;
class Way;
class Element;
class Node;

/**
 * A class for consolidating all the spots that we draw OGR geometries.
 *
 * NOTE:
 *  * This will probably have issues if you set your viewport/window options. Matrix manipulations
 *    should be just fine.
 *  * If you are using matrix manipulations you may see strange things happen when your pen width
 *    is something other than 0. This occurs b/c the pen width gets scaled with everything else.
 *
 */
class GeometryPainter
{
public:

  /**
  * Create a matrix for drawing to a painter in world coordinates. This matrix will maintain
  * the appropriate aspect ratio.
  */
  static QMatrix createMatrix(const QRect& window, const OGREnvelope& world);

  /**
   * Create a matrix for drawing to a painter in world coordinates. This matrix will maintain
   * the appropriate aspect ratio.
   */
  static QMatrix createMatrix(const QRect& window, const OGREnvelope& world,
    const OGRPoint& worldCenter);

  static void drawElement(QPainter& pt, const OsmMap *map, const Element *e, const QMatrix &m);

  static void drawGeometry(QPainter& pt, const OGRGeometry* geom, const QMatrix& m = QMatrix());

  static void drawLineString(QPainter& pt, const OGRLineString* lineString,
    const QMatrix& m = QMatrix());

  static void drawGeometryCollection(QPainter& pt, const OGRGeometryCollection* collection,
    const QMatrix& m = QMatrix());

  static void drawNode(QPainter& pt, const hoot::Node* e, const QMatrix& m);

  static void drawOsmMap(QPainter& pt, const OsmMap* map, const QMatrix& m = QMatrix());

  static void drawPolygon(QPainter& pt, const OGRPolygon* polygon, const QMatrix& m = QMatrix());

  static void drawPoint(QPainter& pt, double x, double y, const QMatrix& m = QMatrix());

  static void drawPoint(QPainter& pt, const OGRPoint* point, const QMatrix& m = QMatrix());

  static void drawWay(QPainter& pt, const OsmMap* map, const hoot::Way* e, const QMatrix& m);

protected:

  static void _convertRingToQPolygon(const OGRLinearRing* ring, QPolygonF& qp,
    const QMatrix& m = QMatrix());
};

}

#endif
