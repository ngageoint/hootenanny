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
 * @copyright Copyright (C) 2018, 2019, 2020, 2021 Maxar (http://www.maxar.com/)
 */
#ifndef PARALLELSCOREEXTRACTOR_H
#define PARALLELSCOREEXTRACTOR_H

#include <hoot/core/algorithms/extractors/WayFeatureExtractor.h>

namespace hoot
{

/**
 * Derive a score from how parallel the ways are
 */
class ParallelScoreExtractor : public WayFeatureExtractor
{
public:

  static QString className() { return "hoot::ParallelScoreExtractor"; }

  ParallelScoreExtractor() = default;
  virtual ~ParallelScoreExtractor() = default;

  virtual QString getName() const { return className(); }

  virtual QString getClassName() const override { return className(); }

  virtual QString getDescription() const
  { return "Calculates how parallel ways are with each other"; }

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const override;
};

}

#endif // PARALLELSCOREEXTRACTOR_H
