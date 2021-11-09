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
#include <hoot/core/criterion/CriterionUtils.h>
#include <hoot/core/criterion/NeedsReviewCriterion.h>
#include <hoot/core/criterion/NotCriterion.h>
#include <hoot/core/criterion/RailwayOneToManySourceCriterion.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(ElementVisitor, RailwayOneToManySecondaryMatchElementRemover)

RailwayOneToManySecondaryMatchElementRemover::RailwayOneToManySecondaryMatchElementRemover()
{
  // We're looking to remove secondary railways with the many to one match identifying tag, but only
  // if they aren't involved in a review.
  addCriterion(
    std::make_shared<ChainCriterion>(
      std::make_shared<RailwayOneToManySourceCriterion>(),
      std::make_shared<NotCriterion>(std::make_shared<NeedsReviewCriterion>())));
  setRecursive(true);
  setRecursiveRemoveRefsFromParents(true);
}

void RailwayOneToManySecondaryMatchElementRemover::setConfiguration(const Settings& /*conf*/)
{
 // overriding here to keep our hardcoded config
}

}
