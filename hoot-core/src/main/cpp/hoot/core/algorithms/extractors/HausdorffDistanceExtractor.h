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
 * @copyright Copyright (C) 2005 VividSolutions (http://www.vividsolutions.com/)
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef HAUSDORFFDISTANCEMATCHER_H
#define HAUSDORFFDISTANCEMATCHER_H

#include <hoot/core/algorithms/extractors/AbstractDistanceExtractor.h>

namespace hoot
{

/**
 * @brief The HausdorffDistanceExtractor class measures how far two subsets of a metric space are
 * from each other.
 * @author RoadMatcher
 * @copyright GPL
 * http://www.vividsolutions.com/products.asp?catg=spaapp&code=roadmatcher
 * The ideas were shamelessly taken from RoadMatcher, but reimplemented in C++ with Hootenanny
 * appropriate data structures.
 */
class HausdorffDistanceExtractor : public AbstractDistanceExtractor
{
public:
  static QString className() { return "hoot::HausdorffDistanceExtractor"; }

  HausdorffDistanceExtractor() = default;
  ~HausdorffDistanceExtractor() = default;

  double distance(const OsmMap& map, const std::shared_ptr<const Element>& target,
                  const std::shared_ptr<const Element>& candidate) const override;

  QString getName() const override { return className(); }
  QString getClassName() const override { return HausdorffDistanceExtractor::className(); }
  QString getDescription() const override
  { return "Calculates the Hausdorff distance between two features"; }
};

}

#endif // HAUSDORFFDISTANCEMATCHER_H
