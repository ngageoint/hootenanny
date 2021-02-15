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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef __INTERNAL_RSTAR_TREE_WRAPPER_H__
#define __INTERNAL_RSTAR_TREE_WRAPPER_H__

//TGS Includes
#include <tgs/TgsExport.h>
#include <tgs/RStarTree/DistanceIterator.h>
#include <tgs/RStarTree/IntersectionIterator.h>
#include <tgs/RStarTree/HilbertRTree.h>

//Std Includes
#include <vector>

namespace Tgs
{
  class InternalRStarTreeWrapper
  {
  public:
    /**
    *  Constructor
    */
    InternalRStarTreeWrapper(unsigned int pageSize, unsigned int dimensions);

    /**
    *  Destructor
    */
    ~InternalRStarTreeWrapper();

    /**
    *  Updates an iterator over all the current R*Tree of all objects intersecting
    * with the input bounding region
    *
    * @param minBounds the minimum values of the bounding region of interest
    * @param maxBounds the minimum values of the bounding region of interest
    * @param returns the list of object ids
    */
    void getIntersectingObjects(const std::vector<double>& minBounds, const std::vector<double>& maxBounds, std::vector<int>& objIds);

    /**
    *  Updates an iterator over all the current R*Tree of all objects within
    * a radius of the given point
    *
    * @param point the point of interest
    * @param radius the radius from the point of interest to collect objects
    * @param returns the list of object ids
    */
    void getObjectsWithinRange(std::vector<double> point, double radius, std::vector<int> & objIds);

    /**
    *  Inserts a new element into the tree
    *  @param uniqueId the key for the element
    *  @param minBounds the list of minimum bounds by each dimension
    *  @param maxBounds the list of maximum bounds by each dimension
    */
    void insert(int uniqueId, std::vector<double> & minBounds, std::vector<double> & maxBounds);

    void bulkInsert(const std::vector<int>& uniqueId, const std::vector<double>& minBounds,
      const std::vector<double>& maxBounds);

  private:
    std::shared_ptr<HilbertRTree> _tree;
    unsigned int _dimensions;
  };
}
#endif
