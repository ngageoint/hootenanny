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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__RTREE_NODE_STORE_H__
#define __TGS__RTREE_NODE_STORE_H__

// Standard Includes
#include <list>
#include <memory>

#include <tgs/HashMap.h>
#include <tgs/RStarTree/RTreeNode.h>
#include <tgs/RStarTree/PageStore.h>

namespace Tgs
{

/**
 * The NodeStore provides a way of persistently storing and efficiently accessing nodes in a
 * tree.
 *
 * At some point an intelligent caching mechanism should be added.
 */
class TGS_EXPORT RTreeNodeStore
{
public:

  RTreeNodeStore(int dimensions, const std::shared_ptr<PageStore>& ps);

  ~RTreeNodeStore();

  /**
   * The returned node is only guaranteed to be valid until another RTreeNodeStore method is
   * called.
   */
  RTreeNode* createNode();

  /**
   * The returned node is only guaranteed to be valid until another RTreeNodeStore method is
   * called.
   *
   * This method is thread safe. It is the only thread safe method in RTreeNodeStore.
   */
  const RTreeNode* getNode(int id) const;

  RTreeNode* getNode(int id);

protected:

  void _flushNodes();
  class RecItem
  {
  public:

    ~RecItem()
    {
      delete pNode;
    }

    std::list<int>::iterator list_it;
    RTreeNode * pNode;
  };

  typedef HashMap<int, RecItem*> NodeMap;

  // mutable cache
  NodeMap _availableNodes;
  int _dimensions;
  std::shared_ptr<PageStore> _storeSp;
  /// pointer to the same thing as above, only faster. Zoom zoom!
  PageStore* _store;

  void _addNode(RecItem * item, int key);
  mutable std::list<int>_nodesList;
};

}

#endif
