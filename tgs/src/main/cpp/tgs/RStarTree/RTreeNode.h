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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef __TGS__RTREE_NODE_H__
#define __TGS__RTREE_NODE_H__

// Tgs
#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/Page.h>
#include <tgs/RStarTree/PageStore.h>

// Standard
#include <iostream>
#include <memory>

namespace Tgs
{

class RTreeNode;
class RTreeNodeStore;
class RTreeNodeTest;

class TGS_EXPORT BoxInternalData
{
public:

  /**
   * Calculates the distance from the centroid of this box to the centroid of Box b.
   */
  double calculateCentroidDistance(const Box& b) const;

  /**
   * Calculate the amount this box would have to expand in order to include the box specified.
   */
  double calculateExpansion(const Box& b) const;

  /**
   * Calculates the overlap between this box and b.
   */
  double calculateOverlap(const Box& b) const;

  double calculateVolume() const;

  int getDimensions() const;

  double getLowerBound(int d) const;

  double getUpperBound(int d) const;

  double getLowerBoundRaw(int d) const;

  double getUpperBoundRaw(int d) const;

  bool isContained(const Box& b) const;

  void setBounds(int d, double lower, double upper) const;

  /**
   * Returns the number of bytes this structure will take up with the given number of
   * dimensions.
   */
  static int size(int dimensions);

  Box toBox() const;

private:

  friend class RTreeNode;

  BoxInternalData(int dimensions, const char* data);
  BoxInternalData(int dimensions, const char* data, const Box& b);

  const char* _data;
  int _dimensions;

};

inline double BoxInternalData::getLowerBoundRaw(int d) const
{
  const double* v = (const double*)_data;
  return v[d * 2];
}

inline double BoxInternalData::getUpperBoundRaw(int d) const
{
  const double* v = (const double*)_data;
  return v[d * 2 + 1];
}

/**
 * This class is filled with yucky pointer math. If you aren't "old school" you probably want
 * to turn tail and run.
 *
 * The const methods are defined as const for a reason. If this is causing you problems you will
 * want to think long and hard before changing them.
 */
class TGS_EXPORT RTreeNode
{
public:

  virtual ~RTreeNode() = default;

  /**
   * This is only intended for RStarTree class
   */
  void addChild(const Box& envelope, int id);

  /**
   * Append this child onto the known list of children. Calling this method is only valid if
   * the node is empty or a non-leaf.
   */
  void addNodeChild(RTreeNode* node);

  /**
   * Append this child onto the known list of user defined children. Calling this method is
   * only valid if the node is empty or a non-leaf.
   */
  void addUserChild(const Box& envelope, int id);

  /**
   * Calculate the envelope of all the boxes and return it.
   */
  Box calculateEnvelope() const;

  /**
   * Remove all children and reset to the default state. This should be called if this is a new
   * RTreeNode w/ uninitialized data.
   */
  void clear();

  /**
   * Returns the index of a child for the given id. If there are more than one children with
   * the given id the first index is returned. If the id isn't found a -1 is returned.
   */
  int convertChildIdToIndex(int id) const;

  /**
   * Returns the number of children that have been assigned.
   */
  int getChildCount() const;

  /**
   * Returns the envelope for a specific child index
   * @param childIndex >= 0 && < getChildCount()
   */
  BoxInternalData getChildEnvelope(int childIndex) const;

  /**
   * This method should rarely be used, and probably only be internal structure, not iterators
   * You probably want to use either getChildUserId. This is not const for that reason.
   */
  int getChildId(int childIndex);

  /**
   * Returns the node ID for a specific child index. Calling this method is not valid if this
   * node is a leaf node.
   * @param childIndex >= 0 && < getChildCount()
   */
  int getChildNodeId(int childIndex) const;

  /**
   * Returns the user ID for a specific child index. Calling this method is not valid if this
   * node is not a leaf node.
   * @param childIndex >= 0 && < getChildCount()
   */
  int getChildUserId(int childIndex) const;

  /**
   * Returns the number of dimensions for this node
   */
  int getDimensions() const { return _dimensions; }

  /**
   * Returns this node's ID
   */
  int getId() const { return _id; }

  int getMaxChildCount() const { return _maxChildCount; }

  int getParentId() const;

  /**
   * Returns true if this is a leaf node.
   */
  bool isLeafNode() const;

  /**
   * Removes the specified child from the list. All children to the right will be shifted one
   * space left.
   */
  void removeChild(int childIndex);

  /**
   * Removes a series of children by index. The vector _will_ be modified.
   */
  void removeChildren(std::vector<int>& childIndexes);

  /**
   * Marks this node as dirty so it will be saved.
   */
  void setDirty() { _page->setDirty(); }

  /**
   * Sets the parent id.
   */
  void setParentId(int id);

  /**
   * Updates the bounding box that this child has.
   */
  void updateChildEnvelope(int childIndex, const Box& b);

  void updateChild(int childIndex, int id, const Box& b);

private:

  friend class RTreeNodeStore;
  friend class RTreeNodeTest;

  struct Header
  {
    int childCount;
    int parentId;
  };

  struct ChildData
  {
    int id;
    const char* getBox() const { return ((const char*)&id) + 4; }
    char* getBox() { return ((char*)&id) + 4; }
  };

  RTreeNode(int dimensions, const std::shared_ptr<Page>& page);
  /**
   * Return the start of the child's data. This includes the box (BoxInternalData) and the
   * index.
   */
  ChildData* _getChildPtr(int index);

  /**
   * Similar to above.
   */
  const ChildData* _getChildPtr(int index) const;

  /**
   * Return the number of bytes that a child takes up.
   */
  int _getChildSize() const;

  Header* _getHeader();

  const Header* _getHeader() const;

  static int _getHeaderSize() { return sizeof(Header); }

  int _maxChildCount;
  int _dimensions;
  mutable std::shared_ptr<Page> _pageSp;
  /// pointer to the same thing as above, only faster. Zoom zoom!
  mutable Page* _page;
  int _id;
};

inline std::ostream& operator<<(std::ostream & o, RTreeNode& n)
{
  int size = n.getChildCount();
  o << "id: " << n.getId() << " parent: " << n.getParentId() << " children: {";

  for (int i = 0; i < size; ++i)
  {
    o << n.getChildId(i);
    if (i != size - 1)
    {
      o << ", ";
    }
  }
  o << "}";
  return o;
}

}

#endif
