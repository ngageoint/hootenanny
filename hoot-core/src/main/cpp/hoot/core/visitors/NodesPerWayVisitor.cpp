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
 * @copyright Copyright (C) 2019, 2021 Maxar (http://www.maxar.com/)
 */

#include "NodesPerWayVisitor.h"

// hoot
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, NodesPerWayVisitor)

NodesPerWayVisitor::NodesPerWayVisitor()
  : _totalWayNodes(0),
    _minNodesPerWay(0),
    _maxNodesPerWay(0),
    _negateCriterion(false)
{
}

void NodesPerWayVisitor::setConfiguration(const Settings& conf)
{
  // TODO: This setConfiguration is kind of bulky for MultipleCriterionConsumerVisitor children.
  // Can we move some of the logic up to the parent? Do the same in the other children as well.

  ConfigOptions configOptions(conf);
  _negateCriterion = configOptions.getElementCriteriaNegate();
  LOG_VART(_negateCriterion);
  const QString critName = configOptions.getNodesPerWayVisitorElementCriterion();
  LOG_VART(critName);
  _setCriterion(critName);
}

void NodesPerWayVisitor::addCriterion(const ElementCriterionPtr& e)
{
  if (!_negateCriterion)
    _customCrit = e;
  else
    _customCrit = std::make_shared<NotCriterion>(e);
}

void NodesPerWayVisitor::_setCriterion(const QString& criterionName)
{
  if (!criterionName.trimmed().isEmpty())
  {
    LOG_VART(criterionName);
    addCriterion(Factory::getInstance().constructObject<ElementCriterion>(criterionName.trimmed()));
  }
}

void NodesPerWayVisitor::visit(const ConstElementPtr& e)
{
  LOG_VART(e->getElementId());
  if (_wayCrit.isSatisfied(e) && (!_customCrit || _customCrit->isSatisfied(e)))
  {
    ConstWayPtr way = std::dynamic_pointer_cast<const Way>(e);
    LOG_VART(way->getElementId());

    const int numWayNodes = way->getNodeCount();
    _totalWayNodes += numWayNodes;
    LOG_VART(_totalWayNodes);
    if (_minNodesPerWay == 0 || numWayNodes < _minNodesPerWay)
    {
      _minNodesPerWay = numWayNodes;
      LOG_VART(_minNodesPerWay);
    }
    if (numWayNodes > _maxNodesPerWay)
    {
      _maxNodesPerWay = numWayNodes;
      LOG_VART(_maxNodesPerWay);
    }
    _numAffected++;
  }
}

double NodesPerWayVisitor::getAverage() const
{
  const double average = _numAffected == 0 ? 0.0 : _totalWayNodes / _numAffected;
  return average;
}

}
