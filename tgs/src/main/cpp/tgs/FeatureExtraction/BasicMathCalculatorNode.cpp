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
 * @copyright Copyright (C) 2015, 2017, 2021, 2022 Maxar (http://www.maxar.com/)
 */

#include "BasicMathCalculatorNode.h"

namespace Tgs
{
  //////////////////////////////////////////////////////////////////////////
  // BinaryCalculatorNode
  //////////////////////////////////////////////////////////////////////////

  const std::map<std::string, std::string> BinaryCalculatorNode::getInputs() const
  {
    std::map<std::string, std::string> result;
    result[V1()] = DOUBLE;
    result[V2()] = DOUBLE;
    return result;
  }

  double BinaryCalculatorNode::getOutput(const int uid) const
  {
    boost::shared_ptr<CalculatorGenomeNode> v1 = getInput(V1());
    boost::shared_ptr<CalculatorGenomeNode> v2 = getInput(V2());
    return _calculate(v1->getOutput(uid), v2->getOutput(uid));
  }
  
  //////////////////////////////////////////////////////////////////////////
  // UnaryCalculatorNode
  //////////////////////////////////////////////////////////////////////////

  const std::map<std::string, std::string> UnaryCalculatorNode::getInputs() const
  {
    std::map<std::string, std::string> result;
    result[V1()] = DOUBLE;
    return result;
  }

  double UnaryCalculatorNode::getOutput(const int uid) const
  {
    boost::shared_ptr<CalculatorGenomeNode> v1 = getInput(V1());
    return _calculate(v1->getOutput(uid));
  }

}
