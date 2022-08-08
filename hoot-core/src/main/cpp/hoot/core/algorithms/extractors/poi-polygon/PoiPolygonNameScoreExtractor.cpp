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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#include "PoiPolygonNameScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/string/LevenshteinDistance.h>
#include <hoot/core/algorithms/string/MeanWordSetDistance.h>
#include <hoot/core/language/ToEnglishTranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonNameScoreExtractor)

std::shared_ptr<ToEnglishTranslator> PoiPolygonNameScoreExtractor::_translator;

PoiPolygonNameScoreExtractor::PoiPolygonNameScoreExtractor() :
_translateTagValuesToEnglish(false),
_namesProcessed(0),
_matchAttemptMade(false)
{
  // default string comp
  _stringComp =
    std::make_shared<MeanWordSetDistance>(
      std::make_shared<LevenshteinDistance>(ConfigOptions().getLevenshteinDistanceAlpha()));
}

void PoiPolygonNameScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  setNameScoreThreshold(config.getPoiPolygonNameScoreThreshold());
  setLevDist(config.getLevenshteinDistanceAlpha());

  const QString stringCompClassName = config.getPoiPolygonNameStringComparer().trimmed();
  if (stringCompClassName.isEmpty())
  {
    throw IllegalArgumentException(
      "No POI/Polygon string comparer specified (must implement StringDistance).");
  }
  else
  {
    _stringComp = Factory::getInstance().constructObject<StringDistance>(stringCompClassName);
    if (!_stringComp)
    {
      throw IllegalArgumentException(
        "Invalid POI/Polygon string comparer (must implement StringDistance): " +
        stringCompClassName);
    }
    std::shared_ptr<StringDistanceConsumer> strDistConsumer =
      std::dynamic_pointer_cast<StringDistanceConsumer>(_stringComp);
    if (strDistConsumer)
    {
      strDistConsumer->setStringDistance(
        std::make_shared<LevenshteinDistance>(ConfigOptions().getLevenshteinDistanceAlpha()));
    }
  }

  setTranslateTagValuesToEnglish(config.getPoiPolygonNameTranslateToEnglish());
  if (_translateTagValuesToEnglish && !_translator)
  {
    _translator =
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        config.getLanguageTranslationTranslator());
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(config.getLanguageTranslationSourceLanguages());
    _translator->setId(className());
  }
}

std::shared_ptr<NameExtractor> PoiPolygonNameScoreExtractor::_getNameExtractor() const
{
  assert(_stringComp);
  if (_translateTagValuesToEnglish)
  {
    std::shared_ptr<ToEnglishTranslateStringDistance> translateStringDist =
      std::make_shared<ToEnglishTranslateStringDistance>(_stringComp, _translator);
    translateStringDist->setTranslateAll(false);
    return std::make_shared<NameExtractor>(translateStringDist);
  }
  else
  {
    return std::make_shared<NameExtractor>(_stringComp);
  }
}

double PoiPolygonNameScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  std::shared_ptr<NameExtractor> nameExtractor = _getNameExtractor();
  double nameScore = nameExtractor->extract(poi, poly);
  _namesProcessed = nameExtractor->getNamesProcessed();
  _matchAttemptMade = nameExtractor->getMatchAttemptMade();
  if (nameScore < 0.001)
  {
    nameScore = 0.0;
  }
  LOG_VART(nameScore);
  return nameScore;
}

}
