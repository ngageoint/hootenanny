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
  static QString t1BestKvp;
  static QString t2BestKvp;

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

  /**
   * Determines if an element is a recreation center
   *
   * @param element the element to examine
   * @return true if it is a recreation center; false otherwise
   */
  static bool isRecCenter(ConstElementPtr element);

  /**
   * Determines if an element is like a building in any way
   *
   * @param element the element to examine
   * @return true if it like a building; false otherwise
   */
  static bool isBuildingIsh(ConstElementPtr element);

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

  void setTypeScoreThreshold(double threshold) { _typeScoreThreshold = threshold; }

  void setDistance(double distance) { _distance = distance; }

private:

  double _typeScoreThreshold;
  static QSet<QString> _allTagKeys;
  double _distance;

  double _getTagScore(ConstElementPtr poi, ConstElementPtr poly) const;
  QStringList _getRelatedTags(const Tags& tags) const;

};

}

#endif // POIPOLYGONTYPESCOREEXTRACTOR_H
