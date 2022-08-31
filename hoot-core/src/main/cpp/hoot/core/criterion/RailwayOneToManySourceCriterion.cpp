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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "RailwayOneToManySourceCriterion.h"

// hoot
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementCriterion, RailwayOneToManySourceCriterion)

bool RailwayOneToManySourceCriterion::isSatisfied(const ConstElementPtr& e) const
{
  // Secondary railways with the custom tag added by Railway.js satisfy this.
  QList<ElementCriterionPtr> crits;
  ElementCriterionPtr typeCrit = std::make_shared<RailwayCriterion>();
  crits.append(typeCrit);
  ElementCriterionPtr statusCrit = std::make_shared<StatusCriterion>(Status::Unknown2);
  crits.append(statusCrit);
  ElementCriterionPtr tagCrit = std::make_shared<TagCriterion>(MetadataTags::HootRailwayOneToManyMatchSecondary(), "yes");
  crits.append(tagCrit);
  return CriterionUtils::combineCriterion(crits)->isSatisfied(e);
}

}
