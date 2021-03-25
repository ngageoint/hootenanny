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
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "LongestCommonNodeString.h"

// Hoot
#include <hoot/core/elements/Way.h>

// Standard
#include <iostream>
#include <vector>
using namespace std;

namespace hoot
{

LongestCommonNodeString::LongestCommonNodeString(const std::shared_ptr<Way>& w1, const std::shared_ptr<Way>& w2)
{
  _w1 = w1;
  _w2 = w2;
}

int LongestCommonNodeString::apply()
{
  _i1 = -1;
  _i2 = -1;

  if (_w1->getNodeCount() == 0 || _w2->getNodeCount() == 0)
  {
       return 0;
  }

  const vector<long>& str1 = _w1->getNodeIds();
  const vector<long>& str2 = _w2->getNodeIds();

  int *curr = new int [_w2->getNodeCount()];
  int *prev = new int [_w2->getNodeCount()];
  int *swap = nullptr;
  int maxSubstr = 0;

  for (size_t i = 0; i < str1.size(); ++i)
  {
    for (size_t j = 0; j < str2.size(); ++j)
    {
      if (str1[i] != str2[j])
      {
        curr[j] = 0;
      }
      else
      {
        if (i == 0 || j == 0)
        {
          curr[j] = 1;
        }
        else
        {
          curr[j] = 1 + prev[j-1];
        }
        if (curr[j] > maxSubstr)
        {
          _i1 = (int)i - curr[j] + 1;
          _i2 = (int)j - curr[j] + 1;
          maxSubstr = curr[j];
        }
      }
    }
    swap = curr;
    curr = prev;
    prev = swap;
  }
  delete [] curr;
  delete [] prev;

  return maxSubstr;
}

}
