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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "GoldenSectionSearch.h"

// Standard
#include <assert.h>
#include <iostream>
#include <math.h>
#include <map>
#include <stdexcept>

using namespace std;

namespace tbs
{

GoldenSectionSearch::GoldenSectionSearch(double epsilon, const int maxCacheSize) :
_maxCacheSize(maxCacheSize),
_epsilon(epsilon)
{
  _phi = (1.0 + sqrt(5.0)) / 2.0;
  _resphi = 2.0 - _phi;
}

double GoldenSectionSearch::argmin(Function& f, double minx, double maxx)
{
  _cache.clear();
  _userF = &f;
  _iterations = 0;

  return _search(minx, (minx + maxx) / 2.0, maxx);
}

double GoldenSectionSearch::_f(double x)
{
  double result;
  map<double, double>::iterator it = _cache.find(x);

  if (it == _cache.end())
  {
    double y = (*_userF)(x);
    // This is just here to keep runaway caches sizes from happening during testing. It can be
    // converted over to an actual LRU cache at some point if needed.
    if (_cache.size() < (unsigned int)_maxCacheSize)
    {
      _cache[x] = y;
    }
    result = y;
  }
  else
  {
    result = it->second;
  }

  return result;
}

double GoldenSectionSearch::_search(double a, double b, double c)
{
  double x;
  _iterations++;

  // sanity check
  assert(_iterations < 1000);

  if (c - b > b - a)
  {
    x = b + _resphi * (c - b);
  }
  else
  {
    x = b - _resphi * (b - a);
  }

  if (fabs(c - a) < _epsilon)
  {
    return (c + a) / 2;
  }

  double fx = _f(x);
  double fb = _f(b);

  // the pseudocode on http://en.wikipedia.org/wiki/Golden_section_search just asserts if
  // f(x) == f(b) however, as can be seen in GoldenSectionSearchTest::runEqualTest it is possible
  // (albeit rare) to have f(x) == f(b). Ugh.
  //
  // Taking out the assert should give the correct answer for monotonic functions with a non-zero
  // slope (except at peak/trough).
  //

  if (fx == fb && _f(a) == _f(c) && _f(a) == _f(b))
  {
    throw invalid_argument("Expected a function where the slope is zero at only one location.");
  }

  if (fx < fb)
  {
    return _searchMid(a, b, c, x);
  }
  else
  {
    return _searchEdge(a, b, c, x);
  }
}

double GoldenSectionSearch::_searchEdge(double a, double b, double c, double x)
{
  if (c - b > b - a)
  {
    return _search(a, b, x);
  }
  else
  {
    return _search(x, b, c);
  }
}

double GoldenSectionSearch::_searchMid(double a, double b, double c, double x)
{
  if (c - b > b - a)
  {
    return _search(b, x, c);
  }
  else
  {
    return _search(a, x, b);
  }
}

}
