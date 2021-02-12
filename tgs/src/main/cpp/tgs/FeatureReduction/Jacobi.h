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
 * @copyright Copyright (C) 2015, 2017 Maxar (http://www.maxar.com/)
 */
#ifndef JACOBI_H
#define JACOBI_H

//#define WANT_MATH
//#define use_namespace
#include <newmat/include.h>
#include <newmat/newmatap.h>
#include <newmat/newmatrm.h>

#define DSIGNIF 64

/**
 * @license http://www.robertnz.net/nm10.htm
 * @author newmat11
 * http://www.robertnz.net/nm_intro.htm
 * A snippet of newmat11 code was taken to optimize it for our purposes. The author of newmat11 had
 * no hand in the change we made and any errors introduced are our own.
 *
 * From the newmat11 documentation as of 6/7/2013:
 *
 * 1.1 Conditions of use
 * I place no restrictions on the use of newmat except that I take no liability for any problems
 * that may arise from its use, distribution or other dealings with it.
 *
 * You can use it in your commercial projects.
 *
 * You can make and distribute modified or merged versions. You can include parts of it in your own
 * software.
 *
 * If you distribute modified or merged versions, please make it clear which parts are mine and
 * which parts are modified.
 *
 * For a substantially modified version, simply note that it is, in part, derived from my software.
 * A comment in the code will be sufficient.
 *
 * The software is provided "as is", without warranty of any kind.
 *
 * Please understand that there may still be bugs and errors. Use at your own risk. I (Robert
 * Davies) take no responsibility for any errors or omissions in this package or for any misfortune
 * that may befall you or others as a result of your use, distribution or other dealings with it.
 */

namespace Tgs
{

/*====================================Jacobi================================
This code was taken from the newmat11 beta library.  I wanted to turn off
the sorting of the eigenvalues and vectors without altering the original 
library.

/// Eigen value decomposition using Jacobi method.


// Copyright (C) 1991,2,3,4: R B Davies
// Altered by Jason Goffeney SPADAC 2008.
*/
class Jacobi
{
public:
  /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<Jacobi>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  Input:
    SymmetricMatrix & X - The covariance matrix
  Input/Output:
    DiagonalMatrix& D - the resulting eigenvalues
    Matrix& V - the resulting eigenvectors

  */
  static void jacobi(const NEWMAT::SymmetricMatrix& X, NEWMAT::DiagonalMatrix& D,
                     NEWMAT::SymmetricMatrix& A, NEWMAT::Matrix& V, bool eivec);

  static void jacobi(const NEWMAT::SymmetricMatrix& X, NEWMAT::DiagonalMatrix& D);

  static void jacobi(const NEWMAT::SymmetricMatrix& X, NEWMAT::DiagonalMatrix& D,
                     NEWMAT::SymmetricMatrix& A);

  static void jacobi(const NEWMAT::SymmetricMatrix& X, NEWMAT::DiagonalMatrix& D,
                     NEWMAT::Matrix& V);

};

}

#endif

