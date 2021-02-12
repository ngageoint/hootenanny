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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */

#ifndef __TGC__INTERSECTION_ITERATOR_H__
#define __TGC__INTERSECTION_ITERATOR_H__

// Standard Includes
#include <list>

#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/Iterator.h>

namespace Tgs
{

class RStarTree;

/**
 * This class returns all items from an RStarTree that fall within a specified range.
 * Distance is calculated as a simple euclidean distance in n dimensional space.
 */
class TGS_EXPORT IntersectionIterator : public Iterator
{
public:

  IntersectionIterator(const RStarTree* tree, const std::vector<double>& minBounds,
                       const std::vector<double>& maxBounds);

  virtual ~IntersectionIterator() {}

  virtual const Box& getBox() const;

  virtual int getId() const;

  virtual bool hasNext();

  virtual bool next();

protected:

  bool _determineIntesection(const Box& box);

private:

  class Result
  {
  public:

    Result() : id(-1) { }

    Result(const Box& box, int id) : box(box), id(id) { }

    Box box;
    int id;
  };

  const RStarTree* _tree;
  std::vector<double> _minBounds;
  std::vector<double> _maxBounds;
  bool _done;
  std::list<int> _pendingNodes;
  std::list<Result> _pendingResults;
  Result _currentResult;

  void _populateNext();
};

}

#endif
