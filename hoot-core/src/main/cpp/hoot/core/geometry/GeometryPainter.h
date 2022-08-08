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
 * @copyright Copyright (C) 2015, 2017, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __GEOMETRY_PAINTER_H__
#define __GEOMETRY_PAINTER_H__

// Qt
#include <QMatrix>
class QPainter;
class QPolygonF;
class QRect;

// OGR
class OGREnvelope;
class OGRLinearRing;

namespace hoot
{

class OsmMap;
class Way;
class Node;

/**
 * A class for consolidating all the spots that we draw OGR geometries.
 *
 * NOTE:
 *  * This will probably have issues if you set your viewport/window options. Matrix manipulations
 *    should be just fine.
 *  * If you are using matrix manipulations you may see strange things happen when your pen width
 *    is something other than 0. This occurs b/c the pen width gets scaled with everything else.
 */
class GeometryPainter
{
public:

  /**
  * Create a matrix for drawing to a painter in world coordinates. This matrix will maintain
  * the appropriate aspect ratio.
  */
  static QMatrix createMatrix(const QRect& window, const OGREnvelope& world);

  static void drawNode(QPainter& pt, const Node* e, const QMatrix& m);
  static void drawPoint(QPainter& pt, double x, double y, const QMatrix& m = QMatrix());
  static void drawWay(QPainter& pt, const OsmMap* map, const Way* e, const QMatrix& m);

private:

  static void _convertRingToQPolygon(
    const OGRLinearRing* ring, QPolygonF& qp, const QMatrix& m = QMatrix());
};

}

#endif
