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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

// Qt
class QIODevice;

// Standard
#include <vector>

// Tgs
#include <tgs/RandomForest/DataFrame.h>

namespace Tgs
{

class Interpolator
{
public:

  static QString className() { return "Tgs::Interpolator"; }

  virtual ~Interpolator() {}

  virtual QString getName() const = 0;

  /**
   * Returns the estimated RMSE for the interpolator.
   */
  virtual double estimateError() = 0;

  /**
   * Interpolates the value at a given point and returns the resulting vector. The point uses the
   * same ordering as the labels in setIndependentColumns.
   */
  virtual const std::vector<double>& interpolate(const std::vector<double>& point) const = 0;

  /**
   * Set the data frame to be used by this interpolator.
   */
  virtual void setData(const std::shared_ptr<const DataFrame>& df) = 0;

  /**
   * Sets a list of columns that represent the data to be interpolated. Each column is interpolated
   * independently of the other columns.
   */
  virtual void setDependentColumns(const std::vector<std::string>& labels) = 0;

  /**
   * Sets a list of columns that represent the dimensions in which the data exists.
   * In most cases the data space is assumed to be euclidean and all labels must be numeric.
   */
  virtual void setIndependentColumns(const std::vector<std::string>& labels) = 0;

  /**
   * Read the interpolator from the specified input stream.
   */
  virtual void readInterpolator(QIODevice& is) = 0;

  /**
   * Returns a human readable summary of the interpolator.
   */
  virtual QString toString() const = 0;

  /**
   * Write the interpolator to the specified output stream.
   */
  virtual void writeInterpolator(QIODevice& os) const = 0;

  /**
   * The max number of optimization loop iterations to allow per loop.  Some interplolators may
   * take an extraordinary time to find a solution (or never find one).  This is meant to possibly
   * be a temporary solution until such issues can be solved.  See hoot #2893.
   */
  virtual void setMaxAllowedPerLoopOptimizationIterations(int maxIterations) = 0;

  virtual int getMaxOptimizationLoopIterations() = 0;
};

}

#endif // INTERPOLATOR_H
