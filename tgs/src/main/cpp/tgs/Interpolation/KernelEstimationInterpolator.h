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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef KERNELESTIMATIONINTERPOLATOR_H
#define KERNELESTIMATIONINTERPOLATOR_H

// Qt
#include <QIODevice>

#include <tgs/Interpolation/BaseInterpolator.h>

namespace Tgs
{

/**
 * Assumes that where there is no data the expected value is zero. This uses gaussian weighting
 * of the input points to calculate an average interpolated result. The size of the gaussian
 * kernel is dermined through cross-validation.
 */
class KernelEstimationInterpolator : public BaseInterpolator
{
public:

  static QString className() { return "Tgs::KernelEstimationInterpolator"; }

  /**
   * When sigma is less than zero (recommended) cross-validation will be used to determine an
   * appropriate sigma.
   */
  KernelEstimationInterpolator(double sigma = -1);

  virtual ~KernelEstimationInterpolator();

  virtual QString getName() const { return className(); }

  double getSigma() const { return _sigma; }

  double getStopDelta() const { return _stopDelta; }

  virtual const std::vector<double>& interpolate(const std::vector<double>& point) const;

  void setSigma(double sigma) { _sigma = sigma; _checkRebuild(); }

  /**
   * Stop looking for a sigma when sigma is within this distance of "optimal".
   */
  void setStopDelta(double stopDelta) { _stopDelta = stopDelta; _checkRebuild(); }

  virtual QString toString() const;

protected:

  double _sigma;
  double _stopDelta;

  virtual void _buildModel();

  virtual double _estimateError(unsigned int index) const;

  virtual void _readInterpolator(QIODevice& is);

  virtual void _writeInterpolator(QIODevice& os) const;
};

}

#endif // KERNELESTIMATIONINTERPOLATOR_H
