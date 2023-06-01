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
 * @copyright Copyright (C) 2016-2023 Maxar (http://www.maxar.com/)
 */
#include "ChainCriterion.h"

// hoot
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ChainCriterion)

ChainCriterion::ChainCriterion(const ElementCriterionPtr& child1, const ElementCriterionPtr& child2)
{
  _criteria.push_back(child1);
  _criteria.push_back(child2);
}

ChainCriterion::ChainCriterion(ElementCriterion* child1, ElementCriterion* child2)
{
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child1));
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child2));
}

ChainCriterion::ChainCriterion(const std::vector<ElementCriterionPtr>& criteria)
{
  for (const auto& crit : criteria)
    _criteria.push_back(std::shared_ptr<ElementCriterion>(crit->clone()));
}

void ChainCriterion::addCriterion(const ElementCriterionPtr& e)
{
  _criteria.push_back(e);
}

void ChainCriterion::setOsmMap(const OsmMap* map)
{
  ConstOsmMapConsumerImpl::setOsmMap(map);
  for (const auto& crit : _criteria)
  {
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer = std::dynamic_pointer_cast<ConstOsmMapConsumer>(crit);
    if (mapConsumer)
      mapConsumer->setOsmMap(map);
  }
}

void ChainCriterion::setConfiguration(const Settings& conf)
{
  for (const auto& crit : _criteria)
  {
    std::shared_ptr<Configurable> configurable = std::dynamic_pointer_cast<Configurable>(crit);
    if (configurable)
    {
      configurable->setConfiguration(conf);
      LOG_DEBUG("Set config on: " << crit->toString());
    }
  }
}

bool ChainCriterion::isSatisfied(const ConstElementPtr& e) const
{
  for (const auto& crit : _criteria)
  {
    if (!crit->isSatisfied(e))
    {
      LOG_TRACE(
        "One chained criterion not satisfied in: " << toString() << ". Filter: " << crit <<
        " not satisfied for: " << e);
      LOG_VART(e->getStatus());
      return false;
    }
  }
  LOG_TRACE("Chained criteria satisfied: " << toString() << " for: " << e);
  return true;
}

QString ChainCriterion::toString() const
{
  QString txt = "ChainCriterion(";
  for (const auto& crit : _criteria)
    txt += crit->toString() + ";";
  txt.chop(1);
  txt += ")";
  return txt;
}

}
