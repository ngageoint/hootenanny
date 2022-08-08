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
#ifndef DISTANCESCOREEXTRACTOR_H
#define DISTANCESCOREEXTRACTOR_H

#include <hoot/core/algorithms/extractors/WayFeatureExtractor.h>

namespace hoot
{

/**
 *
 */
/**
 * @brief The DistanceScoreExtractor class is similar to Metric Distance as described in [1].
 *
 * 1. Savary & Zeitouni, 2005
 */
class DistanceScoreExtractor : public WayFeatureExtractor
{
public:

  static QString className() { return "hoot::DistanceScoreExtractor"; }

  DistanceScoreExtractor() = default;
  DistanceScoreExtractor(ValueAggregatorPtr wayAgg);
  ~DistanceScoreExtractor() = default;

  QString getClassName() const override { return className(); }
  QString getName() const override { return className(); }
  QString getDescription() const override
  { return "Calculates metric distance between features"; }

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const override;
};

}

#endif // DISTANCESCOREEXTRACTOR_H
