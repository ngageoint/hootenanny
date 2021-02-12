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
 * @copyright Copyright (C) 2015, 2017, 2019 Maxar (http://www.maxar.com/)
 */

#include "RTreeNodeStore.h"

// Standard Includes
#include <cassert>
#include <iostream>

namespace Tgs
{
  const int MAX_NODE_COUNT = 100000;

  RTreeNodeStore::RTreeNodeStore(int dimensions, const std::shared_ptr<PageStore>& ps)
  {
    _dimensions = dimensions;
    _storeSp = ps;
    _store = ps.get();
  }

  RTreeNodeStore::~RTreeNodeStore()
  {
    _store->save();
    NodeMap::iterator it = _availableNodes.begin();
    while (it != _availableNodes.end())
    {
      RecItem * item = it->second;
      delete item;
      ++it;
    }
  }

  void RTreeNodeStore::_addNode( RecItem * item, int key )
  {
    _availableNodes[key] = item;
  }

  RTreeNode* RTreeNodeStore::createNode()
  {
    std::shared_ptr<Page> page = _store->createPage();
    page->setDirty();
    RTreeNode* node = new RTreeNode(_dimensions, page);
    node->clear();
    
    RecItem * item = new RecItem();
    item->pNode = node;
    item->list_it = _nodesList.insert(_nodesList.begin(), node->getId());
    _addNode(item, node->getId());

    _flushNodes();
    return node;
  }

  const RTreeNode* RTreeNodeStore::getNode(int id) const
  {
    RTreeNode * node;

    // if exists
    NodeMap::const_iterator it = _availableNodes.find(id);
    if (it == _availableNodes.end())
    {
      RTreeNodeStore* me = const_cast<RTreeNodeStore*>(this);
      std::shared_ptr<Page> page = me->_store->getPage(id);
      node = new RTreeNode(_dimensions, page);
      RecItem * item = new RecItem();
      item->pNode = node;
      item->list_it = _nodesList.insert(_nodesList.begin(), id);
      me->_addNode(item, node->getId());
      me->_flushNodes();
    }
    else 
    {
      RecItem* item = it->second;
      node = item->pNode;
      _nodesList.splice(_nodesList.begin(), _nodesList, item->list_it);
    }

    return node;
  }

  RTreeNode * RTreeNodeStore::getNode( int id ) 
  {
    RTreeNode * node;

    // if exists
    NodeMap::const_iterator it = _availableNodes.find(id);
    if (it != _availableNodes.end())
    {
      RecItem* item = it->second;
      _nodesList.splice(_nodesList.begin(), _nodesList, item->list_it);
      node = item->pNode;
    }
    else
    {
      std::shared_ptr<Page> page = _store->getPage(id);
      node = new RTreeNode(_dimensions, page);
      RecItem * item = new RecItem();
      item->pNode = node;
      item->list_it = _nodesList.insert(_nodesList.begin(), id);
      _addNode(item, node->getId());
      _flushNodes();
    }

    return node;
  }

  void RTreeNodeStore::_flushNodes()
  {
    while ((int)_nodesList.size() > MAX_NODE_COUNT)
    {
      int keyVal = _nodesList.back();
      _nodesList.pop_back();

      NodeMap::iterator it = _availableNodes.find(keyVal);
      if (it != _availableNodes.end())
      {
        RecItem * item = it->second;
        delete item;
        _availableNodes.erase(it);
      }
    }
    assert(_nodesList.size() == _availableNodes.size());
  }
}
