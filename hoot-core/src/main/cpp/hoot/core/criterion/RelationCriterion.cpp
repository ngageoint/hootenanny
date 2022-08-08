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
 * @copyright Copyright (C) 2018, 2019, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RelationCriterion.h"

#include <hoot/core/elements/Relation.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, RelationCriterion)

RelationCriterion::RelationCriterion() : ElementTypeCriterion(ElementType::Relation)
{
}

RelationCriterion::RelationCriterion(const QString& type) :
ElementTypeCriterion(ElementType::Relation),
_type(type.trimmed())
{
}

void RelationCriterion::setConfiguration(const Settings& conf)
{
  _type = ConfigOptions(conf).getRelationCriterionType();
}

bool RelationCriterion::isSatisfied(const ConstElementPtr& e) const
{
  const bool typeMatch = ElementTypeCriterion::isSatisfied(e);
  if (typeMatch)
  {
    if (_type.isEmpty())
    {
      return true;
    }
    else
    {
      return std::dynamic_pointer_cast<const Relation>(e)->getType() == _type;
    }
  }
  return false;
}

}
