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
 * @copyright Copyright (C) 2015, 2017, 2018, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef SUBTREECALLBACK_H
#define SUBTREECALLBACK_H

// hoot
#include <hoot/core/index/metric-hybrid/Node.h>

namespace hoot
{

namespace hybrid
{

template <class KeyType, class DataType>
class SubTreeCallback
{
public:

  virtual Node<KeyType, DataType>* buildTree(size_t start, size_t end, int depth) = 0;

  // default implementation inserts all children. You will likely want to do a final check depending
  // on the data.
  virtual void findLeaf(const Leaf<KeyType, DataType>* leaf, std::set<DataType>& result) const
  {
    for (size_t i = 0; i < leaf->getChildCount(); ++i)
    {
      result.insert(leaf->get(i).second);
    }
  }

  virtual void find(const Node<KeyType, DataType>* n, int depth, std::set<DataType>& result) const = 0;
};

}

}

#endif // SUBTREECALLBACK_H
