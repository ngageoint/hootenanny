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
#include "BuildingMergerCreator.h"

// hoot
#include <hoot/core/conflate/polygon/BuildingMatch.h>
#include <hoot/core/conflate/polygon/BuildingMerger.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, BuildingMergerCreator)

bool BuildingMergerCreator::createMergers(const MatchSet& matches, vector<MergerPtr>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(!matches.empty());

  set<pair<ElementId, ElementId>> eids;

  // Go through all the matches.
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr m = *it;
    LOG_VART(m->toString());
    const BuildingMatch* bm = dynamic_cast<const BuildingMatch*>(m.get());
    // Check to make sure all the input matches are building matches.
    if (bm == nullptr)
    {
      // return an empty result
      LOG_TRACE("Match invalid; skipping merge: " << m->toString());
      return false;
    }
    // Add all the element to element pairs to a set.
    else
    {
      set<pair<ElementId, ElementId>> s = bm->getMatchPairs();
      LOG_VART(s);
      eids.insert(s.begin(), s.end());
    }
  }
  LOG_VART(eids);

  // Only add the building merge if there are elements to merge.
  if (!eids.empty())
  {
    mergers.push_back(std::make_shared<BuildingMerger>(eids));
    result = true;
  }

  return result;
}

vector<CreatorDescription> BuildingMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.emplace_back(
    className(),
    "Generates mergers that merge buildings together",
    false);
  return result;
}

bool BuildingMergerCreator::isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1,
  ConstMatchPtr m2, const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  const BuildingMatch* bm1 = dynamic_cast<const BuildingMatch*>(m1.get());
  const BuildingMatch* bm2 = dynamic_cast<const BuildingMatch*>(m2.get());
  // This shouldn't ever return true, but I'm calling the BuildingMatch::isConflicting just in
  // case the logic changes at some point.
  if (bm1 && bm2)
  {
    return m1->isConflicting(m2, map);
  }
  else
  {
    return false;
  }
}

}
