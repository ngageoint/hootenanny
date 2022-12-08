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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "GeometryPainter.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/elements/Way.h>

// GDAL
#include <ogrsf_frmts.h>

// Qt
#include <QPainter>
#include <QPainterPath>

namespace hoot
{

QMatrix GeometryPainter::createMatrix(const QRect& window, const OGREnvelope& world)
{
  double xRatio = ((double)window.width() - 1.0) / (world.MaxX - world.MinX);
  double yRatio = ((double)window.height() - 1.0) / (world.MaxY - world.MinY);

  qreal m11 = xRatio;
  qreal m12 = 0.0;
  qreal m21 = 0.0;
  qreal m22 = -yRatio;
  qreal dx = -xRatio * world.MinX + 0.5;
  qreal dy = yRatio * world.MinY - 0.5 + window.height();

  QMatrix m(m11, m12, m21, m22, dx, dy);

  return m;
}

void GeometryPainter::drawNode(QPainter& pt, const Node* node, const QMatrix& m)
{
  QPointF p(m.map(QPointF(node->getX(), node->getY())) /*- QPointF(0.5, 0.5)*/);
  pt.drawPoint(p);
}

void GeometryPainter::drawPoint(QPainter& pt, double x, double y, const QMatrix& m)
{
  QPointF p(m.map(QPointF(x, y)));
  pt.drawPoint(p);
}

void GeometryPainter::drawWay(QPainter& pt, const OsmMap* map, const Way* way, const QMatrix& m)
{
  int size = static_cast<int>(way->getNodeCount());
  QPolygonF a(size);

  for (int j = 0; j < size; j++)
  {
    ConstNodePtr n = map->getNode(way->getNodeId(j));
    a[j] = QPointF(m.map(QPointF(n->getX(), n->getY())) - QPointF(0.5, 0.5));
  }

  pt.drawPolyline(a);
}


} // hoot
