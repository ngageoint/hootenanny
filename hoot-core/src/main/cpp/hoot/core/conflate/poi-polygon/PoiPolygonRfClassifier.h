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
 * @copyright Copyright (C) 2016, 2017, 2018, 2019 Maxar (http://www.maxar.com/)
 */
#ifndef POIPOLYGONRFCLASSIFIER_H
#define POIPOLYGONRFCLASSIFIER_H

// hoot
#include <hoot/core/algorithms/extractors/FeatureExtractor.h>
#include <hoot/core/elements/OsmMap.h>

namespace hoot
{

/**
 * This class lets you extract feature information by using the build-model command to export
 * an .arff file, which can be viewed in Weka.
 */
class PoiPolygonRfClassifier
{

public:

  PoiPolygonRfClassifier();

  virtual std::map<QString, double> getFeatures(const ConstOsmMapPtr& m, ElementId eid1,
                                                ElementId eid2) const;

private:

  std::vector<std::shared_ptr<const FeatureExtractor>> _extractors;

  void _createExtractors();
};

typedef std::shared_ptr<PoiPolygonRfClassifier> PoiPolygonRfClassifierPtr;

}

#endif // POIPOLYGONRFCLASSIFIER_H
