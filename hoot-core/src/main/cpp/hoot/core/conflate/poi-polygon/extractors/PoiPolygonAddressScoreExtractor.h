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
#ifndef POIPOLYGONADDRESSSCOREEXTRACTOR_H
#define POIPOLYGONADDRESSSCOREEXTRACTOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/ExactStringDistance.h>
#include <hoot/core/algorithms/AddressParser.h>
#include <hoot/core/language/AddressTranslator.h>

// Qt
#include <QSet>

namespace hoot
{

class Address;

/**
 * Calculates the address similarity score of two features involved in POI/Polygon conflation.
 * Only exact string matches yield a positive score.  Address normalization and translation is
 * handled by libpostal, which uses ML trained on OSM data.  There is an option to do language
 * translation with a custom translator.
 *
 * Since libpostal has built in translation and it has been tuned for addresses, we use that by
 * default.  Hooks to ToEnglishTranslator are left in for now in the case that it provides value by
 * supporting some language that libpostal doesn't.
 *
 * Some effort was spent in validating addresses with libaddressinput
 * (https://github.com/googlei18n/libaddressinput).  It was found that yields no utility since the
 * features we are comparing are geographically close, don't need higher level address info
 * (state, city, etc.), and we basically assume feature address are valid anyway...we're just trying
 * to match them to each other.
 */
class PoiPolygonAddressScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::PoiPolygonAddressScoreExtractor"; }

  PoiPolygonAddressScoreExtractor();

  virtual std::string getClassName() const { return PoiPolygonAddressScoreExtractor::className(); }

  virtual void setConfiguration(const Settings& conf);

  /**
   * Calculates the address similarity score of two features
   *
   * @param map map containing the elements whose score is to be determined
   * @param poi a POI element
   * @param poly a polygon element
   * @return the address score
   */
  virtual double extract(const OsmMap& map, const ConstElementPtr& poi,
                         const ConstElementPtr& poly) const;

  /**
   * Determines if a node has an address
   *
   * @param node the node to examine for an address
   * @return true if the node has an address; false otherwise
   */
  static bool nodeHasAddress(const Node& node);

  /**
   * Determines if an element has an address
   *
   * @param element the element to examine for an address
   * @param map map the element being examined belongs to
   * @return true if the element has an address; false otherwise
   */
  static bool elementHasAddress(const ConstElementPtr& element, const OsmMap& map);

  virtual QString getDescription() const
  { return "Scores address similarity for POI/Polygon conflation"; }

  long getAddressesProcessed() const { return _addressesProcessed; }
  bool getMatchAttemptMade() const { return _matchAttemptMade; }
  void setAllowLenientHouseNumberMatching(bool allow) { _allowLenientHouseNumberMatching = allow; }

private:

  friend class PoiPolygonAddressScoreExtractorTest;

  //when enabled, will attempt to translate address tags to English before address normalization
  //(translation also automatically occurs during normalization)
  bool _translateTagValuesToEnglish;
  mutable long _addressesProcessed;
  mutable bool _matchAttemptMade;
  bool _allowLenientHouseNumberMatching;

  AddressParser _addressParser;
  AddressTranslator _addressTranslator;

  void _collectAddressesFromElement(const Element& element, QList<Address>& addresses) const;
  void _collectAddressesFromWayNodes(const Way& way, QList<Address>& addresses, const OsmMap& map,
                                     const ElementId& poiId = ElementId()) const;
  void _collectAddressesFromRelationMembers(const Relation& relation,
                                            QList<Address>& addresses,
                                            const OsmMap& map,
                                            const ElementId& poiId = ElementId()) const;
};

}

#endif // POIPOLYGONADDRESSSCOREEXTRACTOR_H
