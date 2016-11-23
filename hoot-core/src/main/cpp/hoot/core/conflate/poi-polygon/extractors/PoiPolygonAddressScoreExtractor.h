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
#ifndef POIPOLYGONADDRESSSCOREEXTRACTOR_H
#define POIPOLYGONADDRESSSCOREEXTRACTOR_H

// hoot
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/extractors/FeatureExtractorBase.h>
#include <hoot/core/util/Configurable.h>

namespace hoot
{

/**
 * Calculates the address similarity score of two features.  Only exact string matches yield a
 * positive score.  This translates addresses, but doesn't handle abbreviations.
 */
class PoiPolygonAddressScoreExtractor : public FeatureExtractorBase//, public Configurable
{
public:

  static string className() { return "hoot::PoiPolygonAddressScoreExtractor"; }

  static const QChar ESZETT;
  static const QString ESZETT_REPLACE;

  static const QString HOUSE_NUMBER_TAG_NAME;
  static const QString STREET_TAG_NAME;
  static const QString FULL_ADDRESS_TAG_NAME;
  static const QString FULL_ADDRESS_TAG_NAME_2;

  PoiPolygonAddressScoreExtractor();

  virtual string getClassName() const { return PoiPolygonAddressScoreExtractor::className(); }

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

  //virtual void setConfiguration(const Settings& conf);

  //double getAddressScoreThreshold() { return _addressScoreThreshold; }
  //void setAddressScoreThreshold(double threshold) { _addressScoreThreshold = threshold; }

private:

  //double _addressScoreThreshold;

  void _collectAddressesFromElement(ConstElementPtr element, QSet<QString>& addresses) const;
  void _collectAddressesFromWayNodes(ConstWayPtr way, QSet<QString>& addresses,
                                     const OsmMap& map) const;
  void _collectAddressesFromRelationMembers(ConstRelationPtr relation, QSet<QString>& addresses,
                                     const OsmMap& map) const;
  void _parseAddressesAsRange(const QString houseNum, const QString street,
                              QSet<QString>& addresses) const;
  void _parseAddressesInAltFormat(const Tags& tags, QSet<QString>& addresses) const;
  bool _addressesMatchesOnSubLetter(const QString polyAddress, const QString poiAddress) const;

};

}

#endif // POIPOLYGONADDRESSSCOREEXTRACTOR_H
