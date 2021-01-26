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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef CENTROIDDISTANCEEXTRACTOR_H
#define CENTROIDDISTANCEEXTRACTOR_H

#include <hoot/core/algorithms/extractors/AbstractDistanceExtractor.h>

namespace hoot
{

/**
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class CentroidDistanceExtractor : public AbstractDistanceExtractor
{
public:
  static QString className() { return "hoot::CentroidDistanceExtractor"; }

  CentroidDistanceExtractor() = default;
  virtual ~CentroidDistanceExtractor() = default;

  virtual double distance(const OsmMap& map, const std::shared_ptr<const Element>& target,
    const std::shared_ptr<const Element>& candidate) const override;

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const { return CentroidDistanceExtractor::className(); }

  virtual QString getDescription() const
  { return "Finds feature centroids and calculates the distance between them"; }
};

}

#endif // CENTROIDDISTANCEEXTRACTOR_H
