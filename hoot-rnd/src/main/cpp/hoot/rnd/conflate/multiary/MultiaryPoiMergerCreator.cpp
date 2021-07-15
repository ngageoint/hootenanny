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
 * @copyright Copyright (C) 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "MultiaryPoiMergerCreator.h"

// hoot
#include <hoot/core/util/Log.h>
#include <hoot/rnd/conflate/multiary/MultiaryPoiMerger.h>

using namespace std;

namespace hoot
{

bool MultiaryPoiMergerCreator::createMergers(const MatchSet& matches, std::vector<MergerPtr>& mergers)
  const
{
  LOG_TRACE("Creating mergers with " << className() << "...");

  bool result = false;
  assert(matches.size() > 0);

  std::set<pair<ElementId, ElementId>> eids;

  // go through all the matches
  for (MatchSet::const_iterator it = matches.begin(); it != matches.end(); ++it)
  {
    ConstMatchPtr m = *it;
    set<pair<ElementId, ElementId>> s = m->getMatchPairs();
    eids.insert(s.begin(), s.end());
  }

  if (eids.size() >= 1)
  {
    mergers.push_back(std::make_shared<MultiaryPoiMerger>(eids));
    result = true;
  }

  return result;
}

std::vector<CreatorDescription> MultiaryPoiMergerCreator::getAllCreators() const
{
  CreatorDescription d;
  d.setClassName(className());
  d.setDescription(
    "Generates mergers that identify Multiary POI clusters within a match set and merges each cluster.");
  d.setExperimental(true);
  vector<CreatorDescription> result;
  result.push_back(d);

  return result;
}

bool MultiaryPoiMergerCreator::isConflicting(const ConstOsmMapPtr&, ConstMatchPtr,
  ConstMatchPtr, const QHash<QString, ConstMatchPtr>&) const
{
  // Any relevant matches that contain conflicts will be resolved during the clustering phase.
  return false;
}

}
