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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DELAUNAYINTERPOLATOR_H
#define DELAUNAYINTERPOLATOR_H

// Standard
#include <memory>

// Tgs
#include <tgs/TgsException.h>
#include <tgs/DelaunayTriangulation/DelaunayTriangulation.h>
#include <tgs/Interpolation/BaseInterpolator.h>

namespace Tgs
{

/**
 * This calculates a delaunay triangulation, then determines which triangle an input point lies in
 * that triangle is used to interpolate the value using Barycentric Interpolation. [1] Error is
 * estimated using k-fold validation (by default k=10). Values that are "reasonably" out of bounds
 * will use only the "valid" points within the containing triangle. Values substantially out of
 * bounds will get the nearest neighbor's value.
 *
 * Substantially out of bounds is roughly twice the size of the bounds of the input points.
 *
 * 1. http://classes.soe.ucsc.edu/cmps160/Fall10/resources/barycentricInterpolation.pdf
 */
class DelaunayInterpolator : public BaseInterpolator
{
public:

  static QString className() { return "Tgs::DelaunayInterpolator"; }

  DelaunayInterpolator();

  ~DelaunayInterpolator() = default;

  /**
   * Use k-fold cross validation to estimate the error.
   */
  double estimateError() override;

  QString getName() const override { return className(); }

  const std::vector<double>& interpolate(const std::vector<double>& point) const override;

  QString toString() const override;

protected:

  /// The number of folds in cross validation
  int _kFold;
  std::shared_ptr<DelaunayTriangulation> _dt;
  std::map<Point2d, int> _pointToIndex;
  double _minX, _minY, _maxX, _maxY;

  /**
   * Uses the the specified weight to add the value at p to the result. p must be part of the _dt.
   * If p is part of the outer most triangle then nothing will be added to _result and a 0.0 will
   * be returned. Otherwise the value at p will be weighted w/ w and added to _result and w will
   * be returned.
   */
  double _addToResult(const Point2d& p, double w) const;

  void _buildModel() override;

  /**
   * Calculate the squared error for a given fold.
   */
  double _calculateFoldError(int fold, const std::vector<size_t>& indexes) const;

  double _estimateError(unsigned int /*index*/) const override { throw Tgs::Exception("Not Implemented."); }

  void _readInterpolator(QIODevice& is) override;

  void _writeInterpolator(QIODevice& os) const override;
};

}

#endif // DELAUNAYINTERPOLATOR_H
