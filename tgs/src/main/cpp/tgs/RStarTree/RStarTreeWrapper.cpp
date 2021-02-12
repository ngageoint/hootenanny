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
 * @copyright Copyright (C) 2015, 2017, 2018 Maxar (http://www.maxar.com/)
 */
#include "RStarTreeWrapper.h"

//TGS Includes
#include <tgs/RStarTree/InternalRStarTreeWrapper.h>

namespace Tgs
{
  RStarTreeWrapper::RStarTreeWrapper(unsigned int pageSize, unsigned int dimensions)
  {
    _irtw  = new InternalRStarTreeWrapper(pageSize, dimensions);
  }

  RStarTreeWrapper::~RStarTreeWrapper()
  {
    delete _irtw;
  }

  void RStarTreeWrapper::bulkInsert(const std::vector<int>& uniqueId,
    const std::vector<double>& minBounds,
    const std::vector<double>& maxBounds)
  {
    _irtw->bulkInsert(uniqueId, minBounds, maxBounds);
  }

  void RStarTreeWrapper::getIntersectingObjects(const std::vector<double>& minBounds,
    const std::vector<double>& maxBounds, std::vector<int>& objIds)
  {
    _irtw->getIntersectingObjects(minBounds, maxBounds, objIds);
  }

  void RStarTreeWrapper::getObjectsWithinRange(std::vector<double> point, double radius,
    std::vector<int> & objIds)
  {
    _irtw->getObjectsWithinRange(point, radius, objIds);
  }

  void RStarTreeWrapper::insert(int objId, std::vector<double> & minBounds,
                                std::vector<double> & maxBounds)
  {
    _irtw->insert(objId, minBounds, maxBounds);
  }
}

