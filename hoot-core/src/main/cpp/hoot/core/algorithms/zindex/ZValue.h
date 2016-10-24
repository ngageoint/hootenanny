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
 * @copyright Copyright (C) 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef ZVALUE_H
#define ZVALUE_H

#include <string>
#include <vector>

namespace hoot
{

using namespace std;

class ZValue
{
public:

  static string className() { return "hoot::ZValue"; }

  ZValue() {}

  ZValue(int dimensions, int depth, vector<double> min, vector<double> max);

  ~ZValue();

  long int calculate(vector<double> point);

  /**
   * Calculates the z value by interleaving values that are already scaled to
   * the proper space.
   */
  long int calculate(vector<long int> point);

  /**
   * Calculates the non-interleaved component for one dimension.
   */
  long int calculateComponent(double v, int d);

  void decompose(long int v, vector<long int>& point);

  int getDepth() { return _depth; }

  int getDimensions() { return _dimensions; }

  double getMax(int d) { return _max[d]; }

  double getMin(int d) { return _min[d]; }

  /**
    * Returns the maximum value in any one dimension (number of bins in that
    * direction - 1).
    */
  long int getMaxDimensionRange() { return _range; }

private:
  vector<long int> _b;
  int _depth;
  int _dimensions;
  vector<double> _min;
  vector<double> _max;
  long int _range;
};

}

#endif // ZVALUE_H
