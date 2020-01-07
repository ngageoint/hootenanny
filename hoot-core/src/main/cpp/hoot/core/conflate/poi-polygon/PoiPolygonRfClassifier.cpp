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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "PoiPolygonRfClassifier.h"

// hoot
//#include <hoot/core/algorithms/extractors/AddressScoreExtractor.h>
//#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonAlphaShapeDistanceExtractor.h>
//#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonDistanceExtractor.h>
//#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonNameScoreExtractor.h>
//#include <hoot/core/algorithms/extractors/poi-polygon/PoiPolygonTypeScoreExtractor.h>
#include <hoot/core/util/Factory.h>

using namespace std;

namespace hoot
{

PoiPolygonRfClassifier::PoiPolygonRfClassifier()
{
  _createExtractors();
}

void PoiPolygonRfClassifier::_createExtractors()
{
  _extractors.clear();

//  _extractors.push_back(std::shared_ptr<FeatureExtractor>(new PoiPolygonDistanceExtractor()));
//  _extractors.push_back(std::shared_ptr<FeatureExtractor>(
//    new PoiPolygonAlphaShapeDistanceExtractor()));
//  _extractors.push_back(std::shared_ptr<FeatureExtractor>(new PoiPolygonNameScoreExtractor()));
//  _extractors.push_back(std::shared_ptr<FeatureExtractor>(new PoiPolygonTypeScoreExtractor()));
//  std::shared_ptr<AddressScoreExtractor> addressExtractor1(new AddressScoreExtractor());
//  addressExtractor1->setConfiguration(conf());
//  _extractors.push_back(addressExtractor1);
}

map<QString, double> PoiPolygonRfClassifier::getFeatures(const ConstOsmMapPtr& m,
  ElementId eid1, ElementId eid2) const
{
  map<QString, double> result;

  ConstElementPtr e1 = m->getElement(eid1);
  ConstElementPtr e2 = m->getElement(eid2);

  for (size_t i = 0; i < _extractors.size(); i++)
  {
    double v = _extractors[i]->extract(*m, e1, e2);
    // if it isn't null then include it.
    if (!FeatureExtractor::isNull(v))
    {
      QString factorName =
        QString::fromStdString(_extractors[i]->getName()).replace(QRegExp("[^\\w]"), "_");
      result[factorName] = v;
    }
  }

  return result;
}

}
