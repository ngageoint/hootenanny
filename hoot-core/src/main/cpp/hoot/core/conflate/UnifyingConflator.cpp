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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "UnifyingConflator.h"

// hoot
#include <hoot/core/conflate/matching/MatchThreshold.h>
#include <hoot/core/elements/MapProjector.h>
#include <hoot/core/io/OsmMapWriterFactory.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(OsmMapOperation, UnifyingConflator)

UnifyingConflator::UnifyingConflator()
  : AbstractConflator()
{
}

UnifyingConflator::UnifyingConflator(const std::shared_ptr<MatchThreshold>& matchThreshold)
  : AbstractConflator::AbstractConflator(matchThreshold)
{
}

void UnifyingConflator::apply(OsmMapPtr& map)
{
  _reset();
  _map = map;
  _currentStep = 1;  // tracks the current job task step for progress reporting

  // will reproject if necessary
  MapProjector::projectToPlanar(_map);
  _stats.append(SingleStat("Project to Planar Time (sec)", _timer.getElapsedAndRestart()));

  _updateProgress(_currentStep - 1, "Matching features...");
  _createMatches();
  // Add score tags to all matches that have some score component.
  if (ConfigOptions().getWriterIncludeConflateScoreTags())
    _addConflateScoreTags();

  _currentStep++;

  if (!ConfigOptions().getConflateMatchOnly())
  {
    _updateProgress(_currentStep - 1, "Optimizing feature matches...");
    _matchSets = _optimizeMatches(_matches);
    _currentStep++;

    _updateProgress(_currentStep - 1, "Merging feature matches...");

    std::vector<MergerPtr> relationMergers;
    _createMergers(relationMergers);
    _mergeFeatures(relationMergers);

    _currentStep++;
  }

  // free up any used resources
  _reset();
}

unsigned int UnifyingConflator::getNumSteps() const
{
  if (!ConfigOptions().getConflateMatchOnly())
    return 3;
  else
    return 1;
}

}
