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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "ConflateStatsHelper.h"

// hoot
#include <hoot/core/util/HootException.h>
#include <hoot/core/util/Log.h>
#include <hoot/core/info/SingleStat.h>
#include <hoot/core/info/CreatorDescription.h>

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
    getSingleStat("Total Features", _input1Stats) +
    getSingleStat("Total Features", _input2Stats);

  if (numInputFeaturesTotal > 0.0)
  {
    const double numTotalFeaturesInOutput = getSingleStat("Total Features", _outputStats);
    LOG_VART(numTotalFeaturesInOutput);
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

    // Not passing insertIndex by ref, but if any stat methods are added after this one it will need
    // to be.
    _addRefAsGroundTruthStats(statsToUpdate, insertIndex);
  }
}

void ConflateStatsHelper::_addRefAsGroundTruthStats(QList<SingleStat>& statsToUpdate,
                                                    long insertIndex)
{
  // Some extra stats requested for comparing conflated output when map 1 is known to be ground
  // truth. Although not added to the statistic labels themselves, if map 1 is ground truth then
  // you may assume:
  //
  // "Percentage of Map 1 Matched With Map 2" = percentage of true positive matches
  // "Percentage of Map 1 Not Matched With Map 2" = percentage of false negative mismatches
  // "Percentage of Map 2 Not Matched With Map 1" = percentage of false positive mismatches
  //
  // The same thing applies to the size related stats that follow them.

  const double numMap1Features = getSingleStat("Total Features", _input1Stats);
  const double numMap1UnconflatedFeatures =
    getSingleStat("Total Unmatched Features From Map 1", _outputStats);
  const double numMap2Features = getSingleStat("Total Features", _input2Stats);
  const double numMap2UnconflatedFeatures =
    getSingleStat("Total Unmatched Features From Map 2", _outputStats);
  const double numConflatedFeatures = getSingleStat("Total Conflated Features", _outputStats);
  statsToUpdate.insert(
    insertIndex++,
    SingleStat(
      "Percentage of Number of Total Map 1 Features Matched With Map 2 Features",
      (numConflatedFeatures / numMap1Features) * 100.0));
  statsToUpdate.insert(
    insertIndex++,
    SingleStat(
      "Percentage of Number of Total Map 1 Features Not Matched With Map 2 Features",
      (numMap1UnconflatedFeatures / numMap1Features) * 100.0));
  statsToUpdate.insert(
    insertIndex++,
    SingleStat(
      "Percentage of Number of Total Map 2 Features Not Matched With Map 1 Features",
      (numMap2UnconflatedFeatures / numMap2Features) * 100.0));

  for (CreatorDescription::BaseFeatureType it = CreatorDescription::POI;
       it < CreatorDescription::Unknown;
       it = CreatorDescription::BaseFeatureType(it + 1))
  {
    const QString featureType = CreatorDescription::baseFeatureTypeToString(it);

    // These stats will only exist for feature types whose size are determined by the corresponding
    // measurement type, as set in CalculateStatsOp.

    // TODO: There may be a way to combine the following two code blocks into a single one.
    if (hasSingleStat("Total Meters of " + featureType + "s", _input1Stats) &&
        hasSingleStat("Total Meters of " + featureType + "s", _input2Stats) &&
        hasSingleStat("Meters of Conflated " + featureType + "s", _outputStats) &&
        hasSingleStat("Meters of Unmatched " + featureType + "s From Map 1", _outputStats) &&
        hasSingleStat("Meters of Unmatched " + featureType + "s From Map 2", _outputStats))
    {
      const double map1TotalLength =
        getSingleStat("Total Meters of " + featureType + "s", _input1Stats);
      const double map2TotalLength =
        getSingleStat("Total Meters of " + featureType + "s", _input2Stats);
      if (map1TotalLength > 0.0 && map2TotalLength > 0.0)
      {
        const double map1LengthUnconflated =
          getSingleStat("Meters of Unmatched " + featureType + "s From Map 1", _outputStats);
        const double map2LengthUnconflated =
          getSingleStat("Meters of Unmatched " + featureType + "s From Map 2", _outputStats);
        const double conflatedLength =
          getSingleStat("Meters of Conflated " + featureType + "s", _outputStats);
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Length of Map 1 " + featureType + "s Matched With Map 2 " + featureType +
              "s",
            (conflatedLength / map1TotalLength) * 100.0));
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Length of Map 1 " + featureType  + "s Not Matched With Map 2 " +
              featureType + "s",
            (map1LengthUnconflated / map1TotalLength) * 100.0));
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Length of Map 2 " + featureType  + "s Not Matched With Map 1 " +
              featureType + "s",
            (map2LengthUnconflated / map2TotalLength) * 100.0));
      }
    }
    else if (hasSingleStat("Total Square Meters of " + featureType + "s", _input1Stats) &&
        hasSingleStat("Total Square Meters of " + featureType + "s", _input2Stats) &&
        hasSingleStat("Square Meters of Conflated " + featureType + "s", _outputStats) &&
        hasSingleStat("Square Meters of Unmatched " + featureType + "s From Map 1", _outputStats) &&
        hasSingleStat("Square Meters of Unmatched " + featureType + "s From Map 2", _outputStats))
    {
      const double map1TotalArea =
        getSingleStat("Total Square Meters of " + featureType + "s", _input1Stats);
      const double map2TotalArea =
        getSingleStat("Total Square Meters of " + featureType + "s", _input2Stats);
      if (map1TotalArea > 0.0 && map2TotalArea > 0.0)
      {
        const double map1AreaUnconflated =
          getSingleStat("Square Meters of Unmatched " + featureType + "s From Map 1", _outputStats);
        const double map2AreaUnconflated =
          getSingleStat("Square Meters of Unmatched " + featureType + "s From Map 2", _outputStats);
        const double conflatedArea =
          getSingleStat("Square Meters of Conflated " + featureType + "s", _outputStats);
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Area of Map 1 " + featureType + "s Matched With Map 2 " + featureType +
              "s",
            (conflatedArea / map1TotalArea) * 100.0));
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Area of Map 1 " + featureType  + "s Not Matched With Map 2 " +
              featureType + "s",
            (map1AreaUnconflated / map1TotalArea) * 100.0));
        statsToUpdate.insert(
          insertIndex++,
          SingleStat(
            "Percentage of Area of Map 2 " + featureType  + "s Not Matched With Map 1 " +
              featureType + "s",
            (map2AreaUnconflated / map2TotalArea) * 100.0));
      }
    }
  }
}

bool ConflateStatsHelper::hasSingleStat(const QString& statName, const QList<SingleStat> stats)
{
  for (int i = 0; i < stats.size(); i++)
  {
    if (stats[i].name == statName)
    {
      return true;
    }
  }
  return false;
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

