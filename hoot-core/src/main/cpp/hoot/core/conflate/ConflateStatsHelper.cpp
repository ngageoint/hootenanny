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
#include "ConflateStatsHelper.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/ops/stats/SingleStat.h>

// std
#include <math.h>

namespace hoot
{

ConflateStatsHelper::ConflateStatsHelper(const QList<SingleStat> input1Stats,
                                         const QList<SingleStat> input2Stats,
                                         const QList<SingleStat> outputStats) :
_input1Stats(input1Stats),
_input2Stats(input2Stats),
_outputStats(outputStats)
{

}

void ConflateStatsHelper::updateStats(QList<SingleStat>& statsToUpdate)
{
  updateStats(statsToUpdate, statsToUpdate.size());
}

void ConflateStatsHelper::updateStats(QList<SingleStat>& statsToUpdate, long insertIndex)
{
  if (insertIndex > statsToUpdate.size())
  {
    throw HootException("Invalid stats list index.");
  }

  const double numInputFeaturesTotal =
    getSingleStat("Total Feature Count", _input1Stats) +
    getSingleStat("Total Feature Count", _input2Stats);

  if (numInputFeaturesTotal > 0.0)
  {
    const double numTotalFeaturesInOutput = getSingleStat("Total Feature Count", _outputStats);
    const double differenceBetweenTotalInputFeaturesAndTotalOutputFeatures =
      numTotalFeaturesInOutput - numInputFeaturesTotal;
    statsToUpdate.insert(
      insertIndex++,
      SingleStat(
        "Difference Between Total Features in Output and Total Features in Inputs",
        differenceBetweenTotalInputFeaturesAndTotalOutputFeatures));
    statsToUpdate.insert(
      insertIndex++,
      SingleStat(
        "Percentage Difference Between Total Features in Output and Total Features in Inputs",
        (differenceBetweenTotalInputFeaturesAndTotalOutputFeatures / numInputFeaturesTotal) * 100.0));
  }
}

double ConflateStatsHelper::getSingleStat(const QString& statName, const QList<SingleStat> stats)
{
  for (int i = 0; i < stats.size(); i++)
  {
    if (stats[i].name == statName)
    {
      return stats[i].value;
    }
  }
  throw InternalErrorException("Could not find the specified stat: " + statName);
}

}

