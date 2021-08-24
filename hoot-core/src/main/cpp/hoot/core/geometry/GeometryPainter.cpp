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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
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

void GeometryPainter::_convertRingToQPolygon(const OGRLinearRing* ring, QPolygonF& qp,
  const QMatrix& m)
{
  OGRPoint p;
  qp.resize(ring->getNumPoints());
  for (int i = 0; i < ring->getNumPoints(); i++)
  {
    ring->getPoint(i, &p);
    qp[i] = QPointF(m.map(QPointF(p.getX(), p.getY())) - QPointF(0.5, 0.5));
  }
}

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

QMatrix GeometryPainter::createMatrix(const QRect& window, const OGREnvelope& world,
  const OGRPoint& worldCenter)
{
  // determine if the image is bigger or smaller than the display area and size it accordingly
  double xRatio = ((double)window.width() - 1.0) / (world.MaxX - world.MinX);
  double yRatio = ((double)window.height() - 1.0) / (world.MaxY - world.MinY);
  double ratio;
  if (xRatio < yRatio)
  {
    ratio = xRatio;
  }
  else
  {
    ratio = yRatio;
  }
  double scale = 1.0 * ratio;

  /*
  This code transforms from world's coord system to qt's based on the following assumptions:
  1) Qt's minX & minY are at top left (Quadrant II)
  2) real world minX & minY are at bottom left (Quadrant I)

                  ^
                  |
           IV     |    I
                  |
       <-------------------->
                  |
           III    |    II
                  |
                  Y

    In order to get from II to I, we must do the following:
    1) invert the Y axis
        a) move Y points above the x axis
        b) convert all negative y points to positives
    2) recenter based on worldcenter
    3) scale to current scale (based on zoom and ratio)
    4)  move image to window's center

  */

  // 1a.move Y points above the x axis
  QMatrix moveY;
  moveY.translate(0, -(2 * world.MinY) - (world.MaxY - world.MinY));

  // 1b.  convert all negative y points to positives
  QMatrix swapY;
  swapY.scale(1, -1);

  // 2. recenter based on worldcenter
  QMatrix centerMatrix;
  centerMatrix.translate(-worldCenter.getX(), -worldCenter.getY());

  // 3. scale to current scale (based on zoom and ratio)
  QMatrix scaleMatrix;
  scaleMatrix.scale(scale, scale);

  // 4. move image to window's center
  QMatrix moveToWindowCenter;
  moveToWindowCenter.translate((double)(window.width()) / 2.0,
    (double)(window.height()) / 2.0);

  return moveY * swapY * centerMatrix * scaleMatrix * moveToWindowCenter;
}

void GeometryPainter::drawElement(QPainter& pt, const OsmMap* map, const Element* e,
  const QMatrix& m)
{
  if (dynamic_cast<const Way*>(e) != nullptr)
  {
    drawWay(pt, map, dynamic_cast<const Way*>(e), m);
  }
  else if (dynamic_cast<const Node*>(e) != nullptr)
  {
    drawNode(pt, dynamic_cast<const Node*>(e), m);
  }
  else
  {
    throw HootException("Internal Error: Geometry type is not supported.");
  }
}

void GeometryPainter::drawGeometry(QPainter& pt, const OGRGeometry* geom, const QMatrix& m)
{
  switch (wkbFlatten(geom->getGeometryType()))
  {
    case wkbPoint:
      {
        drawPoint(pt, dynamic_cast<const OGRPoint*>(geom), m);
        break;
      }
    case wkbLineString:
      {
        drawLineString(pt, dynamic_cast<const OGRLineString*>(geom), m);
        break;
      }
    case wkbPolygon:
      {
        drawPolygon(pt, dynamic_cast<const OGRPolygon*>(geom), m);
        break;
      }
    case wkbMultiPoint:
    case wkbMultiLineString:
    case wkbMultiPolygon:
      {
        drawGeometryCollection(pt, dynamic_cast<const OGRGeometryCollection*>(geom), m);
        break;
      }
    default:
      {
        throw HootException("Internal Error: Geometry type is not supported.");
      }
  }
}

void GeometryPainter::drawGeometryCollection(QPainter& pt, const OGRGeometryCollection* collection,
                                             const QMatrix& m)
{
  if (collection == nullptr)
  {
    throw HootException("Internal Error: GeometryPainter::drawGeometryCollection - Null geometry");
  }
  for (int i = 0; i < collection->getNumGeometries(); i++)
  {
    drawGeometry(pt, collection->getGeometryRef(i), m);
  }
}

void GeometryPainter::drawLineString(QPainter& pt, const OGRLineString* lineString,
  const QMatrix& m)
{
  QPolygonF a(lineString->getNumPoints());

  OGRPoint point;
  for (int j = 0; j < lineString->getNumPoints(); j++)
  {
    lineString->getPoint(j, &point);
    a[j] = QPointF(m.map(QPointF(point.getX(), point.getY())) - QPointF(0.5, 0.5));
  }

  pt.drawPolyline(a);
}

void GeometryPainter::drawNode(QPainter& pt, const Node* node, const QMatrix& m)
{
  QPointF p(m.map(QPointF(node->getX(), node->getY())) /*- QPointF(0.5, 0.5)*/);
  pt.drawPoint(p);
}

void GeometryPainter::drawOsmMap(QPainter& pt, const OsmMap* map, const QMatrix& m)
{
  const WayMap& ways = map->getWays();
  for (WayMap::const_iterator it = ways.begin(); it != ways.end(); ++it)
  {
    drawWay(pt, map, it->second.get(), m);
  }

  const NodeMap& nodes = map->getNodes();
  for (NodeMap::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
  {
    drawNode(pt, it->second.get(), m);
  }
}

void GeometryPainter::drawPoint(QPainter& pt, double x, double y, const QMatrix& m)
{
  QPointF p(m.map(QPointF(x, y)));
  pt.drawPoint(p);
}

void GeometryPainter::drawPoint(QPainter& pt, const OGRPoint* point, const QMatrix& m)
{
  QPointF p(m.map(QPointF(point->getX(), point->getY())) /*- QPointF(0.5, 0.5)*/);
  pt.drawPoint(p);
}

void GeometryPainter::drawPolygon(QPainter& pt, const OGRPolygon* polygon, const QMatrix& m)
{
  QPen pen = pt.pen();
  QBrush brush = pt.brush();

  if (polygon->getNumInteriorRings() > 0)
  {
    std::shared_ptr<QImage> image =
      std::make_shared<QImage>(pt.window().size(), QImage::Format_ARGB32);
    if (image->isNull() == true)
    {
      throw HootException("Internal Error: GeometryPainter::drawPolygon "
                          "Error allocating image.");
    }
    image->fill(qRgba(0, 0, 0, 0));
    std::shared_ptr<QPainter> lpt = std::make_shared<QPainter>(image.get());
    lpt->setMatrix(pt.matrix());
    lpt->setPen(pen);
    lpt->setBrush(brush);

    const OGRLinearRing* ring = polygon->getExteriorRing();
    QPolygonF qp;
    _convertRingToQPolygon(ring, qp, m);

    lpt->setPen(Qt::NoPen);
    lpt->setBrush(brush);
    lpt->drawPolygon(qp, Qt::WindingFill);

    lpt->setPen(pen);
    lpt->setBrush(Qt::NoBrush);
    lpt->drawPolygon(qp);
    for (int i = 0; i < polygon->getNumInteriorRings(); i++)
    {
      ring = polygon->getInteriorRing(i);

      // draw the appropriate border around the section we erased.
      _convertRingToQPolygon(ring, qp, m);

      // clear out the hole
      lpt->setPen(Qt::NoPen);
      lpt->setBrush(QColor(0, 0, 0, 0));
      lpt->setCompositionMode(QPainter::CompositionMode_Clear);
      lpt->drawPolygon(qp, Qt::WindingFill);

      lpt->setPen(pen);
      lpt->setBrush(Qt::NoBrush);
      lpt->setCompositionMode(QPainter::CompositionMode_SourceOver);
      lpt->drawPolygon(qp, Qt::WindingFill);
    }

    lpt->end();

    QMatrix matrix = pt.matrix();
    pt.resetMatrix();
    pt.drawImage(pt.window(), *image);
    pt.setMatrix(matrix);
  }
  else
  {
    const OGRLinearRing* ring = polygon->getExteriorRing();
    QPolygonF qp;
    _convertRingToQPolygon(ring, qp, m);

    pt.setPen(Qt::NoPen);
    pt.setBrush(brush);
    pt.drawPolygon(qp, Qt::WindingFill);

    pt.setPen(pen);
    pt.setBrush(Qt::NoBrush);
    pt.drawPolygon(qp);
  }
}

void GeometryPainter::drawWay(QPainter& pt, const OsmMap* map, const Way* way, const QMatrix& m)
{
  int size = way->getNodeCount();
  QPolygonF a(size);

  for (int j = 0; j < size; j++)
  {
    ConstNodePtr n = map->getNode(way->getNodeId(j));
    a[j] = QPointF(m.map(QPointF(n->getX(), n->getY())) - QPointF(0.5, 0.5));
  }

  pt.drawPolyline(a);
}


} // hoot
