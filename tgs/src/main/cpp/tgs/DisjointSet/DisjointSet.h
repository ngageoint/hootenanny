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
 * @copyright Copyright (C) 2006 Pedro Felzenszwalb
 * @copyright Copyright (C) 2015, 2017, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef DISJOINTSET_H
#define DISJOINTSET_H

/**
 * @copyright GPLv2 or later
 * @author Pedro F. Felzenszwalb and Daniel P. Huttenlocher
 * http://cs.brown.edu/~pff/segment/
 */

namespace Tgs
{
// disjoint-set forests using union-by-rank and path compression (sort of).

struct uni_elt
{
  int rank;
  int p;
  int size;
};

class DisjointSet
{
public:
  DisjointSet(int elements);
  ~DisjointSet();
  int find(int x);
  void join(int x, int y);
  int size(int x) const { return elts[x].size; }
  int num_sets() const { return num; }

private:
  //  Don't allow copy constructor
  DisjointSet(const DisjointSet&) { }

  uni_elt *elts;
  int num;
};

inline DisjointSet::DisjointSet(int elements)
{
  elts = new uni_elt[elements];
  num = elements;
  for (int i = 0; i < elements; i++)
  {
    elts[i].rank = 0;
    elts[i].size = 1;
    elts[i].p = i;
  }
}

inline DisjointSet::~DisjointSet()
{
  delete [] elts;
}

inline int DisjointSet::find(int x)
{
  int y = x;
  while (y != elts[y].p)
    y = elts[y].p;
  elts[x].p = y;
  return y;
}

inline void DisjointSet::join(int x, int y)
{
  x = find(x);
  y = find(y);
  if (elts[x].rank > elts[y].rank)
  {
    elts[y].p = x;
    elts[x].size += elts[y].size;
  }
  else
  {
    elts[x].p = y;
    elts[y].size += elts[x].size;
    if (elts[x].rank == elts[y].rank)
      elts[y].rank++;
  }
  num--;
}

}

#endif // DISJOINTSET_H
