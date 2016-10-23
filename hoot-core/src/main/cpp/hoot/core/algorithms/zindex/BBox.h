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
#ifndef BBOX_H
#define BBOX_H

#include <QString>
#include <geos/geom/Envelope.h>

namespace hoot
{

using namespace std;

using namespace geos::geom;

class BBox
{
public:

  static string className() { return "hoot::BBox"; }

  BBox(vector<double> min, vector<double> max);

  BBox(Envelope envelope);

  virtual ~BBox();

  int getDimensions() { return _max.size(); }

  vector<double> getMax() { return _max; }

  vector<double> getMin() { return _min; }

  bool in(vector<double> p);

  bool in(BBox container);

  bool intersects(BBox b);

  /**
  * Returns the minimum distance in any one dimension. This is not
  * necessarily the Euclidean distance.
  */
  double manhattanDistance(BBox b);

  /**
   * @brief toString
   * @return QString
   */
  QString toString();

  double getWidth(int d);

private:
  void _check();
  vector<double> _min;
  vector<double> _max;
};

}

#endif // BBOX_H
