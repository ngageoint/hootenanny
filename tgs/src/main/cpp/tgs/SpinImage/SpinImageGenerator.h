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
 * @copyright Copyright (C) 2015, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__SPIN_IMAGE_GENERATOR_H__
#define __TGS__SPIN_IMAGE_GENERATOR_H__

#include "../TgsExport.h"

namespace Tgs
{
  class PointCloud;
  class SpinImage;
  class SpinImageStack;

  /**
   * Generate the spin images for a point cloud using the method described in [1].
   *
   * [1] Johnson and Hebert, "Surface Registration by Matching Oriented Points"
   * http://www.frc.ri.cmu.edu/projects/artisan/SurfaceRegistration.DIG-97.fmk.pdf
   */
  class  TGS_EXPORT SpinImageGenerator
  {
  public:

    enum
    {
      Normal,
      Up
    } RelativeAngle;

    SpinImageGenerator();

    void generateSingleImage(PointCloud& pc, int index, SpinImage& si);

    void generateStack(PointCloud& pc, SpinImageStack& stack, int maxImageCount = -1);

    double getAlphaMax() const { return _alphaMax; }

    int getAlphaSize() const { return _alphaSize; }

    double getBetaMax() const { return _betaMax; }

    int getBetaSize() const { return _betaSize; }

    int getRelativeAngle() const { return _relativeAngle; }

    /**
     * Sets to relative angle to one of Normal or Up. If Normal is specified then the normal 
     * vector specified in the point cloud is used, otherwise up (positive z axis) is used.
     */
    void setRelativeAngle(int ra) { _relativeAngle = ra; }

    void setBounds(double alphaMax, double betaMax) { _alphaMax = alphaMax; _betaMax = betaMax; }

    void setKThreshold(int k) { _k = k; }

    void setSize(int alphaSize, int betaSize) { _alphaSize = alphaSize; _betaSize = betaSize; }

  private:
    PointCloud* _pc;
    SpinImageStack* _stack;
    int _alphaSize, _betaSize;
    double _alphaMax, _betaMax;
    int _k;
    int _relativeAngle;

    void _populateSpinImage(int id, SpinImage* image);
  };
}

#endif
