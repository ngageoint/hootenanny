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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ContainsNodeCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ContainsNodeCriterion)

ContainsNodeCriterion::ContainsNodeCriterion()
{
}

ContainsNodeCriterion::ContainsNodeCriterion(long nodeId) :
_nodeId(nodeId)
{
}

void ContainsNodeCriterion::setConfiguration(const Settings& s)
{
  ConfigOptions opts(s);
  _nodeId = opts.getContainsNodeCriterionId();
  if (_nodeId == 0)
  {
    throw HootException(
      "Invalid node ID passed to ContainsNodeCriterion: " + QString::number(_nodeId));
  }
}

bool ContainsNodeCriterion::isSatisfied(const ConstElementPtr& e) const
{
  if (e->getElementType() == ElementType::Way)
  {
    ConstWayPtr w = std::dynamic_pointer_cast<const Way>(e);
    return w->hasNode(_nodeId);
  }
  else if (e->getElementType() == ElementType::Relation)
  {
    ConstRelationPtr r = std::dynamic_pointer_cast<const Relation>(e);
    return r->contains(ElementId(ElementType::Node, _nodeId));
  }
  else if (e->getElementType() == ElementType::Node)
  {
    ConstNodePtr n = std::dynamic_pointer_cast<const Node>(e);
    return (n->getId() == _nodeId);
  }
  return false;
}

}
