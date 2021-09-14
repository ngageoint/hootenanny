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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "RemoveElementsVisitor.h"

#include <hoot/core/elements/NodeToWayMap.h>
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/index/OsmMapIndex.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/ops/RemoveElementByEid.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RemoveElementsVisitor)

RemoveElementsVisitor::RemoveElementsVisitor(bool negateCriteria) :
_recursive(false),
_recursiveRemoveRefsFromParents(false),
_startElementCount(0)
{
  _negateCriteria = negateCriteria;
  _chainCriteria = false;
}

void RemoveElementsVisitor::setConfiguration(const Settings& conf)
{
  ConfigOptions configOptions(conf);

  // We may need to separate element.criteria.negate out in to separate options for each consumer
  // of it; otherwise we may end up with conflicts with certain combinations of config option inputs
  // to certain command invocations
  _negateCriteria = configOptions.getElementCriteriaNegate();
  _chainCriteria = configOptions.getRemoveElementsVisitorChainElementCriteria();
  LOG_VARD(_chainCriteria);
  const QStringList critNames = configOptions.getRemoveElementsVisitorElementCriteria();
  LOG_VART(critNames);
  _addCriteria(critNames);
  // TODO: Maybe we should just make MultipleCriterionConsumerVisitor configurable and move this up?
  LOG_VARD(_criteria.size());
  LOG_VARD(_configureChildren);
  if (_configureChildren)
  {
    for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
         it != _criteria.end(); ++it)
    {
      ElementCriterionPtr crit = *it;
      Configurable* c = dynamic_cast<Configurable*>(crit.get());
      if (c != nullptr)
      {
        c->setConfiguration(conf);
      }
    }
  }

  _recursive = configOptions.getRemoveElementsVisitorRecursive();
}

void RemoveElementsVisitor::setOsmMap(OsmMap* map)
{
  _map = map;
  _startElementCount = _map->getElementCount();

  for (std::vector<ElementCriterionPtr>::const_iterator it = _criteria.begin();
       it != _criteria.end(); ++it)
  {
    ElementCriterionPtr crit = *it;
    OsmMapConsumer* consumer = dynamic_cast<OsmMapConsumer*>(crit.get());
    if (consumer != nullptr)
      consumer->setOsmMap(map);
  }
}

void RemoveElementsVisitor::visit(const ElementPtr& e)
{
  if (!e)
  {
    return;
  }

  if (_criteria.empty())
  {
    throw IllegalArgumentException("No criteria specified for RemoveElementsVisitor.");
  }

  LOG_VART(e->getElementId());

  if (_criteriaSatisfied(e))
  {
    if (_recursive)
    {
      LOG_TRACE("Removing element: " << e->getElementId() << " recursively...");
      RecursiveElementRemover(e->getElementId(), _recursiveRemoveRefsFromParents)
        .apply(_map->shared_from_this());
    }
    else
    {
      // This originally called RemoveElementByEid::removeUnusedElementsOnly but was getting some
      // unexpected results when using it with C&R and changed it over to removeElement, which
      // solved the problem and didn't have any negative impact on the rest of the code...could
      // still be worth looking into, though.
      LOG_TRACE("Removing element: " << e->getElementId() << " non-recursively...");
      RemoveElementByEid::removeElement(_map->shared_from_this(), e->getElementId());
    }
    _numAffected++;
  }
  else
  {
    LOG_TRACE("Not removing element: " << e->getElementId());
  }
  _numProcessed++;
}

void RemoveElementsVisitor::removeWays(
  const std::shared_ptr<OsmMap>& pMap, const std::shared_ptr<ElementCriterion>& pCrit)
{
  RemoveElementsVisitor v;
  v.addCriterion(pCrit);
  pMap->visitWaysRw(v);
}

}

