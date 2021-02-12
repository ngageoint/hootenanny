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

#include "RStarTreePrinter.h"

#include <tgs/RStarTree/RStarTree.h>
#include <tgs/RStarTree/RTreeNode.h>

using namespace std;

namespace Tgs
{

RStarTreePrinter::RStarTreePrinter(const std::shared_ptr<const RStarTree>& tree)
  : _tree(tree),
    _indent(0)
{
}

string RStarTreePrinter::_indentStr(int size)
{
  string result;
  for (int i = 0; i < size; i++)
  {
    result = result + "  ";
  }

  return result;
}

void RStarTreePrinter::print(const std::shared_ptr<const RStarTree>& tree)
{
  RStarTreePrinter p(tree);
  p.print();
  cout.flush();
}

void RStarTreePrinter::print()
{
  print(_tree->getRoot(), 0);
}

void RStarTreePrinter::print(const RTreeNode *node, int indent)
{
  string is = _indentStr(indent);

  cout << is << "Node ID: " << node->getId() << endl;
  cout << is << "Parent ID: " << node->getParentId() << endl;
  cout << is << "Envelope: " << node->calculateEnvelope().toString() << endl;
  cout << is << "Child Count: " << node->getChildCount() << endl;
  if (node->isLeafNode())
  {
    cout << is << "Leaf Node" << endl;
  }

  for (int i = 0; i < node->getChildCount(); i++)
  {
    if (node->isLeafNode() == false)
    {
      int id = node->getChildNodeId(i);
      print(_tree->getNode(id), indent + 1);
    }
    else
    {
      RTreeNode* n = const_cast<RTreeNode*>(node);
      if (n->getChildId(i) < 0)
      {
        cout << is << "  *** BAD *** user id: " << n->getChildId(i) << endl;
      }
      else
      {
        cout << is << "  user id: " << n->getChildId(i) << endl;
      }
    }
  }
}

}
