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
 * @copyright Copyright (C) 2021 Maxar (http://www.maxar.com/)
 */
#include "MapCompareUtils.h"

// hoot
#include <hoot/core/scoring/GraphComparator.h>
#include <hoot/core/scoring/RasterComparator.h>
#include <hoot/core/scoring/AttributeComparator.h>

namespace hoot
{

void MapCompareUtils::getAttributeComparisonFinalScores(
  OsmMapPtr map1, OsmMapPtr map2, OsmMapPtr outMap, int& mean, int& confidence,
  const int numIterations)
{
  Tgs::Random::instance()->seed(100);

  {
    AttributeComparator attr(map1, outMap);
    attr.setIterations(numIterations);
    attr.compareMaps();
    int thisConfidence = convertRawScoreToFinalScore(attr.getConfidenceInterval());
    int thisMean = convertRawScoreToFinalScore(attr.getMeanScore());

    confidence = thisConfidence;
    mean = thisMean;
  }

  if (map2 != 0)
  {
    AttributeComparator attr(map2, outMap);
    attr.setIterations(numIterations);
    attr.compareMaps();
    int thisConfidence = convertRawScoreToFinalScore(attr.getConfidenceInterval());
    int thisMean = convertRawScoreToFinalScore(attr.getMeanScore());

    confidence += thisConfidence;
    mean += thisMean;

    confidence /= 2;
    mean /= 2;
  }
}

void MapCompareUtils::getAttributeComparisonFinalScores(
  OsmMapPtr map1, OsmMapPtr map2, int& mean, int& confidence, const int numIterations)
{
  Tgs::Random::instance()->seed(100);

  {
    AttributeComparator attr(map1, map2);
    attr.setIterations(numIterations);
    attr.compareMaps();
    int thisConfidence = convertRawScoreToFinalScore(attr.getConfidenceInterval());
    int thisMean = convertRawScoreToFinalScore(attr.getMeanScore());

    confidence = thisConfidence;
    mean = thisMean;
  }
}

int MapCompareUtils::getAttributeComparisonFinalScore(
  OsmMapPtr map1, OsmMapPtr map2, const int numIterations)
{
  int mean;
  int confidence;
  getAttributeComparisonFinalScores(map1, map2, mean, confidence, numIterations);
  return mean;
}

void MapCompareUtils::getGraphComparisonRawScores(
  OsmMapPtr map1, OsmMapPtr map2, double& mean, double& confidence)
{
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

int MapCompareUtils::getGraphComparisonFinalScore(OsmMapPtr map1, OsmMapPtr map2)
{
  double mean;
  double confidence;
  getGraphComparisonRawScores(map1, map2, mean, confidence);
  return convertRawScoreToFinalScore(mean);
}

void MapCompareUtils::getRasterComparisonRawScores(OsmMapPtr map1, OsmMapPtr map2, double& mean)
{
  RasterComparator raster(map1, map2);
  raster.setPixelSize(5);
  mean += raster.compareMaps();
}

int MapCompareUtils::getRasterComparisonFinalScore(OsmMapPtr map1, OsmMapPtr map2)
{
  double mean;
  getRasterComparisonRawScores(map1, map2, mean);
  return convertRawScoreToFinalScore(mean);
}

int MapCompareUtils::convertRawScoreToFinalScore(const double score)
{
  return score * 1000.0 + 0.5;
}

}
