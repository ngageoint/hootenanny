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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __TGS__DISTANCE_ITERATOR_H__
#define __TGS__DISTANCE_ITERATOR_H__

// Standard Includes
#include <list>

#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/Iterator.h>

namespace Tgs
{

class RStarTree;

/**
 * This class returns all items from an RStarTree that fall within a specified distance.
 * Distance is calculated as a simple euclidean distance in n dimensional space. This is probably
 * not what you desire.
 */
class TGS_EXPORT DistanceIterator : public Iterator
{
public:
  DistanceIterator(RStarTree* tree, const std::vector<double>& point, double distance);

  virtual ~DistanceIterator() {}

  virtual const Box& getBox() const;

  virtual int getId() const;

  virtual bool hasNext();

  virtual bool next();

  int nodeHits;
  int distCalcs;

protected:
  double _calculateDistance(const std::vector<double>& point, const Box& box);

private:
  class Result
  {
  public:

    Result() : id(-1) { }

    Result(const Box& box, int id) : box(box), id(id) { }

    Box box;
    int id;
  };

  RStarTree* _tree;
  std::vector<double> _point;
  double _distance;
  bool _done;
  std::list<int> _pendingNodes;
  std::list<Result> _pendingResults;
  Result _currentResult;

  void _populateNext();
};

}

#endif
