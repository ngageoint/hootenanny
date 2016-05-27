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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NetworkMergerCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/conflate/MarkForReviewMerger.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MergerFactory.h>
#include <hoot/core/conflate/highway/HighwaySnapMerger.h>
#include <hoot/core/conflate/polygon/BuildingMatch.h>

// Standard
#include <typeinfo>

#include "NetworkMatch.h"
#include "NetworkMerger.h"

namespace hoot
{

HOOT_FACTORY_REGISTER(MergerCreator, NetworkMergerCreator)

NetworkMergerCreator::NetworkMergerCreator()
{
  _map = 0;
  _sublineMatcher.reset(
    Factory::getInstance().constructObject<SublineStringMatcher>(
      ConfigOptions().getHighwaySublineStringMatcher()));

  _minSplitSize = ConfigOptions(conf()).getWayMergerMinSplitSize();
}

bool NetworkMergerCreator::createMergers(const MatchSet& matches,
  vector<Merger*>& mergers) const
{
  bool result = false;
  assert(matches.size() > 0);

  const NetworkMatch* m = dynamic_cast<const NetworkMatch*>(*matches.begin());

  if (m)
  {
    set< pair<ElementId, ElementId> > eids;
    if (matches.size() != 1)
    {
      // go through all the matches
      for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
      {
        set< pair<ElementId, ElementId> > s = (*it)->getMatchPairs();
        eids.insert(s.begin(), s.end());
      }

      mergers.push_back(new MarkForReviewMerger(eids, "A complex road situation was found.",
        m->getMatchName(), 1.0));
    }
    else
    {
      mergers.push_back(new HighwaySnapMerger(_minSplitSize, m->getMatchPairs(), _sublineMatcher));
    }
    result = true;
  }

  return result;
}

vector<MergerCreator::Description> NetworkMergerCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Network Merge Creator", true));

  return result;
}

bool NetworkMergerCreator::isConflicting(const ConstOsmMapPtr& map, const Match* m1,
  const Match* m2) const
{
  bool result = false;

  if (dynamic_cast<const NetworkMatch*>(m1) || dynamic_cast<const NetworkMatch*>(m2))
  {
    result = m1->isConflicting(*m2, map);
  }

  return result;
}

bool NetworkMergerCreator::_isConflictingSet(const MatchSet& matches) const
{
  // _map must be set using setOsmMap()
  assert(_map != 0);
  bool conflicting = matches.size() > 1;

  return conflicting;
}

}
