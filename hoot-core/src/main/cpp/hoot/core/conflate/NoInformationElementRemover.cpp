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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */

#include "NoInformationElementRemover.h"

// Hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/visitors/RemoveElementsVisitor.h>
#include <hoot/core/visitors/CriterionCountVisitor.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/ElementCriterion.h>
#include <hoot/core/filters/NoInformationCriterion.h>
#include <hoot/core/filters/UselessElementCriterion.h>

namespace hoot
{
  using namespace std;

HOOT_FACTORY_REGISTER(OsmMapOperation, NoInformationElementRemover)

NoInformationElementRemover::NoInformationElementRemover()
{

}

void NoInformationElementRemover::apply(boost::shared_ptr<OsmMap>& map)
{
  _map = map;

  boost::shared_ptr<NoInformationCriterion> pNoInfoCrit(new NoInformationCriterion());
  boost::shared_ptr<UselessElementCriterion> pUselessCrit(new UselessElementCriterion(map));
  boost::shared_ptr<ChainCriterion> pCrit(new ChainCriterion(pNoInfoCrit, pUselessCrit));
  RemoveElementsVisitor removeElementsVisitor(pCrit);
  removeElementsVisitor.setRecursive(false);
  _map->visitRw(removeElementsVisitor);
}

}
