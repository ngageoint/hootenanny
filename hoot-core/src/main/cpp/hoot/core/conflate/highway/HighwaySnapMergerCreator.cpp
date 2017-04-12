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
#include "HighwaySnapMerger.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/util/ConfigOptions.h>

#include "HighwayMatch.h"
#include "HighwaySnapMergerCreator.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, HighwaySnapMergerCreator)

HighwaySnapMergerCreator::HighwaySnapMergerCreator()
{
  setConfiguration(conf());
}

bool HighwaySnapMergerCreator::createMergers(const MatchSet& matches, vector<Merger*>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  set< pair<ElementId, ElementId> > eids;

  boost::shared_ptr<SublineStringMatcher> sublineMatcher;
  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m = *it;
    LOG_VART(m->toString());
    const HighwayMatch* hm = dynamic_cast<const HighwayMatch*>(m);
    // check to make sure all the input matches are building matches.
    LOG_VART(hm == 0);
    if (hm == 0)
    {
      // return an empty result
      return false;
    }
    // add all the element to element pairs to a set
    else
    {
      // there should only be one HighwayMatch in a set.
      sublineMatcher = hm->getSublineMatcher();
      set< pair<ElementId, ElementId> > s = hm->getMatchPairs();
      eids.insert(s.begin(), s.end());
    }
  }

  // only add the highway merge if there are elements to merge.
  if (eids.size() > 0)
  {
    mergers.push_back(new HighwaySnapMerger(_minSplitSize, eids, sublineMatcher));
    result = true;
  }

  return result;
}

vector<MergerCreator::Description> HighwaySnapMergerCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Highway Snap Merge Creator", false));

  return result;
}

bool HighwaySnapMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  const HighwayMatch* hm1 = dynamic_cast<const HighwayMatch*>(m1);
  const HighwayMatch* hm2 = dynamic_cast<const HighwayMatch*>(m2);

  if (hm1 && hm2)
  {
    return hm1->isConflicting(*hm2, map);
  }
  else
  {
    return false;
  }
}

void HighwaySnapMergerCreator::setConfiguration(const Settings &conf)
{
  _minSplitSize = ConfigOptions(conf).getWayMergerMinSplitSize();
}

}
