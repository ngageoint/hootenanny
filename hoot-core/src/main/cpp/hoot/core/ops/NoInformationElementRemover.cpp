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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#include "NoInformationElementRemover.h"

// Hoot
#include <hoot/core/elements/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/criterion/ChainCriterion.h>
#include <hoot/core/criterion/ElementCriterion.h>
#include <hoot/core/criterion/NoInformationCriterion.h>
#include <hoot/core/criterion/UselessElementCriterion.h>

namespace hoot
{
  using namespace std;

HOOT_FACTORY_REGISTER(OsmMapOperation, NoInformationElementRemover)

void NoInformationElementRemover::apply(std::shared_ptr<OsmMap>& map)
{
  _numAffected = 0;
  _map = map;

  std::shared_ptr<ChainCriterion> pCrit(
    std::make_shared<ChainCriterion>(
      std::make_shared<NoInformationCriterion>(),
      std::make_shared<UselessElementCriterion>(map)));
  RemoveElementsVisitor removeElementsVisitor;
  removeElementsVisitor.setRecursive(false);
  removeElementsVisitor.addCriterion(pCrit);
  _map->visitRw(removeElementsVisitor);
}

}
