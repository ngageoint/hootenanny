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
#include "MedianToDividedRoadClassifier.h"

// hoot
#include <hoot/core/algorithms/aggregator/MeanAggregator.h>
#include <hoot/core/algorithms/aggregator/RmseAggregator.h>
#include <hoot/core/algorithms/aggregator/SigmaAggregator.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/WeightedMetricDistanceExtractor.h>
#include <hoot/core/algorithms/linearreference/WaySublineCollection.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(HighwayClassifier, MedianToDividedRoadClassifier)

const QString MedianToDividedRoadClassifier::MEDIAN_MATCHED_SUBROUTINE_NAME = "MedianMatch";
const QString MedianToDividedRoadClassifier::MEDIAN_MATCHED_DESCRIPTION =
  "Matched by median to divided road matching routine.";

MedianToDividedRoadClassifier::MedianToDividedRoadClassifier()
{
  _createExtractors();
}

MatchClassification MedianToDividedRoadClassifier::classify(
  const ConstOsmMapPtr& map, const ElementId& eid1, const ElementId& eid2,
  const WaySublineMatchString& match)
{
  MatchClassification classification;
  classification.setMiss();

  if (!match.isValid())
  {
    return classification;
  }

  ConstElementPtr element1 = map->getElement(eid1);
  ConstElementPtr element2 = map->getElement(eid2);
  if (element1 && element2)
  {
    for (QMap<std::shared_ptr<const FeatureExtractor>, double>::const_iterator extractorItr =
           _extractors.begin();
         extractorItr != _extractors.end(); ++extractorItr)
    {
      std::shared_ptr<const FeatureExtractor> featureExtractor = extractorItr.key();
      const double minScore = extractorItr.value();
      const double score = featureExtractor->extract(*map, element1, element2);
      if (score < minScore)
      {
        LOG_TRACE(
          featureExtractor->getClassName() << " failed with score: " << score << " for " <<
          element1->getElementId() << " and " << element2->getElementId() <<
          ". Minimum score allowed: " << minScore << ".")
        return classification;
      }
    }
    classification.setMatch();
    LOG_TRACE("Match found between " << eid1 << " and " << eid2 << ".");
  }

  return classification;
}

void MedianToDividedRoadClassifier::_createExtractors()
{
  // We're basically copying and tweaking the default road feature extractors here. Since the
  // original road extractors were used to build a model which we roughly treat as a black box when
  // we use it for classification and we don't know the original score values used in the model,
  // these min score values were determined experimentally and may need tweaking over time. A better
  // long term solution is probably to train a separate model for median to divided road matching.
  _extractors.clear();
  _extractors[
    std::make_shared<EdgeDistanceExtractor>(std::make_shared<RmseAggregator>())] = 0.955;
  _extractors[
     std::make_shared<EdgeDistanceExtractor>(std::make_shared<SigmaAggregator>())] = 0.997;
  _extractors[std::make_shared<AngleHistogramExtractor>()] = 1.0;
  _extractors[
    std::make_shared<WeightedMetricDistanceExtractor>(
      std::make_shared<MeanAggregator>(), std::make_shared<RmseAggregator>(),
      ConfigOptions().getSearchRadiusHighway())] = 0.695;
}

std::map<QString, double> MedianToDividedRoadClassifier::getFeatures(
  const ConstOsmMapPtr& /*m*/, const ElementId& /*eid1*/, const ElementId& /*eid2*/,
   const WaySublineMatchString& /*match*/) const
{
  // not implemented
  return std::map<QString, double>();
}

}
