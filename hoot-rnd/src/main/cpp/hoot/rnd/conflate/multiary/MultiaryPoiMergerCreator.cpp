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
 * @copyright Copyright (C) 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "MultiaryPoiMergerCreator.h"

// hoot
#include <hoot/core/util/Log.h>

#include "MultiaryPoiMerger.h"

using namespace std;

namespace hoot
{

MultiaryPoiMergerCreator::MultiaryPoiMergerCreator()
{
}

bool MultiaryPoiMergerCreator::createMergers(const MatchSet& matches, std::vector<Merger*>& mergers)
  const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  std::set< pair<ElementId, ElementId> > eids;

  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    const Match* m = *it;
    set< pair<ElementId, ElementId> > s = m->getMatchPairs();
    eids.insert(s.begin(), s.end());
  }

  MultiaryPoiMerger* pm = new MultiaryPoiMerger(eids);
  if (eids.size() >= 1)
  {
    mergers.push_back(pm);
    result = true;
  }

  return result;
}

std::vector<MergerCreator::Description> MultiaryPoiMergerCreator::getAllCreators() const
{
  MergerCreator::Description d;
  d.className = className();
  d.description = "Multiary conflation identifies clusters within a match set and merges each "
      "cluster.";
  d.experimental = true;
  vector<MergerCreator::Description> result;
  result.push_back(d);

  return result;
}

bool MultiaryPoiMergerCreator::isConflicting(const ConstOsmMapPtr&, const Match*,
  const Match*) const
{
  // Any relevant matches that contain conflicts will be resolved during the clustering phase.
  return false;
}

}
