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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PertyOp.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/MapProjector.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/util/Settings.h>
#include <hoot/core/visitors/ElementOsmMapVisitor.h>
#include <hoot/core/visitors/CalculateBoundsVisitor.h>
#include <hoot/core/util/ConfigOptions.h>

#include "PertyDuplicatePoiOp.h"
#include "PertyNameVisitor.h"
#include "PertyRemoveTagVisitor.h"
#include "PertyRemoveRandomElementVisitor.h"
#include "PermuteGridCalculator.h"

//Qt
#include <QVector>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, PertyOp)

class ShiftMapVisitor : public ElementOsmMapVisitor
{
public:

  ShiftMapVisitor(const Mat& EX, int cols, const Envelope& e, Meters gridSpacing) :
    _EX(EX),
    _cols(cols),
    _e(e),
    _gridSpacing(gridSpacing)
  {
  }

  virtual void visit(const ConstElementPtr& e)
  {
    if (e->getElementType() == ElementType::Node)
    {
      NodePtr n = _map->getNode(e->getId());
      Coordinate p = n->toCoordinate();
      Vec2d shift = _interpolateShift(p);

      n->setX(n->getX() + shift.val[0]);
      n->setY(n->getY() + shift.val[1]);
    }
  }

  virtual void visit(const shared_ptr<Element>&) {}

  /**
   * User barycentric interpolation to determine the shift at a given point.
   */
  Vec2d _interpolateShift(const Coordinate& p)
  {
    Vec2d result;

    // calculate the grid cell that this point falls in.
    int r = (p.y - _e.getMinY()) / _gridSpacing;
    int c = (p.x - _e.getMinX()) / _gridSpacing;
    double dx = p.x - (_e.getMinX() + c * _gridSpacing);
    double dy = p.y - (_e.getMinY() + r * _gridSpacing);

    ////
    // Using barycentric interpolation.
    ////

    // if we're right on a point (most interestingly if we're right on the upper right point which
    // would cause the following two blocks to fail).
    if (dx == 0.0 && dy == 0.0)
    {
      result.val[0] = _getX(r, c);
      result.val[1] = _getY(r, c);
    }
    // if we're in the lower right triangle
    else if (dx > dy)
    {
      Coordinate t1 = gridCoordinate(r, c);
      Coordinate t2 = gridCoordinate(r, c + 1);
      Coordinate t3 = gridCoordinate(r + 1, c + 1);
      double a1 = triArea2(t3, t2, p);
      double a2 = triArea2(t1, t3, p);
      double a3 = triArea2(t1, t2, p);
      double areaSum = a1 + a2 + a3;

      result.val[0] = (_getX(r, c) * a1 + _getX(r, c + 1) * a2 + _getX(r + 1, c + 1) * a3) /
          areaSum;
      result.val[1] = (_getY(r, c) * a1 + _getY(r, c + 1) * a2 + _getY(r + 1, c + 1) * a3) /
          areaSum;
    }
    // if we're in the upper left triangle
    else
    {
      Coordinate t1 = gridCoordinate(r, c);
      Coordinate t4 = gridCoordinate(r + 1, c);
      Coordinate t3 = gridCoordinate(r + 1, c + 1);

      double a1 = triArea2(t3, t4, p);
      double a3 = triArea2(t1, t4, p);
      double a4 = triArea2(t1, t3, p);
      double areaSum = a1 + a3 + a4;

      result.val[0] = (_getX(r, c) * a1 + _getX(r + 1, c) * a4 + _getX(r + 1, c + 1) * a3) /
          areaSum;
      result.val[1] = (_getY(r, c) * a1 + _getY(r + 1, c) * a4 + _getY(r + 1, c + 1) * a3) /
          areaSum;
    }

    return result;
  }

  Coordinate gridCoordinate(int r, int c)
  {
    return Coordinate((double)c * _gridSpacing + _e.getMinX(),
      (double)r * _gridSpacing + _e.getMinY());
  }

  /**
   * Return 2 * the area of a triangle.
   */
  double triArea2(const Coordinate& a, const Coordinate& b, const Coordinate& c)
  {
    return fabs((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
  }

private:
  const Mat& _EX;
  int _cols;
  const Envelope& _e;
  Meters _gridSpacing;

  double _getX(int row, int col)
  {
    return _EX.at<double>((row * _cols + col) * 2, 0);
  }

  double _getY(int row, int col)
  {
    return _EX.at<double>((row * _cols + col) * 2 + 1, 0);
  }
};

PertyOp::PertyOp() :
_settings(conf())
{
  _configure();
}

void PertyOp::_configure()
{
  ConfigOptions configOptions(_settings);
  setRandomError(configOptions.getPertyRandomErrorX(), configOptions.getPertyRandomErrorY());
  setSystematicError(
    configOptions.getPertySystematicErrorX(), configOptions.getPertySystematicErrorY());
  setGridSpacing(configOptions.getPertyGridSpacing());
  setCsmParameters(configOptions.getPertyCsmBeta(), configOptions.getPertyCsmD());
  setPermuteAlgorithm(configOptions.getPertyAlgorithm());
  setSeed(configOptions.getPertySeed());
  setNamedOps(configOptions.getPertyOps());
}

void PertyOp::setConfiguration(const Settings& conf)
{
  _settings = conf;
  _configure();
}

QString PertyOp::toString()
{
  QString str = "_permuteAlgorithm: " + _permuteAlgorithm;
  if (_gridCalculator.get())
  {
    str += ", " + _gridCalculator->toString();
  }
  return str;
}

void PertyOp::apply(shared_ptr<OsmMap>& map)
{
  // permute the data first
  permute(map);

  // Apply any user specified operations.
  NamedOp namedOps(_namedOps);
  namedOps.setConfiguration(_settings);
  MapProjector::projectToPlanar(map);
  namedOps.apply(map);
}

void PertyOp::setRandomError(Meters sigmaX, Meters sigmaY)
{
  double epsilon = 1e-6;
  _sigmaRx = max(epsilon, sigmaX);
  _sigmaRy = max(epsilon, sigmaY);
}

Mat PertyOp::_calculatePermuteGrid(geos::geom::Envelope env, int& rows, int& cols)
{
  LOG_DEBUG("Using permute algorithm: " + _permuteAlgorithm);
  _gridCalculator.reset(
    Factory::getInstance().constructObject<PermuteGridCalculator>(_permuteAlgorithm));
  _gridCalculator->setCsmParameters(_beta, _D);
  _gridCalculator->setGridSpacing(_gridSpacing);
  _gridCalculator->setRandomError(_sigmaRx, _sigmaRy);
  _gridCalculator->setSeed(_seed);
  _gridCalculator->setSystematicError(_sigmaSx, _sigmaSy);

  return _gridCalculator->permute(env, rows, cols);
}

shared_ptr<OsmMap> PertyOp::generateDebugMap(shared_ptr<OsmMap>& map)
{
  MapProjector::projectToPlanar(map);
  shared_ptr<OsmMap> result(new OsmMap(map->getProjection()));

  LOG_INFO(toString());

  geos::geom::Envelope env = CalculateBoundsVisitor::getGeosBounds(map);
  LOG_INFO("env: " << env.toString());

  int rows, cols;
  Mat EX = _calculatePermuteGrid(env, rows, cols);

  // interpolate values from the grid and shift nodes accordingly
  ShiftMapVisitor v(EX, cols, env, _gridSpacing);
  map->visitRw(v);

  double dSum = 0.0;

  for (int i = 0; i < rows; ++i)
  {
    for (int j =0; j < cols; ++j)
    {
      double x = env.getMinX() + j * _gridSpacing;
      double y = env.getMinY() + i * _gridSpacing;

      double dx = EX.at<double>((i * cols + j) * 2, 0);
      double dy = EX.at<double>((i * cols + j) * 2 + 1, 0);
      dSum += sqrt(dx * dx + dy * dy);

      shared_ptr<Node> n1(new Node(Status::Unknown1, result->createNextNodeId(), x, y, 5));
      shared_ptr<Node> n2(new Node(Status::Unknown1, result->createNextNodeId(), x + dx, y + dy, 5));
      result->addNode(n1);
      result->addNode(n2);

      shared_ptr<Way> w(new Way(Status::Unknown1, result->createNextWayId(), 5.0));
      w->addNode(n1->getId());
      w->addNode(n2->getId());
      w->getTags().addNote(QString("r: %1 c: %2").arg(i).arg(j));
      w->getTags().addNote(QString("dx: %1 dy: %2").arg(dx).arg(dy));
      result->addElement(w);
    }
  }

  return result;
}

void PertyOp::permute(const shared_ptr<OsmMap> &map)
{
  MapProjector::projectToPlanar(map);

  geos::geom::Envelope env = CalculateBoundsVisitor::getGeosBounds(map);

  int rows, cols;
  Mat EX = _calculatePermuteGrid(env, rows, cols);

  // interpolate values from the grid and shift nodes accordingly
  ShiftMapVisitor v(EX, cols, env, _gridSpacing);
  map->visitRw(v);
}

void PertyOp::setPermuteAlgorithm(QString algo)
{
  if (!QVector<std::string>::fromStdVector(
       Factory::getInstance()
         .getObjectNamesByBase(PermuteGridCalculator::className()))
         .contains(algo.toStdString()))
  {
    throw HootException("Unexpected permute algorithm type. " + algo);
  }
  _permuteAlgorithm = algo;
}

}
