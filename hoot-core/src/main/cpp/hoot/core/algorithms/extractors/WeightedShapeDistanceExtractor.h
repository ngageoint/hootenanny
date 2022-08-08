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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019, 2020, 2021, 2022 Maxar (http://www.maxar.com/)
 */
#ifndef WEIGHTEDSHAPEDISTANCEEXTRACTOR_H
#define WEIGHTEDSHAPEDISTANCEEXTRACTOR_H

#include <hoot/core/algorithms/extractors/WayFeatureExtractor.h>

namespace hoot
{

/**
 * @brief The WeightedShapeDistanceExtractor class is similar to Shape Distance as described in [1].
 *
 * 1. Savary & Zeitouni, 2005
 */
class WeightedShapeDistanceExtractor : public WayFeatureExtractor
{
public:

  static QString className() { return "hoot::WeightedShapeDistanceExtractor"; }

  WeightedShapeDistanceExtractor() = default;
  WeightedShapeDistanceExtractor(ValueAggregatorPtr wayAgg);
  ~WeightedShapeDistanceExtractor() = default;

  QString getName() const override { return className(); }
  QString getClassName() const override { return className(); }
  QString getDescription() const override
  { return "Calculates shape distance between features"; }

private:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const override;
};

}

#endif // WEIGHTEDSHAPEDISTANCEEXTRACTOR_H
