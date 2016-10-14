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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef SPARSE2DMATRIX_H
#define SPARSE2DMATRIX_H

// tgs
#include <tgs/HashMap.h>


namespace hoot
{
using namespace std;

struct Sparse2dCellId : public pair<int, int>
{
  Sparse2dCellId()
  {
    first = -1;
    second = -1;
  }

  Sparse2dCellId(int row, int col)
  {
    first = row;
    second = col;
  }

  int& row() { return first; }
  int& col() { return second; }

  int row() const { return first; }
  int col() const { return second; }
};
}

namespace __gnu_cxx
{
template<>
  struct hash<hoot::Sparse2dCellId>
  {
    size_t
    operator()(const hoot::Sparse2dCellId& k) const
    { return Tgs::cantorPairing(k); }
  };
}

namespace hoot
{

/**
 * I looked at the boost implementation, but it didn't have an iterator to iterate over the
 * populated cells. That is the main point of this implementation.
 */
class Sparse2dMatrix
{
public:

  typedef Sparse2dCellId CellId;
  typedef HashMap<CellId, double>::const_iterator const_iterator;

  Sparse2dMatrix();

  double get(const CellId& cid) const
  {
    const_iterator it = _map.find(cid);
    if (it != _map.end())
    {
      return it->second;
    }
    else
    {
      return 0.0;
    }
  }

  double get(int row, int col) const { return get(CellId(row, col)); }

  void set(int row, int col, double v) { _map[CellId(row, col)] = v; }

  HashMap<CellId, double>::const_iterator begin() const { return _map.begin(); }

  HashMap<CellId, double>::const_iterator end() const { return _map.end(); }

  QString toString() const;

private:
  HashMap<CellId, double> _map;
};

}

#endif // SPARSE2DMATRIX_H
