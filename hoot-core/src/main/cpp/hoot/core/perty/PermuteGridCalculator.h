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
#ifndef PERMUTE_GRID_CALCULATOR_H
#define PERMUTE_GRID_CALCULATOR_H

//hoot
#include <hoot/core/Units.h>

// geos
#include <geos/geom/Envelope.h>

// OpenCV
#include <opencv/cv.h>

// Qt
#include <QString>

namespace hoot
{

using namespace cv;
using namespace geos::geom;

/**
 *
 */
class PermuteGridCalculator
{
  public:

    static string className() { return "hoot::PermuteGridCalculator"; }

    virtual ~PermuteGridCalculator() { }

    /**
     * Calculates a permutation grid and the values in that grid for a given envelope. The number of
     * rows and columns are returned in rows and cols.
     */
    virtual Mat permute(geos::geom::Envelope env, int& pointRows, int& pointCols) = 0;

    /**
   * Seeds the permutation process. By default a seed is generated based on time. The seed should
   * be non-negative or -1 to generate a seed based on time.
   */
    void setSeed(int seed) { _seed = seed; }

    /**
   * Sets the systematic error. This is the sigma value for Sx and Sy. The same sigma value is used
   * for all values in each matrix. See [1] for more information.
   */
   void setSystematicError(Meters sigmaX, Meters sigmaY) { _sigmaSx = sigmaX; _sigmaSy = sigmaY; }

   void setCsmParameters(double D) { _D = D; }

    /**
   * Sets the random error. This is the sigma value for Rx and Ry. The same sigma value is used
   * for all values in each matrix. See [1] for more information.
   * @note There were problems with taking the sqrtm when sigma is zero. Now I make sigma a small
   * value (1e-6) when sigma is zero.
   */
    void setRandomError(Meters sigmaX, Meters sigmaY) { _sigmaRx = sigmaX; _sigmaRy = sigmaY; }

    void setGridSpacing(Meters gridSpacing) { _gridSpacing = gridSpacing; }

    virtual QString toString();

    protected:

      Meters _sigmaRx, _sigmaRy;

      Meters _D;
      Meters _gridSpacing;
      int _seed;
      Meters _sigmaSx, _sigmaSy;
};

}

#endif // PERMUTE_GRID_CALCULATOR_H
