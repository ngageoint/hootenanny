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
 * @copyright Copyright (C) 2015, 2017, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "HighwayMergerCreator.h"

// hoot
#include <hoot/core/conflate/highway/HighwayMatch.h>
#include <hoot/core/conflate/linear/LinearSnapMerger.h>
#include <hoot/core/conflate/linear/LinearTagOnlyMerger.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

using namespace std;

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, HighwayMergerCreator)

HighwayMergerCreator::HighwayMergerCreator()
{
  setConfiguration(conf());
}

bool HighwayMergerCreator::createMergers(const MatchSet& matches, vector<MergerPtr>& mergers) const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  set<pair<ElementId, ElementId>> eids;

  std::shared_ptr<SublineStringMatcher> sublineMatcher;
  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr m = *it;
    LOG_VART(m->toString());
    const HighwayMatch* hm = dynamic_cast<const HighwayMatch*>(m.get());
    // check to make sure all the input matches are highway matches.
    if (hm == 0)
    {
      // return an empty result
      LOG_TRACE("Returning empty result due to match not being HighwayMatch: " << m->toString());
      return false;
    }
    // add all the element to element pairs to a set
    else
    {
      // there should only be one HighwayMatch in a set.
      sublineMatcher = hm->getSublineMatcher();
      set<pair<ElementId, ElementId>> s = hm->getMatchPairs();
      //LOG_VART(s);
      eids.insert(s.begin(), s.end());
    }
  }
  LOG_VART(eids);

  // only add the highway merge if there are elements to merge.
  if (eids.size() > 0)
  {
    if (!ConfigOptions().getHighwayMergeTagsOnly())
    {
      mergers.push_back(MergerPtr(new LinearSnapMerger(eids, sublineMatcher)));
    }
    else
    {
      mergers.push_back(MergerPtr(new LinearTagOnlyMerger(eids, sublineMatcher)));
    }
    result = true;
  }

  return result;
}

vector<CreatorDescription> HighwayMergerCreator::getAllCreators() const
{
  vector<CreatorDescription> result;
  result.push_back(
    CreatorDescription(
      className(),
      "Generates mergers that merge roads with the 2nd Generation (Unifying) Algorithm",
      false));
  return result;
}

bool HighwayMergerCreator::isConflicting(const ConstOsmMapPtr& map, ConstMatchPtr m1,
  ConstMatchPtr m2, const QHash<QString, ConstMatchPtr>& /*matches*/) const
{
  const HighwayMatch* hm1 = dynamic_cast<const HighwayMatch*>(m1.get());
  const HighwayMatch* hm2 = dynamic_cast<const HighwayMatch*>(m2.get());

  if (hm1 && hm2)
  {
    const bool conflicting = m1->isConflicting(m2, map);
    if (conflicting)
    {
      LOG_TRACE("Conflicting matches: " << m1 << ", " << m2);
    }
    return conflicting;
  }
  else
  {
    return false;
  }
}

void HighwayMergerCreator::setConfiguration(const Settings &conf)
{
  _minSplitSize = ConfigOptions(conf).getWayMergerMinSplitSize();
}

}
