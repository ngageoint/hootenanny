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
#ifndef POIPOLYGONNAMESCOREEXTRACTOR_H
#define POIPOLYGONNAMESCOREEXTRACTOR_H

// hoot
#include <hoot/core/elements/Element.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>


namespace hoot
{

/**
 * Scores element name similarity
 */
class PoiPolygonNameScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static string className() { return "hoot::PoiPolygonNameScoreExtractor"; }

  PoiPolygonNameScoreExtractor();

  virtual string getClassName() const { return PoiPolygonNameScoreExtractor::className(); }

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

  /**
   * Returns true if the input element has a populated name tag
   *
   * @param element the element to examine
   * @return true if the element has a name tag; false otherwie
   */
  static bool elementHasName(ConstElementPtr element);

  /**
   * Returns an element's name
   *
   * @param element the element to examine
   * @return the element's name if it has one
   */
  static QString getElementName(ConstElementPtr element);

  virtual void setConfiguration(const Settings& conf);

  double getNameScoreThreshold() { return _nameScoreThreshold; }
  void setNameScoreThreshold(double threshold) { _nameScoreThreshold = threshold; }

private:

  double _nameScoreThreshold;

};

}

#endif // POIPOLYGONNAMESCOREEXTRACTOR_H
