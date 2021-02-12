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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#include "ChainCriterion.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, ChainCriterion)

ChainCriterion::ChainCriterion(const ElementCriterionPtr& child1,
                               const ElementCriterionPtr& child2)
{
  _criteria.push_back(child1);
  _criteria.push_back(child2);
}

ChainCriterion::ChainCriterion(ElementCriterion* child1, ElementCriterion* child2)
{
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child1));
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child2));
}

ChainCriterion::ChainCriterion(ElementCriterion* child1, ElementCriterionPtr child2)
{
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child1));
  _criteria.push_back(child2);
}

ChainCriterion::ChainCriterion(ElementCriterion* child1, ElementCriterion* child2,
                               ElementCriterion* child3)
{
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child1));
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child2));
  _criteria.push_back(std::shared_ptr<ElementCriterion>(child3));
}

ChainCriterion::ChainCriterion(const std::vector<std::shared_ptr<ElementCriterion>>& criteria)
{
  for (size_t i = 0; i < criteria.size(); i++)
    _criteria.push_back(std::shared_ptr<ElementCriterion>(criteria[i]->clone()));
}

void ChainCriterion::addCriterion(const ElementCriterionPtr& e)
{
  _criteria.push_back(e);
}

void ChainCriterion::setOsmMap(const OsmMap* map)
{
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    std::shared_ptr<ConstOsmMapConsumer> mapConsumer =
      std::dynamic_pointer_cast<ConstOsmMapConsumer>(_criteria[i]);
    if (mapConsumer)
    {
      mapConsumer->setOsmMap(map);
    }
  }
}

void ChainCriterion::setConfiguration(const Settings& conf)
{
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    std::shared_ptr<Configurable> configurable =
      std::dynamic_pointer_cast<Configurable>(_criteria[i]);
    if (configurable)
    {
      configurable->setConfiguration(conf);
      LOG_DEBUG("Set config on: " << _criteria[i]->toString());
    }
  }
}

bool ChainCriterion::isSatisfied(const ConstElementPtr& e) const
{
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    if (!_criteria[i]->isSatisfied(e))
    {
      LOG_TRACE(
        "One chained criterion not satisfied in: " << toString() << ". Filter not satisfied " <<
        "for: " << e);
      return false;
    }
  }
  LOG_TRACE("One chained criteria satisfied in: " << toString() << ". Filter satisfied for: " << e);
  return true;
}

QString ChainCriterion::toString() const
{
  QString txt = "ChainCriterion(";
  for (size_t i = 0; i < _criteria.size(); i++)
  {
    txt += _criteria.at(i)->toString() + ";";
  }
  txt.chop(1);
  txt += ")";
  return txt;
}

}
