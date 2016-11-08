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
#ifndef RANGE_H
#define RANGE_H

//Qt includes
#include <QString>

namespace hoot
{

using namespace std;

/**
 * Range of values. Min is inclusive. Max is inclusive.
 */

class Range
{
public:

  static string className() { return "hoot::Range"; }

  Range() {}

  Range(long int min, long int max);

  virtual ~Range() {}

  bool hashCode();

  bool operator>(Range r) { return (_min - r.getMin()) > 0; }

  bool operator<(Range r) { return (_min - r.getMin()) < 0; }

  bool operator=(Range r) { return _min == r.getMin(); }

  bool in(long int l);

  bool isValid();

  void setInvalid();

  QString toString();

  long calculateSize() { return (_max - _min) + 1; }

  long int getMin() { return _min; }

  long int getMax() { return _max; }

  void set(long int min, long int max);

private:
  long int _min;
  long int _max;
};

}

#endif // RANGE_H
