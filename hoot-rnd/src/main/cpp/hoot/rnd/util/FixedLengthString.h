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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef FIXEDLENGTHSTRING_H
#define FIXEDLENGTHSTRING_H

// STXXL
#include <stxxl/map>

namespace hoot
{

//this can be raised periodically, as needed
//TODO: move it to a config?
static const int MAX_KEY_LEN = 200;

class FixedLengthString
{
public:

  char data[MAX_KEY_LEN];

  bool operator<(const FixedLengthString& str) const;
  bool operator==(const FixedLengthString& str) const;
  bool operator!=(const FixedLengthString& str) const;
};

struct FixedLengthStringCompare : public std::less<FixedLengthString>
{
  static FixedLengthString max_value()
  {
    FixedLengthString s;
    std::fill(s.data, s.data + MAX_KEY_LEN, 0x7f);
    return s;
  }
};

typedef stxxl::map<FixedLengthString, long, FixedLengthStringCompare, 4096, 4096> FixedLengthStringToLongMap;
typedef stxxl::map<FixedLengthString, FixedLengthString, FixedLengthStringCompare, 4096, 4096> FixedLengthStringToFixedLengthStringMap;

}

#endif // FIXEDLENGTHSTRING_H
