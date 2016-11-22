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
 * @copyright Copyright (C) 2015, 2016 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIPOLYGONTYPESCOREEXTRACTOR_H
#define POIPOLYGONTYPESCOREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Scores element type similarity
 */
class PoiPolygonTypeScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  //hack to get around constness of extract method
  //best type kvp match for the poi
  static QString poiBestKvp;
  //best type kvp match for the poly
  static QString polyBestKvp;

  static string className() { return "hoot::PoiPolygonTypeScoreExtractor"; }

  PoiPolygonTypeScoreExtractor();

  virtual string getClassName() const { return PoiPolygonTypeScoreExtractor::className(); }

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

  /**
   * Determines if an element is a park
   *
   * @param element the element to examine
   * @return true if it is a park; false otherwise
   */
  static bool isPark(ConstElementPtr element);

  //static bool isRecCenter(/*ConstElementPtr element*/const QString elementName);

  //static bool isBuildingIsh(ConstElementPtr element, const QString elementName);

  static bool isParkish(ConstElementPtr element);
  //static bool isPlayArea(/*ConstElementPtr element*/const QString elementName);
  static bool isPlayground(ConstElementPtr element);
  static bool isSport(ConstElementPtr element);
  static bool isSchool(ConstElementPtr element);
  static bool isRestroom(ConstElementPtr element);
  static bool isParking(ConstElementPtr element);

  /**
   * Determines if an element has more than one type associated with it
   *
   * @param element the element to examine
   * @return true if it has more than one type; false otherwise
   */
  static bool hasMoreThanOneType(ConstElementPtr element);

  /**
   * Determines if an element has a type associated with it
   *
   * @param element the element to examine
   * @return true if it has a type; false otherwise
   */
  static bool hasType(ConstElementPtr element);

  double getTypeScoreThreshold() { return _typeScoreThreshold; }
  void setTypeScoreThreshold(double threshold) { _typeScoreThreshold = threshold; }

  double getFeatureDistance() { return _featureDistance; }
  void setFeatureDistance(double distance) { _featureDistance = distance; }

private:

  double _typeScoreThreshold;
  static QSet<QString> _allTagKeys;
  double _featureDistance;

  double _getTagScore(ConstElementPtr poi, ConstElementPtr poly) const;
  QStringList _getRelatedTags(const Tags& tags) const;
  bool _failsCuisineMatch(const Tags& t1, const Tags& t2) const;
  bool _failsSportMatch(const Tags& t1, const Tags& t2) const;

};

}

#endif // POIPOLYGONTYPESCOREEXTRACTOR_H
