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
#include "DiffConflator.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/MapProjector.h>
#include <hoot/core/conflate/MatchFactory.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/conflate/match-graph/GreedyConstrainedMatches.h>
#include <hoot/core/conflate/match-graph/OptimalConstrainedMatches.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/ops/NamedOp.h>
#include <hoot/core/ops/RecursiveElementRemover.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/MetadataTags.h>
#include <hoot/core/conflate/MatchClassification.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/util/Log.h>

// standard
#include <algorithm>

// tgs
#include <tgs/System/SystemInfo.h>
#include <tgs/System/Time.h>
#include <tgs/System/Timer.h>

using namespace std;
using namespace Tgs;

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, DiffConflator)

DiffConflator::DiffConflator() :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _reset();
}

DiffConflator::DiffConflator(boost::shared_ptr<MatchThreshold> matchThreshold) :
  _matchFactory(MatchFactory::getInstance()),
  _settings(Settings::getInstance())
{
  _matchThreshold = matchThreshold;
  _reset();
}


DiffConflator::~DiffConflator()
{
  _reset();
}

void DiffConflator::apply(OsmMapPtr& map)
{
  Timer timer;
  _reset();

  LOG_INFO("Applying pre diff-conflation operations...");
  NamedOp(ConfigOptions().getUnifyPreOps()).apply(map);

  _stats.append(SingleStat("Apply Pre Ops Time (sec)", timer.getElapsedAndRestart()));

  // will reproject if necessary.
  MapProjector::projectToPlanar(map);

  _stats.append(SingleStat("Project to Planar Time (sec)", timer.getElapsedAndRestart()));

  // find all the matches in this map
  if (_matchThreshold.get())
  {
    _matchFactory.createMatches(map, _matches, _bounds, _matchThreshold);
  }
  else
  {
    _matchFactory.createMatches(map, _matches, _bounds);
  }
  LOG_DEBUG("Match count: " << _matches.size());
  LOG_TRACE(SystemInfo::getMemoryUsageString());

  double findMatchesTime = timer.getElapsedAndRestart();
  _stats.append(SingleStat("Find Matches Time (sec)", findMatchesTime));
  _stats.append(SingleStat("Number of Matches Found", _matches.size()));
  _stats.append(SingleStat("Number of Matches Found per Second",
    (double)_matches.size() / findMatchesTime));

  // Now, for differential conflation, let us delete everything in the first dataset involved
  // in the match, and leave whatever is in the second.
  for (std::vector<const Match*>::iterator mit = _matches.begin(); mit != _matches.end(); ++mit)
  {
    std::set< std::pair<ElementId, ElementId> > pairs = (*mit)->getMatchPairs();

    for (std::set< std::pair<ElementId, ElementId> >::iterator pit = pairs.begin();
         pit != pairs.end(); ++pit)
    {
      RecursiveElementRemover(pit->first).apply(map);
    }
  }

  LOG_INFO("Applying post-diff conflation operations...");
  NamedOp(ConfigOptions().getUnifyPostOps()).apply(map);

  _stats.append(SingleStat("Apply Post Ops Time (sec)", timer.getElapsedAndRestart()));
}

void DiffConflator::setConfiguration(const Settings &conf)
{
  _settings = conf;

  _matchThreshold.reset();
  _reset();
}

void DiffConflator::_reset()
{
  _deleteAll(_matches);
}

void DiffConflator::_printMatches(vector<const Match*> matches)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    LOG_DEBUG(matches[i]->toString());
  }
}

void DiffConflator::_printMatches(vector<const Match*> matches, const MatchType& typeFilter)
{
  for (size_t i = 0; i < matches.size(); i++)
  {
    const Match* match = matches[i];
    if (match->getType() == typeFilter)
    {
      LOG_DEBUG(match);
    }
  }
}

}
