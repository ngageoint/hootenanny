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
#include "InternalRStarTreeWrapper.h"

//TGS Includes
#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/MemoryPageStore.h>

//Std Includes
#include <cassert>

namespace Tgs
{
  InternalRStarTreeWrapper::InternalRStarTreeWrapper(unsigned int pageSize, unsigned int dimensions)
    : _dimensions(dimensions)
  {
    std::shared_ptr<MemoryPageStore> mps(new MemoryPageStore((int)pageSize));

    _tree = std::shared_ptr<HilbertRTree>(new HilbertRTree(mps, dimensions));
  }

  InternalRStarTreeWrapper::~InternalRStarTreeWrapper()
  {

  }

  void InternalRStarTreeWrapper::bulkInsert(const std::vector<int>& uniqueId,
    const std::vector<double>& minBounds,
    const std::vector<double>& maxBounds)
  {
    std::vector<Box> boxes;
    int dimensions = _tree->getRoot()->getDimensions();
    int boxCount = minBounds.size() / dimensions;
    boxes.resize(boxCount);
    unsigned int index = 0;
    for (int i = 0; i < boxCount; i++)
    {
      boxes[i].setDimensions(dimensions);
      boxes[i].setBounds(0, minBounds[index], maxBounds[index]);
      index++;
      boxes[i].setBounds(1, minBounds[index], maxBounds[index]);
      index++;
      boxes[i].setBounds(2, minBounds[index], maxBounds[index]);
      index++;
    }
    _tree->bulkInsert(boxes, uniqueId);
  }

  void InternalRStarTreeWrapper::getIntersectingObjects(const std::vector<double>& minBounds,
    const std::vector<double>& maxBounds, std::vector<int>& objIds)
  {
    std::shared_ptr<IntersectionIterator> interItr = std::shared_ptr<IntersectionIterator>(new IntersectionIterator(_tree.get(), minBounds, maxBounds));

    while (interItr->next())
    {
      objIds.push_back(interItr->getId());
    }
  }

  void InternalRStarTreeWrapper::getObjectsWithinRange(std::vector<double> point, double radius, std::vector<int> & objIds)
  {
    std::shared_ptr<DistanceIterator> distItr = std::shared_ptr<DistanceIterator>(new DistanceIterator(_tree.get(), point, radius));

    while (distItr->next())
    {
      objIds.push_back(distItr->getId());
    }
  }

  void InternalRStarTreeWrapper::insert(int objId, std::vector<double> & minBounds,
    std::vector<double> & maxBounds)
  {
    assert(_dimensions == minBounds.size());
    assert(_dimensions == maxBounds.size());

    Box tmpBox(_dimensions);

    for (unsigned int i = 0; i < _dimensions; i++)
    {
      tmpBox.setBounds(i, minBounds[i], maxBounds[i]);
    }

    _tree->insert(tmpBox, objId);
  }

}
