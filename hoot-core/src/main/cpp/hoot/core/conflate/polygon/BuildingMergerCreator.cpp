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
#include "BuildingMergerCreator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

#include "BuildingMatch.h"
#include "BuildingMerger.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, BuildingMergerCreator)

BuildingMergerCreator::BuildingMergerCreator()
{
}

bool BuildingMergerCreator::createMergers(const MatchSet& matches, vector<Merger*>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  set< pair<ElementId, ElementId> > eids;

  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m = *it;
    LOG_VART(m->toString());
    const BuildingMatch* bm = dynamic_cast<const BuildingMatch*>(m);
    LOG_VART(bm == 0);
    // check to make sure all the input matches are building matches.
    if (bm == 0)
    {
      // return an empty result
      return false;
    }
    // add all the element to element pairs to a set
    else
    {
      set< pair<ElementId, ElementId> > s = bm->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }
  }

  // only add the building merge if there are elements to merge.
  if (eids.size() > 0)
  {
    mergers.push_back(new BuildingMerger(eids));
    result = true;
  }

  return result;
}

vector<MergerCreator::Description> BuildingMergerCreator::getAllCreators() const
{
  vector<Description> result;
  result.push_back(Description(className(), "Building Merge Creator", false));
  return result;
}

bool BuildingMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  const BuildingMatch* bm1 = dynamic_cast<const BuildingMatch*>(m1);
  const BuildingMatch* bm2 = dynamic_cast<const BuildingMatch*>(m2);

  // this shouldn't ever return true, but I'm calling the BuildingMatch::isConflicting just in
  // case the logic changes at some point.
  if (bm1 && bm2)
  {
    return bm1->isConflicting(*bm2, map);
  }
  else
  {
    return false;
  }
}

}
