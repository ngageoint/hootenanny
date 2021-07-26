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
 * @copyright Copyright (C) 2015, 2016, 2017, 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#include "BuildingRfClassifier.h"

// hoot
#include <hoot/core/util/Factory.h>
#include <hoot/core/algorithms/aggregator/QuantileAggregator.h>
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/algorithms/extractors/CompactnessExtractor.h>
#include <hoot/core/algorithms/extractors/EdgeDistanceExtractor.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>
#include <hoot/core/algorithms/extractors/SmallerOverlapExtractor.h>
#include <hoot/core/algorithms/extractors/AngleHistogramExtractor.h>
#include <hoot/core/language/ToEnglishTranslateStringDistance.h>

namespace hoot
{

void BuildingRfClassifier::_createBestExtractors() const
{
  _extractors.clear();

  // This set was determined with experimentation using the Jakarta building data and weka (see
  // regression tests).
  _extractors.push_back(std::make_shared<AngleHistogramExtractor>(0.0));
  _extractors.push_back(std::make_shared<AngleHistogramExtractor>(0.3));
  _extractors.push_back(std::make_shared<CompactnessExtractor>());
  _extractors.push_back(std::make_shared<SmallerOverlapExtractor>());
  _extractors.push_back(
    std::make_shared<NameExtractor>(
      std::make_shared<ToEnglishTranslateStringDistance>(
        std::make_shared<MeanWordSetDistance>(
          std::make_shared<LevenshteinDistance>(1.45)))));
  _extractors.push_back(
    std::make_shared<EdgeDistanceExtractor>(std::make_shared<QuantileAggregator>(0.4)));
}

void BuildingRfClassifier::_createExtractors() const
{
  LOG_TRACE("Creating extractors...");
  _createBestExtractors();
}

}
