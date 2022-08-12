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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */

#ifndef POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
#define POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/conflate/phone/PhoneNumberParser.h>

namespace hoot
{

class OsmMap;

/**
 * @brief The PoiPolygonPhoneNumberScoreExtractor class scores phone number similarity between
 * features for POI to Polygon conflation.
 */
class PoiPolygonPhoneNumberScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static QString className() { return "PoiPolygonPhoneNumberScoreExtractor"; }

  PoiPolygonPhoneNumberScoreExtractor();
  ~PoiPolygonPhoneNumberScoreExtractor() = default;

  /**
   * @brief extract returns a score from 0 to 1 representing the similarity of the phone numbers
   * contained on two features.
   * @param map map containing the features
   * @param poi the first element to examine
   * @param poly the second element to examine
   * @return a score
   */
  double extract(const OsmMap& map, const ConstElementPtr& poi, const ConstElementPtr& poly) const override;

  void setConfiguration(const Settings& conf) override;

  QString getClassName() const override { return className(); }
  QString getName() const  override{ return className(); }
  QString getDescription() const override
  { return "Scores phone number similarity for POI/Polygon conflation"; }

  long getPhoneNumbersProcessed() const { return _phoneNumberParser.getPhoneNumbersProcessed(); }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }

private:

  friend class PoiPolygonPhoneNumberScoreExtractorTest;

  mutable long _phoneNumbersProcessed;
  mutable bool _matchAttemptMade;

  PhoneNumberParser _phoneNumberParser;
};

}

#endif // POIPOLYGONPHONENUMBERSCOREEXTRACTOR_H
