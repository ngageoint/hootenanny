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
#ifndef GOLDENSECTIONSEARCH_H
#define GOLDENSECTIONSEARCH_H

#include <tbs/optimization/LineSearch.h>

// Standard
#include <map>

namespace tbs
{

/**
 * Uses the Golden Section Search to look for a local minimum. [1]
 *
 * This implementation will not work on functions where the slope is zero at more than one location.
 * The function should also be monotonic. If it isn't, the result is undefined.
 *
 * This class is re-entrant, but not thread safe.
 *
 * 1. http://en.wikipedia.org/wiki/Golden_section_search
 */
class GoldenSectionSearch : public LineSearch
{
public:
  /**
   * @param epsilon Find x within epsilon of a local minimum.
   */
  GoldenSectionSearch(double epsilon);

  virtual double argmin(Function& f, double minx, double maxx);

private:
  double _phi;
  double _resphi;
  std::map<double, double> _cache;
  double _epsilon;
  Function* _userF;
  int _iterations;

  double _search(double a, double b, double c);
  double _searchEdge(double a, double b, double c, double x);
  double _searchMid(double a, double b, double c, double x);

  double _f(double x);
};

}

#endif // GOLDENSECTIONSEARCH_H
