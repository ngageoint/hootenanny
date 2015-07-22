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
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */

#ifndef __TREE_NODE_H__
#define __TREE_NODE_H__

//STL Includes
#include <vector>

namespace Tgs
{
  /**
  *  The TreeNode is part of an implementation of the Leo Breiman
  * Random Forest Classification algorithm.  This is a single node
  * in a RandomTree within a RandomForest.
  *
  * For more information see:
  * http://oz.berkeley.edu/~breiman/RandomForests/
  */
  struct TreeNode
  {
    TreeNode * leftChild;
    TreeNode * rightChild;

    double splitValue;
    double purityDelta;
    std::string classLabel;
    unsigned int factorIndex;
    std::vector<unsigned int> dataList;
    bool isPure;
    unsigned int nodeId;
    double rangeMin;
    double rangeMax;
  };
}  //End Namespace
#endif

