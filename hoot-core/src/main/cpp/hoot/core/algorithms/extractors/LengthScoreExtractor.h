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
 * @copyright Copyright (C) 2015, 2017, 2018, 2019 DigitalGlobe (http://www.digitalglobe.com/)
 */
#ifndef LENGTHSCOREEXTRACTOR_H
#define LENGTHSCOREEXTRACTOR_H

#include <hoot/core/algorithms/extractors/WayFeatureExtractor.h>

namespace hoot
{

/**
 * See the exploratory funds report for details.
 */
class LengthScoreExtractor : public WayFeatureExtractor
{
public:

  static std::string className() { return "hoot::LengthScoreExtractor"; }

  LengthScoreExtractor(ValueAggregatorPtr wayAgg);

  LengthScoreExtractor() = default;
  virtual ~LengthScoreExtractor() = default;

  virtual std::string getClassName() const override { return className(); }

  virtual QString getDescription() const
  { return "Calculates a score based on the length of a match between way features"; }

protected:

  double _extract(const OsmMap& map, const ConstWayPtr& w1, const ConstWayPtr& w2) const override;
};

}

#endif // LENGTHSCOREEXTRACTOR_H
