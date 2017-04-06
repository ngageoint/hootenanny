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
 * @copyright Copyright (C) 2015, 2017 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef RFEXTRACTORCLASSIFIER_H
#define RFEXTRACTORCLASSIFIER_H

// hoot
#include <hoot/core/OsmMap.h>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{
using namespace Tgs;

class FeatureExtractor;
class WaySublineMatchString;
class MatchClassification;

class RfExtractorClassifier
{
public:

  static std::string className() { return "hoot::RfExtractorClassifier"; }

  static unsigned int logWarnCount;

  RfExtractorClassifier();

  /**
   * Classifies the match type of a building pair and returns the results.
   */
  virtual MatchClassification classify(const ConstOsmMapPtrR map,
    ElementId eid1, ElementId eid2) const;

  virtual map<QString, double> getFeatures(const ConstOsmMapPtrR m,
    ElementId eid1, ElementId eid2) const;

  void import(QDomElement& docRoot);

protected:

  mutable vector< boost::shared_ptr<const FeatureExtractor> > _extractors;
  QStringList _rfFactorLabels;
  auto_ptr<RandomForest> _rf;

  /**
   * Creates all the possible extractors for this classifier. Only the ones that are also listed
   * in the trained RandomForest will be used. The _extractors member variable should be populated
   * upon completion.
   */
  virtual void _createExtractors() const = 0;

  const vector< boost::shared_ptr<const FeatureExtractor> >& _getExtractors() const;
};

}

#endif // RFEXTRACTORCLASSIFIER_H
