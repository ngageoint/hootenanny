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
 * @copyright Copyright (C) 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "NodesPerWayVisitor.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/elements/Way.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesPerWayVisitor)

NodesPerWayVisitor::NodesPerWayVisitor() :
_totalWayNodes(0),
_minNodesPerWay(0),
_maxNodesPerWay(0)
{
  _negateCriteria = false;
  _chainCriteria = false;
}

void NodesPerWayVisitor::setConfiguration(const Settings& conf)
{
  // TODO: This setConfiguration is kind of bulky for MultipleCriterionConsumerVisitor children.
  // Can we move some of the logic up to the parent?

  ConfigOptions configOptions(conf);

  _negateCriteria = configOptions.getElementCriterionNegate();
  _chainCriteria = configOptions.getNodesPerWayVisitorChainElementCriteria();
  const QStringList critNames = configOptions.getNodesPerWayVisitorElementCriteria();
  LOG_VART(critNames);
  _addCriteria(critNames);
  if (_configureChildren)
  {
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      Configurable* c = dynamic_cast<Configurable*>(crit.get());
      if (c != 0)
      {
        c->setConfiguration(conf);
      }
    }
  }
  LOG_VARD(_negateCriteria);
  LOG_VARD(_chainCriteria);
  LOG_VARD(_criteria.size());
}

void NodesPerWayVisitor::visit(const ConstElementPtr& e)
{
  if (_wayCrit.isSatisfied(e) && (_criteria.size() == 0 || _criteriaSatisfied(e)))
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
    const int numWayNodes = way->getNodeCount();
    _totalWayNodes += numWayNodes;
    if (_minNodesPerWay == 0 || numWayNodes < _minNodesPerWay)
    {
      _minNodesPerWay = numWayNodes;
    }
    if (numWayNodes > _maxNodesPerWay)
    {
      _maxNodesPerWay = numWayNodes;
    }
    _numAffected++;
  }
}

}
