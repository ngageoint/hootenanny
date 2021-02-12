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
 * @copyright Copyright (C) 2015 Maxar (http://www.maxar.com/)
 */


#ifndef __TGS__JHEAP_H__
#define __TGS__JHEAP_H__

// Standard Includes
#include <assert.h>
#include <vector>

#include "../TgsException.h"

namespace Tgs
{

/**
* A heap variant that allows items to be inserted multiple times, but keeps only one instance
* in the heap at any time (based on the id).
*
* Order is descending by cost then id.
*
* @optimize when a node gets reinserted it is probably faster to swap it with its parent until
*  it reaches an appropriate hight rather than remove and reinsert.
* @optimize Store all equal values in a linked list at one node rather than put them all into the
*  tree. Given the nature of a cost surface it is very likely that we will have many duplicate
*  values.
*/
template<class _Type>
class TgsJHeap
{
  friend class JHeapTestCase;

public:
  /**
  * A single node in the tree
  */
  class JHeapNode
  {
    friend class JHeapTestCase;

  public:

    JHeapNode()
      {
        _left = NULL;
        _right = NULL;
        _parent = NULL;
        _childCount = 0;
        _index = -1;
      }


    int getChildCount() 
      { 
        return _childCount; 
      }

    _Type getValue() const
      { 
        return _value; 
      }

    int getIndex() 
      { 
        return _index; 
      }

    JHeapNode* getLeft() 
      { 
        return _left; 
      };

    JHeapNode* getRight() 
      { 
        return _right; 
      };

    /**
    * Returns true if this is better than that.
    */
    bool isBetter(const JHeapNode& that)
      {
        // sort by value desc then by index desc
        return (_value < that._value || (_value == that._value && _index < that._index));
      };


    /**
    * promotes one child by moving the lesser child into the better child's right node. This 
    * nodes right child is left empty upon completion.
    */
    void promoteChild()
      {
        // When our right node is already null
        if (_right == NULL)
        {
          return;
        }
        else if (_left == NULL) 
        {
          _left = _right;
          _right = NULL;
          return;
        }
        JHeapNode* skipped = NULL;
        JHeapNode* promotion = NULL;
        // if we have two children, decide which one gets promoted.
        if (_left->isBetter(*_right)) 
        {
          promotion = _left;
          skipped = _right;
        }
        else
        {
          promotion = _right;
          skipped = _left;
        }
        promotion->promoteChild();
        _left = promotion;
        promotion->_right = skipped;
        if (skipped != NULL) 
        {
          skipped->_parent = promotion;
          promotion->_childCount += skipped->_childCount + 1;
        }
        _right = NULL;
      };

    /**
    * Traverses the tree and inserts the node at the appropriate spot.
    */
    void push(JHeapNode* node)
      {
        if (node->_value < _value) 
        {
          throw Exception("Internal Error: attempt to push an inappropriate value in JHeapNode::push");
        }

        node->_childCount = 0;
        node->_left = NULL;
        node->_right = NULL;

        if (_left == NULL) 
        {
          node->_parent = this;
          _left = node;
          _updateChildCount();
          return;
        }
        else if (_right == NULL) 
        {
          node->_parent = this;
          _right = node;
          _updateChildCount();
          return;
        }

        // the side of the tree to insert this node.
        JHeapNode* location = NULL;
        if (_left->_childCount <= _right->_childCount) 
        {
          location = _left;
        }
        else
        {
          location = _right;
        }

        if (node->isBetter(*location)) 
        {
          node->replace(location);
        }
        else
        {
          location->push(node);
        }
      };

    /**
    * Removes this node from the tree & updates all pointers appropriately.
    */
    void remove()
      {
        promoteChild();
        if (_parent != NULL) 
        {
          if (_parent->_left == this) 
          {
            _parent->_left = _left;
          }
          else
          {
            _parent->_right = _left;
          }
        }
        if (_left != NULL) 
        {
          _left->_parent = _parent;
        }
        if (_parent != NULL) 
        {
          _parent->_updateChildCount();
        }
        _parent = NULL;
        _right = NULL;
        _left = NULL;
      };

    /**
    * Replace the given node in the tree, then insert the given node as a child of this node.
    */
    void replace(JHeapNode* node)
      {
        _parent = node->_parent;
        _left = node->_left;
        _right = node->_right;
        if (_left) 
        {
          _left->_parent = this;
        }
        if (_right) 
        {
          _right->_parent = this;
        }
        if (_parent) 
        {
          if (_parent->_left == node) 
          {
            _parent->_left = this;
          }
          else if (_parent->_right == node) 
          {
            _parent->_right = this;
          }
        }
        node->_parent = NULL;
        node->_left = NULL;
        node->_right = NULL;
        _updateChildCount();
        push(node);
      };

    /**
    * Reset to default values.
    */
    void reset()
      {
        _left = NULL;
        _right = NULL;
        _parent = NULL;
        _childCount = 0;
        _index = -1;
      }


    void setValue(float value) 
    { 
      _value = value; 
    };

    void setIndex(int index) 
    { 
      _index = index; 
    };

    JHeapNode* setLeft(JHeapNode* node) 
    { 
      _left = node; 
    };

    void setParent(JHeapNode* node) 
    { 
      _parent = node; 
    };

    JHeapNode* setRight(JHeapNode* node) 
    { 
      _right = node; 
    };

  private:

    JHeapNode* _left;
    JHeapNode* _right;
    JHeapNode* _parent;
    int _childCount;
    /// This index refers to the index in _netCosts (0 to _netCosts.size() - 1). If this value
    /// is -1 then it is assumed that this node isn't actively part of a heap.
    int _index;
    _Type _value;

    /// Updates _childCount based on the values of its left and right children.
    void _updateChildCount()
      {
        _childCount = 0;
        if (_left != NULL) 
        {
          _childCount += _left->_childCount + 1;
        }
        if (_right != NULL) 
        {
          _childCount += _right->_childCount + 1;
        }
        if (_parent != NULL) 
        {
          _parent->_updateChildCount();
        }
      }

    void _check()
      {
        if (_left) 
        {
          _left->_check();
        }
        if (_right) 
        {
          _right->_check();
        }
      }

    int _countChildren()
      {
        int count = 0;
        if (_left) 
        {
          count += _left->_countChildren() + 1;
        }
        if (_right) 
        {
          count += _right->_countChildren() + 1;
        }
        return count;
      }
  };

  TgsJHeap()
  {
    _root = NULL;
  }



  int getChildCount() { return _root != NULL ? _root->getChildCount() + 1 : 0; }

  _Type inspectRootValue() { return _root->getValue(); }
  _Type inspectRootIndex() { return _root->getIndex(); }

  std::vector<JHeapNode>& getNodes() { return _nodes; }

  /**
  * Push a pixel onto the heap. If the pixel is already on the heap, then remove the pixel and 
  * reinsert it.
  * @param id ID of the pixel
  * @param cost Current cost of the pixel
  */
  void push(int id, _Type value)
    {
      assert(_nodes.size() != 0);

      if (_nodes[id].getIndex() != -1) 
      {
        if (_root == &(_nodes[id])) 
        {
          _root->promoteChild();
          JHeapNode* tmp = _root;
          _root = tmp->getLeft();
          if (_root)
          {
            _root->setParent(NULL);
          }
          tmp->reset();
        }
        else
        {
          _nodes[id].remove();
        }
      }
      _nodes[id].setIndex(id);
      _nodes[id].setValue((float)value);
      if (_root == NULL) 
      {
        _root = &(_nodes[id]);
      }
      else
      {
        if (_root->isBetter(_nodes[id])) 
        {
          _root->push(&(_nodes[id]));
        }
        else
        {
          _nodes[id].replace(_root);
          _root = &(_nodes[id]);
        }
      }
    };

  /**
   * Returns the value associated with the root node.
   */
  _Type peekValue() const
  {
    if (_root == NULL)
    {
      throw Exception("No values available");
    }
    else
    {
      return _root->getValue();
    }
  }

  /**
  * Remove the lowest cost value from the heap and return its id.
  */
    int pop()
    {
      assert(_nodes.size() != 0);
      int index;
      if (_root == NULL) 
      {
        index = -1;
      }
      else
      {
        index = _root->getIndex();
        _root->promoteChild();
        JHeapNode* tmp = _root;
        _root = tmp->getLeft();
        tmp->remove();
      }
      return index;
    };


  /**
  * Set the size of the heap. This must be called before the heap is used. It is assumed that
  * the only IDs being used will be from 0 to size - 1.
  */
  void resize(int size, _Type value)
    {
      if (size <= 0) 
      {
        throw Exception("Internal Error: The heap size must be greater than zero.");
      }
      _root = NULL;
      _nodes.resize(size);
      for (int i = 0; i < size; i++)
      {
        _nodes[i].setValue((float)value);
        _nodes[i].reset();
      }
    };


  /**
  * Returns true if there is at least one node in the heap.
  */
  bool hasNodes() const
    {
      return (_root != NULL);
    }

private:

  JHeapNode* _root;

  /// Used to make sure only one node exists for each ID
  std::vector<JHeapNode> _nodes;
};

}

#endif

