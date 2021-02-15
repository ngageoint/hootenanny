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
 * @copyright Copyright (C) 2015, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef LINESEARCH_H
#define LINESEARCH_H

namespace tbs
{

/**
 * Searches over a bounded range for an approximate local minimum.
 */
class LineSearch
{
public:

  class Function
  {
  public:
    /**
     * Calculates a function f for the given x.
     */
    virtual double operator()(double x) = 0;
  };

  /**
   * Searches within the Function f for a local maximum within the bounds minx and minx.
   *
   * The default implementation wraps the users function with a negation and calls argmin.
   *
   * @return An approximation of a local minimum.
   */
  virtual double argmax(Function& f, double minx, double maxx);

  /**
   * Searches within the Function f for a local minimum within the bounds minx and minx.
   *
   * @return An approximation of a local minimum.
   */
  virtual double argmin(Function& f, double minx, double maxx) = 0;
};

}

#endif // LINESEARCH_H
