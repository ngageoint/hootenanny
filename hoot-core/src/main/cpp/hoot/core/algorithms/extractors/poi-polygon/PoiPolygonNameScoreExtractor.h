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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONNAMESCOREEXTRACTOR_H
#define POIPOLYGONNAMESCOREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/algorithms/extractors/NameExtractor.h>

namespace hoot
{

/**
 * Scores element name similarity
 */
class PoiPolygonNameScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::PoiPolygonNameScoreExtractor"; }

  PoiPolygonNameScoreExtractor();
  virtual ~PoiPolygonNameScoreExtractor() = default;

  virtual std::string getClassName() const { return PoiPolygonNameScoreExtractor::className(); }

  /**
   * Returns a score from 0 to 1 representing the similarity of the feature names.  A score of -1
   * means one or both of the features have no names.
   *
   * @param poi the first element to examine
   * @param poly the second element to examine
   * @return a name score
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

  virtual void setConfiguration(const Settings& conf);

  double getNameScoreThreshold() const { return _nameScoreThreshold; }
  void setNameScoreThreshold(double threshold) { _nameScoreThreshold = threshold; }

  double getLevDist() const { return _levDist; }
  void setLevDist(double dist) { _levDist = dist; }

  bool getTranslateTagValuesToEnglish() const { return _translateTagValuesToEnglish; }
  void setTranslateTagValuesToEnglish(bool translate) { _translateTagValuesToEnglish = translate; }

  virtual QString getDescription() const
  { return "Scores name similarity for POI/Polygon conflation"; }

  long getNamesProcessed() const { return _namesProcessed; }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }

private:

  friend class PoiPolygonNameScoreExtractorTest;

  double _nameScoreThreshold;
  double _levDist;

  //when enabled, will attempt to translate address tags to English
  bool _translateTagValuesToEnglish;
  // See comments in PoiPolygonTypeScoreExtractor as to why this is static.
  static std::shared_ptr<ToEnglishTranslator> _translator;

  mutable long _namesProcessed;
  mutable bool _matchAttemptMade;

  std::shared_ptr<NameExtractor> _getNameExtractor() const;
};

}

#endif // POIPOLYGONNAMESCOREEXTRACTOR_H
