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
 * @copyright Copyright (C) 1998, 1999 Eibe Frank, Leonard Trigg, Mark Hall
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "LogisticRegression.h"

// Hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>

// Standard
#include <math.h>

using namespace std;

namespace hoot
{

LogisticRegression::LogisticRegression()
{
}

double LogisticRegression::evaluate(map<QString, double> sample)
{
  // Taken from Weka's documentation:
  //
  // The probability for class j with the exception of the last class is
  // Pj(Xi) = exp(XiBj)/((sum[j=1..(k-1)]exp(Xi*Bj))+1)
  // since we only have two classes:

  // this code shamelessly modified from Weka

  double prob = 0.0;
  double v = 0.0;

  // Log-posterior before normalizing
  for (Coeff::iterator it = _coeff.begin(); it != _coeff.end(); ++it)
  {
    double par = it->second;
    map<QString, double>::const_iterator sit = sample.find(it->first);
    if (sit == sample.end())
    {
      throw HootException("Error finding sample key: " + it->first);
    }
    double data = sample.find(it->first)->second;
    v += par * data;
  }

  v += _intercept;

  // Do so to avoid scaling problems
  prob = 1 / (1 + exp(-v));

  return prob;
}

}
