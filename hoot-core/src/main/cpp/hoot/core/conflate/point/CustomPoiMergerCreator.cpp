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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "CustomPoiMergerCreator.h"

#include "CustomPoiMatch.h"
#include "CustomPoiMerger.h"

// hoot
#include <hoot/core/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, CustomPoiMergerCreator)

CustomPoiMergerCreator::CustomPoiMergerCreator()
{
}

bool CustomPoiMergerCreator::createMergers(const MatchSet& matches,
  vector<Merger*>& mergers) const
{
  bool result = false;
  assert(matches.size() > 0);

  set< pair<ElementId, ElementId> > eids;

  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const CustomPoiMatch* cpm = dynamic_cast<const CustomPoiMatch*>(*it);
    // check to make sure all the input matches are building matches.
    if (cpm == 0)
    {
      // return an empty result
      return false;
    }
    // add all the element to element pairs to a set
    else
    {
      set< pair<ElementId, ElementId> > s = cpm->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }
  }

  // only add the POI merge if there are elements to merge.
  if (eids.size() > 0)
  {
    mergers.push_back(new CustomPoiMerger(eids));
    result = true;
  }

  return result;
}

vector<MergerCreator::Description> CustomPoiMergerCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Custom POI Merge Creator", true));

  return result;
}

bool CustomPoiMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  const CustomPoiMatch* pm1 = dynamic_cast<const CustomPoiMatch*>(m1);
  const CustomPoiMatch* pm2 = dynamic_cast<const CustomPoiMatch*>(m2);

  if (pm1 && pm2)
  {
    return pm1->isConflicting(*pm2, map);
  }
  else
  {
    return false;
  }
}

}
