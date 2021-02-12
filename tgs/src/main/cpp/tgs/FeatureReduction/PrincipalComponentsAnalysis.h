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
 * @copyright Copyright (C) 2015, 2018, 2019 Maxar (http://www.maxar.com/)
 */
 /***************************************************************************
* Copyright (c) 2005-2007 by SPADAC Inc. (formerly Spatial Data Analytics Corporation).  All rights reserved.
****************************************************************************/

#ifndef __TGS__PRINCIPAL_COMPONENTS_ANALYSIS_H__
#define __TGS__PRINCIPAL_COMPONENTS_ANALYSIS_H__

// Standard Includes
#include <vector>

// newmat Includes
namespace NEWMAT
{
  class DiagonalMatrix;
  class Matrix;
}

#include <tgs/TgsExport.h>

namespace Tgs
{
  class DataFrame;

  /**
   * Provides a method for calculating the principal components
   * See http://en.wikipedia.org/wiki/Principal_components_analysis for an explanation of how this
   * is calculated.
   *
   * #####################################################################################
   * ************* NOTE: This has only been tested on 2 dimensional data!!! **************
   * #####################################################################################
   */
  class TGS_EXPORT PrincipalComponentsAnalysis
  {
  public:

    /**
     * Given a data frame, calculate the principal components. The resulting normalized 
     * components are placed in 'components' as a vector of components. E.g. The following will 
     * be returned if your 2D data lies primarily on a 10deg line.
     *
     * {{{
     * [[0.984801, -0.173689]
     *  [0.173689,  0.984801]]
     * }}}
     *
     * In this case you get:
     * {{{
     * double x1 = result.element(0, 0); // .984801
     * double y1 = result.element(1, 0); // .173689
     * double x2 = result.element(0, 1); // -.173689
     * double y2 = result.element(1, 1); // .984801
     * }}}
     *
     * @param df Data frame to compute components on. Nominal and null values are not supported.
     * @param components Return the principal components in this matrix.
     */
    void compute(Tgs::DataFrame& df);

    /**
     * Returns the calculated components
     */
    const std::vector<std::vector<double>>& getComponents() const { return _components; }

    /**
    * Reprojects the given point from the PCA space into normal space
    */
    const std::vector<double>& reprojectFromPca(const std::vector<double>& point);

    /**
     * Reprojects the given point into the PCA space
     */
    const std::vector<double>& reprojectIntoPca(const std::vector<double>& point);

  private:

    std::vector<std::vector<double>> _components;
    std::vector<double> _tmp;

    /**
     * Sort the eigenvectors and eigenvalues in decreasing order based on the eigenvalues.
     */
    static void _sortEigens(NEWMAT::Matrix& eigenVectors, NEWMAT::DiagonalMatrix& eigenValues);
  };
}


#endif
