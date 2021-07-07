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

#ifndef __TGS__R_STAR_TREE_H__
#define __TGS__R_STAR_TREE_H__

// Standard Includes
#include <memory>
#include <set>
#include <vector>

#include <tgs/TgsExport.h>
#include <tgs/RStarTree/Box.h>
#include <tgs/RStarTree/RTreeNodeStore.h>

namespace Tgs
{

class BoxInternalData;
class PageStore;
class RTreeNode;

/**
* A persistent RTree implementation. Yes, I realize that this means we now have two RTree
* implementations, but this implementation is intended as either a in memory or persistent
* implementation and should replace the old implementation in the future.
*
* See http://en.wikipedia.org/wiki/R-tree for a brief overview.
* See [BEC 90] Norbert Beckmann, Hans- N. Beckmann, H.-P. Kriegel, R. Schneider, B. Seeger:
* The R*-Tree: An Efficient and Robust Access Method for Points and Rectangles. SIGMOD
* Conference 1990:
* 322-331 (http://dbs.mathematik.uni-marburg.de/publications/myPapers/1990/BKSS90.pdf)
* See [GUT 84] A Guttman ‘R-trees a dynamic index structure for spatial searching’,
* Proc ACM SIGMOD Int Conf on Management of Data, 47-57, 1984
* (http://www.sai.msu.su/~megera/postgres/gist/papers/gutman-rtree.pdf)
*
* While it would probably be straight forward to make it thread safe for reading, this class is
* not thread safe.
*
* Note: For
*/
class TGS_EXPORT RStarTree
{
public:

  RStarTree(const std::shared_ptr<PageStore>& ps, int dimensions);

  virtual ~RStarTree() = default;

  /**
   * Returns the node with the specified node ID. This can then be used for any number of
   * traversals. Do not hold on to a copy of this pointer, it may disappear the next time
   * getNode is called.
   *
   * This method is the only thread safe method in the RStarTree.
   *
   * Note: When it comes time to support multi-threaded read operations on the RTree we can
   * modify this method so it automatically 'locks' the node and won't flush it out of the cache
   * until the node is released via some new method.
   */
  const RTreeNode* getNode(int nodeId) const;
  /**
   * Returns the root of the tree. This can then be used for any number of traversals. It is
   * best not to cache this value between inserts as the root can change from time to time.
   */
  const RTreeNode* getRoot() const;

  /**
   * Insert a new item
   * @param b - Bounding box of the item
   * @param userId - ID of the new item, this must be >= 0
   */
  void insert(const Box& b, int userId);

  void cleanTree() const;

  /**
   * Traverses the tree and looks for inconsistencies. This is helpful when debugging. Returns
   * false if the sanity check fails.
   */
  bool sanityCheck() const;

  class BoxPair
  {
  public:

    BoxInternalData box;
    int id;

    BoxPair(const BoxPair& bp) :
      box(bp.box)
    {
      id = bp.id;
    }

    BoxPair(const BoxInternalData& b, int id) :
      box(b)
    {
      this->id = id;
    }

    BoxPair& operator=(const BoxPair& bp)
    {
      box = bp.box;
      id = bp.id;
      return *this;
    }
  };

protected:

  enum
  {
    Split,
    Reinsert
  };

  using BoxVector = std::vector<BoxPair>;

  // TODO: store dimensions and such.
  class Header
  {
  public:
    int dimensions;
    int rootId;
    int height;
  };

  int _dimensions;
  RTreeNodeStore _store;

  /**
   * Taken directly from [BEC 90], choose the most efficient index to split on. Returns the
   * index on which the second group starts. E.g. 0 to result -1 is the first group, result to
   * M - 1 is the second group.
   */
  int _chooseSplitIndex(const BoxVector& boxes) const;

  int _getHeight() const;

  RTreeNode* _getNode(int id);
  RTreeNode* _getRoot();
  int _getRootId() const { return _rootId; }

  bool _sanityCheck(const RTreeNode* node) const;

  void _setHeight(int height);
  void _setRootId(int id);

  /**
   * Given a vector of boxes, find a good split to create two new nodes. Sort, the boxes
   * and return the index of the box to split on.
   */
  virtual int _splitBoxes(BoxVector& boxes);

  /**
   * Updates the bounds for the specified node and all parents.
   */
  void _updateBounds(RTreeNode* node);

private:

  double _mPercent;
  int _m;
  int _M;
  double _p;

  Header* _headerStruct;
  std::shared_ptr<Page> _header;
  /// Levels that have been treated for overflow during this insert.
  std::set<int> _overflowedLevels;

  /// call _[get|set]RootId() instead of setting directly.
  int _rootId;

  /// call _[get|set]Height() instead of setting directly.
  int _height;

  void _addChild(RTreeNode* parent, const Box& b, int id);

  /**
   * Returns the level a node is at. By definition the root is at level 0, it's children are
   * at level 1, etc.
   */
  int _calculateNodeLevel(const RTreeNode* node) const;

  /**
   * Taken directly from [BEC 90], choose the most efficient axis to split on.
   */
  void _chooseSplitAxis(BoxVector& boxes) const;

  /**
   * Taken directly from [BEC 90], choose the distribution w/ the minimum overlap value. Resolve
   * ties by choosing the distributions w/ minimum area.
   * @param node - pick a child node from this node. node must not be a leaf.
   * @param box - the box we're trying to place.
   */
  RTreeNode* _chooseSubTree(const Box& b, int level);

  /**
   * Find the child with the least enlargement. This is consistent w/ [GUT 84]'s implementation
   */
  int _findLeastEnlargement(RTreeNode* node, const Box& b) const;
  /**
   * Find the child with the least overlap enlargement. This could potentially be optimized by
   * implementing [BEC 90]'s "determine the nearly minimum overlap cost"
   */
  int _findLeastOverlapEnlargement(RTreeNode* node, const Box& b) const;

  /**
   * Insert an entry at the specified level. If level 0 is a leaf node and it counts up from
   * there.
   */
  void _insert(const Box& b, int id, int level);
  void _reinsert(RTreeNode* node, int level);

  bool _isClose(const double a, const double b) const;

  int _overflowTreatment(RTreeNode* node, RTreeNode*& newNode, int level);

  static void _sortOnDimension(BoxVector& boxes, int dim);

  void _split(RTreeNode* node, RTreeNode*& newNode);
};

inline const RTreeNode* RStarTree::getNode(int nodeId) const
{
  return _store.getNode(nodeId);
}

inline const RTreeNode* RStarTree::getRoot() const
{
  return _store.getNode(_rootId);
}

}

#endif
