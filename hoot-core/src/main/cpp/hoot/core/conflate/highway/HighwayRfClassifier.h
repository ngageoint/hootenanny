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
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef HIGHWAYRFCLASSIFIER_H
#define HIGHWAYRFCLASSIFIER_H

#include "HighwayClassifier.h"

// hoot
#include <hoot/core/conflate/extractors/FeatureExtractor.h>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

class HighwayRfClassifier : public HighwayClassifier
{
public:

  static std::string className() { return "hoot::HighwayRfClassifier"; }

  HighwayRfClassifier();

  virtual MatchClassification classify(const ConstOsmMapPtr& map,
    ElementId eid1, ElementId eid2, const WaySublineMatchString& match);

  virtual map<QString, double> getFeatures(const shared_ptr<const OsmMap>& m,
    ElementId eid1, ElementId eid2, const WaySublineMatchString& match) const;

private:
  mutable shared_ptr<Tgs::RandomForest> _rf;
  mutable QStringList _rfFactorLabels;
  mutable vector< shared_ptr<const FeatureExtractor> > _extractors;

  void _createAllExtractors() const;
  void _createTestExtractors() const;

  const vector< shared_ptr<const FeatureExtractor> >& _getExtractors() const;

  void _init() const;
};

}

#endif // HIGHWAYRFCLASSIFIER_H
