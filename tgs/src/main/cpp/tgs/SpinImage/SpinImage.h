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

#ifndef __TGS__SPIN_IMAGE_H__
#define __TGS__SPIN_IMAGE_H__

// Standard Includes
#include <math.h>
#include <vector>

#include "../TgsExport.h"

namespace Tgs
{
  class Point2d;
  class SpinImageTest;

  /*
   * Generate the spin images for a point cloud using the method described in [1].
   *
   * [1] Johnson and Hebert, "Surface Registration by Matching Oriented Points"
   * http://www.frc.ri.cmu.edu/projects/artisan/SurfaceRegistration.DIG-97.fmk.pdf
   */
  class  TGS_EXPORT SpinImage
  {
  public:

    enum
    {
      /// Compare all cells regardless of value.
      CompareAllCells,
      /// Only compare cells where both image have non-zero values
      CompareNonZeroCells,
      /// Only compare cells where this' cell is non zero
      CompareThisNonZeroCells
    };

    SpinImage();

    void addValue(double alpha, double beta);

    const float& bin(int alphaCell, int betaCell) const { return _bins[betaCell * _alphaBins + alphaCell]; }

    /**
     * Resets the SpinImage back to its constructed state.
     */
    void clear();

    /**
     * Compare this image to another spin image. "this" image should be the pristine training 
     * data. I assume that the training image has little or no clutter.
     */
    double compare(const SpinImage& si, int minUseful = 4) const;

    void exportImage(std::ostream& s, std::string tabDepth) const;

    int getAlphaBins() const { return _alphaBins; }

    int getBetaBins() const { return _betaBins; }

    float getValue(double alpha, double beta) const;

    void importImage(std::istream& s);

    void resize(int alphaBins, int betaBins);

    void setBounds(double maxAlpha, double minBeta, double maxBeta);

    void setCompareAlgorithm(int ca) { _compareAlgo = ca; }

    void setIgnoreReflections(bool ignore) { _ignoreReflection = ignore; }

  private:

    // white box testing
    friend class SpinImageTest;

    /**
     * alpha in rows
     * beta in columns
     * The top left of (0,0) maps to (_minAlpha, _minBeta)
     * The top right of (_alphaBins - 1, 0) maps to (_maxAlpha, _minBeta)
     */
    std::vector<float> _bins;

    int _compareAlgo;
    int _alphaBins, _betaBins;
    double _maxAlpha;
    double _minBeta, _maxBeta;
    /// If this is set to true, then the abs(beta) is used to eliminate problems w/ reflected
    /// normals.
    bool _ignoreReflection;

    float& bin(int alphaCell, int betaCell) { return _bins[betaCell * _alphaBins + alphaCell]; }

    int _calculateIndex(double alpha, double beta) const;

    double _calculateAlphaCenter(int alphaCell) const;

    double _calculateBetaCenter(int betaCell) const;

    double _calculateCorrelation(const SpinImage& si, int& n) const;

    /**
     * Return the nearest bin that has a center below this value
     */
    int _calculateLowerAlpha(double alpha) const;

    /**
     * Return the nearest bin that has a center below this value
     */
    int _calculateLowerBeta(double alpha) const;

    double _distance(const Point2d& p1, const Point2d& p2) const;

    friend bool operator==(const SpinImage& s1, const SpinImage& s2);
  };

  inline bool operator==(const SpinImage& s1, const SpinImage& s2)
  {
    bool result = true;

    result &= s1._compareAlgo == s2._compareAlgo;
    result &= s1._alphaBins == s2._alphaBins;
    result &= s1._betaBins == s2._betaBins;
    result &= fabs(s1._maxAlpha - s2._maxAlpha) < 1e-6;
    result &= fabs(s1._minBeta - s2._minBeta) < 1e-6;
    result &= fabs(s1._maxBeta - s2._maxBeta) < 1e-6;
    result &= s1._ignoreReflection == s2._ignoreReflection;
    result &= s1._bins.size() == s2._bins.size();

    if (result)
    {
      for (unsigned int i = 0; i < s1._bins.size(); i++)
      {
        result &= fabs(s1._bins[i] - s2._bins[i]) < 1e-6;
      }
    }
    return result;
  }

  inline std::ostream& operator<<(std::ostream& s, const SpinImage& si)
  {
    si.exportImage(s, "");
    return s;
  }

}

#endif
