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
#include "Jacobi.h"
#include <math.h>

using namespace NEWMAT;

namespace Tgs
{

void Jacobi::jacobi(const SymmetricMatrix& X, DiagonalMatrix& D, SymmetricMatrix& A,
   Matrix& V, bool eivec)
{
   double epsilon = pow(2.0,(int)(1-DSIGNIF)); //Create an epsilon that is very close to zero based on double circularError
   //Tracer et("Jacobi");
   //REPORT
   int n = X.Nrows();                                               //Get the size of a side of the Covariance Matrix 
   DiagonalMatrix B(n), Z(n);                                       //Create some intermediary matrices
   D.ReSize(n);                                                     //Copy the covariance into A
   A = X;  
   if (eivec)  //If we need eigenvectors
   { 
     V.ReSize(n,n);   //Make sure the eigenvector matrix is the right size
     D = 1.0;         //Set the eigenvalues to all 1.0
     V = D;           //Not sure what happens here (assume diagonals of V are 1.0)
   }

   B << A;       //Copy A into B (use << because they are different types)
   D = B;        //Copy B into D (same type)
   Z = 0.0;      //Set Z to all zeros 
   A.Inject(Z);  //Copies the diagonal of Z onto diagonal of A without changing the rest of A
                 //In this case the diagonals of A are now 0.

   bool converged = false;
   for (int i=1; i<=50; i++)  //Perform 50 iterations
   {
      double sm=0.0; 
      double* a = A.Store(); //Sets *a as a pointer to A 
      int p = A.Storage();   //Number of stored elements

      while (p--) sm += fabs(*a++); //For each element in A sum it (these are the cross covariance terms of X)
      if (sm==0.0) { converged = true; break; }  //If all the non-diagonal elements are 0 then finished
     
      double tresh = (i<4) ? 0.2 * sm / square(n) : 0.0;  //If this is within the first 3 iterations
                                                          //tresh is the ~average of all the elements in A * 0.2
                                                          //else 0
      a = A.Store();   //Reset a to the first element in A
      for (p = 0; p < n; p++)  //For each element in the diagonal
      {
         double* ap1 = a + (p*(p+1))/2;
         double& zp = Z.element(p);   //Get the pth diagonal value of Z
         double& dp = D.element(p);   //Get the pth diagonal value of D

         for (int q = p+1; q < n; q++)  //For the p+1th to nth diagonal 
         {
            double* ap = ap1; 
            double* aq = a + (q*(q+1))/2;
            double& zq = Z.element(q); 
            double& dq = D.element(q);
            double& apq = A.element(q,p);  //Looks at each element in the pth column below the diagonal
            double g = 100 * fabs(apq);    //??
            double adp = fabs(dp); 
            double adq = fabs(dq);

            if (i>4 && g < epsilon*adp && g < epsilon*adq) //If over the 4th iteration and g is small reletive to epsilon
            {
              apq = 0.0;  //Set the element to 0
            }
            else if (fabs(apq) > tresh)
            {
               //REPORT
               double t; 
               double h = dq - dp; 
               double ah = fabs(h);
               if (g < epsilon*ah) {  t = apq / h; }
               else
               {
                  //REPORT
                  double theta = 0.5 * h / apq;
                  t = 1.0 / ( fabs(theta) + sqrt(1.0 + square(theta)) );
                  if (theta<0.0) { t = -t; }
               }
               double c = 1.0 / sqrt(1.0 + square(t)); 
               double s = t * c;
               double tau = s / (1.0 + c); 
               h = t * apq;
               zp -= h; zq += h; dp -= h; dq += h; apq = 0.0;
               int j = p;
               while (j--)
               {
                  g = *ap; h = *aq;
                  *ap++ = g-s*(h+g*tau); *aq++ = h+s*(g-h*tau);
               }
               int ip = p+1; j = q-ip; ap += ip++; aq++;
               while (j--)
               {
                  g = *ap; h = *aq;
                  *ap = g-s*(h+g*tau); *aq++ = h+s*(g-h*tau);
                  ap += ip++;
               }
               if (q < n-1)             // last loop is non-empty
               {
                  int iq = q+1; j = n-iq; ap += ip++; aq += iq++;
                  for (;;)
                  {
                     g = *ap; h = *aq;
                     *ap = g-s*(h+g*tau); *aq = h+s*(g-h*tau);
                     if (!(--j)) break;
                     ap += ip++; aq += iq++;
                  }
               }
               if (eivec)
               {
                  //REPORT
                  RectMatrixCol VP(V,p); RectMatrixCol VQ(V,q);
                  Rotate(VP, VQ, tau, s);
               }
            }
         }
      }
      B = B + Z; D = B; Z = 0.0;
   }
   if (!converged) Throw(ConvergenceException(X));
   //if (eivec) SortSV(D, V, true); //Commented out from original source by J. Goffeney SPADAC
   //else SortAscending(D); //Commented out from original source by J. Goffeney SPADAC
}

void Jacobi::jacobi(const SymmetricMatrix& X, DiagonalMatrix& D)
{ SymmetricMatrix A; Matrix V; Jacobi::jacobi(X,D,A,V,false); }

void Jacobi::jacobi(const SymmetricMatrix& X, DiagonalMatrix& D, SymmetricMatrix& A)
{ Matrix V; Jacobi::jacobi(X,D,A,V,false); }

void Jacobi::jacobi(const SymmetricMatrix& X, DiagonalMatrix& D, Matrix& V)
{ SymmetricMatrix A; Jacobi::jacobi(X,D,A,V,true); }

}
