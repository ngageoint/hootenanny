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
 * @copyright Copyright (C) 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "MapCompareUtils.h"

// hoot
#include <hoot/core/scoring/AttributeComparator.h>
#include <hoot/core/scoring/GraphComparator.h>
#include <hoot/core/scoring/RasterComparator.h>
#include <hoot/core/util/ConfigOptions.h>

namespace hoot
{

void MapCompareUtils::getAttributeComparisonFinalScores(const OsmMapPtr& map1, const OsmMapPtr& map2, int& mean,
                                                        int& confidence, const int numIterations)
{
  if (map1->isEmpty() || map2->isEmpty())
    throw EmptyMapInputException();

  Tgs::Random::instance()->seed(100);

  AttributeComparator attr(map1, map2);
  attr.setIterations(numIterations);
  attr.compareMaps();
  int thisConfidence = convertRawScoreToFinalScore(attr.getConfidenceInterval());
  int thisMean = convertRawScoreToFinalScore(attr.getMeanScore());

  confidence = thisConfidence;
  mean = thisMean;
}

int MapCompareUtils::getAttributeComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2, const int numIterations)
{
  int mean = 0;
  int confidence = 0;
  getAttributeComparisonFinalScores(map1, map2, mean, confidence, numIterations);
  return mean;
}

void MapCompareUtils::getGraphComparisonRawScores(const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean, double& confidence)
{
  if (map1->isEmpty() || map2->isEmpty())
    throw EmptyMapInputException();

  Tgs::Random::instance()->seed(0);
  GraphComparator graph(map1, map2);
  graph.setDebugImages(ConfigOptions().getScoreGraphDebugImages());
  graph.setIterations(1000);
  graph.setPixelSize(10);
  graph.setMaxThreads(ConfigOptions().getGraphComparatorMaxThreads());
  graph.compareMaps();
  double thisConfidence = graph.getConfidenceInterval();
  double thisMean = graph.getMeanScore();

  confidence += thisConfidence;
  mean += thisMean;
}

int MapCompareUtils::getGraphComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  double mean = 0.0;
  double confidence = 0.0;
  getGraphComparisonRawScores(map1, map2, mean, confidence);
  return convertRawScoreToFinalScore(mean);
}

void MapCompareUtils::getRasterComparisonRawScores(const OsmMapPtr& map1, const OsmMapPtr& map2, double& mean)
{
  if (map1->isEmpty() || map2->isEmpty())
    throw EmptyMapInputException();

  RasterComparator raster(map1, map2);
  raster.setPixelSize(5);
  mean += raster.compareMaps();
}

int MapCompareUtils::getRasterComparisonFinalScore(const OsmMapPtr& map1, const OsmMapPtr& map2)
{
  double mean = 0.0;
  getRasterComparisonRawScores(map1, map2, mean);
  return convertRawScoreToFinalScore(mean);
}

int MapCompareUtils::convertRawScoreToFinalScore(const double score)
{
  return score * 1000.0 + 0.5;
}

}
