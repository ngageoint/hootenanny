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
 * @copyright Copyright (C) 2015, 2017, 2018 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef POIRFCLASSIFIER_H
#define POIRFCLASSIFIER_H

// hoot
#include <hoot/core/OsmMap.h>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

class FeatureExtractor;
class MatchClassification;

class PoiRfClassifier
{
public:

  static std::string className() { return "hoot::PoiRfClassifier"; }

  static unsigned int logWarnCount;

  PoiRfClassifier();

  virtual MatchClassification classify(const ConstOsmMapPtr& map,
    ElementId eid1, ElementId eid2);

  virtual std::map<QString, double> getFeatures(const ConstOsmMapPtr &m,
    ElementId eid1, ElementId eid2) const;

private:

  boost::shared_ptr<Tgs::RandomForest> _rf;
  QStringList _rfFactorLabels;
  std::vector< boost::shared_ptr<const FeatureExtractor> > _extractors;

  void _createAllExtractors();
  void _createTestExtractors();

  const std::vector< boost::shared_ptr<const FeatureExtractor> >& _getExtractors() const;

};

typedef boost::shared_ptr<PoiRfClassifier> PoiRfClassifierPtr;

}

#endif // POIRFCLASSIFIER_H
