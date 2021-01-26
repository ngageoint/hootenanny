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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONTYPESCOREEXTRACTOR_H
#define POIPOLYGONTYPESCOREEXTRACTOR_H

// Hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/conflate/poi-polygon/PoiPolygonInfoCache.h>

// Qt
#include <QMultiHash>

namespace hoot
{

/**
 * Scores element type similarity
 */
class PoiPolygonTypeScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static QString className() { return "hoot::PoiPolygonTypeScoreExtractor"; }

  PoiPolygonTypeScoreExtractor(PoiPolygonInfoCachePtr infoCache = PoiPolygonInfoCachePtr());
  virtual ~PoiPolygonTypeScoreExtractor() = default;

  virtual QString getClassName() const { return className(); }

  virtual QString getName() const { return className(); }

  /**
   * Returns a score from 0 to 1 representing the similarity of the feature types.
   *
   * @param map map containing the elements whose score is to be determined
   * @param poi a POI element
   * @param poly a polygon element
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

  virtual void setConfiguration(const Settings& conf);

  double getTypeScoreThreshold() { return _typeScoreThreshold; }
  void setTypeScoreThreshold(double threshold) { _typeScoreThreshold = threshold; }

  double getFeatureDistance() { return _featureDistance; }
  void setFeatureDistance(double distance) { _featureDistance = distance; }

  bool getPrintMatchDistanceTruth() { return _printMatchDistanceTruth; }
  void setPrintMatchDistanceTruth(bool print) { _printMatchDistanceTruth = print; }

  virtual QString getDescription() const
  { return "Scores element type similarity for POI/Polygon conflation"; }

  QStringList getFailedMatchRequirements() const { return _failedMatchRequirements; }
  bool getNoTypeFound() const { return _noTypeFound; }

private:

  double _typeScoreThreshold;
  double _featureDistance;
  bool _printMatchDistanceTruth;
  static QMap<QString, QSet<QString>> _categoriesToSchemaTagValues;

  //when enabled, will scan through all tags and, for any tag keys recognized in the schema, will
  //attempt to translate their values to English if not determined already to be in English
  bool _translateTagValuesToEnglish;
  // This translator is static due to the fact this class gets init'd many times by
  // PoiPolygonMatchCreator via PoiPolygonMatch.  Constructing it from a factory for every
  // instantiation causes performance to suffer.  Arguably, it could be made a static variable on
  // PoiPolygonMatch and then set on each score extractor individually.  However, doing that won't
  // allow you to see the the final statistics printed out individually by translators, like
  // HootServicesTranslatorClient.
  static std::shared_ptr<ToEnglishTranslator> _translator;

  //best type kvp match for the poi
  mutable QString _poiBestKvp;
  //best type kvp match for the poly
  mutable QString _polyBestKvp;
  //custom type matching types that failed
  mutable QStringList _failedMatchRequirements;
  mutable bool _noTypeFound;

  PoiPolygonInfoCachePtr _infoCache;

  double _getTagScore(ConstElementPtr poi, ConstElementPtr poly) const;
  QStringList _getRelatedTags(const Tags& tags) const;
  bool _failsCuisineMatch(const ConstElementPtr& e1, const ConstElementPtr& e2) const;
  bool _failsSportMatch(const ConstElementPtr& e1, const ConstElementPtr& e2) const;
  bool _failsReligionMatch(const ConstElementPtr& e1, const ConstElementPtr& e2) const;

  void _translateTagValue(const QString& tagKey, QString& tagValue) const;
  static QSet<QString> _getTagValueTokens(const QString& category);

  bool _haveConflictingTags(const QString& tagKey, const Tags& t1, const Tags& t2, QString& tag1Val,
                            QString& tag2Val) const;
};

}

#endif // POIPOLYGONTYPESCOREEXTRACTOR_H
