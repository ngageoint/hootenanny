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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "RailwayOneToManySecondaryMatchElementRemover.h"

// hoot
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/RailwayCriterion.h>
#include <hoot/core/criterion/StatusCriterion.h>
#include <hoot/core/criterion/TagCriterion.h>
#include <hoot/core/schema/MetadataTags.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RailwayOneToManySecondaryMatchElementRemover)

RailwayOneToManySecondaryMatchElementRemover::RailwayOneToManySecondaryMatchElementRemover()
{
  ElementCriterionPtr elementCrit =
    std::make_shared<ChainCriterion>(
      std::make_shared<RailwayCriterion>(),
      std::make_shared<StatusCriterion>(Status::Unknown2));
  addCriterion(
    std::make_shared<ChainCriterion>(
      elementCrit,
      std::make_shared<TagCriterion>(MetadataTags::HootRailwayOneToManyMatchSecondary(), "yes")));
  setRecursive(true);
  setRecursiveRemoveRefsFromParents(true);
}

void RailwayOneToManySecondaryMatchElementRemover::setConfiguration(const Settings& /*conf*/)
{
 // overriding here to keep our hardcoded config
}

}
