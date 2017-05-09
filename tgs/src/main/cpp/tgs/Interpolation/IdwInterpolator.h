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
#ifndef IDWINTERPOLATOR_H
#define IDWINTERPOLATOR_H

// tgs
#include <tgs/RStarTree/HilbertRTree.h>

#include "BaseInterpolator.h"

namespace Tgs
{

/**
 * An implementation of Inverse Distance Weighting using the basic form. [1] The IDW calculation
 * takes the first 50 nearest neighbors into consideration and ignores the rest.
 *
 * 1. http://en.wikipedia.org/wiki/Inverse_distance_weighting
 */
class IdwInterpolator : public BaseInterpolator
{
public:
  static string className() { return "Tgs::IdwInterpolator"; }

  /**
   * Constructs a IDW instance where the power parameter, p, will be determined via hold one out
   * evaluation.
   */
  IdwInterpolator(double p = -1.0);

  virtual ~IdwInterpolator();

  virtual string getClassName() const { return IdwInterpolator::className(); }

  virtual const vector<double>& interpolate(const vector<double>& point) const;

  void setP(double p) { _p = p; }

  /**
   * Stop looking for a sigma when sigma is within this distance of "optimal".
   */
  void setStopDelta(double stopDelta) { _stopDelta = stopDelta; }

  virtual string toString() const;

protected:
  double _p;
  double _stopDelta;
  mutable std::auto_ptr<HilbertRTree> _index;

  virtual void _buildModel();

  double _calculateWeight(double d) const;

  virtual double _estimateError(unsigned int index) const;

  /**
   * The ignoreId is used to ignore a specific point when doing hold one back error estimation.
   */
  virtual const vector<double>& _interpolate(const vector<double>& point, int ignoreId) const;

  virtual void _readInterpolator(QIODevice& is);

  virtual void _writeInterpolator(QIODevice& os) const;
};

}

#endif // IDWINTERPOLATOR_H
