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

#ifndef __TGS__HILBERT_R_TREE_H__
#define __TGS__HILBERT_R_TREE_H__

#include <tgs/RStarTree/RStarTree.h>

namespace Tgs
{
  class HilbertCurve;

  /**
  * An implementation of the Hilbert R-Tree algorithm. 
  * See: http://en.wikipedia.org/wiki/Hilbert_R-tree for an overview.
  */
  class TGS_EXPORT HilbertRTree : public RStarTree
  {
  public:

    HilbertRTree(const std::shared_ptr<PageStore>& ps, int dimensions);

    ~HilbertRTree() = default;

    /**
     * This method can only be called before the tree has been built. 
     * In other words, getRoot()->getChildCount() == 0.
     */
    void bulkInsert(const std::vector<Box>& boxes, const std::vector<int>& fids);

    void greedyShuffle();

  protected:

    class UserBoxHolder
    {
    public:

      const Box* box;
      int fid;
      int hilbertValue;

      UserBoxHolder(const Box& b, int f, int hv) :
        box(&b),
        fid(f),
        hilbertValue(hv)
      {
      }

      bool operator<(const UserBoxHolder& b) const
      {
        return hilbertValue < b.hilbertValue;
      }
    };

    static const int ORDER = 8;
    std::shared_ptr<HilbertCurve> _hilbertCurve;
    int _shuffleSize;

    int _splitBoxes(BoxVector& boxes) override;

    void _calculateHilbertValues(const std::vector<Box>& boxes, 
      const std::vector<int>& fids, std::vector<UserBoxHolder>& hilbertBoxes) const;

    double _calculateArea(int parentId);

    double _calculateOverlap(int parentId);

    double _calculatePairwiseOverlap(int parentId, std::vector<double>& overlaps);

    /**
     * Randomly select a child based on the available weights. The weights must be >= 0 and the 
     * sum must be > 0.
     */
    int _chooseWeightedChild(const std::vector<double>& weights) const;

    void _createLeafNodes(const std::vector<UserBoxHolder>& hilbertBoxes, 
      std::vector<int>& result);

    void _createParentNodes(const std::vector<int>& childNodes, 
      std::vector<int>& result);

    /**
     * Greedily and randomly shuffles features for the better
     */
    void _greedyShuffle(int parentId);

    double _sum(const std::vector<double>& v) const;

    double _swapGrandChildNodes(int parentId, const std::vector<double>& overlaps);
  };

  using HilbertRTreePtr = std::shared_ptr<HilbertRTree>;
  using ConstHilbertRTreePtr = std::shared_ptr<const HilbertRTree>;
}


#endif
