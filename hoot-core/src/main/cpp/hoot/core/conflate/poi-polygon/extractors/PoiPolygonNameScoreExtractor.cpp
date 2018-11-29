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
 * @copyright Copyright (C) 2016, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonNameScoreExtractor.h"

// hoot
#include <hoot/core/algorithms/LevenshteinDistance.h>
#include <hoot/core/algorithms/MeanWordSetDistance.h>
#include <hoot/core/language/TranslateStringDistance.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/core/util/Factory.h>
#include <hoot/core/util/Log.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(FeatureExtractor, PoiPolygonNameScoreExtractor)

boost::shared_ptr<ToEnglishTranslator> PoiPolygonNameScoreExtractor::_translator;

PoiPolygonNameScoreExtractor::PoiPolygonNameScoreExtractor() :
_translateTagValuesToEnglish(false),
_namesProcessed(0),
_matchAttemptMade(false)
{
}

void PoiPolygonNameScoreExtractor::setConfiguration(const Settings& conf)
{
  ConfigOptions config = ConfigOptions(conf);

  setNameScoreThreshold(config.getPoiPolygonNameScoreThreshold());
  setLevDist(config.getLevenshteinDistanceAlpha());

  setTranslateTagValuesToEnglish(config.getPoiPolygonNameTranslateToEnglish());
  if (_translateTagValuesToEnglish && !_translator)
  {
    _translator.reset(
      Factory::getInstance().constructObject<ToEnglishTranslator>(
        config.getLanguageTranslationTranslator()));
    _translator->setConfiguration(conf);
    _translator->setSourceLanguages(config.getLanguageTranslationSourceLanguages());
    _translator->setId(QString::fromStdString(className()));
  }
}

boost::shared_ptr<NameExtractor> PoiPolygonNameScoreExtractor::_getNameExtractor() const
{
  if (_translateTagValuesToEnglish)
  {
    TranslateStringDistance* translateStringDist =
      new TranslateStringDistance(
        StringDistancePtr(
          new MeanWordSetDistance(
            StringDistancePtr(
              new LevenshteinDistance(
                //why does this fail when the mem var is used?
                /*_levDist*/ConfigOptions().getLevenshteinDistanceAlpha())))),
        _translator);
    translateStringDist->setTranslateAll(false);
    return
      boost::shared_ptr<NameExtractor>(new NameExtractor(StringDistancePtr(translateStringDist)));
  }
  else
  {
    return
      boost::shared_ptr<NameExtractor>(
        new NameExtractor(
          StringDistancePtr(
            new MeanWordSetDistance(
              StringDistancePtr(
                new LevenshteinDistance(
                  ConfigOptions().getLevenshteinDistanceAlpha()))))));
  }
}

double PoiPolygonNameScoreExtractor::extract(const OsmMap& /*map*/,
                                             const ConstElementPtr& poi,
                                             const ConstElementPtr& poly) const
{
  boost::shared_ptr<NameExtractor> nameExtractor = _getNameExtractor();
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
