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
 * @copyright Copyright (C) 2015, 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RandomForestThread.h"

#include <tgs/TgsException.h>

namespace Tgs
{
  RandomForestThread::RandomForestThread(const std::shared_ptr<DataFrame>& data,
    const std::shared_ptr<RandomTree>& tree, unsigned int numFactors, unsigned int nodeSize,
    bool makeBalanced)
  {
    _data = data;
    _tree = tree;
    _numFactors = numFactors;
    _nodeSize = nodeSize;
    _makeBalanced = makeBalanced;
  }

  void RandomForestThread::run()
  {
    try
    {
      _tree->trainMulticlass(_data, _numFactors, _nodeSize, _makeBalanced);
    }
    catch(const Exception & e)
    {
      std::stringstream pathStr;
      pathStr << typeid(this).name() << "::" << __FUNCTION__ << " Line( " << __LINE__ << "): -->" <<
        e.what();
      emit exceptionThrown(e.what());
    }
  }
}

