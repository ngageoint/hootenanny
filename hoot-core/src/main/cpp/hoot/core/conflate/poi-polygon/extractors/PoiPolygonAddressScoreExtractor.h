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
#include <hoot/core/language/ToEnglishTranslator.h>
#include <hoot/core/util/Configurable.h>
#include <hoot/core/algorithms/ExactStringDistance.h>

// Qt
#include <QMultiMap>

namespace hoot
{

class PoiPolygonAddress;

/**
 * Calculates the address similarity score of two features involved in POI/Polygon conflation.
 * Only exact string matches yield a positive score.  This can translate addresses, but doesn't
 * handle abbreviations.
 *
 * types (10/16/18):
 *
 * block
 * full_address
 * number
 * street
 * street_corner
 * street_prefix
 * street_side
 * street_suffix
 */
class PoiPolygonAddressScoreExtractor : public FeatureExtractorBase, public Configurable
{
public:

  static std::string className() { return "hoot::PoiPolygonAddressScoreExtractor"; }

//  static const QString HOUSE_NUMBER_TAG_NAME;
//  static const QString STREET_TAG_NAME;
//  static const QString FULL_ADDRESS_TAG_NAME;
//  static const QString FULL_ADDRESS_TAG_NAME_2;

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

  static QString getAddressTagValue(const Tags& tags, const QString addressTagType);

private:

  friend class PoiPolygonAddressScoreExtractorTest;

  //when enabled, will attempt to translate address tags to English
  bool _translateTagValuesToEnglish;
  // See comments in PoiPolygonTypeScoreExtractor as to why this is static.
  static boost::shared_ptr<ToEnglishTranslator> _translator;
  mutable long _addressesProcessed;
  mutable bool _matchAttemptMade;

  static QMultiMap<QString, QString> _addressTypeToTagKeys;

  void _collectAddressesFromElement(const Element& element,
                                    QList<PoiPolygonAddress>& addresses) const;
  void _collectAddressesFromWayNodes(const Way& way, QList<PoiPolygonAddress>& addresses,
                                     const OsmMap& map) const;
  void _collectAddressesFromRelationMembers(const Relation& relation,
                                            QList<PoiPolygonAddress>& addresses,
                                            const OsmMap& map) const;
  void _parseAddressesAsRange(const QString houseNum, const QString street,
                              QList<PoiPolygonAddress>& addresses) const;
  void _parseAddressesInAltFormat(const Tags& tags, QList<PoiPolygonAddress>& addresses) const;

  void _translateAddressToEnglish(QString& address) const;

  static void _readAddressTagKeys(const QString configFile);
};

}

#endif // POIPOLYGONADDRESSSCOREEXTRACTOR_H
