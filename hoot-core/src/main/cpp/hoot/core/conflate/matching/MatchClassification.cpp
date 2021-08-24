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
 * @copyright Copyright (C) 2015, 2017, 2018, 2021 Maxar (http://www.maxar.com/)
 */
#include "MatchClassification.h"

// standard
#include <algorithm>
#include <math.h>

namespace hoot
{
using namespace std;

MatchClassification::MatchClassification()
{
  clear();
}

MatchClassification::MatchClassification(double match, double miss, double review) :
  _match(match),
  _miss(miss),
  _review(review)
{
}

bool MatchClassification::isValid() const
{
  const double epsilon = 1e-5;
  return _match >= 0.0 && _miss >= 0.0 && _review >= 0.0 &&
      fabs(1 - _match + _miss + _review) <= epsilon;
}

void MatchClassification::normalize()
{
  _match = max(0.0, _match);
  _miss = max(0.0, _miss);
  _review = max(0.0, _review);

  double sum = _match + _miss + _review;

  if (sum > 0.0)
  {
    _match /= sum;
    _miss /= sum;
    _review /= sum;
  }
  else
  {
    throw IllegalArgumentException("MatchClassifications are not greater than zero.");
  }
}



}
